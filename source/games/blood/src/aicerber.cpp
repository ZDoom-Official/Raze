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

static void cerberusThinkSearch(DBloodActor* actor);
static void cerberusThinkTarget(DBloodActor* actor);
static void cerberusThinkGoto(DBloodActor* actor);
static void cerberusThinkChase(DBloodActor* actor);


AISTATE cerberusIdle = { kAiStateIdle, 0, nullptr, 0, NULL, NULL, &AF(cerberusThinkTarget), NULL };
AISTATE cerberusSearch = { kAiStateSearch, 7, nullptr, 1800, NULL, &AF(aiMoveForward), &AF(cerberusThinkSearch), &cerberusIdle };
AISTATE cerberusChase = { kAiStateChase, 7, nullptr, 0, NULL, &AF(aiMoveForward), &AF(cerberusThinkChase), NULL };
AISTATE cerberusRecoil = { kAiStateRecoil, 5, nullptr, 0, NULL, NULL, NULL, &cerberusSearch };
AISTATE cerberusTeslaRecoil = { kAiStateRecoil, 4, nullptr, 0, NULL, NULL, NULL, &cerberusSearch };
AISTATE cerberusGoto = { kAiStateMove, 7, nullptr, 600, NULL, &AF(aiMoveForward), &AF(cerberusThinkGoto), &cerberusIdle };
AISTATE cerberusBite = { kAiStateChase, 6, &AF(cerberusBiteSeqCallback), 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberusBurn = { kAiStateChase, 6, &AF(cerberusBurnSeqCallback), 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberus3Burn = { kAiStateChase, 6, &AF(cerberusBurnSeqCallback2), 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberus2Idle = { kAiStateIdle, 0, nullptr, 0, NULL, NULL, &AF(cerberusThinkTarget), NULL };
AISTATE cerberus2Search = { kAiStateSearch, 7, nullptr, 1800, NULL, &AF(aiMoveForward), &AF(cerberusThinkSearch), &cerberus2Idle };
AISTATE cerberus2Chase = { kAiStateChase, 7, nullptr, 0, NULL, &AF(aiMoveForward), &AF(cerberusThinkChase), NULL };
AISTATE cerberus2Recoil = { kAiStateRecoil, 5, nullptr, 0, NULL, NULL, NULL, &cerberus2Search };
AISTATE cerberus2Goto = { kAiStateMove, 7, nullptr, 600, NULL, &AF(aiMoveForward), &AF(cerberusThinkGoto), &cerberus2Idle };
AISTATE cerberus2Bite = { kAiStateChase, 6, &AF(cerberusBiteSeqCallback), 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberus2Burn = { kAiStateChase, 6, &AF(cerberusBurnSeqCallback), 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberus4Burn = { kAiStateChase, 6, &AF(cerberusBurnSeqCallback2), 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberusTurn1 = { kAiStateOther, 7, nullptr, 120, NULL, &AF(aiMoveTurn), NULL, &cerberusChase };
AISTATE cerberusTurn2 = { kAiStateOther, 7, nullptr, 120, NULL, &AF(aiMoveTurn), NULL, &cerberusChase };

static constexpr double Cerberus_XYOff = 350. / 16;
static constexpr double Cerberus_ZOff = 100. / 256;

void cerberusBiteSeqCallback(DBloodActor* actor)
{
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();

	DVector3 vec;
	vec.XY() = actor->spr.Angles.Yaw.ToVector() * 64;
	vec.Z = target->spr.pos.Z - actor->spr.pos.Z;
	actFireVector(actor, Cerberus_XYOff, -Cerberus_ZOff, vec, kVectorCerberusHack);
	actFireVector(actor, -Cerberus_XYOff, 0, vec, kVectorCerberusHack);
	actFireVector(actor, 0, 0, vec, kVectorCerberusHack);
}

void cerberusBurnSeqCallback(DBloodActor* actor)
{
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	double height = pDudeInfo->eyeHeight * actor->spr.scale.Y * 0.25;
	if (!actor->ValidateTarget(__FUNCTION__)) return;

	DVector3 pos(actor->spr.pos.XY(), height);
	//auto pos = actor->spr.pos.plusZ(height); //  what it probably should be

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
				}
				else
					Aim.Z = tz1 / 64.;
			}
		}
	}
	switch (actor->GetType()) {
	case kDudeCerberusTwoHead:
		actFireMissile(actor, -Cerberus_XYOff, 0, Aim, kMissileFireballCerberus);
		actFireMissile(actor, Cerberus_XYOff, -Cerberus_ZOff, Aim, kMissileFireballCerberus);
		break;
	case kDudeCerberusOneHead:
		actFireMissile(actor, Cerberus_XYOff, -Cerberus_ZOff, Aim, kMissileFireballCerberus);
		break;
	}
}

void cerberusBurnSeqCallback2(DBloodActor* actor)
{
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	DUDEINFO* pDudeInfo = getDudeInfo(actor);
	double height = pDudeInfo->eyeHeight * actor->spr.scale.Y * 0.25;

	DVector3 pos(actor->spr.pos.XY(), height);
	//auto pos = actor->spr.pos.plusZ(height); //  what it probably should be

	DVector3 Aim(actor->spr.Angles.Yaw.ToVector(), actor->dudeSlope);
	DVector3 Aim2(Aim.XY(), 0);

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
				DUDEINFO* pDudeInfo2 = getDudeInfo(actor2);
				double height1 = (pDudeInfo2->aimHeight * actor2->spr.scale.Y);
				double tz1 = actor2->spr.pos.Z - height - actor->spr.pos.Z;

				if (cansee(pos, actor->sector(), pos2, actor2->sector()))
				{
					nClosest = nDist2;
					Aim.XY() = nAngle.ToVector();
					Aim.Z = tz1 / nDist;
				}
				else
					Aim.Z = tz1 / 64.;
			}
		}
	}
	switch (actor->GetType()) {

	case kDudeCerberusTwoHead:
		actFireMissile(actor, Cerberus_XYOff, -Cerberus_ZOff, Aim, kMissileFlameHound);
		actFireMissile(actor, -Cerberus_XYOff, 0, Aim2, kMissileFlameHound);
		break;
	case kDudeCerberusOneHead:
		actFireMissile(actor, Cerberus_XYOff, -Cerberus_ZOff, Aim, kMissileFlameHound);
		break;
	}
}

static void cerberusThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiThinkTarget(actor);
}

static void cerberusThinkTarget(DBloodActor* actor)
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
		if (actor->GetType() == kDudeCerberusTwoHead)
			aiNewState(actor, NAME_cerberusTurn1);
		else
			aiNewState(actor, NAME_cerberusTurn2);
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

static void cerberusThinkGoto(DBloodActor* actor)
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
	{
		switch (actor->GetType()) {
		case kDudeCerberusTwoHead:
			aiNewState(actor, NAME_cerberusSearch);
			break;
		case kDudeCerberusOneHead:
			aiNewState(actor, NAME_cerberus2Search);
			break;
		}
	}
	aiThinkTarget(actor);
}

static void cerberusThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr) {
		switch (actor->GetType()) {
		case kDudeCerberusTwoHead:
			aiNewState(actor, NAME_cerberusGoto);
			break;
		case kDudeCerberusOneHead:
			aiNewState(actor, NAME_cerberus2Goto);
			break;
		}
		return;
	}

	///assert(actor->IsDudeActor());
	if (!(actor->IsDudeActor())) {
		Printf(PRINT_HIGH, "actor->IsDudeActor()");
		return;
	}

	DUDEINFO* pDudeInfo = getDudeInfo(actor);

	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();

	auto dvec = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle nAngle = dvec.Angle();
	double nDist = dvec.Length();
	aiChooseDirection(actor, nAngle);

	if (target->xspr.health == 0) {
		switch (actor->GetType()) {
		case kDudeCerberusTwoHead:
			aiNewState(actor, NAME_cerberusSearch);
			break;
		case kDudeCerberusOneHead:
			aiNewState(actor, NAME_cerberus2Search);
			break;
		}
		return;
	}

	if (target->IsPlayerActor() && powerupCheck(getPlayer(target), kPwUpShadowCloak) > 0) {
		switch (actor->GetType()) {
		case kDudeCerberusTwoHead:
			aiNewState(actor, NAME_cerberusSearch);
			break;
		case kDudeCerberusOneHead:
			aiNewState(actor, NAME_cerberus2Search);
			break;
		}
		return;
	}


	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.Angles.Yaw, nAngle);
		double height = (pDudeInfo->eyeHeight * actor->spr.scale.Y);
		if (cansee(target->spr.pos, target->sector(), actor->spr.pos.plusZ(-height), actor->sector()))
		{
			if (nDist < pDudeInfo->SeeDist() && nDeltaAngle <= pDudeInfo->Periphery())
			{
				aiSetTarget(actor, actor->GetTarget());

				if (nDist < 0x1b0 && nDist > 0xd0 && nDeltaAngle < DAngle15) 
				{
					switch (actor->GetType()) {
					case kDudeCerberusTwoHead:
						aiNewState(actor, NAME_cerberusBurn);
						break;
					case kDudeCerberusOneHead:
						aiNewState(actor, NAME_cerberus2Burn);
						break;
					}
				}

				else if (nDist < 0xb0 && nDist > 0x50 && nDeltaAngle < DAngle15)
				{
					switch (actor->GetType()) {
					case kDudeCerberusTwoHead:
						aiNewState(actor, NAME_cerberus3Burn);
						break;
					case kDudeCerberusOneHead:
						aiNewState(actor, NAME_cerberus4Burn);
						break;
					}
				}
				else if (nDist < 0x20 && nDeltaAngle < DAngle15)
				{
					int hit = HitScan(actor, actor->spr.pos.Z, DVector3(dvec, 0), CLIPMASK1, 0);
					switch (actor->GetType()) {
					case kDudeCerberusTwoHead:
						switch (hit) {
						case -1:
							aiNewState(actor, NAME_cerberusBite);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeHellHound)
								aiNewState(actor, NAME_cerberusBite);
							break;
						case 0:
						case 4:
							break;
						default:
							aiNewState(actor, NAME_cerberusBite);
							break;
						}
						break;
					case kDudeCerberusOneHead:
						switch (hit) {
						case -1:
							aiNewState(actor, NAME_cerberus2Bite);
							break;
						case 3:
							if (actor->GetType() != gHitInfo.actor()->GetType() && gHitInfo.actor()->GetType() != kDudeHellHound)
								aiNewState(actor, NAME_cerberus2Bite);
							break;
						case 0:
						case 4:
							break;
						default:
							aiNewState(actor, NAME_cerberus2Bite);
							break;
						}
						break;
					}
				}
				return;
			}
		}
	}

	switch (actor->GetType()) {
	case kDudeCerberusTwoHead:
		aiNewState(actor, NAME_cerberusGoto);
		break;
	case kDudeCerberusOneHead:
		aiNewState(actor, NAME_cerberus2Goto);
		break;
	}
	actor->SetTarget(nullptr);
}

END_BLD_NS
