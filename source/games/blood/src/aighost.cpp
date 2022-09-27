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

static void ghostThinkTarget(DBloodActor*);
static void ghostThinkSearch(DBloodActor*);
static void ghostThinkGoto(DBloodActor*);
static void ghostMoveDodgeUp(DBloodActor*);
static void ghostMoveDodgeDown(DBloodActor*);
static void ghostThinkChase(DBloodActor*);
static void ghostMoveForward(DBloodActor*);
static void ghostMoveSlow(DBloodActor*);
static void ghostMoveSwoop(DBloodActor*);
static void ghostMoveFly(DBloodActor*);


AISTATE ghostIdle = { kAiStateIdle, 0, -1, 0, NULL, NULL, ghostThinkTarget, NULL };
AISTATE ghostChase = { kAiStateChase, 0, -1, 0, NULL, ghostMoveForward, ghostThinkChase, &ghostIdle };
AISTATE ghostGoto = { kAiStateMove, 0, -1, 600, NULL, ghostMoveForward, ghostThinkGoto, &ghostIdle };
AISTATE ghostSlash = { kAiStateChase, 6, nGhostSlashClient, 120, NULL, NULL, NULL, &ghostChase };
AISTATE ghostThrow = { kAiStateChase, 6, nGhostThrowClient, 120, NULL, NULL, NULL, &ghostChase };
AISTATE ghostBlast = { kAiStateChase, 6, nGhostBlastClient, 120, NULL, ghostMoveSlow, NULL, &ghostChase };
AISTATE ghostRecoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &ghostChase };
AISTATE ghostTeslaRecoil = { kAiStateRecoil, 4, -1, 0, NULL, NULL, NULL, &ghostChase };
AISTATE ghostSearch = { kAiStateSearch, 0, -1, 120, NULL, ghostMoveForward, ghostThinkSearch, &ghostIdle };
AISTATE ghostSwoop = { kAiStateOther, 0, -1, 120, NULL, ghostMoveSwoop, ghostThinkChase, &ghostChase };
AISTATE ghostFly = { kAiStateMove, 0, -1, 0, NULL, ghostMoveFly, ghostThinkChase, &ghostChase };
AISTATE ghostTurn = { kAiStateMove, 0, -1, 120, NULL, aiMoveTurn, NULL, &ghostChase };
AISTATE ghostDodgeUp = { kAiStateMove, 0, -1, 60, NULL, ghostMoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeUpRight = { kAiStateMove, 0, -1, 90, NULL, ghostMoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeUpLeft = { kAiStateMove, 0, -1, 90, NULL, ghostMoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeDown = { kAiStateMove, 0, -1, 120, NULL, ghostMoveDodgeDown, NULL, &ghostChase };
AISTATE ghostDodgeDownRight = { kAiStateMove, 0, -1, 90, NULL, ghostMoveDodgeDown, NULL, &ghostChase };
AISTATE ghostDodgeDownLeft = { kAiStateMove, 0, -1, 90, NULL, ghostMoveDodgeDown, NULL, &ghostChase };

void ghostSlashSeqCallback(int, DBloodActor* actor)
{
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	DUDEINFO* pDudeInfoT = getDudeInfo(target->spr.type);
	int height = (actor->spr.yrepeat * pDudeInfo->eyeHeight) << 2;
	int height2 = (target->spr.yrepeat * pDudeInfoT->eyeHeight) << 2;
	int dz = height - height2;
	int dx = bcos(actor->int_ang());
	int dy = bsin(actor->int_ang());
	sfxPlay3DSound(actor, 1406, 0, 0);
	actFireVector(actor, 0, 0, dx, dy, dz, kVectorGhost);
	int r1 = Random(50);
	int r2 = Random(50);
	actFireVector(actor, 0, 0, dx + r2, dy - r1, dz, kVectorGhost);
	r1 = Random(50);
	r2 = Random(50);
	actFireVector(actor, 0, 0, dx - r2, dy + r1, dz, kVectorGhost);
}

void ghostThrowSeqCallback(int, DBloodActor* actor)
{
	actFireThing(actor, 0, 0, actor->dudeSlope - 7500, kThingBone, 0xeeeee);
}

void ghostBlastSeqCallback(int, DBloodActor* actor)
{
	wrand(); // ???
	if (!actor->ValidateTarget(__FUNCTION__)) return;
	auto target = actor->GetTarget();
	int height = (actor->spr.yrepeat * getDudeInfo(actor->spr.type)->eyeHeight) << 2;
	int x = actor->int_pos().X;
	int y = actor->int_pos().Y;
	int z = height;
	TARGETTRACK tt = { 0x10000, 0x10000, 0x100, 0x55, 0x1aaaaa };
	Aim aim;
	aim.dx = bcos(actor->int_ang());
	aim.dy = bsin(actor->int_ang());
	aim.dz = actor->dudeSlope;
	int nClosest = 0x7fffffff;
	BloodStatIterator it(kStatDude);
	while (auto actor2 = it.Next())
	{
		if (actor == actor2 || !(actor2->spr.flags & 8))
			continue;
		int x2 = actor2->int_pos().X;
		int y2 = actor2->int_pos().Y;
		int z2 = actor2->int_pos().Z;
		int nDist = approxDist(x2 - x, y2 - y);
		if (nDist == 0 || nDist > 0x2800)
			continue;
		if (tt.at10)
		{
			int t = DivScale(nDist, tt.at10, 12);
			x2 += (actor->int_vel().X * t) >> 12;
			y2 += (actor->int_vel().Y * t) >> 12;
			z2 += (actor->int_vel().Z * t) >> 8;
		}
		int tx = x + MulScale(Cos(actor->int_ang()), nDist, 30);
		int ty = y + MulScale(Sin(actor->int_ang()), nDist, 30);
		int tz = z + MulScale(actor->dudeSlope, nDist, 10);
		int tsr = MulScale(9460, nDist, 10);
		int top, bottom;
		GetActorExtents(actor2, &top, &bottom);
		if (tz - tsr > bottom || tz + tsr < top)
			continue;
		int dx = (tx - x2) >> 4;
		int dy = (ty - y2) >> 4;
		int dz = (tz - z2) >> 8;
		int nDist2 = ksqrt(dx * dx + dy * dy + dz * dz);
		if (nDist2 < nClosest)
		{
			int nAngle = getangle(x2 - x, y2 - y);
			int nDeltaAngle = ((nAngle - actor->int_ang() + 1024) & 2047) - 1024;
			if (abs(nDeltaAngle) <= tt.at8)
			{
				int tz1 = actor2->int_pos().Z - actor->int_pos().Z;
				if (cansee(x, y, z, actor->sector(), x2, y2, z2, actor2->sector()))
				{
					nClosest = nDist2;
					aim.dx = bcos(nAngle);
					aim.dy = bsin(nAngle);
					aim.dz = DivScale(tz1, nDist, 10);
					if (tz1 > -0x333)
						aim.dz = DivScale(tz1, nDist, 10);
					else if (tz1 < -0x333 && tz1 > -0xb33)
						aim.dz = DivScale(tz1, nDist, 10) + 9460;
					else if (tz1 < -0xb33 && tz1 > -0x3000)
						aim.dz = DivScale(tz1, nDist, 10) + 9460;
					else if (tz1 < -0x3000)
						aim.dz = DivScale(tz1, nDist, 10) - 7500;
					else
						aim.dz = DivScale(tz1, nDist, 10);
				}
				else
					aim.dz = DivScale(tz1, nDist, 10);
			}
		}
	}
#ifdef NOONE_EXTENSIONS
	// allow fire missile in non-player targets if not a demo
	if (target->IsPlayerActor() || gModernMap) {
		sfxPlay3DSound(actor, 489, 0, 0);
		actFireMissile(actor, 0, 0, aim.dx, aim.dy, aim.dz, kMissileEctoSkull);
	}
#else
	if (target->IsPlayerActor()) {
		sfxPlay3DSound(actor, 489, 0, 0);
		actFireMissile(actor, 0, 0, aim.dx, aim.dy, aim.dz, kMissileEctoSkull);
	}
#endif
}

static void ghostThinkTarget(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	DUDEEXTRA_STATS* pDudeExtraE = &actor->dudeExtra.stats;
	if (pDudeExtraE->active && pDudeExtraE->thinkTime < 10)
		pDudeExtraE->thinkTime++;
	else if (pDudeExtraE->thinkTime >= 10 && pDudeExtraE->active)
	{
		actor->xspr.goalAng += DAngle45;
		aiSetTarget(actor, actor->basePoint);
		aiNewState(actor, &ghostTurn);
		return;
	}
	if (Chance(pDudeInfo->alertChance))
	{
		for (int p = connecthead; p >= 0; p = connectpoint2[p])
		{
			PLAYER* pPlayer = &gPlayer[p];
			if (pPlayer->actor->xspr.health == 0 || powerupCheck(pPlayer, kPwUpShadowCloak) > 0)
				continue;
			auto ppos = pPlayer->actor->spr.pos;
			auto dvect = ppos.XY() - actor->spr.pos;
			auto pSector = pPlayer->actor->sector();
			int nDist = approxDist(dvect);
			if (nDist > pDudeInfo->seeDist && nDist > pDudeInfo->hearDist)
				continue;
			double height = (pDudeInfo->eyeHeight * actor->spr.yrepeat) * REPEAT_SCALE;
			if (!cansee(ppos, pSector, actor->spr.pos.plusZ(-height), actor->sector()))
				continue;
			int nDeltaAngle = getincangle(actor->int_ang(), getangle(dvect));
			if (nDist < pDudeInfo->seeDist && abs(nDeltaAngle) <= pDudeInfo->periphery)
			{
				pDudeExtraE->thinkTime = 0;
				aiSetTarget(actor, pPlayer->actor);
				aiActivateDude(actor);
				return;
			}
			else if (nDist < pDudeInfo->hearDist)
			{
				pDudeExtraE->thinkTime = 0;
				aiSetTarget(actor, ppos);
				aiActivateDude(actor);
				return;
			}
		}
	}
}

static void ghostThinkSearch(DBloodActor* actor)
{
	aiChooseDirection(actor, actor->xspr.goalAng);
	aiThinkTarget(actor);
}

static void ghostThinkGoto(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	int nAngle = getangle(dvec);
	double nDist = dvec.Length();
	aiChooseDirection(actor, DAngle::fromBuild(nAngle));
	if (nDist < 32 && abs(actor->int_ang() - nAngle) < pDudeInfo->periphery)
		aiNewState(actor, &ghostSearch);
	aiThinkTarget(actor);
}

static void ghostMoveDodgeUp(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
	AdjustVelocity(actor, ADJUSTER{
		if (actor->xspr.dodgeDir > 0)
			t2 += FixedToFloat(pDudeInfo->sideSpeed);
		else
			t2 -= FixedToFloat(pDudeInfo->sideSpeed);
	});

	actor->vel.Z = FixedToFloat(-0x1d555);
}

static void ghostMoveDodgeDown(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
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

static void ghostThinkChase(DBloodActor* actor)
{
	if (actor->GetTarget() == nullptr)
	{
		aiNewState(actor, &ghostGoto);
		return;
	}
	///assert(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax);
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto target = actor->GetTarget();

	DVector2 dxy = target->spr.pos.XY() - actor->spr.pos.XY();
	DAngle dxyAngle = dxy.Angle();
	aiChooseDirection(actor, dxyAngle);
	if (target->xspr.health == 0)
	{
		aiNewState(actor, &ghostSearch);
		return;
	}
	if (target->IsPlayerActor() && powerupCheck(&gPlayer[target->spr.type - kDudePlayer1], kPwUpShadowCloak) > 0)
	{
		aiNewState(actor, &ghostSearch);
		return;
	}
	double nDist = dxy.Length();
	if (nDist <= pDudeInfo->SeeDist())
	{
		DAngle nDeltaAngle = absangle(actor->spr.angle, dxyAngle);
		double height = pDudeInfo->eyeHeight * actor->spr.yrepeat * REPEAT_SCALE;
		// Should be dudeInfo[target->spr.type-kDudeBase]
		double height2 = pDudeInfo->eyeHeight * target->spr.yrepeat * REPEAT_SCALE;
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
				switch (actor->spr.type) {
				case kDudePhantasm:
					if (nDist < 0x200 && nDist > 0x100 && angWithinRange) {
						int hit = HitScan(actor, actor->spr.pos.Z, dxy.X * worldtoint, dxy.Y * worldtoint, 0, CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							aiNewState(actor, &ghostBlast);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->spr.type != gHitInfo.actor()->spr.type && gHitInfo.actor()->spr.type != kDudePhantasm)
								aiNewState(actor, &ghostBlast);
							break;
						default:
							aiNewState(actor, &ghostBlast);
							break;
						}
					}
					else if (nDist < 0x40 && angWithinRange)
					{
						int hit = HitScan(actor, actor->spr.pos.Z, dxy.X * worldtoint, dxy.Y * worldtoint, 0, CLIPMASK1, 0);
						switch (hit)
						{
						case -1:
							aiNewState(actor, &ghostSlash);
							break;
						case 0:
						case 4:
							break;
						case 3:
							if (actor->spr.type != gHitInfo.actor()->spr.type && gHitInfo.actor()->spr.type != kDudePhantasm)
								aiNewState(actor, &ghostSlash);
							break;
						default:
							aiNewState(actor, &ghostSlash);
							break;
						}
					}
					else if ((heightDelta > 32 || floorDelta > 32) && nDist < 0x140 && nDist > 0x80)
					{
						aiPlay3DSound(actor, 1600, AI_SFX_PRIORITY_1, -1);
						aiNewState(actor, &ghostSwoop);
					}
					else if ((heightDelta < 32 || floorDelta < 32) &&angWithinRange)
						aiPlay3DSound(actor, 1600, AI_SFX_PRIORITY_1, -1);
					break;
				}
			}
			return;
		}
		else
		{
			aiNewState(actor, &ghostFly);
			return;
		}
	}

	aiNewState(actor, &ghostGoto);
	actor->SetTarget(nullptr);
}

static void ghostMoveForward(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
		return;
	if (actor->GetTarget() == nullptr)
		actor->spr.angle += DAngle45;
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

static void ghostMoveSlow(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
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
	switch (actor->spr.type) {
	case kDudePhantasm:
		actor->vel.Z = 4.26666;
		break;
	}
}

static void ghostMoveSwoop(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
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
		switch (actor->spr.type) {
		case kDudePhantasm:
			actor->vel.Z = t1;
			break;
		}
	});
}

static void ghostMoveFly(DBloodActor* actor)
{
	if (!(actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax)) {
		Printf(PRINT_HIGH, "actor->spr.type >= kDudeBase && actor->spr.type < kDudeMax");
		return;
	}
	DUDEINFO* pDudeInfo = getDudeInfo(actor->spr.type);
	auto nAng = deltaangle(actor->spr.angle, actor->xspr.goalAng);
	auto nTurnRange = pDudeInfo->TurnRange();
	actor->spr.angle += clamp(nAng, -nTurnRange, nTurnRange);
	double nAccel = pDudeInfo->FrontSpeed() * 4;
	if (abs(nAng) > DAngle60)
	{
		actor->spr.angle += DAngle90;
		return;
	}
	auto dvec = actor->xspr.TargetPos.XY() - actor->spr.pos.XY();
	double nDist = dvec.Length();
	if (Chance(0x4000) && nDist <= 0x40)
		return;
	AdjustVelocity(actor, ADJUSTER{
		t1 += nAccel * 0.5;
		switch (actor->spr.type) {
		case kDudePhantasm:
			actor->vel.Z = -t1;
			break;
		}
	});
}

END_BLD_NS
