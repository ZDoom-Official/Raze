//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT
Copyright (C) NoOne

*********************************************************************
NoOne: Custom Dude system. Includes compatibility with older versions
For full documentation visit: http://cruo.bloodgame.ru/xxsystem/cdud/v2/
*********************************************************************

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


#ifdef NOONE_EXTENSIONS
#pragma once
#include <variant>
#include "nnexts.h"
#include "sound.h"
#include "view.h"
#include "globals.h"
#include "sound.h"

BEGIN_BLD_NS

#define kCdudeFileNamePrefix        "CDUD"
#define kCdudeFileNamePrefixWild    "CDUD*"
#define kCdudeFileExt               "CDU"
enum
{
kCdudeVer1                 = 1,
kCdudeVer2                 = 2,

kCdudeMaxVelocity          = 2345648,
kCdudeMaxSounds            = 3,
kCdudeMaxWeapons           = 8,
kCdudeMaxDmgScale          = 32767,
kCdudeDmgCheckDelay        = 1,
kCdudeMaxDispersion        = 3500,
kCdudeDefaultSeq           = 11520,
kCdudeV1MaxAttackDist      = 20000,
kCdudeMaxDropItems         = 5,
kCdudeDefaultAnimScale     = 256,
kCdudeMaxEffectGroups      = 16,
kCdudeMaxEffects           = 3,
kCdudeFXEffectBase         = 0,
kCudeFXEffectCallbackBase  = 512,
kCdudeGIBEffectBase        = 1024,

kCdudeBurningHealth        = (25 << 4),
};

constexpr double kCdudeMinSeeDist = 3000 * inttoworld;
constexpr double kCdudeMinHearDist = (kCdudeMinSeeDist * 0.5);

class DCustomDude;
extern VMNativeFunction** const gCdudeCustomCallback[];

enum enum_VALUE_TYPE {
kValNone                        = 0,
kValAny,
kValFix,
kValUfix,
kValPerc,
kValArrC,
kValArrA,
kValBool,
kValIdKeywordBase,
kValCdud                        = kValIdKeywordBase,
kValWeapon,
kValFX,
kValGIB,
kValVector,
kValProjectile,
kValThrow,
kValVdud,
kValKamikaze,
kValSpecial,
kValIdKeywordMax,
kValMax                         = kValIdKeywordMax,
};

enum enum_ERROR {
kErrInvalidValType              = 0,
kErrInvalidParam,
kErrInvalidParamPos,
kErrInvalidRange,
kErrInvalidResultC,
kErrInvalidVersion,
kErrReqParamNotFound,
kErrInvalidArrayLen1,
kErrInvalidArrayLen2,
kErrReqGroupNotFound,
kErrInvaliValuePos,
kErrMax,
};

enum enum_PAR_GROUP {
kParGroupGeneral                = 0,
kParGroupVelocity,
kParGroupAnimation,
kParGroupSound,
kParGroupWeapon,
kParGroupDodge,
kParGroupRecoil,
kParGroupKnockout,
kParGroupDamage,
kParGroupFXEffect,
kParGroupMovePat,
kParGroupDropItem,
kParGroupParser,
};

enum enum_PAR_GENERAL {
kParGeneralVersion              = 0,
kParGeneralMass,
kParGeneralMedium,
kParGeneralHealth,
kParGeneralClipdist,
kParGeneralMorphTo,
kParGeneralActiveTime,
};


enum enum_PAR_PARSER {
kParParserWarnings              = 0,
};

enum enum_PAR_EVENT {
kParEventOnDmg                  = 0,
kParEventOnAimTargetWrong,
};

enum enum_PAR_DAMAGE {
kParDmgSource                   = kDmgMax,
};

enum enum_PAR_DAMAGE_SOURCE {
kDmgSourceDude                  = 0x01,
kDmgSourcePlayer                = 0x02,
kDmgSourceSelf                  = 0x04,
kDmgSourceFriend                = 0x08,
kDmgSourceEnemy                 = 0x10,
kDmgSourceSlave                 = 0x20,
kDmgSourceKin                   = 0x40,
};

enum enum_PAR_VELOCITY {
kParVelocityForward             = 0,
kParVelocityTurn,
kParVelocityDodge,
kParVelocityMax,
};

enum enum_PAR_APPEARANCE {
kAppearSeq                      = 0,
kAppearClb,
kAppearSnd,
kAppearScale,
kAppearPic,
kAppearPal,
kAppearShade,
kAppearSize,
kAppearOffs1,
};


enum enum_PAR_MEDIUM {
kParMediumAny                   = 0,
kParMediumLand,
kParMediumWater,
};

enum enum_PAR_WEAPON {
kParWeaponId                    = 0,
kParWeaponAkimbo,
kParWeaponDist,
kParWeaponPosture,
kParWeaponDisp,
kParWeaponAttackAng,
kParWeaponMedium,
kParWeaponAmmo,
kParWeaponPickChance,
kParWeaponAttackSetup,
kParWeaponShotSetup,
kParWeaponShotSnd,
kParWeaponShotAppearance,
kParWeaponAttackAnim,
kParWeaponDudeHealth,
kParWeaponTargetHealth,
kParWeaponSkill,
kParWeaponCooldown,
kParWeaponStyle,
};

enum enum_PAR_ATTACK {
kParAttackTime                  = 0,
kParAttackInterrupt,
kParAttackTurn2Target,
kParAttackNumShots,
kParAttackInertia,
};

enum enum_PAR_SHOT {
kParWeaponShotOffs              = 0,
kParWeaponShotVel,
kParWeaponShotSlope,
kParWeaponShotFollow,
kParWeaponShotClipdist,
kParWeaponShotImpact,
kParWeaponShotRemTimer,
};

enum enum_PAR_WEAP_STYLE {
kParWeaponStyleOffset           = 0,
kParWeaponStyleAngle,
};

enum enum_PAR_EFFECT {
kParEffectId                    = 0,
kParEffectTimer,
kParEffectOffset,
kParEffectAppearance,
kParEffectAiState,
kParEffectAnimID,
kParEffectAnimFrame,
kParEffectAngle,
kParEffectVelocity,
kParEffectSlope,
kParEffectPosture,
kParEffectMedium,
kParEffectRemTime,
kParEffectChance,
kParEffectAllUnique,
kParEffectSrcVel,
kParEffectFx2Gib,
};

enum enum_PAR_MOVE {
kParMoveFallHeight              = 0,
};

enum enum_PAR_DROP_ITEM {
kParDropItem                    = 0,
kParDropItemType,
kParDropItemChance,
kParDropItemSkill,
kParDropItemSprChance,
};

enum enum_PAR_GIB {
kParGibType,
kParGibSoundID,
kParGibForce,
kParGibFlags,
kParGibPhysics,
};

enum enum_PAR_TRIG_FLAGS {
kParTigNone                     = 0x00,
kParTrigVector                  = 0x01,
kParTrigTouch                   = 0x02,
kParTrigImpact                  = 0x04,
kParTrigLocked                  = 0x08,
};

enum enum_CDUD_POSTURE {
kCdudePosture                   = 0,
kCdudePostureL                  = kCdudePosture,
kCdudePostureC,
kCdudePostureW,
kCdudePostureMax,
};

enum enum_CDUD_SOUND {
kCdudeSnd                       = 0,
kCdudeSndTargetSpot             = kCdudeSnd,
kCdudeSndGotHit,
kCdudeSndDeathNormal,
kCdudeSndBurning,
kCdudeSndDeathExplode,
kCdudeSndTargetDead,
kCdudeSndTargetChase,
kCdudeSndTransforming,
kCdudeSndWake,
kCdudeSndMax,
kCdudeSndCompatAttack1,
kCdudeSndCompatAttack2,
kCdudeSndCompatAttack3,
};

enum enum_CDUD_WEAPON_TYPE {
kCdudeWeaponNone                = 0,
kCdudeWeaponHitscan,
kCdudeWeaponMissile,
kCdudeWeaponThrow,
kCdudeWeaponSummon,
kCdudeWeaponSummonCdude,
kCdudeWeaponKamikaze,
kCdudeWeaponSpecialBeastStomp,
kCdudeWeaponMax,
};

enum enum_CDUD_WEAPON_TYPE_SPECIAL {
kCdudeWeaponIdSpecialBase       = 900,
kCdudeWeaponIdSpecialBeastStomp = kCdudeWeaponIdSpecialBase,
kCdudeWeaponIdSpecialMax
};

enum enum_CDUD_AISTATE {
kCdudeStateBase                 = 0,
kCdudeStateIdle                 = kCdudeStateBase,
kCdudeStateMoveBase,
kCdudeStateSearch               = kCdudeStateMoveBase,
kCdudeStateDodge,
kCdudeStateChase,
kCdudeStateFlee,
kCdudeStateMoveMax,
kCdudeStateRecoil               = kCdudeStateMoveMax,
kCdudeStateRecoilT,
kCdudeBurnStateSearch,
kCdudeStateMorph,
kCdudeStateKnockEnter,
kCdudeStateKnock,
kCdudeStateKnockExit,
kCdudeStateSleep,
kCdudeStateWake,
kCdudeStateGenIdle,
kCdudeStateNormalMax,
kCdudeStateDeathBase            = kCdudeStateNormalMax,
kCdudeStateDeathBurn,
kCdudeStateDeathVector,
kCdudeStateDeathExplode,
kCdudeStateDeathChoke,
kCdudeStateDeathSpirit,
kCdudeStateDeathElectric,
kCdudeStateDeathMax,
kCdudeStateAttackBase           = kCdudeStateDeathMax,
kCdudeStateAttackMax            = kCdudeStateAttackBase + kCdudeMaxWeapons,
kCdudeStateMax                  = kCdudeStateAttackMax,
kCdudeStateMove,
kCdudeStateDeath,
kCdudeStateAttack,
kCdudeAnimScale,
};

enum enum_CDUD_STATUS {
kCdudeStatusNormal              = 0x00,
kCdudeStatusAwaked              = 0x01,
kCdudeStatusForceCrouch         = 0x02,
kCdudeStatusSleep               = 0x04,
kCdudeStatusMorph               = 0x08,
kCdudeStatusBurning             = 0x10,
kCdudeStatusDying               = 0x20,
kCdudeStatusRespawn             = 0x40,
};

struct PARAM
{
	unsigned int id;
	const char* text;
};

class ARG_PICK_WEAPON
{
	public:
		DAngle angle;
		double _distance;
		uint8_t dudeHealth;
		uint8_t targHealth;
		ARG_PICK_WEAPON(DBloodActor* actor, DBloodActor* target)
		{
			DVector2 distv = target->spr.pos.XY() - actor->spr.pos.XY();
			_distance = distv.Length();
			angle = absangle(distv.Angle(), actor->spr.Angles.Yaw);
			dudeHealth = CountHealthPerc(actor);
			targHealth = CountHealthPerc(target);
		}

		ARG_PICK_WEAPON(DBloodActor* actor, DBloodActor* target, double nDist, DAngle nAng)
		{
			_distance = nDist;
			angle = nAng;
			dudeHealth = CountHealthPerc(actor);
			targHealth = CountHealthPerc(target);
		}

		int CountHealthPerc(DBloodActor* actor)
		{
			int nHealth = ClipLow(nnExtGetStartHealth(actor), 1);
			return ClipHigh((kPercFull * actor->xspr.health) / nHealth, 255);
		}
};

class CUSTOMDUDE_SOUND_TPL
{
public:
	int id[kCdudeMaxSounds];
	uint8_t medium;
	bool ai;
	bool interruptable;
	bool once;
	int16_t volume;
};

class CUSTOMDUDE_SOUND
{
	public:
		FSoundID id[kCdudeMaxSounds];
		uint8_t medium;
		bool ai;
		bool interruptable;
		bool once;
		int16_t volume;
		FSoundID  Pick() { return id[Random(kCdudeMaxSounds)]; }
		CUSTOMDUDE_SOUND& operator=(const CUSTOMDUDE_SOUND_TPL& tpl)
		{
			for (int i = 0; i < kCdudeMaxSounds; i++) id[i] = soundEngine->FindSoundByResID(tpl.id[i]);
			medium = tpl.medium;
			ai = tpl.ai;
			interruptable = tpl.interruptable;
			once = tpl.once;
			volume = tpl.volume;
			return *this;
		}
		void Play(DBloodActor* pSpr) { Play(pSpr, Pick()); }
		void Play(DBloodActor* pSpr, FSoundID nID)
		{
			if (nID != NO_SOUND)
			{
				int nClock = PlayClock;
				bool uwater = spriteIsUnderwater(pSpr, true);
				int nRand = Random2(80);

				if (!medium || ((medium & 0x01) && !uwater) || ((medium & 0x02) && uwater))
				{
					DUDEEXTRA* pExtra = &pSpr->dudeExtra;
					if (once)
					{
						if (soundEngine->EnumerateChannels([=](FSoundChan* chan)
							{
								for (int j = 0; j < kCdudeMaxSounds; j++)
								{
									if (chan->OrgID == id[j])
										return -1;
								}
								return 0;
							})) return;
					}

					if (interruptable)
					{
						pExtra->time = nClock;
						Kill(pSpr);
					}

					if (ai)
					{
						if (pExtra->time <= nClock)
						{
							sfxKill3DSound(pSpr, AI_SFX_PRIORITY_2, -1);
							sfxPlay3DSoundVolume(pSpr, nID, AI_SFX_PRIORITY_2, 0, 0x0, volume);
							pExtra->prio = AI_SFX_PRIORITY_2;
							pExtra->time = nClock + 384 + nRand;
						}
					}
					else
					{
						sfxPlay3DSoundVolume(pSpr, nID, -1, 0, 0x0, volume);
					}
				}
			}
		}

		void Kill(DBloodActor* pSpr)
		{
			int i = kCdudeMaxSounds;
			while (--i >= 0)
				sfxKill3DSound(pSpr, -1, id[i]);
		}

		void KillOutside(DBloodActor* pSpr)
		{
			if (soundEngine->EnumerateChannels([=](FSoundChan* chan)
				{
					if (chan->Source == pSpr)
					{
						for (int j = 0; j < kCdudeMaxSounds; j++)
						{
							if (chan->OrgID == id[j])
								return -1;
						}
						sfxKill3DSound(pSpr, -1, chan->OrgID);
					}
					return 0;
				})) return;
		}
};

class APPEARANCE
{
	public:
		CUSTOMDUDE_SOUND sound;
		unsigned short scl[2];
		bool available;
		bool soundAvailable;
		unsigned int seq;
		FTextureID pic;
		uint8_t clb;
		uint8_t xrp;
		uint8_t yrp;
		uint8_t xof;
		uint8_t yof;
		uint16_t pal;
		uint16_t shd;
		void Clear(void)
		{
			memset(this, 0, sizeof(APPEARANCE));
			pal = -1;
			shd = 127;
		}

		void SetScale(DBloodActor* actor)
		{
			if (available)
			{
				int nScale = 0;
				
				if (scl[1] > 0)
					nScale = ClipRange(scl[1] + Random2(scl[1] - scl[0]), scl[0], scl[1]);
				else
					nScale = scl[0];

				nnExtSprScaleSet(actor, nScale);

				if (actor->hasX())
					actor->xspr.scale = nScale;
			}
		}

		void Set(DBloodActor* pSpr)
		{
			if (!available)
				return;

			pSpr->addX();
			
			bool xs = true, ys = true, plu = true;
			SEQINST* pInst;
			VMFunction* nCallback = nullptr;
			int nSeq = seq;

			if (soundAvailable)
			{
				sound.KillOutside(pSpr);
				sound.Play(pSpr);
			}

			if (pic.isValid())
			{
				seqKill(pSpr);
				pSpr->spr.setspritetexture(pic);
				nSeq = -1;
			}

			if (nSeq >= 0)
			{
				pInst = GetInstance(pSpr);
				if (pInst)
				{
					seqCanOverride(pInst->pSequence, 0, &xs, &ys, &plu);
					nCallback = pInst->callback;
				}

				if (nSeq > 0)
				{
					seqSpawn(nSeq, pSpr, clb ? nullptr : nCallback);
					pInst = GetInstance(pSpr);
					if (pInst)
						seqCanOverride(pInst->pSequence, 0, &xs, &ys, &plu);
				}

				if (clb && pInst)
				{
					pSpr->xspr.sysData2 = clb - 1;
					if (pSpr->xspr.sysData2 > 10)
						pInst->callback = *gCdudeCustomCallback[pSpr->xspr.sysData2];
					else
						pInst->callback  = AF(callbackSeqCustom);
				}
			}

			if (shd != 127)
			{
				seqKill(pSpr);
				pSpr->spr.shade = (int8_t)shd;
			}

			if (pal >= 0)
			{
				if (!plu)
					seqKill(pSpr);

				pSpr->spr.pal = (int8_t)pal;
			}

			if (xrp)
			{
				if (!xs)
					seqKill(pSpr);

				pSpr->spr.scale.X = xrp * REPEAT_SCALE;
			}

			if (yrp)
			{
				if (!ys)
					seqKill(pSpr);

				pSpr->spr.scale.Y = yrp * REPEAT_SCALE;
			}

			if (xof) pSpr->spr.xoffset = xof;
			if (yof) pSpr->spr.yoffset = yof;

			if (scl[0])
				SetScale(pSpr);
		}
};

class CUSTOMDUDE_WEAPON
{
	public:
		uint16_t  type     ;
		uint16_t  numshots ;
		uint16_t  id       ;
		uint16_t  sharedId ;
		DAngle  angle    ;
		uint16_t  medium   ;
		unsigned int  pickChance;
		uint8_t  available     ;
		uint8_t  posture       ;
		uint8_t  interruptable ;
		uint8_t  turnToTarget  ;
		uint8_t  stateID       ;
		uint8_t  nextStateID   ;
		unsigned int  clipMask;
		unsigned int  group;
		unsigned int  dispersion[2];
		double  distRange[2];
		uint8_t targHpRange[2];
		uint8_t dudeHpRange[2];
		CUSTOMDUDE_SOUND sound;
		struct SHOT
		{
			double _velocity;
			signed   int slope;
			DAngle targetFollow;
			uint8_t clipdist;
			uint8_t impact;
			int16_t remTime;
			APPEARANCE appearance;
			DVector3 offset;
		}
		shot;
		struct AMMO
		{
			uint16_t cur, total;
			void SetTotal(int nVal) { total = nVal; }
			void SetFull()          { Set(total); }
			void Set(int nVal)      { cur = ClipRange(nVal, 0, total); }
			void Inc(int nBy = 1)   { Set(cur + nBy); }
			void Dec(int nBy = 1)   { Set(cur - nBy); }
		}
		ammo;
		struct COOLDOWN
		{
			unsigned int clock;
			unsigned int delay;
			unsigned int useCount;
			unsigned int totalUseCount;
			void SetTimer(void)
			{
				clock = ClipLow(delay * kTicsPerFrame, kTicsPerFrame);
				clock += (unsigned int)PlayClock;
			}

			int Check(void)
			{
				if ((unsigned int)PlayClock < clock)
					return 2;
				
				if (totalUseCount)
				{
					useCount = ClipHigh(useCount + 1, totalUseCount);
					if (useCount < totalUseCount)
						return 0;
				}
				
				if (delay)
				{
					SetTimer();
					return 1;
				}

				return 0;
			}
		}
		cooldown;
		struct SHOT_STYLE
		{
			bool available;
			DAngle angle;
			DVector3 offset;
		}
		style;
		void Clear()
		{
			memset(this, 0, sizeof(CUSTOMDUDE_WEAPON));
			
			angle           = DAngle45;
			numshots        = 1;
			pickChance      = 0x10000;
			stateID         = kCdudeStateAttackBase;
			turnToTarget    = true;

			distRange[1]    = 20000 * inttoworld;
			dudeHpRange[1]  = 255;
			targHpRange[1]  = 255;

			shot.remTime    = -1;
			shot._velocity = FLT_MAX;
			shot.slope    = INT32_MAX;
		}
		bool HaveAmmmo(void)        { return (!ammo.total || ammo.cur); }
		double  GetDistance(void)     { return max(distRange[1] - distRange[0], 0.); }
		int  GetNumshots(void)      { return (ammo.total) ? ClipHigh(ammo.cur, numshots) : numshots; }
		bool IsTimeout(void)        { return ((unsigned int)PlayClock < cooldown.clock); }
		bool HaveSlope(void)        { return (shot.slope != INT32_MAX); }
		bool HaveVelocity(void)     { return (shot._velocity != FLT_MAX); }

};

class CUSTOMDUDE_GIB
{
	public:
		uint8_t available  ;
		uint8_t force      ;
		uint8_t trFlags    ;
		uint8_t physics    ;
		PClass* thingType  ;
		unsigned int data1;
		unsigned int data2;
		unsigned int data3;
		unsigned int data4;
		void Clear()
		{
			memset(this, 0, sizeof(CUSTOMDUDE_GIB));
			physics     = (kPhysMove | kPhysGravity | kPhysFalling);
			thingType   = BloodThingObjectExplodeClass;
			force       = true;
		}

		void Setup(DBloodActor* pSpr)
		{
			int nStat = pSpr->spr.statnum;
			if (nStat != kStatThing)
			{
				if (!force)
					return;

				evKillActor(pSpr, AF(RemoveActor));
				ChangeActorStat(pSpr, kStatThing);
			}
			pSpr->addX();

			pSpr->ChangeType(kThingBase + thingType);
			pSpr->spr.flags &= ~(kPhysMove | kPhysGravity | kPhysFalling);
			pSpr->spr.flags |= physics;
			
			if (!(pSpr->spr.flags & kPhysMove))
				pSpr->vel.XY().Zero();

			if (nStat == kStatFX)
			{
				// scale velocity a bit
				pSpr->vel.Z *= 1.35;
			}

			pSpr->xspr.health       = pSpr->IntVar("defhealth") << 4;
			pSpr->xspr.data1        = data1;
			pSpr->xspr.data2        = data2;
			pSpr->xspr.data3        = data3;
			pSpr->xspr.data4        = data4;

			pSpr->xspr.isTriggered  = false;
			pSpr->xspr.triggerOnce  = true;
			pSpr->xspr.state        = 1;

			if (trFlags & kParTrigVector)
			{
				pSpr->spr.cstat |= CSTAT_SPRITE_BLOCK_HITSCAN;
				pSpr->xspr.Vector = true;
			}

			if (trFlags & kParTrigTouch)
			{
				pSpr->spr.cstat |= CSTAT_SPRITE_BLOCK;
				pSpr->xspr.Touch = true;
			}
			
			if (trFlags & kParTrigImpact)
				pSpr->xspr.Impact = true;

			pSpr->xspr.locked = true;
			if (!(trFlags & kParTrigLocked))
				evPostActor(pSpr, 16, (COMMAND_ID)kCmdUnlock, nullptr);
		}
};

class CUSTOMDUDE_EFFECT
{
	public:
		TArray<int> pAnims;	// SEQ IDs
		TArray<int> pFrames; // SEQ frame indices
		TArray<int> pStates; // type to be determined

		unsigned short id[kCdudeMaxEffects];
		unsigned int clock;
		signed   int liveTime;
		double _velocity;
		double _velocitySlope;
		DAngle angle;
		uint8_t posture;
		uint8_t medium;
		bool allUnique ;
		bool srcVelocity;
		unsigned int chance;
		unsigned short delay[2];
		CUSTOMDUDE_GIB spr2gib;
		APPEARANCE appearance;
		DVector3 offset;
		void Clear()
		{
			pAnims.Clear();
			pFrames.Clear();
			pStates.Clear();
			// do not memset the TArrays!
			memset(id, 0, sizeof(CUSTOMDUDE_EFFECT) - myoffsetof(CUSTOMDUDE_EFFECT, id));
			angle       = DAngle360;
			_velocity    = -1;
			chance      = 0x10000;
			srcVelocity = 1;

		}

		bool CanSpawn(DBloodActor* pSpr)
		{
			int nFrame = 1;
			int nACount = pAnims.Size();
			int nFCount = pFrames.Size();
			if (nACount || nFCount)
			{
				SEQINST* pInst = GetInstance(pSpr);
				if (pInst)
				{
					if (nACount && !pAnims.Contains(pInst->nSeqID))
						return false;

					nFrame = pInst->frameIndex + 1;
				}
				else if (nACount)
				{
					return false;
				}

				if (nFCount)
					return pFrames.Contains(nFrame);
			}

			return true;
		}

		void SetDelay(void)
		{
			if (delay[1] > 0)
			{
				clock = ClipRange(delay[1] + Random2(delay[1] - delay[0]), delay[0], delay[1]);
			}
			else
			{
				clock = delay[0];
			}

			clock = ClipLow(clock * kTicsPerFrame, kTicsPerFrame);
			clock += (unsigned int)PlayClock;
		}

		void Setup(DBloodActor* pSrc, DBloodActor* pEff, bool relVel)
		{
			DVector3 dvel(0, 0, _velocitySlope);
			DAngle nAng = ((angle != DAngle360) ? (pSrc->spr.Angles.Yaw + angle) : RandomAngle()).Normalized360();
			double nVel = _velocity;
			int rp = Random(15);

			pEff->ownerActor = pSrc;

			dvel.Z   += Random2D(perc2val(rp, dvel.Z));
			nAng += RandomAngle(perc2val(rp >> 1, nAng.Buildang())).Normalized360();
			pEff->spr.Angles.Yaw = nAng.Normalized360();

			appearance.Set(pEff);

			if (nVel >= 0)
			{
				nVel += Random2D(perc2val(rp, nVel));
				dvel.XY() = nAng.ToVector(); // todo: determine factor (dx = (Cos(nAng) >> 16);)
				
				if (nVel == 0)
					relVel = false;
			}
			else
			{
				nVel = abs(dvel.Z);
			}

			if (relVel)
			{
				nnExtScaleVelocityRel(pEff, nVel, dvel);
			}
			else
			{
				nnExtScaleVelocity(pEff, nVel, dvel);
			}

			if (srcVelocity)
			{
				pEff->vel += pSrc->vel;
			}

			if (spr2gib.available)
				spr2gib.Setup(pEff);

			if (liveTime)
			{
				evKillActor(pEff, AF(RemoveActor));
				if (liveTime > 0)
					evPostActor(pEff, liveTime, AF(RemoveActor));
			}
		}

		void Spawn(int nID, DBloodActor* pSpr)
		{
			auto pos = pSpr->spr.pos;
			nnExtOffsetPos(offset, pSpr->spr.Angles.Yaw, pos);

			if (nID >= kCdudeGIBEffectBase)
			{
				nID -= kCdudeGIBEffectBase;
				
				TArray<DBloodActor*> fxList;
				if (nnExtGibSprite(pSpr, fxList, (GIBTYPE)nID, &pos, nullptr))
				{
					for(auto pFX : fxList)
					{
						Setup(pSpr, pFX, true);
					}
				}
			}
			else if (nID >= kCudeFXEffectCallbackBase)
			{
				auto func = *gCdudeCustomCallback[nID - kCudeFXEffectCallbackBase];
				evKillActor(pSpr, func);
				evPostActor(pSpr, 0, func);
			}
			else
			{
				nID -= kCdudeFXEffectBase;
				DBloodActor* pFX;
				if ((pFX = gFX.fxSpawnActor((FX_ID)nID, pSpr->spr.sectp, pos)) != nullptr)
					Setup(pSpr, pFX, false);
			}
		}
		
		void Spawn(DBloodActor* pSpr)
		{
			if (Chance(chance))
			{
				if (allUnique)
				{
					for (int i = 0; i < kCdudeMaxEffects; i++)
						Spawn(id[i], pSpr);
				}
				else
				{
					Spawn(Pick(), pSpr);
				}
			}
		}

		int Pick() { return id[Random(kCdudeMaxEffects)]; }
};


class  CUSTOMDUDE_DAMAGE
{
	public:
		unsigned short id[kDmgMax];
		uint8_t ignoreSources;
		void Set(int nVal, int nFor) { id[nFor] = ClipRange(nVal, 0, kCdudeMaxDmgScale); }
		void Inc(int nVal, int nFor) { Set(id[nFor] + abs(nVal), nFor); }
		void Dec(int nVal, int nFor) { Set(id[nFor] - abs(nVal), nFor); }
};

class  CUSTOMDUDE_DODGE
{
	public:
		struct DAMAGE
		{
			unsigned int times;
			unsigned int timer;
			unsigned int chance;
			unsigned int dmgReq;
			bool Allow(int nDamage)
			{
				if (nDamage > (int)dmgReq)
				{
					unsigned int nClock = (unsigned int)PlayClock;
					unsigned int nChance = chance;

					if (timer > nClock)
					{
						times += (5 - gGameOptions.nDifficulty);
						nChance = ClipHigh(perc2val(times, nChance), nChance);
						return Chance(nChance);
					}

					times = 0;
					timer = nClock + kCdudeDmgCheckDelay;
					return Chance(nChance);
				}

				return 0;
			}
		}
		onDamage;
		struct AIMMISS
		{
			unsigned int chance;
			bool Allow(void) { return Chance(chance); }
		}
		onAimMiss;
};

class  CUSTOMDUDE_RECOIL
{
	public:
		unsigned int times;
		unsigned int timer;
		unsigned int chance;
		unsigned int dmgReq;
		bool Allow(int nDamage)
		{
			if (nDamage > (int)dmgReq)
			{
				unsigned int nClock = (unsigned int)PlayClock;
				unsigned int nChance = chance;

				if (timer > nClock)
				{
					times += (5 - gGameOptions.nDifficulty);
					nChance = ClipHigh(perc2val(times, nChance), nChance);
					return Chance(nChance);
				}

				times = 0;
				timer = nClock + kCdudeDmgCheckDelay;
				return Chance(nChance);
			}

			return 0;
		}
};

class  CUSTOMDUDE_KNOCKOUT
{
	public:
		unsigned int times;
		unsigned int timer;
		unsigned int chance;
		unsigned int dmgReq;
		bool Allow(int nDamage)
		{
			if (nDamage > (int)dmgReq)
			{
				unsigned int nClock = (unsigned int)PlayClock;
				unsigned int nChance = chance;

				if (timer > nClock)
				{
					times += (5 - gGameOptions.nDifficulty);
					nChance = ClipHigh(perc2val(times, nChance), nChance);
					return Chance(nChance);
				}

				times = 0;
				timer = nClock + kCdudeDmgCheckDelay;
				return Chance(nChance);
			}

			return 0;
		}
};

class CUSTOMDUDE_VELOCITY
{
	public:
		unsigned int id[kParVelocityMax];
		void Set(int nVal, int nFor) { id[nFor] = ClipRange(nVal, 0, kCdudeMaxVelocity); }
		void Inc(int nVal, int nFor) { Set(id[nFor] + abs(nVal), nFor); }
		void Dec(int nVal, int nFor) { Set(id[nFor] - abs(nVal), nFor); }
};

class CUSTOMDUDE_DROPITEM
{
	public:
		struct itemdesc
		{
			PClass* cls;
			int perc;
		};

		itemdesc items[kCdudeMaxDropItems];
		unsigned int  sprDropItemChance;
		void Clear()
		{
			memset(this, 0, sizeof(CUSTOMDUDE_DROPITEM));
			sprDropItemChance = 0x10000;
		}

		int Pick(DBloodActor* pSpr, TArray<PClass*>& pOut)
		{

			// add key
			if (pSpr->xspr.key)
				pOut.AddUnique(GetSpawnType(kItemKeyBase + (pSpr->xspr.key - 1)));

			// add item
			if (pSpr->xspr.dropMsg && Chance(sprDropItemChance))
				pOut.AddUnique(GetSpawnType(pSpr->xspr.dropMsg));

			// add all items with 100% chance
			for (auto& item : items)
			{
				if (item.cls && item.perc >= 100)
					pOut.AddUnique(item.cls);
			}

			// add item with < 100% chance
			for (auto& item : items)
			{
				if (item.cls)
				{
					if (item.perc < 100 && Chance(perc2val(item.perc, 0x10000)))
					{
						pOut.AddUnique(item.cls);
						break;
					}
				}
			}
			return pOut.Size();
		}
};

using NextDudeInfo = std::variant<bool, int, PClass*, DBloodActor*>;

class DCustomDude : public DObject
{
	DECLARE_CLASS(DCustomDude, DObject)
	HAS_OBJECT_POINTERS

	size_t PropagateMark() override;
	void OnDestroy();
	void Serialize(FSerializer& arc) override;

	// Note: we will likely have to write out the entire shit here to make this savegame robust...
	public:
		uint8_t version;
		uint8_t initialized;
		uint8_t numEffects;
		uint8_t numWeapons;
		DUDEINFO* pInfo;
		DBloodActor* pSpr;	// this is our owner so no TObjPtr is needed
		TObjPtr<DBloodActor *> pLeech;
		CUSTOMDUDE_WEAPON    weapons[kCdudeMaxWeapons];                             // the weapons it may have
		CUSTOMDUDE_WEAPON*   pWeapon;                                               // pointer to current weapon
		CUSTOMDUDE_DAMAGE    damage;                                                // damage control
		CUSTOMDUDE_VELOCITY  velocity[kCdudePostureMax];                            // velocity control
		CUSTOMDUDE_SOUND     sound[kCdudeSndMax];                                   // ai state sounds
		CUSTOMDUDE_DODGE     dodge;                                                 // dodge control
		CUSTOMDUDE_RECOIL    recoil;                                                // recoil control
		CUSTOMDUDE_KNOCKOUT  knockout;                                              // knock control
		CUSTOMDUDE_DROPITEM  dropItem;                                              // drop item control
		CUSTOMDUDE_EFFECT    effects[kCdudeMaxEffectGroups];                        // fx, gib effect stuff
		AISTATE states[kCdudeStateMax][kCdudePostureMax];                           // includes states for weapons
		TArray<TObjPtr<DBloodActor*>> pSlaves;                                                            // summoned dudes under control of this dude
		TArray<int> triggerSeqs;					// this originally hacked the global ser
		uint8_t medium     ;                       // medium in which it can live
		uint8_t posture    ;                       // current posture
		unsigned int mass       ;                       // mass in KG
		FTextureID largestPic ;                       // in all states to compare on crouching
		sectortype* prevSector ;                       // the recent sector dude was in
		double _seeDist    ;                       // dudeInfo duplicate for sleeping
		double _hearDist   ;                       // dudeInfo duplicate for sleeping
		DAngle periphery  ;                       // dudeInfo duplicate for sleeping
		unsigned int fallHeight ;                       // in pixels
		NextDudeInfo nextDude;

		//signed   int nextDude   ;                       
		//----------------------------------------------------------------------------------------------------
		void PlaySound(int nState)                     { return (sound[nState].Play(pSpr)); }
		int  GetStateSeq(int nState, int nPosture)     { return states[nState][nPosture].seqId; }
		int  GetVelocity(int nPosture, int nVelType)   { return velocity[nPosture].id[nVelType]; }
		int  GetVelocity(int nVelType)                 { return GetVelocity(posture, nVelType); }
		double  GetVelocityF(int nPosture, int nVelType)   { return FixedToFloat(velocity[nPosture].id[nVelType]); }
		double  GetVelocityF(int nVelType)                 { return GetVelocityF(posture, nVelType); }
		bool IsUnderwater(void)                        { return (pSpr->xspr.medium != kMediumNormal); }
		bool IsCrouching(void)                         { return (posture == kCdudePostureC); }
		bool SeqPlaying(void)                          { return (seqGetStatus(pSpr) >= 0); }
		bool IsAttacking(void)                         { return (pSpr->xspr.aiState->stateType == kAiStateAttack); }
		bool IsKnockout(void)                          { return (pSpr->xspr.aiState->stateType == kAiStateKnockout); }
		bool IsRecoil(void)                            { return (pSpr->xspr.aiState->stateType == kAiStateRecoil); }
		bool IsBurning(void)                           { return StatusTest(kCdudeStatusBurning); }
		bool IsMorphing(void)                          { return StatusTest(kCdudeStatusMorph); }
		bool IsDying(void)                             { return StatusTest(kCdudeStatusDying); }
		bool IsSleeping(void)                          { return StatusTest(kCdudeStatusSleep); }
		bool IsLeechBroken(void)                       { return (pLeech && pLeech->xspr.locked); }
		// ---------------------------------------------------------------------------------------------------
		void StatusSet(int nStatus)                    { pSpr->xspr.sysData1 |= nStatus; }
		void StatusRem(int nStatus)                    { pSpr->xspr.sysData3 &= ~nStatus; }
		bool StatusTest(int nStatus)                   { return ((pSpr->xspr.sysData3 & nStatus) > 0); }
		//----------------------------------------------------------------------------------------------------
		bool CanRecoil(void)                           { return (GetStateSeq(kCdudeStateRecoil, posture) > 0); }
		bool CanElectrocute(void)                      { return (GetStateSeq(kCdudeStateRecoilT, posture) > 0); }
		bool CanKnockout(void)                         { return (GetStateSeq(kCdudeStateKnock, posture)); }
		bool CanBurn(void)                             { return (GetStateSeq(kCdudeBurnStateSearch, posture) > 0); }
		bool CanCrouch(void)                           { return (GetStateSeq(kCdudeStateSearch, kCdudePostureC) > 0); }
		bool CanSwim(void)                             { return (GetStateSeq(kCdudeStateSearch, kCdudePostureW) > 0); }
		bool CanSleep(void)                            { return (!StatusTest(kCdudeStatusAwaked) && GetStateSeq(kCdudeStateSleep, posture) > 0); }
		bool CanMove(void)                             { return (GetStateSeq(kCdudeStateSearch, posture) > 0); }
		//----------------------------------------------------------------------------------------------------
		int  GetDamage(DBloodActor* nSource, int nDmgType);
		bool IsPostureMatch(int nPosture);
		bool IsMediumMatch(int nMedium);
		bool IsTooTight(void);
		//----------------------------------------------------------------------------------------------------
		CUSTOMDUDE_WEAPON* PickWeapon(ARG_PICK_WEAPON* pArg);
		double  AdjustSlope(DBloodActor* nTarget, double zOffs);
		bool AdjustSlope(double nDist, double* nSlope);
		//----------------------------------------------------------------------------------------------------
		void InitSprite(void);
		void Activate(void);
		void Process(void);
		void ProcessEffects(void);
		void Recoil(void);
		int  Damage(DBloodActor* nFrom, int nDmgType, int nDmg);
		void Kill(DBloodActor* nFrom, int nDmgType, int nDmg);
		//----------------------------------------------------------------------------------------------------
		bool CanMove(sectortype* pXSect, bool Crusher, bool Water, bool Uwater, bool Depth, double bottom, double floorZ);
		bool FindState(AISTATE* pState, int* nStateType, int* nPosture);
		void NewState(int nStateType, int nTimeOverride = -1);
		bool NewState(AISTATE* pState);
		void NextState(int nStateType, int nTimeOverride = 0);
		void NextState(AISTATE* pState, int nTimeOverride = 0);
		AISTATE* PickDeath(int nDmgType);
		void SyncState(void);
		//----------------------------------------------------------------------------------------------------
		void LeechPickup(void);
		void LeechKill(bool delSpr);
		void UpdateSlaves(void);
		void DropItems(void);
		void ClearEffectCallbacks(void);
		//----------------------------------------------------------------------------------------------------
};

class CUSTOMDUDEV1_SETUP;
class CUSTOMDUDEV2_SETUP;
class CUSTOMDUDE_SETUP
{
		friend CUSTOMDUDEV1_SETUP;
		friend CUSTOMDUDEV2_SETUP;
	private:
		const char* pValue;  
		DCustomDude* pDude;   
		const PARAM* pGroup;       
		char key[256];       
		int nWarnings;       
		int hIni;
		FString strIni;
		std::unique_ptr<IniFile> pIni;
		const PARAM* pParam;
		char val[256];
		bool showWarnings;
		/*------------------------------------------------------------*/
		int FindParam(const char* str, const PARAM* pDb);
		const PARAM* FindParam(int nParam, const PARAM* pDb);
		int ParamLength(const PARAM* pDb);
		/*-------------------------------------------------*/
		bool DescriptLoad(int nID);
		void DescriptClose(void);
		bool DescriptGroupExist(const char* pGroupName);
		bool DescriptParamExist(const char* pGroupName, const char* pParamName);
		int  DescriptCheck(void);
		const char* DescriptGetValue(const char* pGroupName, const char* pParamName);
		/*------------------------------------------------------------*/
		void Warning(const char* pFormat, ...);
		const char* GetValType(int nType);
		const char* GetError(int nErr);
		/*------------------------------------------------------------*/
		void DamageSetDefault(void);
		void DamageScaleToSkill(int nSkill);
		/*------------------------------------------------------------*/
		void VelocitySetDefault(int nMaxVel);
		/*------------------------------------------------------------*/
		void WeaponSoundSetDefault(CUSTOMDUDE_WEAPON* pWeapon);
		void WeaponDispersionSetDefault(CUSTOMDUDE_WEAPON* pWeapon);
		void WeaponRangeSet(CUSTOMDUDE_WEAPON* pWeapon, int nMin, int nMax);
		/*------------------------------------------------------------*/
		void AnimationConvert(int baseID);
		void AnimationFill(AISTATE* pState, int nAnim);
		void AnimationFill(void);
		/*------------------------------------------------------------*/
		void SoundConvert(int baseID);
		void SoundFill(CUSTOMDUDE_SOUND* pSound, int nSnd);
		void SoundFill(void);
		/*------------------------------------------------------------*/
		void FindLargestPic(void);
		void RandomizeDudeSettings(void);
		void SetupSlaves(void);
		void SetupLeech(void);
		/*------------------------------------------------------------*/
		DCustomDude* SameDudeExist(DCustomDude* pCmp);
		DCustomDude* GetFirstDude(int nID);
		bool IsFirst(DCustomDude* pCmp);
		void DoSetup(DBloodActor* actor);
		void DoSetup(DCustomDude* pOver = nullptr);

	public:
		bool FindAiState(AISTATE stateArr[][kCdudePostureMax], int arrLen, AISTATE* pNeedle, int* nType, int* nPosture);
		static void Setup(DBloodActor* actor);
		static void Setup(DCustomDude* pOver = nullptr);

};

class CUSTOMDUDEV1_SETUP : public CUSTOMDUDE_SETUP
{
	private:
		void DamageScaleToSurface(int nSurface);
		void DamageScaleToWeapon(CUSTOMDUDE_WEAPON* pWeapon);
		void WeaponMeleeSet(CUSTOMDUDE_WEAPON* pWeap);
		void WeaponConvert(int nWeaponID);
		void SetupIncarnation(void);
		void SetupBasics(void);
		void SetupDamage(void);
	public:
		void Setup(void);
};

class CUSTOMDUDEV2_SETUP : public CUSTOMDUDE_SETUP
{
	private:
		bool ParseVelocity(const char* str, CUSTOMDUDE_VELOCITY* pVelocity);
		bool ParseAppearance(const char* str, APPEARANCE* pAppear);
		bool ParseSound(const char* str, CUSTOMDUDE_SOUND* pSound);
		bool ParseAnimation(const char* str, AISTATE* pState, bool asPosture);
		int ParseRange(const char* str, int nValType, int out[2], int nBaseVal = 0);
		int  ParseMedium(const char* str);
		bool ParseOffsets(const char* str, DVector3& pOut);
		bool ParseShotSetup(const char* str, CUSTOMDUDE_WEAPON* pWeap);
		bool ParseAttackSetup(const char* str, CUSTOMDUDE_WEAPON* pWeap);
		bool ParseWeaponStyle(const char* str, CUSTOMDUDE_WEAPON* pWeap);
		bool ParseWeaponBasicInfo(const char* str, CUSTOMDUDE_WEAPON* pWeap);
		int ParsePosture(const char* str);
		bool ParseOnEventDmg(const char* str, int* pOut, int nLen);
		bool ParseDropItem(const char* str, CUSTOMDUDE_DROPITEM::itemdesc& out);
		bool ParseSkill(const char* str);
		int  ParseKeywords(const char* str, const PARAM* pDb);
		int  ParseIDs(const char* str, int nValType, TArray<int>& pOut, int nMax = 0);
		int  ParseIDs(const char* str, int nValType, int* pOut, int nMax);
		int  ParseEffectIDs(const char* str, const char* paramName, unsigned short* pOut, int nLen = 0);
		int  ParseStatesToList(const char* str, TArray<int>& pOut);	// todo: determine type.
		bool ParseGibSetup(const char* str, CUSTOMDUDE_GIB* pOut);
		/*-------------------------------------------------*/
		int  CheckArray(const char* str, int nMin = 0, int nMax = 0, int nDefault = 1);
		int  CheckValue(const char* str, int nValType, int nDefault);
		int  CheckValue(const char* str, int nValType, int nMin, int nMax);
		int  CheckValue(const char* str, int nValType, int nMin, int nMax, int nDefault);
		int  CheckRange(const char* str, int nVal, int nMin, int nMax);
		int  CheckParam(const char* str, const PARAM* pDb);
		/*-------------------------------------------------*/
		void SetupGeneral(void);
		void SetupVelocity(void);
		void SetupAnimation(AISTATE* pState, bool asPosture);
		void SetupAnimation(void);
		void SetupSound(CUSTOMDUDE_SOUND* pSound);
		void SetupMovePattern(void);
		void SetupSound(void);
		void SetupDamage(void);
		void SetupRecoil(void);
		void SetupDodge(void);
		void SetupKnockout(void);
		void SetupWeapons(void);
		void SetupEffect(void);
		void SetupDropItem(void);
public:
		void Setup(void);
};

bool IsCustomDude(DBloodActor* pSpr)        { return (pSpr->GetType() == kDudeModernCustom); }
DCustomDude* cdudeGet(DBloodActor* pSpr);
DBloodActor* cdudeSpawn(DBloodActor* pSource, DBloodActor* pSprite, double nDist);
void cdudeLeechOperate(DBloodActor* pSprite);

END_BLD_NS
#endif