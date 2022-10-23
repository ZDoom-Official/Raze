//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT

This file is part of NBlood.

NBlood is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------
#include "ns.h"	// Must come before everything else!


#include "v_2ddrawer.h"
#include "common_game.h"
#include "v_draw.h"
#include "blood.h"

BEGIN_BLD_NS

extern void (*qavClientCallback[])(int, void*);


//==========================================================================
//
// QAV interpolation functions
//
//==========================================================================

using QAVPrevTileFinder = TILE_FRAME * (*)(FRAMEINFO* const thisFrame, FRAMEINFO* const prevFrame, const int i);

struct QAVInterpProps
{
	QAVPrevTileFinder PrevTileFinder;
	bool loopable;
	TMap<int, TArray<int>> IgnoreData;

	bool CanInterpFrameTile(const int nFrame, const int i)
	{
		// Check whether the current frame's tile is skippable.
		auto thisFrame = IgnoreData.CheckKey(nFrame);
		return thisFrame ? !thisFrame->Contains(i) : true;
	}
};

static TMap<FString, QAVPrevTileFinder> qavPrevTileFinders;
static TMap<int, QAVInterpProps> qavInterpProps;

static void qavInitTileFinderMap()
{
	// Interpolate between frames if the picnums match. This is safest but could miss interpolations between suitable picnums.
	qavPrevTileFinders.Insert("picnum", [](FRAMEINFO* const thisFrame, FRAMEINFO* const prevFrame, const int i) -> TILE_FRAME* {
		return prevFrame->tiles[i].texid == thisFrame->tiles[i].texid ? &prevFrame->tiles[i] : nullptr;
		});

	// Interpolate between frames if the tilenum is valid. This can be problematic if tile indices change between frames.
	qavPrevTileFinders.Insert("index", [](FRAMEINFO* const thisFrame, FRAMEINFO* const prevFrame, const int i) -> TILE_FRAME* {
		return prevFrame->tiles[i].texid.isValid() ? &prevFrame->tiles[i] : nullptr;
		});

	// Find previous frame by iterating all previous frame's tiles and return on first matched x coordinate.
	qavPrevTileFinders.Insert("x", [](FRAMEINFO* const thisFrame, FRAMEINFO* const prevFrame, const int i) -> TILE_FRAME* {
		for (int j = 0; j < 8; j++) if (thisFrame->tiles[i].x == prevFrame->tiles[j].x)
		{
			return &prevFrame->tiles[j];
		}
		return nullptr;
		});

	// Find previous frame by iterating all previous frame's tiles and return on first matched y coordinate.
	qavPrevTileFinders.Insert("y", [](FRAMEINFO* const thisFrame, FRAMEINFO* const prevFrame, const int i) -> TILE_FRAME* {
		for (int j = 0; j < 8; j++) if (thisFrame->tiles[i].y == prevFrame->tiles[j].y)
		{
			return &prevFrame->tiles[j];
		}
		return nullptr;
		});
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static QAVPrevTileFinder qavGetInterpType(const FString& type)
{
	if (!qavPrevTileFinders.CountUsed()) qavInitTileFinderMap();
	return *qavPrevTileFinders.CheckKey(type);
}

bool GameInterface::IsQAVInterpTypeValid(const FString& type)
{
	return qavGetInterpType(type) != nullptr;
}

void GameInterface::AddQAVInterpProps(const int res_id, const FString& interptype, const bool loopable, const TMap<int, TArray<int>>&& ignoredata)
{
	qavInterpProps.Insert(res_id, { qavGetInterpType(interptype), loopable, std::move(ignoredata) });
}

void GameInterface::RemoveQAVInterpProps(const int res_id)
{
	qavInterpProps.Remove(res_id);
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void DrawFrame(double x, double y, double z, double a, double alpha, FTextureID texid, int stat, int shade, int palnum, bool to3dview)
{
	double scale = z * (1. / 65536.);
	if (!to3dview)
	{
		auto tex = TexMan.GetGameTexture(texid);
		int renderstyle = (stat & RS_NOMASK) ? STYLE_Normal : STYLE_Translucent;
		int pin = (stat & kQavOrientationLeft) ? -1 : (stat & RS_ALIGN_R) ? 1 : 0;
		auto translation = TRANSLATION(Translation_Remap, palnum);
		bool topleft = !!(stat & RS_TOPLEFT);


		bool xflip = !!(stat & 0x100); // repurposed flag
		bool yflip = !!(stat & 4);
		auto color = shadeToLight(shade);

		DrawTexture(twod, tex, x, y, DTA_ScaleX, scale, DTA_ScaleY, scale, DTA_Rotate, a, DTA_LegacyRenderStyle, renderstyle, DTA_Alpha, alpha, DTA_Pin, pin, DTA_TranslationIndex, translation,
			DTA_TopLeft, topleft, DTA_CenterOffsetRel, topleft ? 0 : 2, DTA_FullscreenScale, FSMode_Fit320x200, DTA_FlipOffsets, true, DTA_Color, color,
			DTA_FlipX, xflip, DTA_FlipY, yflip, TAG_DONE);
	}
	else
	{
		// there's some disagreements about flag values between QAV and the drawer. Shuffle these around.
		if (stat & 4) stat |= RS_YFLIPHUD;
		stat &= ~4;
		if (stat & 0x100) stat |= RS_XFLIPHUD;
		stat &= ~0x100;
		if ((stat & kQavOrientationLeft)) stat |= RS_ALIGN_L;
		stat &= ~kQavOrientationLeft;

		hud_drawsprite(x, y, scale, a, texid, shade, palnum, stat, alpha);
	}
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void QAV::Draw(int ticks, int stat, int shade, int palnum, bool to3dview, double const interpfrac, DAngle angle)
{
	assert(ticksPerFrame > 0);

	auto const interpdata = qavInterpProps.CheckKey(res_id);

	auto const nFrame = clamp(ticks / ticksPerFrame, 0, nFrames - 1);
	FRAMEINFO* const thisFrame = &frames[nFrame];

	auto const oFrame = clamp((nFrame == 0 && interpdata && interpdata->loopable ? nFrames : nFrame) - 1, 0, nFrames - 1);
	FRAMEINFO* const prevFrame = &frames[oFrame];

	bool const interpolate = interpdata && cl_hudinterpolation && cl_bloodqavinterp && (nFrames > 1) && (nFrame != oFrame) && (interpfrac != 1.);

	for (int i = 0; i < 8; i++)
	{
		if (thisFrame->tiles[i].texid.isValid())
		{
			TILE_FRAME* const thisTile = &thisFrame->tiles[i];
			TILE_FRAME* const prevTile = interpolate && interpdata->CanInterpFrameTile(nFrame, i) ? interpdata->PrevTileFinder(thisFrame, prevFrame, i) : nullptr;

			double tileX;
			double tileY;
			double tileZ;
			DAngle tileA;
			double tileAlpha;
			int tileShade;
			auto const tileStat = stat | thisTile->stat;

			if (prevTile)
			{
				double prevAlpha = ((stat | prevTile->stat) & RS_TRANS1) ? glblend[0].def[!!((stat | prevTile->stat) & RS_TRANS2)].alpha : 1.f;
				double thisAlpha = (tileStat & RS_TRANS1) ? glblend[0].def[!!(tileStat & RS_TRANS2)].alpha : 1.f;

				tileX = interpolatedvalue<double>(prevTile->x, thisTile->x, interpfrac);
				tileY = interpolatedvalue<double>(prevTile->y, thisTile->y, interpfrac);
				tileZ = interpolatedvalue<double>(prevTile->z, thisTile->z, interpfrac);
				tileA = interpolatedvalue(prevTile->angle, thisTile->angle, interpfrac);
				tileShade = (int)interpolatedvalue<double>(prevTile->shade, thisTile->shade, interpfrac);
				tileAlpha = interpolatedvalue(prevAlpha, thisAlpha, interpfrac);
			}
			else
			{
				tileX = thisTile->x;
				tileY = thisTile->y;
				tileZ = thisTile->z;
				tileA = thisTile->angle;
				tileShade = thisTile->shade;
				tileAlpha = (tileStat & RS_TRANS1) ? glblend[0].def[!!(tileStat & RS_TRANS2)].alpha : 1.f;
			}

			DrawFrame(tileX + x, tileY + y, tileZ, (angle - tileA).Degrees(), tileAlpha, thisTile->texid, tileStat, tileShade + shade, (palnum <= 0 ? thisTile->palnum : palnum), to3dview);
		}
	}
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void QAV::Play(int start, int end, int nCallback, DBloodPlayer* pData)
{
	auto pActor = pData ? pData->GetActor() : nullptr;
	assert(ticksPerFrame > 0);
	int frame;
	int ticks;
	if (start < 0)
		frame = (start + 1) / ticksPerFrame;
	else
		frame = start / ticksPerFrame + 1;

	for (ticks = ticksPerFrame * frame; ticks <= end; frame++, ticks += ticksPerFrame)
	{
		if (frame >= 0 && frame < nFrames)
		{
			FRAMEINFO* pFrame = &frames[frame];
			SOUNDINFO* pSound = &pFrame->sound;

			// by NoOne: handle Sound kill flags
			if (!VanillaMode() && pSound->sndFlags > 0 && pSound->sndFlags <= kFlagSoundKillAll) {
				for (int i = 0; i < nFrames; i++) {
					FRAMEINFO* pFrame2 = &frames[i];
					SOUNDINFO* pSound2 = &pFrame2->sound;
					if (pSound2->sound != 0) {
						if (pSound->sndFlags != kFlagSoundKillAll && pSound2->priority != pSound->priority) continue;
						else if (pActor) {
							// We need stop all sounds in a range
							for (int a = 0; a <= pSound2->sndRange; a++)
								sfxKill3DSound(pActor, -1, pSound2->sound + a);
						}
						else {
							sndKillAllSounds();
						}
					}
				}
			}

			if (pSound->sound > 0) {
				int sound = pSound->sound;

				// by NoOne: add random rage sound feature
				if (pSound->sndRange > 0 && !VanillaMode())
					sound += Random((pSound->sndRange == 1) ? 2 : pSound->sndRange);

				if (pActor == nullptr) sndStartSample(sound, -1, -1, 0);
				else sfxPlay3DSound(pActor, sound, 16 + pSound->priority, 6);
			}

			if (pFrame->nCallbackId > 0 && nCallback != -1) {
				qavClientCallback[nCallback](pFrame->nCallbackId, pData);
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void QAV::Precache(int palette)
{
	for (int i = 0; i < nFrames; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (frames[i].tiles[j].texid.isValid())
				markTextureForPrecache(frames[i].tiles[j].texid, palette);
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void qavProcessTicker(QAV* const pQAV, int* duration, int* lastTick)
{
	if (*duration > 0)
	{
		const auto thisTick = I_GetTime(pQAV->ticrate);
		if (const auto numTicks = thisTick - (*lastTick))
		{
			*lastTick = thisTick;
			*duration -= pQAV->ticksPerFrame * numTicks;
		}
	}
	*duration = ClipLow(*duration, 0);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void qavProcessTimer(DBloodPlayer* const pPlayer, QAV* const pQAV, int* duration, double* interpfrac, bool const fixedduration, bool const ignoreWeaponTimer)
{
	// Process clock based on QAV's ticrate and last tick value.
	if (!paused)
	{
		qavProcessTicker(pQAV, &pPlayer->qavTimer, &pPlayer->qavLastTick);
	}
	else
	{
		pPlayer->qavLastTick = I_GetTime(pQAV->ticrate);
	}

	if (pPlayer->weaponTimer == 0 && pPlayer->qavTimer == 0 && !ignoreWeaponTimer)
	{
		// Check if we're playing an idle QAV as per the ticker's weapon timer.
		*duration = fixedduration ? pQAV->duration - 1 : I_GetBuildTime() % pQAV->duration;
		*interpfrac = 1.;
	}
	else if (pPlayer->qavTimer == 0)
	{
		// If qavTimer is 0, play the last frame uninterpolated. Sometimes the timer can be just ahead of weaponTimer.
		*duration = pQAV->duration - 1;
		*interpfrac = 1.;
	}
	else
	{
		// Apply normal values.
		*duration = pQAV->duration - pPlayer->qavTimer;
		*interpfrac = !cl_interpolate || cl_capfps || paused ? 1. : I_GetTimeFrac(pQAV->ticrate);
	}
}


//---------------------------------------------------------------------------
//
// This is to eliminate a huge design issue in NBlood that was apparently copied verbatim from the DOS-Version.
// Sequences were cached in the resource and directly returned from there in writable form, with byte swapping directly performed in the cache on Big Endian systems.
// To avoid such unsafe operations this caches the read data separately.
//
//---------------------------------------------------------------------------

extern FMemArena seqcache; // Use the same storage as the SEQs.
static TMap<int, QAV*> qavcache;
QAV* getQAV(int res_id)
{
	auto p = qavcache.CheckKey(res_id);
	if (p != nullptr) return *p;

	int index = fileSystem.FindResource(res_id, "QAV");
	if (index < 0)
	{
		return nullptr;
	}
	auto fr = fileSystem.OpenFileReader(index);

	// Start reading QAV for nFrames, skipping padded data.
	for (int i = 0; i < 8; i++) fr.ReadUInt8();
	int nFrames = fr.ReadInt32();
	auto qavdata = (QAV*)seqcache.Alloc(sizeof(QAV) + ((nFrames - 1) * sizeof(FRAMEINFO)));

	// Write out QAV data.
	qavdata->nFrames = nFrames;
	qavdata->ticksPerFrame = fr.ReadInt32();
	qavdata->duration = fr.ReadInt32();
	qavdata->x = fr.ReadInt32();
	qavdata->y = fr.ReadInt32();
	/*qavdata->nSprite =*/ fr.ReadInt32();
	for (int i = 0; i < 4; i++) fr.ReadUInt8();

	// Read FRAMEINFO data.
	for (int i = 0; i < qavdata->nFrames; i++)
	{
		qavdata->frames[i].nCallbackId = fr.ReadInt32();

		// Read SOUNDINFO data.
		qavdata->frames[i].sound.sound = fr.ReadInt32();
		qavdata->frames[i].sound.priority = fr.ReadUInt8();
		qavdata->frames[i].sound.sndFlags = fr.ReadUInt8();
		qavdata->frames[i].sound.sndRange = fr.ReadUInt8();
		for (int j = 0; j < 1; j++) fr.ReadUInt8();

		// Read TILE_FRAME data.
		for (int j = 0; j < 8; j++)
		{
			int tile = fr.ReadInt32();
			qavdata->frames[i].tiles[j].texid = tile <= 0 ? FNullTextureID() : tileGetTextureID(tile);
			qavdata->frames[i].tiles[j].x = fr.ReadInt32();
			qavdata->frames[i].tiles[j].y = fr.ReadInt32();
			qavdata->frames[i].tiles[j].z = fr.ReadInt32();
			qavdata->frames[i].tiles[j].stat = fr.ReadInt32();
			qavdata->frames[i].tiles[j].shade = fr.ReadInt8();
			qavdata->frames[i].tiles[j].palnum = fr.ReadUInt8();
			qavdata->frames[i].tiles[j].angle = mapangle(fr.ReadUInt16());
		}
	}

	// Write out additions.
	qavdata->res_id = res_id;
	qavdata->ticrate = 120. / qavdata->ticksPerFrame;

	qavcache.Insert(res_id, qavdata);
	return qavdata;
}


END_BLD_NS
