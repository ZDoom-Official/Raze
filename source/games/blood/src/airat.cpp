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



#include "blood.h"

BEGIN_BLD_NS

AISTATE ratIdle = { kAiStateIdle, 0, nullptr, 0, NULL, NULL, &AF(aiThinkTarget), NULL };
AISTATE ratSearch = { kAiStateSearch, 7, nullptr, 1800, NULL, &AF(aiMoveForward), &AF(ratThinkSearch), &ratIdle };
AISTATE ratChase = { kAiStateChase, 7, nullptr, 0, NULL, &AF(aiMoveForward), &AF(ratThinkChase), NULL };
AISTATE ratDodge = { kAiStateMove, 7, nullptr, 0, NULL, NULL, NULL, &ratChase };
AISTATE ratRecoil = { kAiStateRecoil, 7, nullptr, 0, NULL, NULL, NULL, &ratDodge };
AISTATE ratGoto = { kAiStateMove, 7, nullptr, 600, NULL, &AF(aiMoveForward), &AF(ratThinkGoto), &ratIdle };
AISTATE ratBite = { kAiStateChase, 6, &AF(ratBiteSeqCallback), 120, NULL, NULL, NULL, &ratChase };

void ratBiteSeqCallback(DBloodActor* actor)
{
	assert(actor->IsDudeActor());
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	if (target->IsPlayerActor())
	{
		DVector3 vec(actor->spr.Angles.Yaw.ToVector() * 64, target->spr.pos.Z - actor->spr.pos.Z);
		actFireVector(actor, 0, 0, vec, kVectorRatBite);
	}
}

void ratThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiThinkTarget(actor);
}

void ratThinkGoto(DBloodActor* actor)
{
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (nDist < 32 && absangle(actor->spr.Angles.Yaw, nAngle) < pDudeInfo->Periphery())
		aiNewState(actor, &ratSearch);
	aiThinkTarget(actor);
}

void ratThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		aiNewState(actor, &ratGoto);
		return;
	}
	assert(actor->IsDudeActor());
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	auto target = actor->GetTarget();

	auto dvec = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);
	if (target->xspr.health == 0)
	{
		aiNewState(actor, &ratSearch);
		return;
	}
	if (target->IsPlayerActor() && powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0)
	{
		aiNewState(actor, &ratSearch);
		return;
	}

	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, nAngle);
		double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDist < pDudeInfo->SeeDist() && abs(nDeltaAngle) <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());
				if (nDist < 57.5625 && nDeltaAngle < DAngle15)
					aiNewState(actor, &ratBite);
				return;
			}
		}
	}

	aiNewState(actor, &ratGoto);
	actor->SetTarget(nullptr);
}

END_BLD_NS
