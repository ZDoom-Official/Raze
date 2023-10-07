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


static void playStatueBreakSnd(DBloodActor* actor) {

	aiPlay3DSound(actor, 313, AI_SFX_PRIORITY_1, -1);
}

void SlashFSeqCallback(DBloodActor* actor)
{
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	DUDEINFO* pDudeInfoT = getDudeInfo(target);
	double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
	double height2 = (pDudeInfoT->eyeHeight * target->spr.scale.Y);
	DVector3 vec(actor->spr.Angles.Yaw.ToVector() * 64, height - height2);

	actFireVector(actor, 0, 0, vec, kVectorGargSlash);
	double r1 = RandomF(50, 8);
	double r2 = RandomF(50, 8);
	actFireVector(actor, 0, 0, vec + DVector2(r1, r2), kVectorGargSlash);
	r1 = RandomF(50, 8);
	r2 = RandomF(50, 8);
	actFireVector(actor, 0, 0, vec + DVector2(r1, r2), kVectorGargSlash);
}

void ThrowFSeqCallback(DBloodActor* actor)
{
	actFireThing(actor, 0., 0., actor->dudeSlope * 0.25 - 0.11444, kThingBone, 0xeeeee / 65536.);
}

void BlastSSeqCallback(DBloodActor* actor)
{
	wrand(); // ???
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	double height = (actor->spr.scale.Y * getDudeInfo(actor)->eyeHeight);
	DVector3 pos(actor->spr.pos.XY(), height);

	DVector3 Aim(actor->spr.Angles.Yaw.ToVector(), actor->dudeSlope);
	double nClosest = 0x7fffffff;

	BloodStatIterator it(kStatDude);
	while (auto actor2 = it.Next())
	{
		if (actor == actor2 || !(actor2->spr.flags & 8))
			continue;

		auto pos2 = actor2->spr.pos;
		double nDist = (pos2 - pos).Length();
		if (nDist == 0 || nDist > 0x280)
			continue;

		pos2 += actor->vel * nDist * (65536. / 0x1aaaaa);

		DVector3 tvec = pos;
		tvec.XY() += actor->spr.Angles.Yaw.ToVector() * nDist;
		tvec.Z += actor->dudeSlope * nDist;

		double tsr = nDist * 9.23828125;
		double top, bottom;
		GetActorExtents(actor2, &top, &bottom);
		if (tvec.Z - tsr > bottom || tvec.Z + tsr < top)
			continue;

		double nDist2 = (tvec - pos2).Length();
		if (nDist2 < nClosest)
		{
			DAngle nAngle = (pos2.XY() - pos.XY()).Angle();
			DAngle nDeltaAngle = absangle(nAngle, actor->spr.Angles.Yaw);
			if (nDeltaAngle <= DAngle45)
			{
				double tz1 = actor2->spr.pos.Z - actor->spr.pos.Z;

				if (cansee(pos, actor->sector(), pos2, actor2->sector()))
				{
					nClosest = nDist2;
					Aim.XY() = nAngle.ToVector();
					Aim.Z = tz1 / nDist;

					// This does not make any sense...
					if (tz1 < -3.2 && tz1 > -48)
						Aim.Z += 0.5774;
					else if (tz1 <= -48)
						Aim.Z -= 0.45776;
				}
				else
					Aim.Z = tz1 / nDist;
			}
		}
	}
	// allow to fire missile in non-player targets
	if (target->IsPlayerActor() || currentLevel->featureflags & kFeatureEnemyAttacks)
	{
		actFireMissile(actor, -7.5, 0, Aim, kMissileArcGargoyle);
		actFireMissile(actor, 7.5, 0, Aim, kMissileArcGargoyle);
	}

}

void ThrowSSeqCallback(DBloodActor* actor)
{
	actFireThing(actor, 0., 0., actor->dudeSlope * 0.25 - 0.11444, kThingBone, Chance(0x6000) ? 19.2 : 17.066666);
}

static void gargThinkTarget(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	
	if (actor->dudeExtra.active && actor->dudeExtra.thinkTime < 10)
		actor->dudeExtra.thinkTime++;
	else if (actor->dudeExtra.thinkTime >= 10 && actor->dudeExtra.active)
	{
		actor->xspr.goalAng += DAngle45;
		aiSetTarget(actor, actor->basePoint);
		aiNewState(actor, NAME_gargoyleTurn);
		return;
	}
	if (Chance(pDudeInfo->alertChance))
	{
		for (int p = connecthead; p >= 0; p = connectpoint2[p])
		{
			DBloodPlayer* pPlayer = getPlayer(p);
			if (pPlayer->GetActor()->xspr.health == 0 || powerupCheck(pPlayer, kPwUpShadowCloak) > 0)
				continue;
			auto ppos = pPlayer->GetActor()->spr.pos;
			auto dvect = ppos.XY() - actor->spr.pos;
			auto pSector = pPlayer->GetActor()->sector();
			double nDist = dvect.Length();
			if (nDist > pDudeInfo->SeeDist() && nDist > pDudeInfo->HearDist())
				continue;
			double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
			if (!cansee(ppos, pSector, actor->spr.pos.plusZ(-height), actor->sector()))
				continue;
			DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, dvect.Angle());
			if (nDist < pDudeInfo->SeeDist() && nDeltaAngle <= pDudeInfo->Periphery())
			{
				actor->dudeExtra.thinkTime = 0;
				aiSetTarget(actor, pPlayer->GetActor());
				aiActivateDude(actor);
			}
			else if (nDist < pDudeInfo->HearDist())
			{
				actor->dudeExtra.thinkTime = 0;
				aiSetTarget(actor, ppos);
				aiActivateDude(actor);
			}
			else
				continue;
			break;
		}
	}
}

static void gargThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiLookForTarget(actor);
}

static void gargThinkGoto(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (nDist < 32 && absangle(actor->spr.Angles.Yaw, nAngle) < pDudeInfo->Periphery())
		aiNewState(actor, NAME_gargoyleFSearch);
	aiThinkTarget(actor);
}

static void gargMoveDodgeUp(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	AdjustVelocity(actor, ADJUSTER{
		if (actor->xspr.dodgeDir > 0)
			t2 += FixedToFloat(pDudeInfo->sideSpeed);
		else
			t2 -= FixedToFloat(pDudeInfo->sideSpeed);
	});

	actor->vel.Z = FixedToFloat(-0x1d555);
}

static void gargMoveDodgeDown(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	if (actor->xspr.dodgeDir == 0)
		return;
	AdjustVelocity(actor, ADJUSTER{
		if (actor->xspr.dodgeDir > 0)
			t2 += FixedToFloat(pDudeInfo->sideSpeed);
		else
			t2 -= FixedToFloat(pDudeInfo->sideSpeed);
	});

	actor->vel.Z = 4.26666;
}

static void gargThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		aiNewState(actor, NAME_gargoyleFGoto);
		return;
	}
	///assert(actor->IsDudeActor());
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto target = actor->GetTarget();

	DVector2 dxy = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle dxyAngle = dxy.Angle();
	aiChooseDirection(actor, dxyAngle);
	if (target->xspr.health == 0)
	{
		aiNewState(actor, NAME_gargoyleFSearch);
		return;
	}
	if (target->IsPlayerActor() && powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0)
	{
		aiNewState(actor, NAME_gargoyleFSearch);
		return;
	}
	double nDist = dxy.Length();
	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, dxyAngle);
		double height = pDudeInfo->eyeHeight * actor->spr.scale.Y;
		// Should be dudeInfo[target]
		double height2 = pDudeInfo->eyeHeight * target->spr.scale.Y;
		double top, bottom;
		GetActorExtents(actor, &top, &bottom);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDist < pDudeInfo->SeeDist() && nDeltaAngle <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());
				double floorZ = getflorzofslopeptr(actor->sector(), actor->spr.pos);
				double floorDelta = floorZ - bottom;
				double heightDelta = height2 - height;
				bool angWithinRange = nDeltaAngle < DAngle15;
				switch (actor->GetType())
				{
				case kDudeGargoyleFlesh:
					if (nDist < 0x180 && nDist > 0xc0 && angWithinRange)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dxy, 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							sfxPlay3DSound(actor, 1408, 0, 0);
							aiNewState(actor, NAME_gargoyleFThrow);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeGargoyleStone)
							{
								sfxPlay3DSound(actor, 1408, 0, 0);
								aiNewState(actor, NAME_gargoyleFThrow);
							}
							break;
						default:
							sfxPlay3DSound(actor, 1408, 0, 0);
							aiNewState(actor, NAME_gargoyleFThrow);
							break;
						}
					}
					else if (nDist < 0x40 && angWithinRange)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dxy, 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							sfxPlay3DSound(actor, 1406, 0, 0);
							aiNewState(actor, NAME_gargoyleFSlash);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeGargoyleStone)
							{
								sfxPlay3DSound(actor, 1406, 0, 0);
								aiNewState(actor, NAME_gargoyleFSlash);
							}
							break;
						default:
							sfxPlay3DSound(actor, 1406, 0, 0);
							aiNewState(actor, NAME_gargoyleFSlash);
							break;
						}
					}
					else if ((heightDelta > 32 || floorDelta > 32) && nDist < 0x140 && nDist > 0xa0)
					{
						aiPlay3DSound(actor, 1400, AI_SFX_PRIORITY_1, -1);
						aiNewState(actor, NAME_gargoyleSwoop);
					}
					else if ((heightDelta < 32 || floorDelta < 32) && angWithinRange)
						aiPlay3DSound(actor, 1400, AI_SFX_PRIORITY_1, -1);
					break;
				case kDudeGargoyleStone:
					if (nDist < 0x180 && nDist > 0xc0 && angWithinRange)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dxy, 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							sfxPlay3DSound(actor, 1457, 0, 0);
							aiNewState(actor, NAME_gargoyleSBlast);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeGargoyleFlesh)
							{
								sfxPlay3DSound(actor, 1457, 0, 0);
								aiNewState(actor, NAME_gargoyleSBlast);
							}
							break;
						default:
							sfxPlay3DSound(actor, 1457, 0, 0);
							aiNewState(actor, NAME_gargoyleSBlast);
							break;
						}
					}
					else if (nDist < 0x40 && angWithinRange)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dxy, 0), CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							aiNewState(actor, NAME_gargoyleFSlash);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeGargoyleFlesh)
								aiNewState(actor, NAME_gargoyleFSlash);
							break;
						default:
							aiNewState(actor, NAME_gargoyleFSlash);
							break;
						}
					}
					else if ((heightDelta > 32 || floorDelta > 32) && nDist < 0x140 && nDist > 0x80)
					{
						if (actor->GetType() == kDudeGargoyleFlesh)
							aiPlay3DSound(actor, 1400, AI_SFX_PRIORITY_1, -1);
						else
							aiPlay3DSound(actor, 1450, AI_SFX_PRIORITY_1, -1);
						aiNewState(actor, NAME_gargoyleSwoop);
					}
					else if ((heightDelta < 32 || floorDelta < 32) && angWithinRange)
						aiPlay3DSound(actor, 1450, AI_SFX_PRIORITY_1, -1);
					break;
				}
			}
			return;
		}
		else
		{
			aiNewState(actor, NAME_gargoyleFly);
			return;
		}
	}

	aiNewState(actor, NAME_gargoyleFGoto);
	actor->SetTarget(nullptr);
}

static void entryFStatue(DBloodActor* actor)
{
	DUDEINFO* pDudeInfo = &dudeInfo[6];
	actHealDude(actor, pDudeInfo->startHealth, pDudeInfo->startHealth);
	actor->ChangeType(kDudeGargoyleFlesh);
}

static void entrySStatue(DBloodActor* actor)
{
	DUDEINFO* pDudeInfo = &dudeInfo[7];
	actHealDude(actor, pDudeInfo->startHealth, pDudeInfo->startHealth);
	actor->ChangeType(kDudeGargoyleStone);
}

static void gargMoveForward(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
		return;
	if (actor->GetTarget() == nullptr)
		actor->spr.Angles.Yaw += DAngle45;
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	double nDist = dvec.Length();
	if ((unsigned int)Random(64) < 32 && nDist <= 0x40)
		return;
	AdjustVelocity(actor, ADJUSTER{
		if (actor->GetTarget() == nullptr)
			t1 += nAccel;
		else
			t1 += nAccel * 0.5;
	});

}

static void gargMoveSlow(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
	{
		actor->xspr.goalAng += DAngle90;
		return;
	}
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	double nDist = dvec.Length();
	if (Chance(0x600) && nDist <= 0x40)
		return;
	AdjustVelocity(actor, ADJUSTER{
		t1 += nAccel * 0.5;
		t2 *= 0.5;
	});

	switch (actor->GetType()) {
	case kDudeGargoyleFlesh:
		actor->vel.Z = 4.26666;
		break;
	case kDudeGargoyleStone:
		actor->vel.Z = FixedToFloat(0x35555);
		break;
	}
}

static void gargMoveSwoop(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
	{
		actor->xspr.goalAng += DAngle90;
		return;
	}
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	double nDist = dvec.Length();
	if (Chance(0x600) && nDist <= 0x40)
		return;

	AdjustVelocity(actor, ADJUSTER{
		t1 += nAccel * 0.5;
		switch (actor->GetType()) {
		case kDudeGargoyleFlesh:
			actor->vel.Z = t1;
			break;
		case kDudeGargoyleStone:
			actor->vel.Z = t1;
			break;
		}
	});

}

static void gargMoveFly(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto nAng = deltaangle(actor->spr.Angles.Yaw, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.Angles.Yaw += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
	{
		actor->spr.Angles.Yaw += DAngle90;
		return;
	}
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	double nDist = dvec.Length();
	if (Chance(0x4000) && nDist <= 0x40)
		return;
	
	AdjustVelocity(actor, ADJUSTER{
		t1 += nAccel * 0.5;
		switch (actor->GetType()) {
		case kDudeGargoyleFlesh:
			actor->vel.Z = -t1;
			break;
		case kDudeGargoyleStone:
			actor->vel.Z = -t1;
			break;
		}
	});

}

END_BLD_NS
