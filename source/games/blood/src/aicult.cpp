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

#include "build.h"

#include "blood.h"

BEGIN_BLD_NS


void TommySeqCallback(DBloodActor* actor)
{
	DVector3 vect(actor->spr.Angles.Yaw.ToVector(), actor->dudeSlope);
	vect.X += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
	vect.Y += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
	vect.Z += Random3F((5 - gGameOptions.nDifficulty) * 500, 14);
	actFireVector(actor, 0, 0, vect, kVectorBullet);
	sfxPlay3DSound(actor, 4001, -1, 0);
}

void TeslaSeqCallback(DBloodActor* actor)
{
	if (Chance(gCultTeslaFireChance[gGameOptions.nDifficulty]))
	{
		DVector3 vect(actor->spr.Angles.Yaw.ToVector(), actor->dudeSlope);
		vect.X += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
		vect.Y += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
		vect.Z += Random3F((5 - gGameOptions.nDifficulty) * 500, 14);
		actFireMissile(actor, 0, 0, vect, kMissileTeslaRegular);
		sfxPlay3DSound(actor, 470, -1, 0);
	}
}

void ShotSeqCallback(DBloodActor* actor)
{
	DVector3 vect(actor->spr.Angles.Yaw.ToVector(), actor->dudeSlope);
	vect.X += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
	vect.Y += Random3F((5 - gGameOptions.nDifficulty) * 1000, 14);
	vect.Z += Random3F((5 - gGameOptions.nDifficulty) * 500, 14);
	for (int i = 0; i < 8; i++)
	{
		double r3 = Random3F(500 , 14);
		double r2 = Random3F(1000, 14);
		double r1 = Random3F(1000, 14);
		actFireVector(actor, 0, 0, vect + DVector3(r1, r2, r3), kVectorShell);
	}
	if (Chance(0x8000))
		sfxPlay3DSound(actor, 1001, -1, 0);
	else
		sfxPlay3DSound(actor, 1002, -1, 0);
}

void cultThrowSeqCallback(DBloodActor* actor)
{
	int nMissile = kThingArmedTNTStick;
	if (gGameOptions.nDifficulty > 2)
		nMissile = kThingArmedTNTBundle;
	uint8_t v4 = Chance(0x6000);
	sfxPlay3DSound(actor, 455, -1, 0);
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	assert(actor->IsDudeActor());
	auto dv = target->spr.pos - actor->spr.pos;
	double nDist = dv.XY().Length();
	if (nDist > 0x1e0)
		v4 = 0;

	auto* pMissile = actFireThing(actor, 0., 0., dv.Z / 32768 - FixedToFloat(14500), nMissile, nDist * (2048. / 64800));
	if (v4)
		pMissile->xspr.Impact = 1;
	else
		evPostActor(pMissile, 120 * (1 + Random(2)), kCmdOn, actor);
}

void cultThrowSeqCallback2(DBloodActor* actor)
{
	int nMissile = kThingArmedTNTStick;
	if (gGameOptions.nDifficulty > 2)
		nMissile = kThingArmedTNTBundle;
	sfxPlay3DSound(actor, 455, -1, 0);
	auto pMissile = actFireThing(actor, 0., 0., actor->dudeSlope * 0.25 - 0.14435, nMissile, 0x133333 / 65536.);
	evPostActor(pMissile, 120 * (2 + Random(2)), kCmdOn, actor);
}

void cultThrowSeqCallback3(DBloodActor* actor)
{
	int nMissile = kThingArmedTNTStick;
	if (gGameOptions.nDifficulty > 2)
		nMissile = kThingArmedTNTBundle;
	sfxPlay3DSound(actor, 455, -1, 0);
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	assert(actor->IsDudeActor());
	auto dv = target->spr.pos - actor->spr.pos;
	double nDist = dv.XY().Length();

	auto* pMissile = actFireThing(actor, 0., 0., dv.Z / 32768 - FixedToFloat(14500), nMissile, nDist * (32. / 64800));
	pMissile->xspr.Impact = 1;
}

static bool TargetNearExplosion(sectortype* sector)
{
	BloodSectIterator it(sector);
	while (auto actor = it.Next())
	{
		if (actor->GetType() == kThingArmedTNTStick || actor->spr.statnum == kStatExplosion)
			return true;
	}
	return false;
}

static void cultThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiLookForTarget(actor);
}

static void cultThinkGoto(DBloodActor* actor)
{
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (nDist < 320 && absangle(actor->spr.Angles.Yaw, nAngle) < pDudeInfo->Periphery())
	{
		switch (actor->xspr.medium)
		{
		case kMediumNormal:
			aiNewState(actor, NAME_cultistSearch);
			break;
		case kMediumWater:
		case kMediumGoo:
			aiNewState(actor, NAME_cultistSwimSearch);
			break;
		}
	}
	aiThinkTarget(actor);
}

static void cultThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		switch (actor->xspr.medium)
		{
		case kMediumNormal:
			aiNewState(actor, NAME_cultistGoto);
			break;
		case kMediumWater:
		case kMediumGoo:
			aiNewState(actor, NAME_cultistSwimGoto);
			break;
		}
		return;
	}
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto target = actor->GetTarget();

	auto dvec = target->spr.pos - actor->spr.pos;
	double nDist = dvec.XY().Length();
	DAngle nAngle = dvec.Angle();
	aiChooseDirection(actor, nAngle);

	if (target->xspr.health == 0)
	{
		switch (actor->xspr.medium)
		{
		case kMediumNormal:
			aiNewState(actor, NAME_cultistSearch);
			if (actor->GetType() == kDudeCultistTommy)
				aiPlay3DSound(actor, 4021 + Random(4), AI_SFX_PRIORITY_1, -1);
			else
				aiPlay3DSound(actor, 1021 + Random(4), AI_SFX_PRIORITY_1, -1);
			break;
		case kMediumWater:
		case kMediumGoo:
			aiNewState(actor, NAME_cultistSwimSearch);
			break;
		}
		return;
	}
	if (target->IsPlayerActor() && powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0)
	{
		switch (actor->xspr.medium)
		{
		case kMediumNormal:
			aiNewState(actor, NAME_cultistSearch);
			break;
		case kMediumWater:
		case kMediumGoo:
			aiNewState(actor, NAME_cultistSwimSearch);
			break;
		}
		return;
	}
	if (nDist > 0 && nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, nAngle);
		double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDist < pDudeInfo->SeeDist() && nDeltaAngle <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());
				actor->dudeSlope = nDist == 0 ? 0 : (target->spr.pos.Z - actor->spr.pos.Z) / nDist;
				switch (actor->GetType()) 
				{
				case kDudeCultistTommy:
					if (nDist < 0x1e0 && nDist > 0xe0 && nDeltaAngle < DAngle15 && !TargetNearExplosion(target->sector())
						&& (target->spr.flags & 2) && gGameOptions.nDifficulty > 2 && target->IsPlayerActor() && getPlayer(target)->isRunning
						&& Chance(0x8000))
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistTThrow);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistTThrow);
							break;
						default:
							aiNewState(actor, NAME_cultistTThrow);
							break;
						}
					}
					else if (nDist < 0x460 && nDeltaAngle < DAngle1 * 5)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTProneFire);
							else if (dudeIsPlayingSeq(actor, 13) && (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo))
								aiNewState(actor, NAME_cultistTSwimFire);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun)
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistTFire);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistTProneFire);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistTSwimFire);
							}
							else
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistDodge);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistProneDodge);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSwimDodge);
							}
							break;
						default:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistTSwimFire);
							break;
						}
					}
					break;
				case kDudeCultistShotgun:
					if (nDist < 0x2c0 && nDist > 0x140 && !TargetNearExplosion(target->sector())
						&& (target->spr.flags & 2) && gGameOptions.nDifficulty >= 2 && target->IsPlayerActor() && !getPlayer(target)->isRunning
						&& Chance(0x8000))
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistSThrow);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistSThrow);
							break;
						default:
							aiNewState(actor, NAME_cultistSThrow);
							break;
						}
					}
					else if (nDist < 0x320 && nDeltaAngle < DAngle1 * 5)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistSSwimFire);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistTommy)
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistSFire);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistSProneFire);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSSwimFire);
							}
							else
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistDodge);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistProneDodge);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSwimDodge);
							}
							break;
						default:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistSSwimFire);
							break;
						}
					}
					break;
				case kDudeCultistTesla:
					if (nDist < 0x1e0 && nDist > 0xe0 && !TargetNearExplosion(target->sector())
						&& (target->spr.flags & 2) && gGameOptions.nDifficulty > 2 && target->IsPlayerActor() && getPlayer(target)->isRunning
						&& Chance(0x8000))
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistTsThrow);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistTsThrow);
							break;
						default:
							aiNewState(actor, NAME_cultistTsThrow);
							break;
						}
					}
					else if (nDist < 0x320 && nDeltaAngle < DAngle1 * 5)
					{
						FName pCultistTsProneFire = !cl_bloodvanillaenemies && !VanillaMode() ? NAME_cultistTsProneFireFixed : NAME_cultistTsProneFire;
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTsFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, pCultistTsProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistTsSwimFire);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistTommy)
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistTsFire);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, pCultistTsProneFire);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistTsSwimFire);
							}
							else
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistDodge);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistProneDodge);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSwimDodge);
							}
							break;
						default:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistTsFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, pCultistTsProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistTsSwimFire);
							break;
						}
					}
					break;
				case kDudeCultistTNT:
					if (nDist < 0x2c0 && nDist > 0x140 && nDeltaAngle < DAngle15 && (target->spr.flags & 2) && target->IsPlayerActor())
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistDThrow);
							break;
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistDThrow);
							break;
						default:
							aiNewState(actor, NAME_cultistDThrow);
							break;
						}
					}
					else if (nDist < 0x140 && nDeltaAngle < DAngle15 && (target->spr.flags & 2) && target->IsPlayerActor())
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != 1 && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistDThrow2);
							break;
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistDThrow2);
							break;
						default:
							aiNewState(actor, NAME_cultistDThrow2);
							break;
						}
					}
					break;
				case kDudeCultistBeast:
					if (nDist < 0x1e0 && nDist > 0xe0 && !TargetNearExplosion(target->sector())
						&& (target->spr.flags & 2) && gGameOptions.nDifficulty > 2 && target->IsPlayerActor() && getPlayer(target)->isRunning
						&& Chance(0x8000))
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistSThrow);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistShotgun && actor->xspr.medium != kMediumWater && actor->xspr.medium != kMediumGoo)
								aiNewState(actor, NAME_cultistSThrow);
							break;
						default:
							aiNewState(actor, NAME_cultistSThrow);
							break;
						}
					}
					else if (nDist < 0x320 && abs(nDeltaAngle) < DAngle1 * 5)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec.XY(), 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistSSwimFire);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeCultistTommy)
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistSFire);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistSProneFire);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSSwimFire);
							}
							else
							{
								if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistDodge);
								else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
									aiNewState(actor, NAME_cultistProneDodge);
								else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
									aiNewState(actor, NAME_cultistSwimDodge);
							}
							break;
						default:
							if (!dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSFire);
							else if (dudeIsPlayingSeq(actor, 14) && actor->xspr.medium == kMediumNormal)
								aiNewState(actor, NAME_cultistSProneFire);
							else if (actor->xspr.medium == kMediumWater || actor->xspr.medium == kMediumGoo)
								aiNewState(actor, NAME_cultistSSwimFire);
							break;
						}
					}
					break;
				}
				return;
			}
		}
	}
	switch (actor->xspr.medium)
	{
	case kMediumNormal:
		aiNewState(actor, NAME_cultistGoto);
		break;
	case kMediumWater:
	case kMediumGoo:
		aiNewState(actor, NAME_cultistSwimGoto);
		break;
	}
	actor->SetTarget(nullptr);
}

END_BLD_NS
