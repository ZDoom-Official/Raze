//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------
#include "ns.h"



#include "names2.h"
#include "panel.h"
#include "tags.h"
#include "ai.h"
#include "sprite.h"
#include "misc.h"
#include "weapon.h"

BEGIN_SW_NS

DECISION CoolieBattle[] =
{
    {700,   &AF(InitCoolieCharge    )     },
    {990,   &AF(InitActorMoveCloser )        },
    {1000,  &AF(InitActorSetDecide)},
    {1024,  &AF(InitActorRunAway    )       }
};

DECISION CoolieOffense[] =
{
    {700,   &AF(InitCoolieCharge    )     },
    {1015,  &AF(InitActorMoveCloser )        },
    {1024,  &AF(InitActorSetDecide) }
};

DECISIONB CoolieBroadcast[] =
{
    {16, attr_ambient          },
    {1024,  0 }
};

DECISION CoolieSurprised[] =
{
    {700,   &AF(InitActorMoveCloser  )       },
    {703,   &AF(InitActorSetDecide) },
    {1024,  &AF(InitActorDecide      )      }
};

DECISION CoolieEvasive[] =
{
    {10,   &AF(InitActorEvade)  },
    {1024, nullptr            }
};

DECISION CoolieLostTarget[] =
{
    {900,   &AF(InitActorFindPlayer  )       },
    {1024,  &AF(InitActorWanderAround)       }
};

DECISION CoolieCloseRange[] =
{
    {400,   &AF(InitCoolieCharge   )      },
    {1024,  &AF(InitActorReposition)            }
};

PERSONALITY CooliePersonality =
{
    CoolieBattle,
    CoolieOffense,
    CoolieBroadcast,
    CoolieSurprised,
    CoolieEvasive,
    CoolieLostTarget,
    CoolieCloseRange,
    CoolieCloseRange
};

ATTRIBUTE CoolieAttrib =
{
    {60, 80, 100, 200},                 // Speeds
    {3, 0, -2, -3},                     // Tic Adjusts
    3,                                 // MaxWeapons;
    {
        DIGI_COOLIEAMBIENT, DIGI_COOLIEALERT, DIGI_COOLIEALERT,
        DIGI_COOLIEPAIN, 0, DIGI_CGMATERIALIZE,
        DIGI_COOLIEEXPLODE,0,0,0
    }
};


//////////////////////
//
// COOLIE RUN
//
//////////////////////

#define COOLIE_RATE 12

FState s_CoolieRun[] =
{
        {SPR_COOLIE_RUN, 'A', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieRun[1]},
        {SPR_COOLIE_RUN, 'B', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieRun[2]},
        {SPR_COOLIE_RUN, 'C', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieRun[3]},
        {SPR_COOLIE_RUN, 'D', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieRun[0]}
};

//////////////////////
//
// COOLIE CHARGE
//
//////////////////////

#define COOLIE_RATE 12

FState s_CoolieCharge[] =
{
        {SPR_COOLIE_CHARGE, 'A', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieCharge[1]},
        {SPR_COOLIE_CHARGE, 'B', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieCharge[2]},
        {SPR_COOLIE_CHARGE, 'C', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieCharge[3]},
        {SPR_COOLIE_CHARGE, 'D', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieCharge[0]}
};

//////////////////////
//
// COOLIE STAND
//
//////////////////////

FState s_CoolieStand[] =
{
        {SPR_COOLIE_RUN, 'A', COOLIE_RATE, &AF(DoCoolieMove), &s_CoolieStand[0]}
};

//////////////////////
//
// COOLIE PAIN
//
//////////////////////

#define COOLIE_PAIN_RATE 60

FState s_CooliePain[] =
{
        {SPR_COOLIE_PAIN, 'A', COOLIE_PAIN_RATE, &AF(CooliePain), &s_CooliePain[0]},
};

//////////////////////
//
// COOLIE DIE
//
//////////////////////

#define COOLIE_DIE_RATE 30

FState s_CoolieDie[] =
{
    {SPR_COOLIE_DIE, 'A', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[1]},

    {SPR_COOLIE_DIE, 'A', 0|SF_QUICK_CALL, &AF(SpawnCoolieExp), &s_CoolieDie[2]},

    {SPR_COOLIE_DIE, 'B', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[3]},
    {SPR_COOLIE_DIE, 'C', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[4]},
    {SPR_COOLIE_DIE, 'D', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[5]},
    {SPR_COOLIE_DIE, 'E', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[6]},
    {SPR_COOLIE_DIE, 'F', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[7]},
    {SPR_COOLIE_DIE, 'G', COOLIE_DIE_RATE, &AF(NullCoolie), &s_CoolieDie[8]},
    {SPR_COOLIE_DIE, 'H', COOLIE_DIE_RATE, &AF(DoCoolieWaitBirth), &s_CoolieDie[8]},

    {SPR_COOLIE_DIE, 'H', COOLIE_DIE_RATE*5, &AF(DoActorDebris), &s_CoolieDie[10]},
    {SPR_COOLIE_DIE, 'H', 0|SF_QUICK_CALL, &AF(SpawnCoolg), &s_CoolieDie[11]},
    {SPR_COOLIE_DEAD_NOHEAD, 'A', SF_QUICK_CALL, &AF(QueueFloorBlood), &s_CoolieDie[12]},
    {SPR_COOLIE_DEAD_NOHEAD, 'A', COOLIE_DIE_RATE, &AF(DoActorDebris), &s_CoolieDie[12]}
};

FState s_CoolieDead[] =
{
    {SPR_COOLIE_DEAD, 'A', COOLIE_DIE_RATE, &AF(DoActorDebris), &s_CoolieDead[0]},
};

ACTOR_ACTION_SET CoolieActionSet =
{
    s_CoolieStand,
    s_CoolieRun,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr, //climb
    s_CooliePain, //pain
    s_CoolieDie,
    nullptr,
    s_CoolieDead,
    nullptr,
    nullptr,
    {s_CoolieCharge},
    {1024},
    {s_CoolieCharge},
    {1024},
    {nullptr},
    nullptr,
    nullptr
};

//---------------------------------------------------------------------------
//
// This is used by multiple enemies.
//
//---------------------------------------------------------------------------

void EnemyDefaults(DSWActor* actor, ACTOR_ACTION_SET* action, PERSONALITY* person)
{
    unsigned int wpn;
    int wpn_cnt;
    int depth = 0;

    switch (actor->user.ID)
    {
    case PACHINKO1:
    case PACHINKO2:
    case PACHINKO3:
    case PACHINKO4:
    case PACHINKOWINLIGHT:
    case TOILETGIRL_R0:
    case WASHGIRL_R0:
    case CARGIRL_R0:
    case MECHANICGIRL_R0:
    case SAILORGIRL_R0:
    case PRUNEGIRL_R0:
    case TRASHCAN:
    case BUNNY_RUN_R0:
        break;
    default:
    {
        Level.addKillCount();
    }

    break;
    }

    actor->spr.cstat &= ~(CSTAT_SPRITE_RESTORE);

    actor->user.spal = actor->spr.pal;

    actor->clipdist = 16;

    actor->user.zclip = 48;
    actor->user.lo_step = 32;

    actor->user.floor_dist = actor->user.zclip - actor->user.lo_step;
    actor->user.ceiling_dist = ActorSizeZ(actor) - actor->user.zclip;

    actor->user.Radius = 400;

    actor->user.MaxHealth = actor->user.Health;

    actor->user.PainThreshold = (actor->user.Health >> 4) - 1;

    actor->spr.cstat |= (CSTAT_SPRITE_BLOCK|CSTAT_SPRITE_BLOCK_HITSCAN);
    actor->spr.extra |= (SPRX_PLAYER_OR_ENEMY);

    //actor->setPicFromState();
    change_actor_stat(actor, STAT_ENEMY);

    actor->user.Personality = person;
    actor->user.__legacyState.ActorActionSet = action;

    DoActorZrange(actor);

    //KeepActorOnFloor(actor); // for swimming actors

    // make sure we start in the water if thats where we are
    if (actor->user.lo_sectp)
    {

        if (actor->user.lo_sectp->hasU() && (actor->user.lo_sectp->extra & SECTFX_SINK))
        {
            depth = FixedToInt(actor->user.lo_sectp->depth_fixed);
        }
        else
        {
            SWSectIterator it(actor->user.lo_sectp);
            while (auto itActor = it.Next())
            {
                if (itActor->spr.picnum == ST1 && itActor->spr.hitag == SECT_SINK)
                {
                    depth = itActor->spr.lotag;
                }
            }
        }
    }

    if (depth && abs(actor->spr.pos.Z - actor->user.loz) < 8)
    {
        actor->spr.pos.Z += depth;
        actor->user.loz = actor->spr.pos.Z;
        actor->backupz();
    }

    if (!action)
        return;

    actor->setStateGroup(NAME_Run);

    actor->setActionDecide();

    // find the number of long range attacks
    for (wpn = wpn_cnt = 0; wpn < SIZ(actor->user.__legacyState.ActorActionSet->Attack); wpn++)
    {
        if (actor->hasState(NAME_Attack, wpn))
            wpn_cnt++;
        else
            break;
    }

    // for actors this tells the number of weapons available
    // for player it tells the current weapon
    actor->user.WeaponNum = int8_t(wpn_cnt);
}

int SetupCoolie(DSWActor* actor)
{
    if (!(actor->spr.cstat & CSTAT_SPRITE_RESTORE))
    {
        SpawnUser(actor,COOLIE_RUN_R0,s_CoolieRun[0]);
        actor->user.Health = HEALTH_COOLIE;
    }

    actor->user.__legacyState.Attrib = &CoolieAttrib;
    DoActorSetSpeed(actor, NORM_SPEED);
    actor->user.__legacyState.StateEnd = s_CoolieDie;

    EnemyDefaults(actor, &CoolieActionSet, &CooliePersonality);
    actor->setStateGroup(NAME_Run);
    actor->setPicFromState();

    actor->spr.scale = DVector2(0.65625, 0.65625);

    actor->user.Flags |= (SPR_XFLIP_TOGGLE);

    return 0;
}

DEFINE_ACTION_FUNCTION(DSWCoolie, Initialize)
{
    PARAM_SELF_PROLOGUE(DSWActor);
    SetupCoolie(self);
    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int NewCoolg(DSWActor*);
int SpawnCoolg(DSWActor* actor)
{
    // Don't do a ghost every time
    if (RandomRange(1000) > 700 || Skill < MinEnemySkill - 1)
    {
        return(0);
    }

    NewCoolg(actor);
    PlaySpriteSound(actor,attr_extra1,v3df_follow);

    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int CooliePain(DSWActor* actor)
{
    if (actor->user.Flags & (SPR_SLIDING))
        DoActorSlide(actor);

    if (!(actor->user.Flags & SPR_CLIMBING))
        KeepActorOnFloor(actor);

    DoActorSectorDamage(actor);

    if ((actor->user.WaitTics -= ACTORMOVETICS) <= 0)
        InitActorDecide(actor);

    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int NullCoolie(DSWActor* actor)
{
    if (actor->user.Flags & (SPR_SLIDING))
        DoActorSlide(actor);

    if (!(actor->user.Flags & SPR_CLIMBING))
        KeepActorOnFloor(actor);

    DoActorSectorDamage(actor);

    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int DoCoolieMove(DSWActor* actor)
{
    if (actor->user.Flags & (SPR_SLIDING))
        DoActorSlide(actor);

    if (actor->user.track >= 0)
        ActorFollowTrack(actor, ACTORMOVETICS);
    else
        actor->callAction();

    KeepActorOnFloor(actor);

    if (DoActorSectorDamage(actor))
    {
        return 0;
    }

	if ((actor->spr.pos.XY() - actor->user.targetActor->spr.pos.XY()).Length() < 75)
    {
        UpdateSinglePlayKills(actor);
        DoActorDie(actor, actor, 0);
        return 0;
    }

    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int InitCoolieCharge(DSWActor* actor)
{
    if (RANDOM_P2(1024) > 950)
        PlaySound(DIGI_COOLIESCREAM, actor, v3df_follow);

    DoActorSetSpeed(actor, FAST_SPEED);

    InitActorMoveCloser(actor);

    NewStateGroup(actor, s_CoolieCharge);

    return 0;
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int DoCoolieWaitBirth(DSWActor* actor)
{
    if ((actor->user.Counter -= ACTORMOVETICS) <= 0)
    {
        ChangeState(actor,&s_CoolieDie[9]);
    }

    return 0;
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

#include "saveable.h"

static saveable_data saveable_coolie_data[] =
{
    SAVE_DATA(CooliePersonality),

    SAVE_DATA(CoolieAttrib),

    SAVE_DATA(s_CoolieRun),
    SAVE_DATA(s_CoolieCharge),
    SAVE_DATA(s_CoolieStand),
    SAVE_DATA(s_CooliePain),
    SAVE_DATA(s_CoolieDie),
    SAVE_DATA(s_CoolieDead),

    SAVE_DATA(CoolieActionSet),
};

saveable_module saveable_coolie =
{
    // code
    nullptr, 0,

    // data
    saveable_coolie_data,
    SIZ(saveable_coolie_data)
};
END_SW_NS
