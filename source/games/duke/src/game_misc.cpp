//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2020 - Christoph Oelckers

This file is part of Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1996 - Todd Replogle
Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms
Modifications for JonoF's port by Jonathon Fowler (jf@jonof.id.au)
*/
//------------------------------------------------------------------------- 

// This file collects several functions from the original game.c
// that do not fit any particular category.

#include "ns.h"	// Must come before everything else!

#include "automap.h"
#include "duke3d.h"
#include "m_argv.h"
#include "mapinfo.h"
#include "texturemanager.h"
#include "statusbar.h"
#include "st_start.h"
#include "i_interface.h"
#include "prediction.h"
#include "sbar.h"
#include "glbackend/glbackend.h"
#include "gamestate.h"
#include "dukeactor.h"

BEGIN_DUKE_NS


FFont* IndexFont;
FFont* DigiFont;

//---------------------------------------------------------------------------
//
// debug output
//
//---------------------------------------------------------------------------

FString GameInterface::GetCoordString()
{
	int snum = screenpeek;
	FString out;

	out.Format("pos= %d, %d, %d - angle = %2.3f - sector = %d, lotag = %d, hitag = %d",
		ps[snum].posx, ps[snum].posy, ps[snum].posz, ps[snum].angle.ang.asdeg(), ps[snum].cursectnum,
		sector[ps[snum].cursectnum].lotag, sector[ps[snum].cursectnum].hitag);

	return out;
}


GameStats GameInterface::getStats()
{
	struct player_struct* p = &ps[myconnectindex];
	return { p->actors_killed, p->max_actors_killed, p->secret_rooms, p->max_secret_rooms, p->player_par / REALGAMETICSPERSEC, p->frag };
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

template<class func>
void runbonus(func completion)
{
	// MP scoreboard
	if (playerswhenstarted > 1 && !ud.coop)
	{
		dobonus(1, completion);
	}
	else completion(false);

}

template <class func>
void runtwoscreens(func completion)
{
	// shareware and TEN screens
	if (isShareware() && !isRR())
		showtwoscreens(completion);
	else completion(false);
}

static void endthegame(bool)
{
	endoomName = isRR() ? "redneck.bin" : !isShareware() ? "duke3d.bin" : "dukesw.bin";
	ST_Endoom();
}


void gameexitfrommenu()
{
	runbonus([](bool aborted) { runtwoscreens(endthegame); });
}

void GameInterface::ExitFromMenu() 
{ 
	gameexitfrommenu();
}

//---------------------------------------------------------------------------
//
// This now redirects the messages to the console's notification display
// which has all the features to reasonably do this in Duke style.
//
//---------------------------------------------------------------------------

void FTA(int q, struct player_struct* p)
{
	if (q < 0 || gamestate != GS_LEVEL)
		return;

	if (p->ftq != q || (ud.levelclock - p->ftt > TICRATE && q != QUOTE_DEAD))
	{
		p->ftq = q;
		auto qu = quoteMgr.GetQuote(q);
		if (p == &ps[screenpeek] && qu[0] != '\0')
		{
#if 0
			if (q >= 70 && q <= 72)
			{
				// Todo: redirect this to a centered message (these are "need a key" messages)
			}
			else
#endif
			{
				Printf(PRINT_MEDIUM | PRINT_NOTIFY, "%s\n", qu);
			}
		}
	}
	p->ftt = ud.levelclock;
}

//==========================================================================
//
// Draws the background
//
//==========================================================================

void GameInterface::DrawBackground()
{
	twod->ClearScreen();
	auto tex = tileGetTexture(TILE_MENUSCREEN);
	PalEntry color = 0xff808080;
	if (!hud_bgstretch)
		DrawTexture(twod, tex, 0, 0, DTA_FullscreenEx, FSMode_ScaleToFit43, DTA_Color, color, TAG_DONE);
	else
		DrawTexture(twod, tex, 0, 0, DTA_VirtualWidth, twod->GetWidth(), DTA_VirtualHeight, twod->GetHeight(), DTA_KeepRatio, true, DTA_Color, color, TAG_DONE);
}

//---------------------------------------------------------------------------
//
// this is from ZDoom
//
//---------------------------------------------------------------------------

void V_AddBlend (float r, float g, float b, float a, float v_blend[4])
{
	r = clamp(r/255.f, 0.f, 0.25f);
	g = clamp(g/255.f, 0.f, 0.25f);
	b = clamp(b/255.f, 0.f, 0.25f);
	a = clamp(a/255.f, 0.f, 0.25f);

	
	float a2, a3;

	if (a <= 0)
		return;
	a2 = v_blend[3] + (1-v_blend[3])*a;	// new total alpha
	a3 = v_blend[3]/a2;		// fraction of color from old

	v_blend[0] = v_blend[0]*a3 + r*(1-a3);
	v_blend[1] = v_blend[1]*a3 + g*(1-a3);
	v_blend[2] = v_blend[2]*a3 + b*(1-a3);
	v_blend[3] = a2;
}
 
//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

 void setgamepalette(int palid)
{
	if (palid >= MAXBASEPALS || palid < 0) palid = 0;
	auto& fstint = lookups.tables[MAXPALOOKUPS - 1];
	if (palid == WATERPAL) fstint.tintColor = PalEntry(224, 192, 255);
	else if (palid == SLIMEPAL) fstint.tintColor = PalEntry(208, 255, 192);
	else fstint.tintColor = 0xffffff;
	videoSetPalette(palid);
}

//---------------------------------------------------------------------------
//
// 'rest' in this case means everything not part of the 3D scene and its weapon sprite.
//
//---------------------------------------------------------------------------

void drawoverlays(double smoothratio)
{
	unsigned char fader = 0, fadeg = 0, fadeb = 0, fadef = 0, tintr = 0, tintg = 0, tintb = 0, tintf = 0, dotint = 0;

	struct player_struct* pp;
	int cposx, cposy, cang;

	pp = &ps[screenpeek];

	float blend[4] = {};

	// this does pain tinting etc from the CON
	V_AddBlend(pp->pals.r, pp->pals.g, pp->pals.b, pp->pals.a, blend);
	// loogies courtesy of being snotted on
	if (pp->loogcnt > 0)
	{
		V_AddBlend(0, 63, 0, (pp->loogcnt >> 1), blend);
	}
	if (blend[3])
	{
		// result must be multiplied by 4 and normalised to 255. (4*255 = 1020)
		auto comp = [&](int i, int maxv=255) { return clamp(int(blend[i] * 1020), 0, maxv); };
		videoFadePalette(comp(0), comp(1), comp(2), comp(3, 192)); // Never fully saturate the alpha channel
	}
	else
		videoclearFade();

	MarkSectorSeen(pp->cursectnum);

	if (ud.cameraactor == nullptr)
	{
		if (automapMode != am_full)
		{
			if (!isRR() && pp->newOwner != nullptr)
				cameratext(pp->newOwner);
			else
			{
				fi.displayweapon(screenpeek, smoothratio);
				if (pp->over_shoulder_on == 0)
					fi.displaymasks(screenpeek, smoothratio);
			}
			if (!isRR())
				moveclouds(smoothratio);
		}

		if (automapMode != am_off)
		{
			dointerpolations(smoothratio);

			if (pp->newOwner == nullptr && playrunning())
			{
				if (screenpeek == myconnectindex && numplayers > 1)
				{
					cposx = omyx + mulscale16(myx - omyx, smoothratio);
					cposy = omyy + mulscale16(myy - omyy, smoothratio);
					cang = omyang.asbuild() + mulscale16(((myang.asbuild() + 1024 - omyang.asbuild()) & 2047) - 1024, smoothratio);
				}
				else
				{
					cposx = pp->oposx + mulscale16(pp->posx - pp->oposx, smoothratio);
					cposy = pp->oposy + mulscale16(pp->posy - pp->oposy, smoothratio);
					cang = pp->angle.oang.asbuild() + mulscale16(((pp->angle.ang.asbuild() + 1024 - pp->angle.oang.asbuild()) & 2047) - 1024, smoothratio);
				}
			}
			else
			{
				cposx = pp->oposx;
				cposy = pp->oposy;
				cang = pp->angle.oang.asbuild();
			}
			DrawOverheadMap(cposx, cposy, cang);
			restoreinterpolations();
		}
	}

	DrawBorder();

	StatusBar->UpdateStatusBar();

	if (ps[myconnectindex].newOwner == nullptr && ud.cameraactor == nullptr)
	{
		DrawCrosshair(TILE_CROSSHAIR, ps[screenpeek].last_extra, -getHalfLookAng(pp->angle.olook_ang.asq16(), pp->angle.look_ang.asq16(), cl_syncinput, smoothratio), pp->over_shoulder_on ? 2.5 : 0, isRR() ? 0.5 : 1);
	}

	if (paused == 2)
		fi.PrintPaused();
}




//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void cameratext(DDukeActor *cam)
{
	auto drawitem = [=](int tile, double x, double y, bool flipx, bool flipy)
	{
		DrawTexture(twod, tileGetTexture(tile), x, y, DTA_ViewportX, windowxy1.x, DTA_ViewportY, windowxy1.y, DTA_ViewportWidth, windowxy2.x - windowxy1.x + 1, DTA_CenterOffsetRel, true,
			DTA_ViewportHeight, windowxy2.y - windowxy1.y + 1, DTA_FlipX, flipx, DTA_FlipY, flipy, DTA_FullscreenScale, FSMode_Fit320x200, TAG_DONE);
	};
	if (!cam->temp_data[0])
	{
		drawitem(TILE_CAMCORNER, 24, 33, false, false);
		drawitem(TILE_CAMCORNER + 1, 320 - 26, 33, false, false);
		drawitem(TILE_CAMCORNER + 1, 24, 163, true, true);
		drawitem(TILE_CAMCORNER + 1, 320 - 26, 163, false, true);

		if (ud.levelclock & 16)
			drawitem(TILE_CAMLIGHT, 46, 32, false, false);
	}
	else
	{
		int flipbits = (ud.levelclock << 1) & 48;

		for (int x = -64; x < 394; x += 64)
			for (int y = 0; y < 200; y += 64)
				drawitem(TILE_STATIC, x, y, !!(ud.levelclock & 8), !!(ud.levelclock & 16));
	}
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void dobonus(int bonusonly, const CompletionFunc& completion)
{
	if (isRR()) dobonus_r(bonusonly, completion);
	else dobonus_d(bonusonly, completion);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int startrts(int lumpNum, int localPlayer)
{
	if (SoundEnabled() &&
		RTS_IsInitialized() && rtsplaying == 0 && (snd_speech & (localPlayer ? 1 : 4)))
	{
		auto sid = RTS_GetSoundID(lumpNum - 1);
		if (sid != -1)
		{
			S_PlaySound(sid, CHAN_AUTO, CHANF_UI);
			rtsplaying = 7;
			return 1;
		}
	}

	return 0;
}

ReservedSpace GameInterface::GetReservedScreenSpace(int viewsize)
{
	// todo: factor in the frag bar: tilesiz[TILE_FRAGBAR].y
	int sbar = tilesiz[TILE_BOTTOMSTATUSBAR].y;
	if (isRR())
	{
		sbar >>= 1;
	}
	return { 0, sbar };
}

::GameInterface* CreateInterface()
{
	return new GameInterface;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool GameInterface::DrawAutomapPlayer(int cposx, int cposy, int czoom, int cang)
{
	int i, j, k, l, x1, y1, x2, y2, x3, y3, x4, y4, ox, oy, xoff, yoff;
	int dax, day, cosang, sinang, xspan, yspan, sprx, spry;
	int xrepeat, yrepeat, tilenum, daang;
	int xvect, yvect, xvect2, yvect2;
	int p;
	PalEntry col;
	spritetype* spr;

	xvect = -bsin(cang) * czoom;
	yvect = -bcos(cang) * czoom;
	xvect2 = mulscale16(xvect, yxaspect);
	yvect2 = mulscale16(yvect, yxaspect);

	//Draw sprites
	auto pactor = ps[screenpeek].GetActor();
	for (i = 0; i < numsectors; i++)
	{
		if (!gFullMap || !show2dsector[i]) continue;
		DukeSectIterator it(i);
		while (auto act = it.Next())
		{
			spr = &act->s;

			if (act == pactor || (spr->cstat & 0x8000) || spr->cstat == 257 || spr->xrepeat == 0) continue;

			col = PalEntry(0, 170, 170);
			if (spr->cstat & 1) col = PalEntry(170, 0, 170);

			sprx = spr->x;
			spry = spr->y;

			if ((spr->cstat & 257) != 0) switch (spr->cstat & 48)
			{
			case 0:
				//                    break;

				ox = sprx - cposx;
				oy = spry - cposy;
				x1 = dmulscale16(ox, xvect, -oy, yvect);
				y1 = dmulscale16(oy, xvect2, ox, yvect2);

				ox = bcos(spr->ang, -7);
				oy = bsin(spr->ang, -7);
				x2 = dmulscale16(ox, xvect, -oy, yvect);
				y2 = dmulscale16(oy, xvect, ox, yvect);

				x3 = mulscale16(x2, yxaspect);
				y3 = mulscale16(y2, yxaspect);

				drawlinergb(x1 - x2 + (xdim << 11), y1 - y3 + (ydim << 11),
					x1 + x2 + (xdim << 11), y1 + y3 + (ydim << 11), col);
				drawlinergb(x1 - y2 + (xdim << 11), y1 + x3 + (ydim << 11),
					x1 + x2 + (xdim << 11), y1 + y3 + (ydim << 11), col);
				drawlinergb(x1 + y2 + (xdim << 11), y1 - x3 + (ydim << 11),
					x1 + x2 + (xdim << 11), y1 + y3 + (ydim << 11), col);
				break;

			case 16:
				if (spr->picnum == TILE_LASERLINE)
				{
					x1 = sprx;
					y1 = spry;
					tilenum = spr->picnum;
					xoff = tileLeftOffset(tilenum) + spr->xoffset;
					if ((spr->cstat & 4) > 0) xoff = -xoff;
					k = spr->ang;
					l = spr->xrepeat;
					dax = bsin(k) * l;
					day = -bcos(k) * l;
					l = tileWidth(tilenum);
					k = (l >> 1) + xoff;
					x1 -= mulscale16(dax, k);
					x2 = x1 + mulscale16(dax, l);
					y1 -= mulscale16(day, k);
					y2 = y1 + mulscale16(day, l);

					ox = x1 - cposx;
					oy = y1 - cposy;
					x1 = dmulscale16(ox, xvect, -oy, yvect);
					y1 = dmulscale16(oy, xvect2, ox, yvect2);

					ox = x2 - cposx;
					oy = y2 - cposy;
					x2 = dmulscale16(ox, xvect, -oy, yvect);
					y2 = dmulscale16(oy, xvect2, ox, yvect2);

					drawlinergb(x1 + (xdim << 11), y1 + (ydim << 11),
						x2 + (xdim << 11), y2 + (ydim << 11), col);
				}

				break;

			case 32:
				tilenum = spr->picnum;
				xoff = tileLeftOffset(tilenum) + spr->xoffset;
				yoff = tileTopOffset(tilenum) + spr->yoffset;
				if ((spr->cstat & 4) > 0) xoff = -xoff;
				if ((spr->cstat & 8) > 0) yoff = -yoff;

				k = spr->ang;
				cosang = bcos(k);
				sinang = bsin(k);
				xspan = tileWidth(tilenum);
				xrepeat = spr->xrepeat;
				yspan = tileHeight(tilenum);
				yrepeat = spr->yrepeat;

				dax = ((xspan >> 1) + xoff) * xrepeat;
				day = ((yspan >> 1) + yoff) * yrepeat;
				x1 = sprx + dmulscale16(sinang, dax, cosang, day);
				y1 = spry + dmulscale16(sinang, day, -cosang, dax);
				l = xspan * xrepeat;
				x2 = x1 - mulscale16(sinang, l);
				y2 = y1 + mulscale16(cosang, l);
				l = yspan * yrepeat;
				k = -mulscale16(cosang, l);
				x3 = x2 + k;
				x4 = x1 + k;
				k = -mulscale16(sinang, l);
				y3 = y2 + k;
				y4 = y1 + k;

				ox = x1 - cposx;
				oy = y1 - cposy;
				x1 = dmulscale16(ox, xvect, -oy, yvect);
				y1 = dmulscale16(oy, xvect2, ox, yvect2);

				ox = x2 - cposx;
				oy = y2 - cposy;
				x2 = dmulscale16(ox, xvect, -oy, yvect);
				y2 = dmulscale16(oy, xvect2, ox, yvect2);

				ox = x3 - cposx;
				oy = y3 - cposy;
				x3 = dmulscale16(ox, xvect, -oy, yvect);
				y3 = dmulscale16(oy, xvect2, ox, yvect2);

				ox = x4 - cposx;
				oy = y4 - cposy;
				x4 = dmulscale16(ox, xvect, -oy, yvect);
				y4 = dmulscale16(oy, xvect2, ox, yvect2);

				drawlinergb(x1 + (xdim << 11), y1 + (ydim << 11),
					x2 + (xdim << 11), y2 + (ydim << 11), col);

				drawlinergb(x2 + (xdim << 11), y2 + (ydim << 11),
					x3 + (xdim << 11), y3 + (ydim << 11), col);

				drawlinergb(x3 + (xdim << 11), y3 + (ydim << 11),
					x4 + (xdim << 11), y4 + (ydim << 11), col);

				drawlinergb(x4 + (xdim << 11), y4 + (ydim << 11),
					x1 + (xdim << 11), y1 + (ydim << 11), col);

				break;
			}
		}
	}

	for (p = connecthead; p >= 0; p = connectpoint2[p])
	{
		auto pspr = &ps[p].GetActor()->s;
		ox = pspr->x - cposx;
		oy = pspr->y - cposy;
		daang = (pspr->ang - cang) & 2047;

		x1 = mulscale(ox, xvect, 16) - mulscale(oy, yvect, 16);
		y1 = mulscale(oy, xvect2, 16) + mulscale(ox, yvect2, 16);

		if (p == screenpeek || ud.coop == 1)
		{
			auto& pp = ps[p];
			if (pspr->xvel > 16 && pp.on_ground)
				i = TILE_APLAYERTOP + ((ud.levelclock >> 4) & 3);
			else
				i = TILE_APLAYERTOP;

			j = klabs(pp.truefz - pp.posz) >> 8;
			j = mulscale(czoom * (pspr->yrepeat + j), yxaspect, 16);

			if (j < 22000) j = 22000;
			else if (j > (65536 << 1)) j = (65536 << 1);

			DrawTexture(twod, tileGetTexture(i), xdim / 2. + x1 / 4096., ydim / 2. + y1 / 4096., DTA_TranslationIndex, TRANSLATION(Translation_Remap + setpal(&pp), pspr->pal), DTA_CenterOffset, true,
				DTA_Rotate, daang * (-360./2048), DTA_Color, shadeToLight(pspr->shade), DTA_ScaleX, j / 65536., DTA_ScaleY, j / 65536., TAG_DONE);
		}
	}
	return true;
}




END_DUKE_NS

