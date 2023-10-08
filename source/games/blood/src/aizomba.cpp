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

void HackSeqCallback(DBloodActor* actor)
{
	auto target = actor->GetTarget();
	if (!target) return;
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	DUDEINFO* pDudeInfoT = getDudeInfo(target);
	DVector3 dv;
	dv.XY() = (actor->xspr.TargetPos.XY() - actor->spr.pos.XY()).Resized(64);

	double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
	double height2 = (pDudeInfoT->eyeHeight * target->spr.scale.Y);
	dv.Z = height - height2;

	sfxPlay3DSound(actor, 1101, 1, 0);
	actFireVector(actor, 0, 0, dv, kVectorAxe);
}

void StandSeqCallback(DBloodActor* actor)
{
	sfxPlay3DSound(actor, 1102, -1, 0);
}

static void zombaThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiLookForTarget(actor);
}

static void zombaThinkGoto(DBloodActor* actor)
{
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (nDist < 51.3125 && absangle(actor->spr.Angles.Yaw, nAngle) < pDudeInfo->Periphery())
		aiNewState(actor, NAME_zombieASearch);
	aiThinkTarget(actor);
}

static void zombaThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		aiNewState(actor, NAME_zombieASearch);
		return;
	}
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();

	auto dvec = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);

	if (target->xspr.health == 0)
	{
		aiNewState(actor, NAME_zombieASearch);
		return;
	}
	if (target->IsPlayerActor() && (powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0 || powerupCheck(getPlayer(target), kPwUpDeathMaskUseless) > 0))
	{
		aiNewState(actor, NAME_zombieAGoto);
		return;
	}
	// If the zombie gets whacked while rising from the grave it never executes this change and if it isn't done here at the very latest, will just aimlessly run around.
	if (!VanillaMode() && actor->GetType() == kDudeZombieAxeBuried)
		actor->ChangeType(kDudeZombieAxeNormal);


	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, nAngle);
		double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDeltaAngle <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());
				if (nDist < 0x40 && nDeltaAngle < DAngle15)
					aiNewState(actor, NAME_zombieAHack);
				return;
			}
		}
	}

	aiNewState(actor, NAME_zombieAGoto);
	actor->SetTarget(nullptr);
}

static void zombaThinkPonder(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		aiNewState(actor, NAME_zombieASearch);
		return;
	}
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();

	auto dvec = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (target->xspr.health == 0)
	{
		aiNewState(actor, NAME_zombieASearch);
		return;
	}
	if (target->IsPlayerActor() && (powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0 || powerupCheck(getPlayer(target), kPwUpDeathMaskUseless) > 0))
	{
		aiNewState(actor, NAME_zombieAGoto);
		return;
	}

	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, nAngle);
		double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDeltaAngle <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());
				if (nDist < 0x40)
				{
					if (nDeltaAngle < DAngle15)
					{
						sfxPlay3DSound(actor, 1101, 1, 0);
						aiNewState(actor, NAME_zombieAHack);
					}
					return;
				}
			}
		}
	}

	aiNewState(actor, NAME_zombieAChase);
}

static void myThinkTarget(DBloodActor* actor)
{
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	for (int p = connecthead; p >= 0; p = connectpoint2[p])
	{
		DBloodPlayer* pPlayer = getPlayer(p);
		auto owneractor = actor->GetOwner();
		if (owneractor == nullptr || owneractor == pPlayer->GetActor() || pPlayer->GetActor()->xspr.health == 0 || powerupCheck(pPlayer, kPwUpShadowCloak) > 0)
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
		if (nDist < pDudeInfo->SeeDist() && abs(nDeltaAngle) <= pDudeInfo->Periphery())
		{
			aiSetTarget(actor, pPlayer->GetActor());
			aiActivateDude(actor);
		}
		else if (nDist < pDudeInfo->HearDist())
		{
			aiSetTarget(actor, ppos);
			aiActivateDude(actor);
		}
		else
			continue;
		break;
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void myThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	myThinkTarget(actor);
}

static void entryEZombie(DBloodActor* actor)
{
	actor->ChangeType(kDudeZombieAxeNormal);
	actor->spr.flags |= 1;
}

static void entryAIdle(DBloodActor* actor)
{
	actor->SetTarget(nullptr);
}

static void entryEStand(DBloodActor* actor)
{
	sfxPlay3DSound(actor, 1100, -1, 0);
	actor->spr.Angles.Yaw = (actor->xspr.TargetPos - actor->spr.pos).Angle();
}

END_BLD_NS
