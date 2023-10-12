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

BEGIN_BLD_NS

// By NoOne: renamed dude struct
struct DUDEINFO {
	int16_t seqStartID; // seq
	int16_t startHealth; // health
	uint16_t mass; // mass
	int at6; // unused?
	uint8_t clipdist; // clipdist
	int eyeHeight;
	int aimHeight; // used by just Cerberus
	int hearDist; // hear radius
	int seeDist; // sight radius
	int periphery; // periphery
	int meleeDist; // unused?
	int fleeHealth; // at which hp level enemy will turn in burning dude
	int hinderDamage; // recoil damage
	int changeTarget; // chance to change target when attacked someone else
	int changeTargetKin; // chance to change target when attacked by same type
	int alertChance;
	uint8_t lockOut; // indicates if this dude can trigger something via trigger flags
	int frontSpeed; // acceleration
	int sideSpeed; // dodge
	int backSpeed; // backward speed (unused)
	int angSpeed; // turn speed
	int nGibType[3]; // which gib used when explode dude
	int16_t startDamage[7]; // start damage shift
	int16_t __filler__[7]; // not used anymore
	int at8c; // unused ?
	int at90; // unused ?

	// converters to floating point format
	inline double HearDist() const
	{
		return hearDist * maptoworld;
	}

	inline double SeeDist() const
	{
		return seeDist * maptoworld;
	}

	inline double MeleeDist() const
	{
		return meleeDist * maptoworld;
	}

	inline DAngle Periphery() const
	{
		return mapangle(periphery);
	}

	inline double FrontSpeed() const
	{
		return FixedToFloat(frontSpeed);
	}

	inline DAngle TurnRange() const
	{
		return DAngle::fromQ16(angSpeed << 3);
	}
	
	double fClipdist() const { return clipdist * 0.25; }

};

extern DUDEINFO gPlayerTemplate[4];

DBloodPlayer* getPlayer(DBloodActor* actor);
DBloodPlayer* safeGetPlayer(DBloodActor* actor);

END_BLD_NS
