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
#pragma once

#include "common_game.h"
#include "actor.h"
#include "db.h"

BEGIN_BLD_NS


enum AI_SFX_PRIORITY {
	AI_SFX_PRIORITY_0 = 0,
	AI_SFX_PRIORITY_1,
	AI_SFX_PRIORITY_2,
};


struct TARGETTRACK {
	int TotalKills;
	int Kills;
	int at8; // view angle
	int atc;
	int at10; // Move predict
};

extern const int gCultTeslaFireChance[5];

bool dudeIsPlayingSeq(DBloodActor* pSprite, int nSeq);
void aiPlay3DSound(DBloodActor* pSprite, int a2, AI_SFX_PRIORITY a3, int a4);
void aiNewState(DBloodActor* actor, FName pAIState);
void aiNewState(DBloodActor* actor, FDefiningState* pAIState);
void aiChooseDirection(DBloodActor* actor, DAngle a3);
void aiMoveForward(DBloodActor*pXSprite);
void aiMoveTurn(DBloodActor*pXSprite);
void aiMoveDodge(DBloodActor *actor);
void aiActivateDude(DBloodActor *actor);
void aiSetTarget(DBloodActor* ac, const DVector3& pos);
void aiSetTarget(DBloodActor* actor, DBloodActor* target);
int aiDamageSprite(DBloodActor* source, DBloodActor* actor, DAMAGE_TYPE nDmgType, int nDamage);
void aiThinkTarget(DBloodActor* actor);
void aiLookForTarget(DBloodActor* actor);
void aiProcessDudes(void);
void aiInit(void);
void aiInitSprite(DBloodActor* pSprite);
bool CanMove(DBloodActor* pSprite, DBloodActor* target, DAngle nAngle, double nRange);

END_BLD_NS
