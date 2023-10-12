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
//#include <iostream>

#include "ns.h"	// Must come before everything else!


#include "build.h"
#include "automap.h"
#include "savegamehelp.h"

#include "blood.h"
#include "texids.h"

BEGIN_BLD_NS

const VECTORDATA gVectorData[] = {

	// Tine
	{
		kDamageBullet,
		17,
		174762,
		1152,
		10240,
		0,
		1,
		20480,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_43, FX_6, FX_NONE, 502,
		FX_43, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_7, 502,
		FX_43, FX_6, FX_7, 502,
		FX_NONE, FX_NONE, FX_NONE, 503,
		FX_43, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 503,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

	// Shell
	{
		kDamageBullet,
		4,
		65536,
		0,
		8192,
		0,
		1,
		12288,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_43, FX_6, FX_NONE, -1,
		FX_43, FX_0, FX_NONE, -1,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_43, FX_6, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

	// Bullet
	{
		kDamageBullet,
		7,
		21845,
		0,
		32768,
		0,
		1,
		12288,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_7, 510,
		FX_NONE, FX_5, FX_7, 511,
		FX_43, FX_6, FX_NONE, 512,
		FX_43, FX_0, FX_NONE, 513,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_7, 512,
		FX_43, FX_6, FX_7, 512,
		FX_NONE, FX_NONE, FX_NONE, 513,
		FX_43, FX_NONE, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,

	},

	// Tommy AP
	{
		kDamageBullet,
		20,
		65536,
		0,
		16384,
		0,
		1,
		20480,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_7, 510,
		FX_NONE, FX_5, FX_7, 511,
		FX_43, FX_6, FX_NONE, 512,
		FX_43, FX_0, FX_NONE, 513,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_7, 512,
		FX_43, FX_6, FX_7, 512,
		FX_NONE, FX_NONE, FX_NONE, 513,
		FX_43, FX_NONE, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

	// Shell AP
	{
		kDamageBullet,
		6,
		87381,
		0,
		12288,
		0,
		1,
		6144,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_43, FX_6, FX_NONE, -1,
		FX_43, FX_0, FX_NONE, -1,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_43, FX_6, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

	// Tommy regular
	{
		kDamageBullet,
		12,
		65536,
		0,
		16384,
		0,
		1,
		12288,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_43, FX_5, FX_7, 510,
		FX_NONE, FX_5, FX_7, 511,
		FX_43, FX_6, FX_NONE, 512,
		FX_43, FX_0, FX_NONE, 513,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_7, 512,
		FX_43, FX_6, FX_7, 512,
		FX_NONE, FX_NONE, FX_NONE, 513,
		FX_43, FX_NONE, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_6, FX_NONE, 513,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

	// Bat bite
	{
		kDamageBullet,
		4,
		0,
		921,
		0,
		0,
		1,
		4096,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	// Eel bite
	{
		kDamageBullet,
		12,
		0,
		1177,
		0,
		0,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	// Gill bite
	{
		kDamageBullet,
		9,
		0,
		1177,
		0,
		0,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Beast slash
	{
		kDamageExplode,
		50,
		43690,
		1024,
		8192,
		0,
		4,
		32768,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Axe
	{
		kDamageBullet,
		18,
		436906,
		1024,
		16384,
		0,
		2,
		20480,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

			// Cleaver
	{
		kDamageBullet,
		9,
		218453,
		1024,
		0,
		0,
		1,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

			// Phantasm slash
	{
		kDamageBullet,
		20,
		436906,
		1024,
		16384,
		0,
		3,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

			// Gargoyle Slash
	{
		kDamageBullet,
		16,
		218453,
		1024,
		8192,
		0,
		4,
		20480,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
	},

			// Cerberus bite
	{
		kDamageBullet,
		19,
		218453,
		614,
		8192,
		0,
		2,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Hound bite
	{
		kDamageBullet,
		10,
		218453,
		614,
		8192,
		0,
		2,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Rat bite
	{
		kDamageBullet,
		4,
		0,
		921,
		0,
		0,
		1,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Spider bite
	{
		kDamageBullet,
		8,
		0,
		614,
		0,
		0,
		1,
		24576,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

			// Unk
	{
		kDamageBullet,
		9,
		0,
		512,
		0,
		0,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_5, FX_NONE, 500,
		FX_NONE, FX_5, FX_NONE, 501,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_0, FX_NONE, 503,
		FX_NONE, FX_4, FX_NONE, -1,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_6, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 502,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	{
		(DAMAGE_TYPE)-1,
		0,
		0,
		2560,
		0,
		0,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_34, FX_35, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	// Tchernobog burn vector
	{
		kDamageBurn,
		2,
		0,
		0,
		0,
		15,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	// Vodoo 1.0 vector
	{
		kDamageSpirit,
		25,
		0,
		0,
		0,
		0,
		0,
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, -1,
	},

	// 22 kVectorGenDudePunch
	{
	kDamageFall, 
		37, 
		874762, 
		620, 
		0, 
		0, 
		0, 
		0,
		FX_NONE, FX_NONE, FX_NONE, -1,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
		FX_NONE, FX_NONE, FX_NONE, 357,
	},
};

const EXPLOSION explodeInfo[] = {
	{
		40,
		10,
		10,
		75,
		450,
		0,
		60,
		80,
		40
	},
	{
		80,
		20,
		10,
		150,
		900,
		0,
		60,
		160,
		60
	},
	{
		120,
		40,
		15,
		225,
		1350,
		0,
		60,
		240,
		80
	},
	{
		80,
		5,
		10,
		120,
		20,
		10,
		60,
		0,
		40
	},
	{
		120,
		10,
		10,
		180,
		40,
		10,
		60,
		0,
		80
	},
	{
		160,
		15,
		10,
		240,
		60,
		10,
		60,
		0,
		120
	},
	{
		40,
		20,
		10,
		120,
		0,
		10,
		30,
		60,
		40
	},
	{
		80,
		20,
		10,
		150,
		800,
		5,
		60,
		160,
		60
	},
};

static const int16_t gPlayerGibThingComments[] = {
	734, 735, 736, 737, 738, 739, 740, 741, 3038, 3049
};

const int16_t DudeDifficulty[5] = {
	512, 384, 256, 208, 160
};

struct POSTPONE
{
	DBloodActor* sprite;
	int status;
};

TArray<POSTPONE> gPost;

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool IsUnderwaterSector(sectortype* pSector)
{
	return !!pSector->hasX() && pSector->xs().Underwater;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actInitTraps()
{
	BloodStatIterator it(kStatTraps);
	while (auto act = it.Next())
	{
		if (act->GetType() == kTrapExploder)
		{
			act->spr.cstat &= ~CSTAT_SPRITE_BLOCK;
			act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
			if (!act->hasX()) continue;
			act->xspr.waitTime = ClipLow(act->xspr.waitTime, 1);
			act->xspr.state = 0;
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actInitThings()
{
	BloodStatIterator it(kStatThing);
	while (auto act = it.Next())
	{
		if (!act->hasX()) continue;

		int nType = act->GetType() - kThingBase;

		act->xspr.health = act->IntVar("defhealth");
		// allow levels to set custom clipdist.
		if (act->clipdist == 0 || !(currentLevel->featureflags & kFeatureCustomClipdist))
			act->clipdist = act->FloatVar("defclipdist");

		act->spr.flags = act->IntVar("defflags");
		if (act->spr.flags & kPhysGravity) act->spr.flags |= kPhysFalling;
		act->vel.Zero();

		switch (act->GetType())
		{
		case kThingArmedProxBomb:
		case kTrapMachinegun:
#ifdef NOONE_EXTENSIONS
		case kModernThingTNTProx:
#endif
			act->xspr.state = 0;
			break;
		case kThingBloodChunks:
		{
			SEQINST* pInst = GetInstance(act);
			if (pInst)
			{
				seqSpawn(pInst->nName, pInst->nSeqID, act);
			}
			break;
		}
		default:
			act->xspr.state = 1;
			break;
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actInitDudes()
{
	if (gGameOptions.nMonsterSettings == 0)
	{
		Level.setKills(0);
		BloodStatIterator it(kStatDude);
		while (auto act = it.Next())
		{
			if (act->hasX() && act->xspr.key > 0) // Drop Key
				actDropObject(act, kItemKeyBase + (act->xspr.key - 1));
			DeleteSprite(act);
		}
	}
	else
	{
		int TotalKills = 0;

		BloodStatIterator it(kStatDude);
		while (auto act = it.Next())
		{
			if (!act->IsDudeActor())
				I_Error("Non-enemy sprite (%d) in the enemy sprite list.\n", act->GetIndex());
			if (AllowedKillType(act)) TotalKills++;
		}
		Level.setKills(TotalKills);

		it.Reset(kStatDude);
		while (auto act = it.Next())
		{
			///////////////

			auto startdamage = static_cast<DBloodActor*>(GetDefaultByType(act->GetClass()))->dmgControl;
			for (int j = 0; j < 7; j++)
				act->dmgControl[j] = MulScale(DudeDifficulty[gGameOptions.nDifficulty], startdamage[j], 8);


			if (!act->hasX()) continue;

			if (!act->IsPlayerActor())
			{
				act->clipdist = act->fClipDist();
				act->spr.cstat |= CSTAT_SPRITE_BLOOD_PUSHABLE | CSTAT_SPRITE_BLOCK_ALL;
				act->vel.Zero();

				if ((currentLevel->featureflags & kFeatureEnemyAttacks) && act->xspr.sysData2 > 0)
					act->xspr.health = clamp(act->xspr.sysData2 << 4, 1, 65535);
				else
					act->xspr.health = act->startHealth() << 4;
			}
			act->spr.xint = 0;	// not used by default, but if pre-inited it can be used as an internal flag.

			seqSpawn(act->seqStartName(), act->seqStartID(), act);
		}
		aiInit();
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actInit(TArray<DBloodActor*>& actors)
{
#ifdef NOONE_EXTENSIONS
	if (!gModernMap) nnExtResetGlobals();
	else nnExtInitModernStuff(actors);
#endif

	BloodStatIterator it(kStatItem);
	while (auto act = it.Next())
	{
		if (act->GetType() == kItemWeaponVoodooDoll)
		{
			act->ChangeType(kItemAmmoVoodooDoll);
			break;
		}
	}

	actInitTraps();
	actInitThings();
	actInitDudes();
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void ConcussSprite(DBloodActor* source, DBloodActor* actor, const DVector3& pos, double damage)
{
	auto vect = actor->spr.pos - pos;
	double dist2 = vect.LengthSquared() + 0x400;
	damage *= 0x400 / dist2;

	if (actor->spr.flags & kPhysMove)
	{
		if (!actor->IsDudeActor() && !actor->IsThingActor())
		{
			Printf(PRINT_HIGH, "Unexpected type in ConcussSprite(): Sprite: %d  Type: %d  Stat: %d", actor->GetIndex(), (int)actor->GetType(), (int)actor->spr.statnum);
			return;
		}

		double mass = actor->mass();
		if (mass > 0)
		{
			auto tex = TexMan.GetGameTexture(actor->spr.spritetexture());
			double size = tex->GetDisplayWidth() * actor->spr.scale.X * tex->GetDisplayHeight() * actor->spr.scale.Y * (1. / 0x20000);
			actor->vel += vect * Scale(damage, size, mass);
		}
	}
	actDamageSprite(source, actor, kDamageExplode, int(damage));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actWallBounceVector(DBloodActor* actor, walltype* pWall, double factor)
{
	auto delta = pWall->delta().Unit();
	auto vel = actor->vel.XY();
	double t = vel.X * -delta.Y + vel.Y * delta.X;
	double t2 = t * (factor+1);
	actor->vel.XY() = (vel - DVector2(-delta.Y * t2, delta.X * t2));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DVector4 actFloorBounceVector(DBloodActor* actor, double oldz, sectortype* pSector, double factor)
{
	double t = 1. - factor;
	if (pSector->floorheinum == 0)
	{
		double t2 = oldz * t;
		return { actor->vel.X, actor->vel.Y, oldz - t2, t2};
	}

	DVector3 p(actor->vel.XY(), oldz);
	DAngle angle = pSector->walls[0].delta().Angle() + DAngle90;
	auto t2 = pSector->floorheinum * (1. / SLOPEVAL_FACTOR);
	auto t3 = DVector2(-1, t2).Length();
	auto t4 = DVector3(angle.ToVector() * (t2 / t3), -1 / t3);
	auto t8 = actor->vel.dot(t4);
	return DVector4(p - (t4 * t8 * (1 + factor)), t8 * t);
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actRadiusDamage(DBloodActor* source, const DVector3& pos, sectortype* pSector, int nDist, int baseDmg, int distDmg, DAMAGE_TYPE dmgType, int flags, int burn)
{
	auto pOwner = source->GetOwner();
	const bool newSectCheckMethod = !cl_bloodvanillaexplosions && pOwner && pOwner->IsDudeActor() && !VanillaMode(); // use new sector checking logic
	auto sectorMap = GetClosestSpriteSectors(pSector, pos.XY(), nDist, nullptr, newSectCheckMethod);
	if (flags & 2)
	{
		BloodStatIterator it(kStatDude);
		while (auto act2 = it.Next())
		{
			if (act2 != source || (flags & 1))
			{
				if (act2->hasX())
				{
					if (act2->spr.flags & 0x20) continue;
					if (!CheckSector(sectorMap, act2)) continue;
					if (!CheckProximity(act2, pos, pSector, nDist << 4)) continue;

					double dist = (pos - act2->spr.pos).Length();
					if (dist > nDist) continue;

					int totaldmg;
					if (dist != 0) totaldmg = int(baseDmg + ((nDist - dist) * distDmg) / nDist);
					else totaldmg = baseDmg + distDmg;

					actDamageSprite(source, act2, dmgType, totaldmg << 4);
					if (burn) actBurnSprite(pOwner, act2, burn);
				}
			}
		}
	}
	if (flags & 4)
	{
		BloodStatIterator it(kStatDude);
		while (auto act2 = it.Next())
		{
			if (act2->spr.flags & 0x20) continue;
			if (!CheckSector(sectorMap, act2)) continue;
			if (!CheckProximity(act2, pos, pSector, nDist << 4)) continue;

			if (act2->xspr.locked) continue;

			double dist = (pos.XY() - act2->spr.pos.XY()).Length();
			if (dist > nDist) continue;

			int totaldmg;
			if (dist != 0) totaldmg = int(baseDmg + ((nDist - dist) * distDmg) / nDist);
			else totaldmg = baseDmg + distDmg;

			actDamageSprite(source, act2, dmgType, totaldmg << 4);
			if (burn) actBurnSprite(pOwner, act2, burn);
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actNapalmMove(DBloodActor* actor)
{
	auto pOwner = actor->GetOwner();

	actPostSprite(actor, kStatDecoration);
	seqSpawn(NAME_None, 9, actor);
	if (Chance(0x8000)) actor->spr.cstat |= CSTAT_SPRITE_XFLIP;

	sfxPlay3DSound(actor, 303, 24 + (actor->spr.flags & 3), 1);
	actRadiusDamage(pOwner, actor->spr.pos, actor->sector(), 128, 0, 60, kDamageExplode, 15, 120);

	if (actor->xspr.data4 > 1)
	{
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		int spawnparam[2];
		spawnparam[0] = actor->xspr.data4 >> 1;
		spawnparam[1] = actor->xspr.data4 - spawnparam[0];
		auto ang = actor->spr.Angles.Yaw;
		actor->vel.Zero();
		for (int i = 0; i < 2; i++)
		{
			double t1 = RandomD(3.2) + 3.2;
			auto rndang = Random2A(0x71);
			actor->spr.Angles.Yaw = (ang + rndang).Normalized360();
			auto spawned = actFireThing(actor, 0., 0., -0.5774, kThingNapalmBall, t1);
			spawned->SetOwner(actor->GetOwner());
			seqSpawn(NAME_None, 61, spawned, AF(NapalmSeqCallback));
			spawned->xspr.data4 = spawnparam[i];
		}
	}
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool actHealDude(DBloodActor* actor, int add, int threshold)
{
	if (!actor) return false;
	add <<= 4;
	threshold <<= 4;
	if (actor->xspr.health < (unsigned)threshold)
	{
		if (actor->IsPlayerActor()) sfxPlay3DSectorSound(actor->spr.pos, 780, actor->sector());
		actor->xspr.health = min<uint32_t>(actor->xspr.health + add, threshold);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static bool actKillDudeStage1(DBloodActor* actor, DAMAGE_TYPE damageType)
{
	switch (actor->GetType())
	{
#ifdef NOONE_EXTENSIONS
	case kDudeModernCustom:
		if (actKillModernDude(actor, damageType)) return true;
		break;
#endif
	case kDudeCerberusTwoHead: // Cerberus
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 1, actor, nullptr);
		return true;

	case kDudeCultistTommy:
	case kDudeCultistShotgun:
	case kDudeCultistTesla:
	case kDudeCultistTNT:
		if (damageType == kDamageBurn && actor->xspr.medium == kMediumNormal)
		{
			actor->ChangeType(kDudeBurningCultist);
			aiNewState(actor, NAME_BurnGoto);
			actHealDude(actor, actor->startHealth(), actor->startHealth());
			return true;
		}
		break;

	case kDudeBeast:
		if (damageType == kDamageBurn && actor->xspr.medium == kMediumNormal)
		{
			actor->ChangeType(kDudeBurningBeast);
			aiNewState(actor, NAME_BurnGoto);
			actHealDude(actor, actor->startHealth(), actor->startHealth());
			return true;
		}
		break;

	case kDudeInnocent:
		if (damageType == kDamageBurn && actor->xspr.medium == kMediumNormal)
		{
			actor->ChangeType(kDudeBurningInnocent);
			aiNewState(actor, NAME_BurnGoto);
			actHealDude(actor, actor->startHealth(), actor->startHealth());
			return true;
		}
		break;

	case kDudeTinyCaleb:
		if (cl_bloodvanillaenemies || VanillaMode())
			break;
		if (damageType == kDamageBurn && actor->xspr.medium == kMediumNormal)
		{
			actor->ChangeType(kDudeBurningTinyCaleb);
			aiNewState(actor, NAME_BurnGoto);
			actHealDude(actor, actor->startHealth(), actor->startHealth());
			return true;
		}
		break;
	}
	return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void checkAddFrag(DBloodActor* killerActor, DBloodActor* actor)
{
	if (VanillaMode())
	{
		if (killerActor->IsPlayerActor())
		{
			auto pPlayer = getPlayer(killerActor);
			if (gGameOptions.nGameType == 1)
				pPlayer->fragCount++;
		}
	}
	else if (gGameOptions.nGameType == 1 && killerActor->IsPlayerActor() && actor->spr.statnum == kStatDude)
	{
		if (AllowedKillType(actor))
		{
			auto pKillerPlayer = getPlayer(killerActor);
			pKillerPlayer->fragCount++;
		}

	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void checkDropObjects(DBloodActor* actor)
{
	if (actor->xspr.key > 0) actDropObject(actor, kItemKeyBase + actor->xspr.key - 1);
	if (actor->xspr.dropMsg > 0) actDropObject(actor, actor->xspr.dropMsg);

	switch (actor->GetType())
	{
	case kDudeCultistTommy:
	{
		int nRand = Random(100);
		if (nRand < 10) actDropObject(actor, kItemWeaponTommygun);
		else if (nRand < 50) actDropObject(actor, kItemAmmoTommygunFew);
		break;
	}
	case kDudeCultistShotgun:
	{
		int nRand = Random(100);
		if (nRand <= 10) actDropObject(actor, kItemWeaponSawedoff);
		else if (nRand <= 50) actDropObject(actor, kItemAmmoSawedoffFew);
		break;
	}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static int checkDamageType(DBloodActor* actor, DAMAGE_TYPE damageType)
{
	int nSeq;

	switch (damageType)
	{
	case kDamageExplode:
	{
		nSeq = 2;

		auto snd = actor->SoundVar(NAME_explodeSound);
		sfxPlay3DSound(actor, snd, -1, 0);
		break;
	}
	case kDamageBurn:
		nSeq = 3;
		sfxPlay3DSound(actor, 351, -1, 0);
		break;
	case kDamageSpirit:
		switch (actor->GetType()) {
		case kDudeZombieAxeNormal:
		case kDudeZombieAxeBuried:
			nSeq = 14;
			break;
		case kDudeZombieButcher:
			nSeq = 11;
			break;
		default:
			nSeq = 1;
			break;
		}
		break;
	case kDamageFall:
		nSeq = 1;
		break;
	default:
		nSeq = 1;
		break;
	}
	return nSeq;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void spawnGibs(DBloodActor* actor, int type, fixed_t velz)
{
	double top, bottom;
	GetActorExtents(actor, &top, &bottom);
	DVector3 gibPos(actor->spr.pos.XY(), top);
	DVector3 gibVel(actor->vel.XY() * 0.5, FixedToFloat(velz));
	GibSprite(actor, GIBTYPE_27, &gibPos, &gibVel);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void zombieAxeNormalDeath(DBloodActor* actor, int nSeq)
{
	sfxPlay3DSound(actor, 1107 + Random(2), -1, 0);
	if (nSeq == 2)
	{
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor, AF(DudeToGibCallback1));
		spawnGibs(actor, GIBTYPE_27, -0xccccc);
	}
	else if (nSeq == 1 && Chance(0x4000))
	{
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 7, actor, AF(DudeToGibCallback1));
		evPostActor(actor, 0, AF(fxZombieBloodSpurt));
		sfxPlay3DSound(actor, 362, -1, 0);
		actor->xspr.data1 = 35;
		actor->xspr.data2 = 5;

		spawnGibs(actor, GIBTYPE_27, -0x111111);
	}
	else if (nSeq == 14)seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor, nullptr);
	else if (nSeq == 3) seqSpawn(actor->seqStartName(), actor->seqStartID() + 13, actor, AF(DudeToGibCallback2));
	else seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor, AF(DudeToGibCallback1));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void burningCultistDeath(DBloodActor* actor, int nSeq)
{
	if (Chance(0x4000) && nSeq == 3) sfxPlay3DSound(actor, 718, -1, 0);
	else sfxPlay3DSound(actor, 1018 + Random(2), -1, 0);

	if (Chance(0x8000))
	{
		for (int i = 0; i < 3; i++)
			GibSprite(actor, GIBTYPE_7, nullptr, nullptr);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 16 - Random(1), actor, AF(DudeToGibCallback1));
	}
	else
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 15, actor, AF(DudeToGibCallback2));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void zombieAxeBurningDeath(DBloodActor* actor, int nSeq)
{
	if (Chance(0x8000) && nSeq == 3)
		sfxPlay3DSound(actor, 1109, -1, 0);
	else
		sfxPlay3DSound(actor, 1107 + Random(2), -1, 0);

	if (Chance(0x8000))
	{
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 13, actor, AF(DudeToGibCallback1));
		spawnGibs(actor, GIBTYPE_27, -0xccccc);
	}
	else
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 13, actor, AF(DudeToGibCallback2));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void zombieButcherDeath(DBloodActor* actor, int nSeq)
{
	if (nSeq == 14)
	{
		sfxPlay3DSound(actor, 1206, -1, 0);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 11, actor);
		return;
	}
	sfxPlay3DSound(actor, 1204 + Random(2), -1, 0);
	if (nSeq == 3)
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 10, actor);
	else
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void genericDeath(DBloodActor* actor, int nSeq, int sound1, int seqnum)
{
	if (Chance(0x4000) && nSeq == 3) sfxPlay3DSound(actor, sound1 + 2, -1, 0);
	else sfxPlay3DSound(actor, sound1 + Random(2), -1, 0);
	seqSpawn(seqnum, actor);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actKillDude(DBloodActor* killerActor, DBloodActor* actor, DAMAGE_TYPE damageType, int damage)
{
	assert(actor->IsDudeActor()&& actor->hasX());

	if (actKillDudeStage1(actor, damageType)) return;

	for (int p = connecthead; p >= 0; p = connectpoint2[p])
	{
		if (getPlayer(p)->fragger == actor && getPlayer(p)->deathTime > 0)
			getPlayer(p)->fragger = nullptr;
	}
	if (actor->GetType() != kDudeCultistBeast)
		trTriggerSprite(actor, kCmdOff, killerActor);

	actor->spr.flags |= 7;
	checkAddFrag(killerActor, actor);
	checkDropObjects(actor);

	int nSeq = checkDamageType(actor, damageType);

	if (!getSequence(actor->seqStartName(), actor->seqStartID() + nSeq))
	{
		seqKill(actor);
		AddKill(killerActor, actor);
		actPostSprite(actor, kStatFree);
		return;
	}

	auto Owner = actor->GetOwner();
	switch (actor->GetType())
	{
	case kDudeZombieAxeNormal:
		zombieAxeNormalDeath(actor, nSeq);
		break;

	case kDudeCultistTommy:
	case kDudeCultistShotgun:
	case kDudeCultistTesla:
	case kDudeCultistTNT:
		sfxPlay3DSound(actor, 1018 + Random(2), -1, 0);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor, nSeq == 3 ? AF(DudeToGibCallback2) : AF(DudeToGibCallback1));
		break;

	case kDudeBurningCultist:
		burningCultistDeath(actor, nSeq);
		damageType = kDamageExplode;
		break;

#ifdef NOONE_EXTENSIONS
	case kDudeModernCustom:
		modernCustomDudeDeath(actor, nSeq, damageType);
		genDudePostDeath(actor, damageType, damage);
		return;

	case kDudeModernCustomBurning:
		modernCustomDudeBurningDeath(actor, nSeq);
		genDudePostDeath(actor, kDamageExplode, damage);
		return;
#endif

	case kDudeBurningZombieAxe:
		zombieAxeBurningDeath(actor, nSeq);
		damageType = kDamageExplode;
		break;

	case kDudeBurningZombieButcher:
		genericDeath(actor, nSeq, 1204, 4608 + 10);	// this was hardcoded to the non-burning butcher's base seq.
		break;

	case kDudeBurningInnocent:
		damageType = kDamageExplode;
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 7, actor, AF(DudeToGibCallback1));
		break;

	case kDudeZombieButcher:
		zombieButcherDeath(actor, nSeq);
		break;

	case kDudeGargoyleFlesh:
		genericDeath(actor, nSeq, 1403, actor->seqStartID() + nSeq);
		break;

	case kDudeGargoyleStone:
		genericDeath(actor, nSeq, 1453, actor->seqStartID() + nSeq);
		break;

	case kDudePhantasm:
		genericDeath(actor, nSeq, 1603, actor->seqStartID() + nSeq);
		break;

	case kDudeHellHound:
		genericDeath(actor, nSeq, 1303, actor->seqStartID() + nSeq);
		break;

	case kDudeHand:
		genericDeath(actor, nSeq, 1903, actor->seqStartID() + nSeq);
		break;

	case kDudeSpiderBrown:
		if (Owner) Owner->dudeExtra.birthCounter--;
		genericDeath(actor, nSeq, 1803, actor->seqStartID() + nSeq);
		break;

	case kDudeSpiderRed:
		if (Owner) Owner->dudeExtra.birthCounter--;
		genericDeath(actor, nSeq, 1803, actor->seqStartID() + nSeq);
		break;

	case kDudeSpiderBlack:
		if (Owner) Owner->dudeExtra.birthCounter--;
		genericDeath(actor, nSeq, 1803, actor->seqStartID() + nSeq);
		break;

	case kDudeSpiderMother:
		sfxPlay3DSound(actor, 1850, -1, 0);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
		break;

	case kDudeGillBeast:
		genericDeath(actor, nSeq, 1703, actor->seqStartID() + nSeq);
		break;

	case kDudeBoneEel:
		genericDeath(actor, nSeq, 1503, actor->seqStartID() + nSeq);
		break;

	case kDudeBat:
		genericDeath(actor, nSeq, 2003, actor->seqStartID() + nSeq);
		break;

	case kDudeRat:
		genericDeath(actor, nSeq, 2103, actor->seqStartID() + nSeq);
		break;

	case kDudePodGreen:
	case kDudeTentacleGreen:
	case kDudePodFire:
	case kDudeTentacleFire:
		if ((actor->spr.cstat & CSTAT_SPRITE_YFLIP)) actor->spr.cstat &= ~CSTAT_SPRITE_YFLIP;
		switch (actor->GetType())
		{
		case kDudePodGreen:
			genericDeath(actor, nSeq, 2203, actor->seqStartID() + nSeq);
			break;
		case kDudeTentacleGreen:
			sfxPlay3DSound(actor, damage == 5 ? 2471 : 2472, -1, 0);
			seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
			break;
		case kDudePodFire:
			sfxPlay3DSound(actor, damage == 5 ? 2451 : 2452, -1, 0);
			seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
			break;
		case kDudeTentacleFire:
			sfxPlay3DSound(actor, 2501, -1, 0);
			seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
			break;
		}
		break;

	case kDudePodMother:
	case kDudeTentacleMother:
		genericDeath(actor, nSeq, 2203, actor->seqStartID() + nSeq);
		break;

	case kDudeCerberusTwoHead:
	case kDudeCerberusOneHead:
		genericDeath(actor, nSeq, 2303, actor->seqStartID() + nSeq);
		break;

	case kDudeTchernobog:
		sfxPlay3DSound(actor, 2380, -1, 0);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
		break;

	case kDudeBurningTinyCaleb:
		damageType = kDamageExplode;
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 11, actor, AF(DudeToGibCallback1));
		break;

	case kDudeBeast:
		sfxPlay3DSound(actor, 9000 + Random(2), -1, 0);
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor, nSeq == 3 ? AF(DudeToGibCallback2) : AF(DudeToGibCallback1));
		break;

	case kDudeBurningBeast:
		damageType = kDamageExplode;
		seqSpawn(actor->seqStartName(), actor->seqStartID() + 12, actor, AF(DudeToGibCallback1));
		break;

	default:
		seqSpawn(actor->seqStartName(), actor->seqStartID() + nSeq, actor);
		break;
	}

	if (damageType == kDamageExplode)
	{
		auto gibType = actor->IntArray("gibtype");
		for (int i = 0; i < 3; i++)
			if (gibType[i] > -1)
				GibSprite(actor, (GIBTYPE)gibType[i], nullptr, nullptr);
		for (int i = 0; i < 4; i++)
			fxSpawnBlood(actor, damage);
	}
	AddKill(killerActor, actor);
	actCheckRespawn(actor);
	actor->ChangeType(kThingBloodChunks);
	actPostSprite(actor, kStatThing);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static int actDamageDude(DBloodActor* source, DBloodActor* actor, int damage, DAMAGE_TYPE damageType)
{
	if (!actor->IsDudeActor())
	{
		Printf(PRINT_HIGH, "Bad Dude Failed: initial=%s type=%d %s\n", actor->originalType->TypeName.GetChars(), (int)actor->GetType(), (int)(actor->spr.flags & kHitagRespawn) ? "RESPAWN" : "NORMAL");
		return damage >> 4;
	}

	int nDamageFactor = actor->dmgControl[damageType];

	if (!nDamageFactor) return 0;
	else if (nDamageFactor != 256) damage = MulScale(damage, nDamageFactor, 8);

	if (!actor->IsPlayerActor())
	{
		if (actor->xspr.health <= 0) return 0;
		damage = aiDamageSprite(source, actor, damageType, damage);
		if (actor->xspr.health <= 0)
			actKillDude(source, actor, ((damageType == kDamageExplode && damage < 160) ? kDamageFall : damageType), damage);
	}
	else
	{
		auto pPlayer = getPlayer(actor);
		if (actor->xspr.health > 0 || playerSeqPlaying(pPlayer, 16))
			damage = playerDamageSprite(source, pPlayer, damageType, damage);

	}
	return damage;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static int actDamageThing(DBloodActor* source, DBloodActor* actor, int damage, DAMAGE_TYPE damageType, DBloodPlayer* pSourcePlayer)
{
	assert(actor->IsThingActor());
	int nType = actor->GetType() - kThingBase;
	int nDamageFactor = actor->dmgControl[damageType];

	if (!nDamageFactor) return 0;
	else if (nDamageFactor != 256) damage = MulScale(damage, nDamageFactor, 8);

	actor->xspr.health = ClipLow(actor->xspr.health - damage, 0);
	if (actor->xspr.health <= 0)
	{
		auto Owner = actor->GetOwner();
		switch (actor->GetType())
		{
		case kThingDroppedLifeLeech:
#ifdef NOONE_EXTENSIONS
		case kModernThingEnemyLifeLeech:
#endif
			GibSprite(actor, GIBTYPE_14, nullptr, nullptr);
			actor->xspr.data1 = actor->xspr.data2 = actor->xspr.data3 = actor->xspr.DudeLockout = 0;
			actor->xspr.stateTimer = actor->xspr.data4 = actor->xspr.isTriggered = 0;

#ifdef NOONE_EXTENSIONS
			if (Owner && Owner->GetType() == kDudeModernCustom)
				Owner->SetSpecialOwner(); // indicates if custom dude had life leech.
#endif
			break;

		default:
			if (!(actor->spr.flags & kHitagRespawn))
				actor->SetOwner(source);
			break;
		}

		trTriggerSprite(actor, kCmdOff, source);

		switch (actor->GetType())
		{
		case kThingObjectGib:
		case kThingObjectExplode:
		case kThingBloodBits:
		case kThingBloodChunks:
		case kThingZombieHead:
			if (damageType == 3 && pSourcePlayer && PlayClock > pSourcePlayer->laughCount && Chance(0x4000))
			{
				sfxPlay3DSound(pSourcePlayer->GetActor(), gPlayerGibThingComments[Random(10)], 0, 2);
				pSourcePlayer->laughCount = PlayClock + 3600;
			}
			break;
		case kTrapMachinegun:
			seqSpawn(NAME_None, 28, actor);
			break;

		case kThingFluorescent:
			seqSpawn(NAME_None, 12, actor);
			GibSprite(actor, GIBTYPE_6, nullptr, nullptr);
			break;

		case kThingSpiderWeb:
			seqSpawn(NAME_None, 15, actor);
			break;

		case kThingMetalGrate:
			seqSpawn(NAME_None, 21, actor);
			GibSprite(actor, GIBTYPE_4, nullptr, nullptr);
			break;

		case kThingFlammableTree:
			switch (actor->xspr.data1)
			{
			case -1:
				GibSprite(actor, GIBTYPE_14, nullptr, nullptr);
				sfxPlay3DSectorSound(actor->spr.pos, 312, actor->sector());
				actPostSprite(actor, kStatFree);
				break;

			case 0:
				seqSpawn(NAME_None, 25, actor, AF(TreeToGibCallback));
				sfxPlay3DSound(actor, 351, -1, 0);
				break;

			case 1:
				seqSpawn(NAME_None, 26, actor, AF(TreeToGibCallback));
				sfxPlay3DSound(actor, 351, -1, 0);
				break;
			}
			break;
		}
	}
	return damage;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int actDamageSprite(DBloodActor* source, DBloodActor* actor, DAMAGE_TYPE damageType, int damage)
{
	if (actor->spr.flags & 32 || !actor->hasX())
		return 0;

	if ((actor->xspr.health == 0 && actor->spr.statnum != kStatDude) || actor->xspr.locked)
		return 0;

	if (source == nullptr) source = actor;

	DBloodPlayer* pSourcePlayer = nullptr;
	if (source->IsPlayerActor()) pSourcePlayer = getPlayer(source);
	if (!gGameOptions.bFriendlyFire && IsTargetTeammate(pSourcePlayer, actor)) return 0;

	switch (actor->spr.statnum)
	{
	case kStatDude:
		damage = actDamageDude(source, actor, damage, damageType);
		break;
	case kStatThing:
		damage = actDamageThing(source, actor, damage, damageType, pSourcePlayer);
		break;
	}

	return damage >> 4;
}

//---------------------------------------------------------------------------
//
// this was condensed to the parts actually in use.
//
//---------------------------------------------------------------------------

void actHitcodeToData(int a1, HitInfo* pHitInfo, DBloodActor** pActor, walltype** ppWall)
{
	assert(pHitInfo != nullptr);
	DBloodActor* actor = nullptr;
	walltype* pWall = nullptr;
	switch (a1)
	{
	case 3:
	case 5:
		actor = pHitInfo->actor();
		break;
	case 0:
	case 4:
		pWall = pHitInfo->hitWall;
		break;
	default:
		break;
	}
	if (pActor) *pActor = actor;
	if (ppWall) *ppWall = pWall;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actImpactMissile(DBloodActor* missileActor, int hitCode)
{
	auto missileOwner = missileActor->GetOwner();

	DBloodActor* actorHit = nullptr;
	walltype* pWallHit = nullptr;

	actHitcodeToData(hitCode, &gHitInfo, &actorHit, &pWallHit);

	bool pIsThing = false;
	bool pIsDude = false;

	if (hitCode == 3 && actorHit)
	{
		switch (actorHit->spr.statnum)
		{
		case kStatThing:
			pIsThing = true;
			break;
		case kStatDude:
			pIsDude = true;
			break;
		}
	}
	switch (missileActor->GetType())
	{
	case kMissileLifeLeechRegular:
		if (hitCode == 3 && actorHit && (pIsThing || pIsDude))
		{
			DAMAGE_TYPE rand1 = (DAMAGE_TYPE)Random(7);
			int rand2 = (7 + Random(7)) << 4;
			int nDamage = actDamageSprite(missileOwner, actorHit, rand1, rand2);
			int nThingDamage = actorHit->dmgControl[kDamageBurn];

			if (nThingDamage != 0)
				actBurnSprite(missileActor->GetOwner(), actorHit, 360);

			// by NoOne: make Life Leech heal user, just like it was in 1.0x versions
			if (gGameOptions.weaponsV10x && !VanillaMode() && pIsDude)
			{
				if (missileOwner->IsDudeActor() && missileOwner->hasX() && missileOwner->xspr.health != 0)
					actHealDude(missileOwner, nDamage >> 2, missileOwner->startHealth());
			}
		}

		if (missileActor->hasX())
		{
			actPostSprite(missileActor, kStatDecoration);
			if (missileActor->spr.Angles.Yaw == DAngle180) sfxPlay3DSound(missileActor, 307, -1, 0);
			missileActor->ChangeType(kSpriteDecoration);
			seqSpawn(NAME_None, 9, missileActor);
		}
		else
		{
			actPostSprite(missileActor, kStatFree);
		}

		break;
	case kMissileTeslaAlt:
		teslaHit(missileActor, hitCode);
		switch (hitCode)
		{
		case 0:
		case 4:
			if (pWallHit)
			{
				auto pFX = gFX.fxSpawnActor(FX_52, missileActor->sector(), missileActor->spr.pos);
				if (pFX) pFX->spr.Angles.Yaw = (pWallHit->delta().Angle() + DAngle90).Normalized360();
			}
			break;
		}
		GibSprite(missileActor, GIBTYPE_24, NULL, NULL);
		actPostSprite(missileActor, kStatFree);
		break;

	case kMissilePukeGreen:
		seqKill(missileActor);
		if (hitCode == 3 && actorHit && (pIsThing || pIsDude))
		{
			int nDamage = (15 + Random(7)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
		}
		actPostSprite(missileActor, kStatFree);
		break;

	case kMissileArcGargoyle:
		sfxKill3DSound(missileActor, -1, -1);
		sfxPlay3DSectorSound(missileActor->spr.pos, 306, missileActor->sector());
		GibSprite(missileActor, GIBTYPE_6, NULL, NULL);

		if (hitCode == 3 && actorHit && (pIsThing || pIsDude))
		{
			int nDamage = (25 + Random(20)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageSpirit, nDamage);
		}
		actPostSprite(missileActor, kStatFree);
		break;

	case kMissileLifeLeechAltNormal:
	case kMissileLifeLeechAltSmall:
		sfxKill3DSound(missileActor, -1, -1);
		sfxPlay3DSectorSound(missileActor->spr.pos, 306, missileActor->sector());

		if (hitCode == 3 && actorHit && (pIsThing || pIsDude))
		{
			int nDmgMul = (missileActor->GetType() == kMissileLifeLeechAltSmall) ? 6 : 3;
			int nDamage = (nDmgMul + Random(nDmgMul)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageSpirit, nDamage);
		}
		actPostSprite(missileActor, kStatFree);
		break;

	case kMissileFireball:
	case kMissileFireballNapalm:
		if (hitCode == 3 && actorHit && (pIsThing || pIsDude))
		{
			if (pIsThing && actorHit->GetType() == kThingTNTBarrel && actorHit->xspr.burnTime == 0)
				evPostActor(actorHit, 0, AF(fxFlameLick));

			int nDamage = (50 + Random(50)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
		}
		actExplodeSprite(missileActor);
		break;

	case kMissileFlareAlt:
		sfxKill3DSound(missileActor, -1, -1);
		actExplodeSprite(missileActor);
		break;

	case kMissileFlareRegular:
		sfxKill3DSound(missileActor, -1, -1);
		if ((hitCode == 3 && actorHit) && (pIsThing || pIsDude))
		{
			int nThingDamage = actorHit->dmgControl[kDamageBurn];
			if (nThingDamage != 0)
			{
				if (pIsThing && actorHit->GetType() == kThingTNTBarrel && actorHit->xspr.burnTime == 0)
					evPostActor(actorHit, 0, AF(fxFlameLick));

				actBurnSprite(missileOwner, actorHit, 480);
				actRadiusDamage(missileOwner, missileActor->spr.pos, missileActor->sector(), 16, 20, 10, kDamageBullet, 6, 480);

				// by NoOne: allow additional bullet damage for Flare Gun
				if (gGameOptions.weaponsV10x && !VanillaMode())
				{
					int nDamage = (20 + Random(10)) << 4;
					actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
				}
			}
			else
			{
				int nDamage = (20 + Random(10)) << 4;
				actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
			}

			if (GetExtInfo(actorHit->spr.spritetexture()).surftype == kSurfFlesh)
			{
				missileActor->spr.setspritetexture(aTexIds[kTexFLESHHIT]);
				missileActor->SetTarget(actorHit);
				missileActor->xspr.TargetPos.Z = (missileActor->spr.pos.Z - actorHit->spr.pos.Z);
				missileActor->xspr.goalAng = (missileActor->spr.pos.XY() - actorHit->spr.pos.XY()).Angle() - actorHit->spr.Angles.Yaw;
				missileActor->xspr.state = 1;
				actPostSprite(missileActor, kStatFlare);
				missileActor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
				break;
			}
		}
		GibSprite(missileActor, GIBTYPE_17, NULL, NULL);
		actPostSprite(missileActor, kStatFree);
		break;

	case kMissileFlameSpray:
	case kMissileFlameHound:
		if (hitCode == 3 && actorHit && actorHit->hasX())
		{
			if ((actorHit->spr.statnum == kStatThing || actorHit->spr.statnum == kStatDude) && actorHit->xspr.burnTime == 0)
				evPostActor(actorHit, 0, AF(fxFlameLick));

			actBurnSprite(missileOwner, actorHit, (4 + gGameOptions.nDifficulty) << 2);
			actDamageSprite(missileOwner, actorHit, kDamageBurn, 8);
		}
		break;

	case kMissileFireballCerberus:
		actExplodeSprite(missileActor);
		if (hitCode == 3 && actorHit && actorHit->hasX())
		{
			if ((actorHit->spr.statnum == kStatThing || actorHit->spr.statnum == kStatDude) && actorHit->xspr.burnTime == 0)
				evPostActor(actorHit, 0, AF(fxFlameLick));

			actBurnSprite(missileOwner, actorHit, (4 + gGameOptions.nDifficulty) << 2);
			actDamageSprite(missileOwner, actorHit, kDamageBurn, 8);
			int nDamage = (25 + Random(10)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
		}
		actExplodeSprite(missileActor);
		break;

	case kMissileFireballTchernobog:
		actExplodeSprite(missileActor);
		if (hitCode == 3 && actorHit && actorHit->hasX())
		{
			if ((actorHit->spr.statnum == kStatThing || actorHit->spr.statnum == kStatDude) && actorHit->xspr.burnTime == 0)
				evPostActor(actorHit, 0, AF(fxFlameLick));

			actBurnSprite(missileOwner, actorHit, 32);
			actDamageSprite(missileOwner, actorHit, kDamageSpirit, 12);
			int nDamage = (25 + Random(10)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageBullet, nDamage);
		}
		actExplodeSprite(missileActor);
		break;

	case kMissileEctoSkull:
		sfxKill3DSound(missileActor, -1, -1);
		sfxPlay3DSectorSound(missileActor->spr.pos, 522, missileActor->sector());
		actPostSprite(missileActor, kStatDebris);
		seqSpawn(NAME_None, 20, missileActor);
		if (hitCode == 3 && actorHit && actorHit->hasX())
		{
			if (actorHit->spr.statnum == kStatDude)
			{
				int nDamage = (25 + Random(10)) << 4;
				actDamageSprite(missileOwner, actorHit, kDamageSpirit, nDamage);
			}
		}
		break;

	case kMissileButcherKnife:
		actPostSprite(missileActor, kStatDebris);
		missileActor->spr.cstat &= ~CSTAT_SPRITE_ALIGNMENT_WALL;
		missileActor->ChangeType(kSpriteDecoration);
		seqSpawn(NAME_None, 20, missileActor);
		if (hitCode == 3 && actorHit && actorHit->hasX())
		{
			if (actorHit->spr.statnum == kStatDude)
			{
				int nDamage = (10 + Random(10)) << 4;
				actDamageSprite(missileOwner, actorHit, kDamageSpirit, nDamage);
				if (missileOwner->xspr.health > 0)
					actHealDude(missileOwner, 10, missileOwner->startHealth());
			}
		}
		break;

	case kMissileTeslaRegular:
		sfxKill3DSound(missileActor, -1, -1);
		sfxPlay3DSectorSound(missileActor->spr.pos, 518, missileActor->sector());
		GibSprite(missileActor, (hitCode == 2) ? GIBTYPE_23 : GIBTYPE_22, NULL, NULL);
		evKillActor(missileActor);
		seqKill(missileActor);
		actPostSprite(missileActor, kStatFree);
		if (hitCode == 3 && actorHit)
		{
			int nDamage = (15 + Random(10)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageTesla, nDamage);
		}
		break;

	default:
		seqKill(missileActor);
		actPostSprite(missileActor, kStatFree);
		if (hitCode == 3 && actorHit)
		{
			int nDamage = (10 + Random(10)) << 4;
			actDamageSprite(missileOwner, actorHit, kDamageFall, nDamage);
		}
		break;
	}

#ifdef NOONE_EXTENSIONS
	if (gModernMap && actorHit && actorHit->hasX() && actorHit->xspr.state != actorHit->xspr.restState && actorHit->xspr.Impact)
		trTriggerSprite(actorHit, kCmdSpriteImpact, missileActor);
#endif
	missileActor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actKickObject(DBloodActor* kicker, DBloodActor* kicked)
{
	double nSpeed = max(kicker->vel.XY().Length() * 2, FixedToFloat(0xaaaaa));
	kicked->vel.X = nSpeed * (kicker->spr.Angles.Yaw + Random2A(85)).Cos();
	kicked->vel.Y = nSpeed * (kicker->spr.Angles.Yaw + Random2A(85)).Sin();
	kicked->vel.Z = nSpeed * -0.5;
	kicked->spr.flags = 7;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actTouchFloor(DBloodActor* actor, sectortype* pSector)
{
	XSECTOR* pXSector = pSector->hasX() ? &pSector->xs() : nullptr;

	bool doDamage = (pXSector && (pSector->type == kSectorDamage || pXSector->damageType > 0));
	// don't allow damage for damage sectors if they are not enabled
#ifdef NOONE_EXTENSIONS
	if (gModernMap && doDamage && pSector->type == kSectorDamage && !pXSector->state)
		doDamage = false;
#endif

	if (doDamage) {
		DAMAGE_TYPE nDamageType;
		if (pSector->type == kSectorDamage) nDamageType = (DAMAGE_TYPE)ClipRange(pXSector->damageType, kDamageFall, kDamageTesla);
		else nDamageType = (DAMAGE_TYPE)ClipRange(pXSector->damageType - 1, kDamageFall, kDamageTesla);

		int nDamage;
		if (pXSector->data) nDamage = ClipRange(pXSector->data, 0, 1000);
		else nDamage = 1000;

		actDamageSprite(actor, actor, nDamageType, Scale(4, nDamage, 120) << 4);
	}
	if (GetExtInfo(pSector->floortexture).surftype == kSurfLava)
	{
		actDamageSprite(actor, actor, kDamageBurn, 16);
		sfxPlay3DSound(actor, 352, 5, 2);
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void checkCeilHit(DBloodActor* actor)
{
	auto& coll = actor->hit.ceilhit;
	switch (coll.type)
	{
	case kHitWall:
		break;
	case kHitSprite:
		auto actor2 = coll.actor();
		if (actor2 && actor2->hasX())
		{
			if ((actor2->spr.statnum == kStatThing || actor2->spr.statnum == kStatDude) && !actor->vel.isZero())
			{
				auto adelta = actor2->spr.pos - actor->spr.pos;
				actor2->vel.XY() += adelta.XY() * (1. / SLOPEVAL_FACTOR);

				if (actor2->spr.statnum == kStatThing)
				{
					auto flags = actor2->IntVar("defflags");
					if (flags & 1) actor2->spr.flags |= 1;
					if (flags & 2) actor2->spr.flags |= 4;
				}
				else
				{
					actor2->spr.flags |= 5;
					// add size shroom abilities
					if ((actor->IsPlayerActor() && isShrunk(actor)) || (actor2->IsPlayerActor() && isGrown(actor2))) 
					{
						int mass1 = actor2->mass();
						int mass2 = actor->mass();
						if (mass1 > mass2)
						{
							int dmg = int(4 * (abs((mass1 - mass2) * actor2->clipdist) - actor->clipdist));
							if (actor2->IsDudeActor())
							{
								if (dmg > 0) actDamageSprite(actor2, actor, (Chance(0x2000)) ? kDamageFall : (Chance(0x4000)) ? kDamageExplode : kDamageBullet, dmg);
								if (Chance(0x0200)) actKickObject(actor2, actor);
							}
						}
					}
					if (!actor->IsPlayerActor() || getPlayer(actor)->godMode == 0)
					{
						switch (actor2->GetType())
						{
						case kDudeTchernobog:
							actDamageSprite(actor2, actor, kDamageExplode, actor->xspr.health << 2);
							break;
#ifdef NOONE_EXTENSIONS
						case kDudeModernCustom:
						case kDudeModernCustomBurning:
							int dmg = 0;
							if (!actor->IsDudeActor() || (dmg = ClipLow((actor2->mass() - actor->mass()) >> 1, 0)) == 0)
								break;

							if (!actor->IsPlayerActor())
							{
								actDamageSprite(actor2, actor, kDamageFall, dmg);
								if (actor->hasX() && !actor->isActive()) aiActivateDude(actor);
							}
							else if (powerupCheck(getPlayer(actor), kPwUpJumpBoots) > 0) actDamageSprite(actor2, actor, kDamageExplode, dmg);
							else actDamageSprite(actor2, actor, kDamageFall, dmg);
							break;
#endif

						}

					}
				}
			}

			if (actor2->GetType() == kTrapSawCircular && actor2->hasX())
			{
				if (!actor2->xspr.state) actDamageSprite(actor, actor, kDamageBullet, 1);
				else {
					actor2->xspr.data1 = 1;
					actor2->xspr.data2 = ClipHigh(actor2->xspr.data2 + 8, 600);
					actDamageSprite(actor, actor, kDamageBullet, 16);
				}
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool IsBurningDude(DBloodActor* actor)
{
	if (actor == nullptr || !actor->IsDudeActor()) return false;
	return actor->classflags() & 1;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void checkHit(DBloodActor* actor)
{
	const auto& coll = actor->hit.hit;
	switch (coll.type)
	{
	case kHitWall:
		break;
	case kHitSprite:
		if (coll.actor() && coll.actor()->hasX())
		{
			auto actor2 = coll.actor();

			// add size shroom abilities
			if ((actor2->IsPlayerActor() && isShrunk(actor2)) || (actor->IsPlayerActor() && isGrown(actor)))
			{
				if (actor->vel.X != 0 && actor2->IsDudeActor())
				{
					int mass1 = actor->mass();
					int mass2 = actor2->mass();
					if (mass1 > mass2)
					{
						actKickObject(actor, actor2);
						sfxPlay3DSound(actor, 357, -1, 1);
						int dmg = (mass1 - mass2) + abs(int(actor->vel.X));
						if (dmg > 0) actDamageSprite(actor, actor2, (Chance(0x2000)) ? kDamageFall : kDamageBullet, dmg);
					}
				}
			}

			switch (actor2->GetType())
			{
			case kThingKickablePail:
				actKickObject(actor, actor2);
				break;

			case kThingZombieHead:
				sfxPlay3DSectorSound(actor->spr.pos, 357, actor->sector());
				actKickObject(actor, actor2);
				actDamageSprite(nullptr, actor2, kDamageFall, 80);
				break;

			default:
				if (IsBurningDude(actor))
				{
					// This does not make sense
					//actor->xspr.burnTime = ClipLow(actor->xspr.burnTime - 4, 0);
					actDamageSprite(actor->GetBurnSource(), actor, kDamageBurn, 8);
				}
				break;
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void checkFloorHit(DBloodActor* actor)
{
	const auto& coll = actor->hit.florhit;
	switch (coll.type)
	{
	case kHitWall:
		break;
	case kHitSector:
		actTouchFloor(actor, coll.hitSector);
		break;
	case kHitSprite:
		if (coll.actor() && coll.actor()->hasX())
		{
			auto actor2 = coll.actor();

			// add size shroom abilities
			if ((actor2->IsPlayerActor() && isShrunk(actor2)) || (actor->IsPlayerActor() && isGrown(actor)))
			{

				int mass1 = actor->mass();
				int mass2 = actor2->mass();

				if (mass1 > mass2 && actor2->IsDudeActor())
				{
					if ((actor2->IsPlayerActor() && Chance(0x500)) || !actor2->IsPlayerActor())
						actKickObject(actor, actor2);

					int dmg = int((mass1 - mass2) + actor->clipdist * 4);
					if (dmg > 0) actDamageSprite(actor, actor2, (Chance(0x2000)) ? kDamageFall : kDamageBullet, dmg);
				}
			}

			DBloodPlayer* pPlayer = nullptr;
			if (actor->IsPlayerActor()) pPlayer = getPlayer(actor);

			switch (actor2->GetType())
			{
			case kThingKickablePail:
				if (pPlayer)
				{
					if (pPlayer->kickPower > PlayClock) return;
					pPlayer->kickPower = PlayClock + 60;
				}
				actKickObject(actor, actor2);
				sfxPlay3DSectorSound(actor->spr.pos, 357, actor->sector());
				sfxPlay3DSound(actor, 374, 0, 0);
				break;
			case kThingZombieHead:
				if (pPlayer)
				{
					if (pPlayer->kickPower > PlayClock) return;
					pPlayer->kickPower = PlayClock + 60;
				}
				actKickObject(actor, actor2);
				sfxPlay3DSectorSound(actor->spr.pos, 357, actor->sector());
				actDamageSprite(nullptr, actor2, kDamageFall, 80);
				break;
			case kTrapSawCircular:
				if (!actor2->xspr.state) actDamageSprite(actor, actor, kDamageBullet, 1);
				else
				{
					actor2->xspr.data1 = 1;
					actor2->xspr.data2 = ClipHigh(actor2->xspr.data2 + 8, 600);
					actDamageSprite(actor, actor, kDamageBullet, 16);
				}
				break;
			default:
				if (actor2->IsDudeActor() && (actor2->classflags() & 2))
				if (pPlayer && !isShrunk(actor))
					actDamageSprite(actor, actor2, kDamageBullet, 8);
				break;
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void ProcessTouchObjects(DBloodActor* actor)
{
	checkCeilHit(actor);
	checkHit(actor);
	checkFloorHit(actor);

#ifdef NOONE_EXTENSIONS
	// add more trigger statements for Touch flag
	if (gModernMap && actor->IsDudeActor())
	{
		DBloodActor* actor2 = nullptr;
		for (auto* coll : { &actor->hit.hit, &actor->hit.florhit, &actor->hit.ceilhit })
		{
			if (coll->type == kHitSprite)
			{
				actor2 = coll->actor();
				break;
			}
		}

		if (actor2 && actor2->hasX())
		{
			triggerTouchSprite(actor, actor2);
		}

		// Touch walls
		const auto& coll = actor->hit.hit;
		walltype* pHWall = nullptr;
		if (coll.type == kHitWall)
		{
			pHWall = coll.hitWall;
			if (pHWall && pHWall->hasX())
			{
				triggerTouchWall(actor, pHWall);
			}
		}
	}
#endif
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actAirDrag(DBloodActor* actor, fixed_t drag)
{
	DVector2 windvel{};
	assert(actor->sector());
	sectortype* pSector = actor->sector();
	if (pSector->hasX())
	{
		XSECTOR* pXSector = &pSector->xs();
		if (pXSector->windVel && (pXSector->windAlways || pXSector->busy))
		{
			double wind = FixedToFloat<4>(pXSector->windVel);
			if (!pXSector->windAlways && pXSector->busy) wind *= FixedToFloat(pXSector->busy);
			windvel = pXSector->windAng.ToVector() * wind;
		}
	}
	actor->vel += DVector3(windvel - actor->vel.XY(), -actor->vel.Z) * FixedToFloat(drag);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static Collision MoveThing(DBloodActor* actor)
{
	assert(actor->hasX());
	assert(actor->GetType() >= kThingBase && actor->GetType() < kThingMax);
	auto pSector = actor->sector();
	assert(pSector);
	double top, bottom;
	Collision lhit;

	lhit.setNone();
	GetActorExtents(actor, &top, &bottom);
	const int bakCompat = enginecompatibility_mode;
	if (actor->vel.X != 0 || actor->vel.Y != 0)
	{
		auto bakCstat = actor->spr.cstat;
		actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
		if ((actor->GetOwner()) && !cl_bloodvanillaexplosions && !VanillaMode())
			enginecompatibility_mode = ENGINECOMPATIBILITY_NONE; // improved clipmove accuracy
		ClipMove(actor->spr.pos, &pSector, actor->vel.XY(), actor->clipdist, (actor->spr.pos.Z - top) * 0.25, (bottom - actor->spr.pos.Z) * 0.25, CLIPMASK0, lhit);
		actor->hit.hit = lhit;
		enginecompatibility_mode = bakCompat; // restore
		actor->spr.cstat = bakCstat;
		assert(pSector);
		if (actor->sector() != pSector)
		{
			assert(pSector);
			ChangeActorSect(actor, pSector);
		}

		auto& coll = actor->hit.hit;
		if (coll.type == kHitWall)
		{
			actWallBounceVector(actor, coll.hitWall, actor->FloatVar("bouncefactor"));
			switch (actor->GetType())
			{
			case kThingZombieHead:
				sfxPlay3DSound(actor, 607, 0, 0);
				actDamageSprite(nullptr, actor, kDamageFall, 80);
				break;

			case kThingKickablePail:
				sfxPlay3DSound(actor, 374, 0, 0);
				break;
			}
		}
	}
	else
	{
		assert(pSector);
		auto pSector2 = pSector;
		updatesectorz(actor->spr.pos, &pSector2);
		if (pSector2) pSector = pSector2;
	}

	actor->spr.pos.Z += actor->vel.Z;

	double ceilZ, floorZ;
	Collision ceilColl, floorColl;
	GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, actor->clipdist, CLIPMASK0);
	GetActorExtents(actor, &top, &bottom);

	if ((actor->spr.flags & 2) && bottom < floorZ)
	{
		actor->spr.pos.Z += FixedToFloat<8>(455);
		actor->vel.Z += FixedToFloat(58254);
		if (actor->GetType() == kThingZombieHead)
		{
			auto* fxActor = gFX.fxSpawnActor(FX_27, actor->sector(), actor->spr.pos);
			if (fxActor)
			{
				auto vect1 = DVector2(64./15., 0.).Rotated(DAngle::fromBuild((PlayClock * 11) & 2047));
				auto vect2 = DVector2(vect1.X, 0.).Rotated(DAngle::fromBuild((PlayClock * 5) & 2047));
				auto vect3 = DVector2(vect1.Y, vect2.Y).Rotated(DAngle::fromBuild((PlayClock * 3) & 2047));
				fxActor->vel = actor->vel + DVector3(vect2.X, vect3.X, vect3.Y);
			}
		}
	}
	if (CheckLink(actor)) GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, actor->clipdist, CLIPMASK0);

	GetActorExtents(actor, &top, &bottom);
	if (bottom >= floorZ)
	{
		actTouchFloor(actor, actor->sector());
		actor->hit.florhit = floorColl;
		actor->spr.pos.Z += floorZ - bottom;

		double veldiff = actor->vel.Z - actor->sector()->velFloor;
		if (veldiff > 0)
		{
			actor->spr.flags |= 4;

			auto vec4 = actFloorBounceVector(actor, veldiff, actor->sector(), actor->FloatVar("bouncefactor"));
			actor->vel.XY() = vec4.XY();
			int vax = FloatToFixed(vec4.W);

			int nDamage = MulScale(vax, vax, 30) - actor->IntVar("dmgresist");
			if (nDamage > 0) actDamageSprite(actor, actor, kDamageFall, nDamage);

			actor->vel.Z = vec4.Z;
			if (actor->sector()->velFloor == 0 && abs(actor->vel.Z) < 1)
			{
				actor->vel.Z = 0;
				actor->spr.flags &= ~4;
			}

			switch (actor->GetType())
			{
			case kThingNapalmBall:
				if (actor->vel.Z == 0 || Chance(0xA000)) actNapalmMove(actor);
				break;

			case kThingZombieHead:
				if (abs(actor->vel.Z) > 8)
				{
					sfxPlay3DSound(actor, 607, 0, 0);
					actDamageSprite(nullptr, actor, kDamageFall, 80);
				}
				break;

			case kThingKickablePail:
				if (abs(actor->vel.Z) > 8)
					sfxPlay3DSound(actor, 374, 0, 0);
				break;
			}

			lhit.setSector(pSector);
		}
		else if (actor->vel.Z == 0)

			actor->spr.flags &= ~4;
	}
	else
	{
		actor->hit.florhit.setNone();

		if (actor->spr.flags & 2)
			actor->spr.flags |= 4;
	}

	if (top <= ceilZ)
	{
		actor->hit.ceilhit = ceilColl;
		actor->spr.pos.Z += max(ceilZ - top, 0.);
		if (actor->vel.Z < 0)
		{
			actor->vel.XY() *= 0.75;
			actor->vel.Z *= -0.25;

			switch (actor->GetType())
			{
			case kThingZombieHead:
				if (abs(actor->vel.Z) > 8)
				{
					sfxPlay3DSound(actor, 607, 0, 0);
					actDamageSprite(nullptr, actor, kDamageFall, 80);
				}
				break;

			case kThingKickablePail:
				if (abs(actor->vel.Z) > 8)
					sfxPlay3DSound(actor, 374, 0, 0);
				break;
			}
		}
	}
	else actor->hit.ceilhit.setNone();

	if (bottom >= floorZ)
	{
		double nVel = actor->vel.XY().Length();
		double nVelClipped = min(nVel, (16. / 15.));
		Collision& coll = floorColl;

		if (coll.type == kHitSprite)
		{
			auto hitActor = coll.actor();
			if ((hitActor->spr.cstat & CSTAT_SPRITE_ALIGNMENT_MASK) == CSTAT_SPRITE_ALIGNMENT_FACING)
			{
				actor->vel.XY() += (actor->spr.pos.XY() - hitActor->spr.pos.XY()) / SLOPEVAL_FACTOR;
				lhit = actor->hit.hit;
			}
		}
		if (nVel > 0)
		{
			actor->vel.XY() -= actor->vel.XY() * nVelClipped / nVel;
		}
	}
	if (actor->vel.X != 0 || actor->vel.Y != 0)
		actor->spr.Angles.Yaw = actor->vel.Angle();
	return lhit;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void MoveDude(DBloodActor* actor)
{
	DBloodPlayer* pPlayer = nullptr;
	if (actor->IsPlayerActor()) pPlayer = getPlayer(actor);
	if (!(actor->IsDudeActor()))
	{
		Printf(PRINT_HIGH, "%s is not a dude actor type\n", actor->GetClass()->TypeName.GetChars());
		return;
	}

	double top, bottom;
	GetActorExtents(actor, &top, &bottom);
	double bz = (bottom - actor->spr.pos.Z) / 4;
	double tz = (actor->spr.pos.Z - top) / 4;
	double wdf = actor->clipdist;
	auto pSector = actor->sector();
#ifdef NOONE_EXTENSIONS
	int nAiStateType = (actor->xspr.aiState) ? actor->xspr.aiState->Type : -1;
#endif

	assert(pSector);

	if (actor->vel.X != 0 || actor->vel.Y != 0)
	{
		if (pPlayer && gNoClip)
		{
			actor->spr.pos += actor->vel.XY();
			updatesector(actor->spr.pos, &pSector);
			if (!pSector) pSector = actor->sector();
		}
		else
		{
			auto bakCstat = actor->spr.cstat;
			actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
			// Note: vel is Q16.16, ClipMove wants Q28.4, which passes it on to clipmove which wants Q14.18. Anyone confused yet...?
			ClipMove(actor->spr.pos, &pSector, actor->vel.XY(), wdf, tz, bz, CLIPMASK0, actor->hit.hit);
			if (pSector == nullptr)
			{
				pSector = actor->sector();
				if (actor->spr.statnum == kStatDude || actor->spr.statnum == kStatThing)
					actDamageSprite(actor, actor, kDamageFall, 1000 << 4);
			}

			if (pSector->type >= kSectorPath && pSector->type <= kSectorRotate)
			{
				auto pSector2 = pSector;
				if (pushmove(actor->spr.pos, &pSector2, wdf, tz, bz, CLIPMASK0) == -1)
					actDamageSprite(actor, actor, kDamageFall, 1000 << 4);
				if (pSector2 != nullptr)
					pSector = pSector2;
			}
			assert(pSector);
			actor->spr.cstat = bakCstat;
		}
		auto& coll = actor->hit.hit;
		switch (coll.type)
		{
		case kHitSprite:
		{
			auto hitActor = coll.actor();
			auto Owner = hitActor->GetOwner();

			if (hitActor->spr.statnum == kStatProjectile && !(hitActor->spr.flags & 32) && actor != Owner)
			{
				auto hitInfo = gHitInfo;
				gHitInfo.hitActor = actor;
				actImpactMissile(hitActor, 3);
				gHitInfo = hitInfo;
			}
#ifdef NOONE_EXTENSIONS
			if (!gModernMap && hitActor->hasX() && hitActor->xspr.Touch && !hitActor->xspr.state && !hitActor->xspr.isTriggered)
				trTriggerSprite(coll.actor(), kCmdSpriteTouch, actor);
#else
			if (hitActor->hasX() && hitActor->xspr.Touch && !hitActor->xspr.state && !hitActor->xspr.isTriggered)
				trTriggerSprite(coll.actor(), kCmdSpriteTouch);
#endif

			if (actor->lockout() && hitActor->hasX() && hitActor->xspr.Push && !hitActor->xspr.key && !hitActor->xspr.DudeLockout && !hitActor->xspr.state && !hitActor->xspr.busy && !pPlayer)
				trTriggerSprite(coll.actor(), kCmdSpritePush, actor);

			break;
		}
		case kHitWall:
		{
			walltype* pHitWall = coll.hitWall;
			XWALL* pHitXWall = nullptr;
			if (pHitWall->hasX()) pHitXWall = &pHitWall->xw();

			if (actor->lockout() && pHitXWall && pHitXWall->triggerPush && !pHitXWall->key && !pHitXWall->dudeLockout && !pHitXWall->state && !pHitXWall->busy && !pPlayer)
				trTriggerWall(pHitWall, kCmdWallPush, actor);

			if (pHitWall->twoSided())
			{
				sectortype* pHitSector = pHitWall->nextSector();
				XSECTOR* pHitXSector = pHitSector->hasX() ? &pHitSector->xs() : nullptr;

				if (actor->lockout() && pHitXSector && pHitXSector->Wallpush && !pHitXSector->Key && !pHitXSector->dudeLockout && !pHitXSector->state && !pHitXSector->busy && !pPlayer)
					trTriggerSector(pHitSector, kCmdSectorPush, actor);
			}
			actWallBounceVector(actor, pHitWall, 0);
			break;
		}
		}
	}
	else
	{
		assert(pSector);
		auto pSector2 = pSector;
		updatesectorz(actor->spr.pos, &pSector2);
		if (pSector2) pSector = pSector2;
	}

	XSECTOR* pXSector = pSector->hasX() ? &pSector->xs() : nullptr;

	if (actor->sector() != pSector)
	{
		assert(actor->sector());
		auto pOldSector = actor->sector();
		XSECTOR* pXOldSector = pOldSector->hasX() ? &pOldSector->xs() : nullptr;

		if (pXOldSector && pXOldSector->Exit && (pPlayer || !pXOldSector->dudeLockout))
			trTriggerSector(pOldSector, kCmdSectorExit, actor);
		ChangeActorSect(actor, pSector);

		if (pXSector && pXSector->Enter && (pPlayer || !pXSector->dudeLockout))
		{
			if (pSector->type == kSectorTeleport)
				pXSector->actordata = actor;
			trTriggerSector(pSector, kCmdSectorEnter, actor);
		}

		pSector = actor->sector();
	}
	int bUnderwater = 0;
	int bDepth = 0;
	if (pXSector)
	{
		if (pXSector->Underwater) bUnderwater = 1;
		if (pXSector->Depth) bDepth = 1;
	}
	DBloodActor* pUpperLink = barrier_cast<DBloodActor*>(pSector->upperLink);
	DBloodActor* pLowerLink = barrier_cast<DBloodActor*>(pSector->lowerLink);
	if (pUpperLink && (pUpperLink->GetType() == kMarkerUpWater || pUpperLink->GetType() == kMarkerUpGoo)) bDepth = 1;
	if (pLowerLink && (pLowerLink->GetType() == kMarkerLowWater || pLowerLink->GetType() == kMarkerLowGoo)) bDepth = 1;
	if (pPlayer) wdf += 1;
	if (actor->vel.Z) actor->spr.pos.Z += actor->vel.Z;

	double ceilZ, floorZ;
	Collision ceilColl, floorColl;
	GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, wdf, CLIPMASK0, PARALLAXCLIP_CEILING | PARALLAXCLIP_FLOOR);
	GetActorExtents(actor, &top, &bottom);

	if (actor->spr.flags & 2)
	{
		double vc = FixedToFloat(58254);
		if (bDepth)
		{
			if (bUnderwater)
			{
				double cz = getceilzofslopeptr(pSector, actor->spr.pos);
				if (cz > top)
					vc += ((bottom - cz) * -FixedToFloat(80099)) / (bottom - top);
				else
					vc = 0;
			}
			else
			{
				double fz = getflorzofslopeptr(pSector, actor->spr.pos);
				if (fz < bottom)
					vc += ((bottom - fz) * -FixedToFloat(80099)) / (bottom - top);
			}
		}
		else
		{
			if (bUnderwater)
				vc = 0;
			else if (bottom >= floorZ)
				vc = 0;
		}
		if (vc)
		{
			actor->spr.pos.Z += (vc * 4) / 2;
			actor->vel.Z += vc;
		}
	}
	if (pPlayer && actor->vel.Z > FixedToFloat(0x155555) && !pPlayer->fallScream && actor->xspr.height > 0)
	{
		const bool playerAlive = (actor->xspr.health > 0) || VanillaMode(); // only trigger falling scream if player is alive or vanilla mode
		if (playerAlive)
		{
			pPlayer->fallScream = 1;
			sfxPlay3DSound(actor, 719, 0, 0);
		}
	}
	DVector3 const oldpos = actor->spr.pos;
	int nLink = CheckLink(actor);
	if (nLink)
	{
		GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, wdf, CLIPMASK0, PARALLAXCLIP_CEILING | PARALLAXCLIP_FLOOR);
		if (pPlayer)
			playerCorrectInertia(pPlayer, oldpos);
		switch (nLink)
		{
		case kMarkerLowWater:
		case kMarkerLowGoo:
			actor->xspr.medium = kMediumNormal;
			if (pPlayer)
			{
				pPlayer->posture = 0;
				pPlayer->bubbleTime = 0;
				if (!pPlayer->cantJump && (pPlayer->cmd.ucmd.actions & SB_JUMP))
				{
					actor->vel.Z = FixedToFloat(-0x6aaaa);
					pPlayer->cantJump = 1;
				}
				sfxPlay3DSound(actor, 721, -1, 0);
			}
			else
			{
				switch (actor->GetType())
				{
				case kDudeCultistTommy:
				case kDudeCultistShotgun:
					aiNewState(actor, NAME_cultistGoto);
					break;
				case kDudeGillBeast:
					aiNewState(actor, NAME_gillBeastGoto);
					actor->spr.flags |= 6;
					break;
				case kDudeBoneEel:
					actKillDude(actor, actor, kDamageFall, 1000 << 4);
					break;
				}

#ifdef NOONE_EXTENSIONS
				if (actor->IsDudeActor() && actor->xspr.health > 0 && aiInPatrolState(nAiStateType))
					aiPatrolState(actor, kAiStatePatrolMoveL); // continue patrol when going from water
#endif
			}
			break;
		case kMarkerUpWater:
		case kMarkerUpGoo:
		{
			int chance = 0xa00; int medium = kMediumWater;
			if (nLink == kMarkerUpGoo) {
				medium = kMediumGoo;
				chance = 0x400;
			}

			actor->xspr.medium = medium;

			if (pPlayer)
			{
#ifdef NOONE_EXTENSIONS
				// look for palette in data2 of marker. If value <= 0, use default ones.
				if (gModernMap)
				{
					pPlayer->nWaterPal = 0;
					auto pUpper = barrier_cast<DBloodActor*>(pSector->upperLink);
					if (pUpper && pUpper->hasX()) pPlayer->nWaterPal = pUpper->xspr.data2;
				}
#endif

				pPlayer->posture = 1;
				actor->xspr.burnTime = 0;
				pPlayer->bubbleTime = int(abs(actor->vel.Z * 16));
				evPostActor(actor, 0, AF(PlayerBubble));
				sfxPlay3DSound(actor, 720, -1, 0);
			}
			else
			{
				switch (actor->GetType())
				{
				case kDudeCultistTommy:
				case kDudeCultistShotgun:
					actor->xspr.burnTime = 0;
					evPostActor(actor, 0, AF(EnemyBubble));
					sfxPlay3DSound(actor, 720, -1, 0);
					aiNewState(actor, NAME_cultistSwimGoto);
					break;
				case kDudeBurningCultist:
				{
					const bool fixRandomCultist = !cl_bloodvanillaenemies && actor->WasDudeActor() && actor->originalType != actor->GetClass() && !VanillaMode(); // fix burning cultists randomly switching types underwater
					if (Chance(chance))
						actor->ChangeType(kDudeCultistTommy);
					else
						actor->ChangeType(kDudeCultistShotgun);
					if (fixRandomCultist) // fix burning cultists randomly switching types underwater
						actor->ChangeType(actor->originalType); // restore back to spawned cultist type
					actor->xspr.burnTime = 0;
					evPostActor(actor, 0, AF(EnemyBubble));
					sfxPlay3DSound(actor, 720, -1, 0);
					aiNewState(actor, NAME_cultistSwimGoto);
					break;
				}
				case kDudeZombieAxeNormal:
					actor->xspr.burnTime = 0;
					evPostActor(actor, 0, AF(EnemyBubble));
					sfxPlay3DSound(actor, 720, -1, 0);
					aiNewState(actor, NAME_zombieAGoto);
					break;
				case kDudeZombieButcher:
					actor->xspr.burnTime = 0;
					evPostActor(actor, 0, AF(EnemyBubble));
					sfxPlay3DSound(actor, 720, -1, 0);
					aiNewState(actor, NAME_zombieFGoto);
					break;
				case kDudeGillBeast:
					actor->xspr.burnTime = 0;
					evPostActor(actor, 0, AF(EnemyBubble));
					sfxPlay3DSound(actor, 720, -1, 0);
					aiNewState(actor, NAME_gillBeastSwimGoto);

					actor->spr.flags &= ~6;
					break;
				case kDudeGargoyleFlesh:
				case kDudeHellHound:
				case kDudeSpiderBrown:
				case kDudeSpiderRed:
				case kDudeSpiderBlack:
				case kDudeBat:
				case kDudeRat:
				case kDudeBurningInnocent:
					actKillDude(actor, actor, kDamageFall, 1000 << 4);
					break;
				}

#ifdef NOONE_EXTENSIONS
				if (gModernMap) {

					if (actor->GetType() == kDudeModernCustom) {

						evPostActor(actor, 0, AF(EnemyBubble));
						if (!canSwim(actor)) actKillDude(actor, actor, kDamageFall, 1000 << 4);
						break;
					}

					// continue patrol when fall into water
					if (actor->IsDudeActor() && actor->xspr.health > 0 && aiInPatrolState(nAiStateType))
						aiPatrolState(actor, kAiStatePatrolMoveW);

				}
#endif

			}
			break;
		}
		}
	}
	GetActorExtents(actor, &top, &bottom);
	if (pPlayer && bottom >= floorZ)
	{
		double floorZ2 = floorZ;
		auto floorColl2 = floorColl;
		GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, actor->clipdist, CLIPMASK0, PARALLAXCLIP_CEILING | PARALLAXCLIP_FLOOR);
		if (bottom <= floorZ && actor->spr.pos.Z - floorZ2 < bz)
		{
			floorZ = floorZ2;
			floorColl = floorColl2;
		}
	}
	if (floorZ <= bottom)
	{
		actor->hit.florhit = floorColl;
		actor->spr.pos.Z += floorZ - bottom;
		double veldiff = actor->vel.Z - actor->sector()->velFloor;
		if (veldiff > 0)
		{
			auto vec4 = actFloorBounceVector(actor, veldiff, actor->sector(), 0);
			actor->vel.XY() = vec4.XY();
			int vax = FloatToFixed(vec4.W);

			int nDamage = MulScale(vax, vax, 30);
			if (pPlayer)
			{
				pPlayer->fallScream = 0;

				if (nDamage > (15 << 4) && (actor->spr.flags & 4))
					playerLandingSound(pPlayer);
				if (nDamage > (30 << 4))
					sfxPlay3DSound(actor, 701, 0, 0);
			}
			nDamage -= 100 << 4;
			if (nDamage > 0)
				actDamageSprite(actor, actor, kDamageFall, nDamage);
			actor->vel.Z = vec4.Z;
			if (abs(actor->vel.Z) < 1)
			{
				actor->vel.Z = actor->sector()->velFloor;
				actor->spr.flags &= ~4;
			}
			else
				actor->spr.flags |= 4;

			switch (tileGetSurfType(floorColl))
			{
			case kSurfWater:
				gFX.fxSpawnActor(FX_9, actor->sector(), DVector3(actor->spr.pos.XY(), floorZ));
				break;
			case kSurfLava:
			{
				auto pFX = gFX.fxSpawnActor(FX_10, actor->sector(), DVector3(actor->spr.pos.XY(), floorZ));
				if (pFX)
				{
					for (int i = 0; i < 7; i++)
					{
						auto pFX2 = gFX.fxSpawnActor(FX_14, pFX->sector(), pFX->spr.pos);
						if (pFX2)
						{
							pFX2->vel.X = Random2F(0x6aaaa);
							pFX2->vel.Y = Random2F(0x6aaaa);
							pFX2->vel.Z = -Random2F(0xd5555);
						}
					}
				}
				break;
			}
			}
		}
		else if (actor->vel.Z == 0)

			actor->spr.flags &= ~4;
	}
	else
	{
		actor->hit.florhit.setNone();

		if (actor->spr.flags & 2)
			actor->spr.flags |= 4;
	}
	if (top <= ceilZ)
	{
		actor->hit.ceilhit = ceilColl;
		actor->spr.pos.Z += max(ceilZ - top, 0.);

		if (actor->vel.Z <= 0 && (actor->spr.flags & 4))
			actor->vel.Z = -actor->vel.Z * (1. / 8.);
	}
	else
		actor->hit.ceilhit.setNone();

	GetActorExtents(actor, &top, &bottom);

	actor->xspr.height = int(max(floorZ - bottom, 0.));
	if (actor->vel.X != 0 || actor->vel.Y != 0)
	{
		if (floorColl.type == kHitSprite)
		{
			auto hitAct = floorColl.actor();
			if ((hitAct->spr.cstat & CSTAT_SPRITE_ALIGNMENT_MASK) == CSTAT_SPRITE_ALIGNMENT_FACING)
			{
				actor->vel.XY() += (actor->spr.pos - hitAct->spr.pos).XY() * (1. / SLOPEVAL_FACTOR);
				return;
			}
		}
		if (IsUnderwaterSector(actor->sector()))
			return;
		if (actor->xspr.height >= 0x100)
			return;
		int nDrag = gDudeDrag;
		if (actor->xspr.height > 0)
			nDrag -= Scale(gDudeDrag, actor->xspr.height, 256);

		// this cannot be floatified due to the effect of mulscale16r on the value.
		actor->vel.X += FixedToFloat(-mulscale16r(FloatToFixed(actor->vel.X), nDrag));
		actor->vel.Y += FixedToFloat(-mulscale16r(FloatToFixed(actor->vel.Y), nDrag));

		if (actor->vel.XY().Length() < 0.0625)
		{
			actor->vel.XY().Zero();
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int MoveMissile(DBloodActor* actor)
{
	auto Owner = actor->GetOwner();
	int cliptype = -1;
	ESpriteFlags bakCstat = 0;
	if (Owner)
	{
		bakCstat = Owner->spr.cstat;
		if (Owner->IsDudeActor())
		{
			Owner->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
		}
	}
	gHitInfo.clearObj();
	if (actor->GetType() == kMissileFlameSpray) actAirDrag(actor, 0x1000);

	if (actor->GetTarget() != nullptr && !actor->vel.isZero())
	{
		auto target = actor->GetTarget();

		if (target->spr.statnum == kStatDude && target->hasX() && target->xspr.health > 0)
		{
			double vel = actor->FloatVar("speed");
			actor->vel.XY() = DVector2(vel, 0).Rotated((target->spr.pos - actor->spr.pos).Angle());

			double deltaz = (target->spr.pos.Z - actor->spr.pos.Z) / (10 * 256);

			if (target->spr.pos.Z < actor->spr.pos.Z) deltaz = -deltaz;
			actor->vel.Z += deltaz;
		}
	}
	auto vel = actor->vel;
	double top, bottom;
	GetActorExtents(actor, &top, &bottom);
	int i = 1;
	const int bakCompat = enginecompatibility_mode;
	const bool isFlameSprite = (actor->GetType() == kMissileFlameSpray || actor->GetType() == kMissileFlameHound); // do not use accurate clipmove for flame based sprites (changes damage too much)
	while (1)
	{
		auto ppos = actor->spr.pos;
		auto pSector2 = actor->sector();
		const auto bakSpriteCstat = actor->spr.cstat;
		if (Owner && !isFlameSprite && !cl_bloodvanillaexplosions && !VanillaMode())
		{
			enginecompatibility_mode = ENGINECOMPATIBILITY_NONE; // improved clipmove accuracy
			actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL; // remove self collisions for accurate clipmove
		}
		Collision clipmoveresult;
		ClipMove(ppos, &pSector2, vel.XY(), actor->clipdist, (ppos.Z - top) / 4, (bottom - ppos.Z) / 4, CLIPMASK0, clipmoveresult, 1);
		enginecompatibility_mode = bakCompat; // restore
		actor->spr.cstat = bakSpriteCstat;
		auto pSector = pSector2;
		if (pSector2 == nullptr)
		{
			cliptype = -1;
			break;
		}
		if (clipmoveresult.type == kHitSprite)
		{
			gHitInfo.hitActor = clipmoveresult.actor();
			cliptype = 3;
		}
		else if (clipmoveresult.type == kHitWall)
		{
			gHitInfo.hitWall = clipmoveresult.hitWall;
			if (!gHitInfo.hitWall->twoSided()) cliptype = 0;
			else
			{
				double fz, cz;
				calcSlope(clipmoveresult.hitWall->nextSector(), ppos, &cz, &fz);
				if (ppos.Z <= cz || ppos.Z >= fz) cliptype = 0;
				else cliptype = 4;
			}
		}
		if (cliptype == 4)
		{
			walltype* pWall = clipmoveresult.hitWall;
			if (pWall->hasX())
			{
				XWALL* pXWall = &pWall->xw();
				if (pXWall->triggerVector)
				{
					trTriggerWall(pWall, kCmdWallImpact, Owner? Owner : actor);
					if (!(pWall->cstat & CSTAT_WALL_BLOCK_HITSCAN))
					{
						cliptype = -1;
						if (i-- > 0)
							continue;
						cliptype = 0;
						break;
					}
				}
			}
		}
		if (cliptype >= 0 && cliptype != 3)
		{
			double nVel = actor->vel.XY().Length();
			ppos.XY() -= actor->vel.XY() / nVel;
			vel.Z -= actor->vel.Z / nVel;
			updatesector(ppos, &pSector);
			pSector2 = pSector;
		}
		double ceilZ, floorZ;
		Collision ceilColl, floorColl;
		GetZRangeAtXYZ(ppos, pSector2, &ceilZ, &ceilColl, &floorZ, &floorColl, actor->clipdist, CLIPMASK0);
		GetActorExtents(actor, &top, &bottom);
		top += vel.Z;
		bottom += vel.Z;
		if (bottom >= floorZ)
		{
			actor->hit.florhit = floorColl;
			vel.Z += floorZ - bottom;
			cliptype = 2;
		}
		if (top <= ceilZ)
		{
			actor->hit.ceilhit = ceilColl;
			vel.Z += std::max(ceilZ - top, 0.);
			cliptype = 1;
		}
		actor->spr.pos = ppos.plusZ(vel.Z);
		updatesector(ppos, &pSector);
		if (pSector != nullptr && pSector != actor->sector())
		{
			assert(pSector);
			ChangeActorSect(actor, pSector);
		}
		CheckLink(actor);
		gHitInfo.hitSector = actor->sector();
		gHitInfo.hitpos = actor->spr.pos;
		break;
	}
	if (Owner) Owner->spr.cstat = bakCstat;

	return cliptype;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actExplodeSprite(DBloodActor* actor)
{
	if (!actor->hasX()) return;

	if (actor->spr.statnum == kStatExplosion) return;
	sfxKill3DSound(actor, -1, -1);
	evKillActor(actor);

	int nType = kExplosionStandard;

	switch (actor->GetType())
	{
	case kMissileFireballNapalm:
		nType = kExplosionNapalm;
		seqSpawn(NAME_None, 4, actor);
		if (Chance(0x8000)) actor->spr.cstat |= CSTAT_SPRITE_XFLIP;
		sfxPlay3DSound(actor, 303, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kMissileFlareAlt:
		nType = kExplosionFireball;
		seqSpawn(NAME_None, 9, actor);
		if (Chance(0x8000)) actor->spr.cstat |= CSTAT_SPRITE_XFLIP;
		sfxPlay3DSound(actor, 306, 24 + (actor->GetIndex() & 3), FX_GlobalChannel);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kMissileFireballCerberus:
	case kMissileFireballTchernobog:
		nType = kExplosionFireball;
		seqSpawn(NAME_None, 5, actor);
		sfxPlay3DSound(actor, 304, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kThingArmedTNTStick:
		nType = kExplosionSmall;
		if (actor->hit.florhit.type == kHitNone) seqSpawn(4, actor);
		else seqSpawn(NAME_None, 3, actor);
		sfxPlay3DSound(actor, 303, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kThingArmedProxBomb:
	case kThingArmedRemoteBomb:
	case kThingArmedTNTBundle:
#ifdef NOONE_EXTENSIONS
	case kModernThingTNTProx:
#endif
		nType = kExplosionStandard;
		if (actor->hit.florhit.type == kHitNone) seqSpawn(NAME_None, 4, actor);
		else
			seqSpawn(NAME_None, 3, actor);
		sfxPlay3DSound(actor, 304, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kThingArmedSpray:
		nType = kExplosionSpray;
		seqSpawn(NAME_None, 5, actor);
		sfxPlay3DSound(actor, 307, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;

	case kThingTNTBarrel:
	{
		auto spawned = actSpawnSprite(actor->sector(), actor->spr.pos, 0, 1);
		spawned->SetOwner(actor->GetOwner());
		if (actCheckRespawn(actor))
		{
			actor->xspr.state = 1;
			actor->xspr.health = actor->IntVar("defhealth") << 4;
		}
		else actPostSprite(actor, kStatFree);

		nType = kExplosionLarge;
		seqSpawn(NAME_None, 4, spawned);
		actor = spawned;

		sfxPlay3DSound(actor, 305, -1, 0);
		GibSprite(actor, GIBTYPE_14, nullptr, nullptr);
		break;
	}
	case kTrapExploder:
	{
		// Defaults for exploder
		nType = kExplosionStandard;
		int nSnd = 304;
		int nSeq = 4;

		// allow to customize hidden exploder trap
		if (currentLevel->featureflags & kFeatureCustomTrapExploder)
		{
			nType = actor->xspr.data1;  // Explosion type
			int tSeq = actor->xspr.data2; // SEQ id
			int tSnd = actor->xspr.data3; // Sound Id

			if (nType <= kExplosionStandard || nType > kExplodeMax) { nType = 1; nSeq = 4; nSnd = 304; }
			else if (nType == kExplosionLarge) { nSeq = 4; nSnd = 305; }
			else if (nType == kExplosionFireball) { nSeq = 9; nSnd = 307; }
			else if (nType == kExplosionSpray) { nSeq = 5; nSnd = 307; }
			else if (nType <= kExplosion6) { nSeq = 4; nSnd = 303; }
			else if (nType == kExplosionNapalm) { nSeq = 4; nSnd = 303; }
			else if (nType == kExplosionMax) { nType = 0; nSeq = 3; nSnd = 303; }

			// Override previous sound and seq assigns
			if (tSeq > 0) nSeq = tSeq;
			if (tSnd > 0) nSnd = tSnd;
		}

		seqSpawn(NAME_None, nSeq, actor);
		sfxPlay3DSound(actor, nSnd, -1, 0);
		break;
	}
	case kThingPodFireBall:
		nType = kExplosionFireball;
		seqSpawn(NAME_None, 9, actor);
		sfxPlay3DSound(actor, 307, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		fxSpawnPodStuff(actor, 240);
		break;

	default:
		nType = kExplosionStandard;
		seqSpawn(NAME_None, 4, actor);
		if (Chance(0x8000)) actor->spr.cstat |= CSTAT_SPRITE_XFLIP;
		sfxPlay3DSound(actor, 303, -1, 0);
		GibSprite(actor, GIBTYPE_5, nullptr, nullptr);
		break;
	}
	actor->vel.Zero();
	actPostSprite(actor, kStatExplosion);
	actor->spr.scale = DVector2(explodeInfo[nType].repeat * REPEAT_SCALE, explodeInfo[nType].repeat* REPEAT_SCALE);

	actor->spr.flags &= ~3;
	actor->ChangeType(nType);
	const EXPLOSION* pExplodeInfo = &explodeInfo[nType];
	actor->SetTarget(nullptr);
	actor->explosionhackflag = true;
	actor->xspr.data1 = pExplodeInfo->ticks;
	actor->xspr.data2 = pExplodeInfo->quakeEffect;
	actor->xspr.data3 = pExplodeInfo->flashEffect;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actActivateGibObject(DBloodActor* actor)
{
	int gib1 = ClipRange(actor->xspr.data1, 0, 31);
	int gib2 = ClipRange(actor->xspr.data2, 0, 31);
	int gib3 = ClipRange(actor->xspr.data3, 0, 31);
	int sound = actor->xspr.data4;
	int dropmsg = actor->xspr.dropMsg;

	if (gib1 > 0) GibSprite(actor, (GIBTYPE)(gib1 - 1), nullptr, nullptr);
	if (gib2 > 0) GibSprite(actor, (GIBTYPE)(gib2 - 1), nullptr, nullptr);
	if (gib3 > 0 && actor->xspr.burnTime > 0) GibSprite(actor, (GIBTYPE)(gib3 - 1), nullptr, nullptr);
	if (sound > 0) sfxPlay3DSectorSound(actor->spr.pos, sound, actor->sector());
	if (dropmsg > 0) actDropObject(actor, dropmsg);

	if (!(actor->spr.cstat & CSTAT_SPRITE_INVISIBLE) && !(actor->spr.flags & kHitagRespawn))
		actPostSprite(actor, kStatFree);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actCheckProximity()
{
	BloodStatIterator it(kStatThing);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32) continue;

		if (actor->hasX())
		{
			switch (actor->GetType())
			{
			case kThingBloodBits:
			case kThingBloodChunks:
			case kThingZombieHead:
				if (actor->xspr.locked && PlayClock >= actor->xspr.TargetPos.X) actor->xspr.locked = 0;
				break;
			}

			if (actor->xspr.burnTime > 0)
			{
				actor->xspr.burnTime = ClipLow(actor->xspr.burnTime - 4, 0);
				actDamageSprite(actor->GetBurnSource(), actor, kDamageBurn, 8);
			}

			if (actor->xspr.Proximity)
			{
#ifdef NOONE_EXTENSIONS
				// don't process locked or 1-shot things for proximity
				if (gModernMap && (actor->xspr.locked || actor->xspr.isTriggered))
					continue;
#endif

				if (actor->GetType() == kThingDroppedLifeLeech) actor->SetTarget(nullptr);
				BloodStatIterator it1(kStatDude);
				while (auto dudeactor = it1.Next())
				{
					auto nextdude = it1.Peek();

					if (dudeactor->spr.flags & 32 || !dudeactor->hasX()) continue;

					if ((unsigned int)dudeactor->xspr.health > 0)
					{
						int proxyDist = 96;
#ifdef NOONE_EXTENSIONS
						// allow dudeLockout for proximity flag
						if (gModernMap && actor->GetType() != kThingDroppedLifeLeech && actor->xspr.DudeLockout && !dudeactor->IsPlayerActor())
							continue;

						if (actor->GetType() == kModernThingEnemyLifeLeech) proxyDist = 512;
#endif
						if (actor->GetType() == kThingDroppedLifeLeech && actor->GetTarget() == nullptr)
						{
							auto Owner = actor->GetOwner();
							if (!Owner->IsPlayerActor()) continue;

							auto pPlayer = getPlayer(Owner);
							DBloodPlayer* pPlayer2 = dudeactor->IsPlayerActor() ? getPlayer(dudeactor) : nullptr;

							if (dudeactor == Owner || dudeactor->GetType() == kDudeZombieAxeBuried || dudeactor->GetType() == kDudeRat || dudeactor->GetType() == kDudeBat) continue;
							if (gGameOptions.nGameType == 3 && pPlayer2 && pPlayer->teamId == pPlayer2->teamId) continue;
							if (gGameOptions.nGameType == 1 && pPlayer2) continue;
							proxyDist = 512;
						}

						if (CheckProximity(dudeactor, actor->spr.pos, actor->sector(), proxyDist))
						{
							switch (actor->GetType())
							{
							case kThingDroppedLifeLeech:
								if (!Chance(0x4000) && nextdude) continue;
								if (dudeactor->spr.cstat & CSTAT_SPRITE_BLOCK) actor->SetTarget(dudeactor);
								else continue;
								break;

#ifdef NOONE_EXTENSIONS
							case kModernThingTNTProx:
								if (!dudeactor->IsPlayerActor()) continue;
								actor->spr.pal = 0;
								break;

							case kModernThingEnemyLifeLeech:
								if (actor->GetTarget() != dudeactor) continue;
								break;
#endif

							default:
								break;
							}
							if (actor->GetOwner() == nullptr) actor->SetOwner(dudeactor);
							trTriggerSprite(actor, kCmdSpriteProximity, dudeactor);
						}
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actCheckThings()
{
	BloodStatIterator it(kStatThing);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32) continue;
		if (!actor->hasX()) continue;

		auto pSector = actor->sector();

		XSECTOR* pXSector = pSector->hasX() ? &pSector->xs() : nullptr;
		if (pXSector && pXSector->panVel && (pXSector->panAlways || pXSector->state || pXSector->busy))
		{
			auto flags = actor->IntVar("defflags");
			if (flags & 1) actor->spr.flags |= 1;
			if (flags & 2) actor->spr.flags |= 4;
		}

		if (actor->spr.flags & 3)
		{
			viewBackupSpriteLoc(actor);
			if (pXSector && pXSector->panVel)
			{
				double top, bottom;
				GetActorExtents(actor, &top, &bottom);
				if (getflorzofslopeptr(pSector, actor->spr.pos) <= bottom)
				{
					DAngle angle = pXSector->panAngle;
					double speed = 0;
					if (pXSector->panAlways || pXSector->state || pXSector->busy)
					{
						speed = pXSector->panVel / 128.;
						if (!pXSector->panAlways && pXSector->busy) speed *= FixedToFloat(pXSector->busy);
					}
					if (pSector->floorstat & CSTAT_SECTOR_ALIGN) angle += pSector->walls[0].normalAngle();

					actor->vel += angle.ToVector() * speed;
				}
			}
			actAirDrag(actor, 128);

			if (((actor->GetIndex() >> 8) & 15) == (gFrameCount & 15) && (actor->spr.flags & 2))	actor->spr.flags |= 4;
			if ((actor->spr.flags & 4) || !actor->vel.isZero() || actor->sector()->velFloor || actor->sector()->velCeil)
			{
				Collision hit = MoveThing(actor);
				if (hit.type)
				{
					if (actor->xspr.Impact) trTriggerSprite(actor, kCmdOff, hit.type == kHitSprite? hit.safeActor() : nullptr);
					actOnHit(actor, hit);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actCheckProjectiles()
{
	BloodStatIterator it(kStatProjectile);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32)
			continue;
		viewBackupSpriteLoc(actor);
		int hit = MoveMissile(actor);
		if (hit >= 0) actImpactMissile(actor, hit);
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
static TArray<walltype*> affectedXWalls; // keep this outside the function so that it only needs to be allocated once

static void actCheckExplosion()
{
	BloodStatIterator it(kStatExplosion);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32)
			continue;

		if (!actor->hasX()) continue;

		auto Owner = actor->GetOwner();
		int nType = actor->GetType();
		assert(nType >= 0 && nType < kExplodeMax);
		const EXPLOSION* pExplodeInfo = &explodeInfo[nType];
		const auto apos = actor->spr.pos;
		auto pSector = actor->sector();
		int radius = pExplodeInfo->radius;

#ifdef NOONE_EXTENSIONS
		// Allow to override explosion radius by data4 field of any sprite which have statnum 2 set in editor
		// or of Hidden Exploder.
		if (gModernMap && actor->xspr.data4 > 0)
			radius = actor->xspr.data4;
#endif

		// GetClosestSpriteSectors() has issues checking some sectors due to optimizations
		// the new flag newSectCheckMethod for GetClosestSpriteSectors() does rectify these issues, but this may cause unintended side effects for level scripted explosions
		// so only allow this new checking method for dude spawned explosions
		affectedXWalls.Clear();
		const bool newSectCheckMethod = !cl_bloodvanillaexplosions && Owner && Owner->IsDudeActor() && !VanillaMode(); // use new sector checking logic
		auto sectorMap = GetClosestSpriteSectors(pSector, apos.XY(), radius, &affectedXWalls, newSectCheckMethod);

		for (auto pWall : affectedXWalls)
		{
			trTriggerWall(pWall, kCmdWallImpact, Owner);
		}

		BloodStatIterator it1(kStatDude);
		while (auto dudeactor = it1.Next())
		{
			if (dudeactor->spr.flags & 32) continue;

			if (CheckSector(sectorMap, dudeactor))
			{
				if (actor->xspr.data1 && CheckProximity(dudeactor, apos, pSector, radius))
				{
					if (pExplodeInfo->dmg && actor->explosionhackflag)
					{
						actor->explosionhackflag = false;
						actDamageSprite(Owner, dudeactor, kDamageFall, (pExplodeInfo->dmg + Random(pExplodeInfo->dmgRng)) << 4);
					}
					if (pExplodeInfo->dmgType) ConcussSprite(actor, dudeactor, apos, pExplodeInfo->dmgType);

					if (pExplodeInfo->burnTime && dudeactor->hasX())
					{
						if (!dudeactor->xspr.burnTime) evPostActor(dudeactor, 0, AF(fxFlameLick));
						actBurnSprite(Owner, dudeactor, pExplodeInfo->burnTime << 2);
					}
				}
			}
		}

		it1.Reset(kStatThing);
		while (auto thingactor = it1.Next())
		{
			if (thingactor->spr.flags & 32) continue;

			if (CheckSector(sectorMap, thingactor))
			{
				if (actor->xspr.data1 && CheckProximity(thingactor, apos, pSector, radius) && thingactor->hasX())
				{
					if (!thingactor->xspr.locked)
					{
						if (pExplodeInfo->dmgType) ConcussSprite(Owner, thingactor, apos, pExplodeInfo->dmgType);

						if (pExplodeInfo->burnTime)
						{
							if (thingactor->GetType() == kThingTNTBarrel && !thingactor->xspr.burnTime)
								evPostActor(thingactor, 0, AF(fxFlameLick));
							actBurnSprite(Owner, thingactor, pExplodeInfo->burnTime << 2);
						}
					}
				}
			}
		}

		for (int p = connecthead; p >= 0; p = connectpoint2[p])
		{
			auto dv = apos - getPlayer(p)->GetActor()->spr.pos;
			int nDist = int(dv.LengthSquared() + 0x40000);

			int t = DivScale(actor->xspr.data2, nDist, 16);
			getPlayer(p)->flickerEffect += t;
		}

#ifdef NOONE_EXTENSIONS
		if (actor->xspr.data1 != 0)
		{
			// add impulse for sprites from physics list
			if (gPhysSpritesCount > 0 && pExplodeInfo->dmgType != 0)
			{
				for (int i = 0; i < gPhysSpritesCount; i++)
				{
					if (gPhysSpritesList[i] == nullptr) continue;
					DBloodActor* physactor = gPhysSpritesList[i];
					if (!physactor->insector() || (physactor->spr.flags & kHitagFree) != 0) continue;

					if (!CheckSector(sectorMap, physactor) || !CheckProximity(physactor, apos, pSector, radius)) continue;
					else debrisConcuss(Owner, i, apos, pExplodeInfo->dmgType);
				}
			}

			// trigger sprites from impact list
			if (gImpactSpritesCount > 0) {
				for (int i = 0; i < gImpactSpritesCount; i++)
				{
					if (gImpactSpritesList[i] == nullptr) continue;

					DBloodActor* impactactor = gImpactSpritesList[i];
					if (!impactactor->hasX() || !impactactor->insector() || (impactactor->spr.flags & kHitagFree) != 0)	continue;

					if (!CheckSector(sectorMap, impactactor) || !CheckProximity(impactactor, apos, pSector, radius))
						continue;

					trTriggerSprite(impactactor, kCmdSpriteImpact, Owner);
				}
			}

		}

		if (!gModernMap || !(actor->spr.flags & kModernTypeFlag1))
		{
			// if data4 > 0, do not remove explosion. This can be useful when designer wants put explosion generator in map manually via sprite statnum 2.
			actor->xspr.data1 = ClipLow(actor->xspr.data1 - 4, 0);
			actor->xspr.data2 = ClipLow(actor->xspr.data2 - 4, 0);
			actor->xspr.data3 = ClipLow(actor->xspr.data3 - 4, 0);
		}
#else
		actor->xspr.data1 = ClipLow(actor->xspr.data1 - 4, 0);
		actor->xspr.data2 = ClipLow(actor->xspr.data2 - 4, 0);
		actor->xspr.data3 = ClipLow(actor->xspr.data3 - 4, 0);
#endif

		if (actor->xspr.data1 == 0 && actor->xspr.data2 == 0 && actor->xspr.data3 == 0 && seqGetStatus(actor) < 0)
			actPostSprite(actor, kStatFree);
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actCheckTraps()
{
	BloodStatIterator it(kStatTraps);
	while (auto actor = it.Next())
	{
		if ((actor->spr.flags & 32) || !actor->hasX())
			continue;

		switch (actor->GetType()) {
		case kTrapSawCircular:
			actor->xspr.data2 = ClipLow(actor->xspr.data2 - 4, 0);
			break;

		case kTrapFlame:
			if (actor->xspr.state && seqGetStatus(actor) < 0)
			{
				auto pos = actor->spr.pos;
				double t = actor->xspr.data1 * (128. / 120.);
				auto vec = actor->spr.Angles.Yaw.ToVector() * t;
				for (int i = 0; i < 2; i++)
				{
					auto pFX = gFX.fxSpawnActor(FX_32, actor->sector(), pos);
					if (pFX)
					{
						pFX->vel.X = vec.X + Random2F(0x8888);
						pFX->vel.Y = vec.Y + Random2F(0x8888);
						pFX->vel.Z = Random2F(0x8888);
					}
					pos += vec / 2;
				}
				actFireVector(actor, 0., 0., DVector3(actor->spr.Angles.Yaw.ToVector(), Random2F(0x8888) * 4), kVectorTchernobogBurn, actor->xspr.data1 << 5);
			}
			break;
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void actCheckDudes()
{
	BloodStatIterator it(kStatDude);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32)
			continue;

		if (actor->hasX())
		{
			bool burn = IsBurningDude(actor);
			const bool fixBurnGlitch = !cl_bloodvanillaenemies && burn && !VanillaMode(); // if enemies are burning, always apply burning damage per tick
			if ((actor->xspr.burnTime > 0) || fixBurnGlitch)
			{
				if (!burn)
					actor->xspr.burnTime = ClipLow(actor->xspr.burnTime - 4, 0);
				actDamageSprite(actor->GetBurnSource(), actor, kDamageBurn, 8);
			}

#ifdef NOONE_EXTENSIONS
			// handle incarnations of custom dude
			if (actor->GetType() == kDudeModernCustom && actor->xspr.txID > 0 && actor->xspr.sysData1 == kGenDudeTransformStatus)
			{
				actor->vel.XY().Zero();
				if (seqGetStatus(actor) < 0) genDudeTransform(actor);
			}
#endif
			if (actor->xspr.health <= 0 && seqGetStatus(actor) < 0)
			{
				// generalize the morph effect of the dying two headed cerberus.
				auto dmt = actor->PointerVar<PClassActor>("deathMorphType");
				if (dmt)
				{
					actor->ChangeType(dmt);
					actor->xspr.health = actor->startHealth() << 4;
					actor->spr.xint = 1;
					if (actor->GetTarget() != nullptr) aiSetTarget(actor, actor->GetTarget());
					aiActivateDude(actor);
				}
			}
			if (actor->xspr.Proximity && !actor->xspr.isTriggered)
			{
				BloodStatIterator it1(kStatDude);
				while (auto actor2 = it1.Next())
				{
					if (actor2->spr.flags & 32) continue;

					if (actor2->IsPlayerActor() && (unsigned int)actor2->xspr.health > 0)
					{
						if (CheckProximity(actor2, actor->spr.pos, actor->sector(), 128))
							trTriggerSprite(actor, kCmdSpriteProximity, actor2);
					}
				}
			}
			if (actor->IsPlayerActor())
			{
				auto pPlayer = getPlayer(actor);
				if (pPlayer->voodooTargets) voodooTarget(pPlayer);
				if (pPlayer->hand && Chance(0x8000)) actDamageSprite(actor, actor, kDamageDrown, 12);

				if (pPlayer->isUnderwater)
				{
					bool bActive = packItemActive(pPlayer, 1);

					if (bActive || pPlayer->godMode) pPlayer->underwaterTime = 1200;
					else pPlayer->underwaterTime = ClipLow(pPlayer->underwaterTime - 4, 0);

					if (pPlayer->underwaterTime < 1080 && packCheckItem(pPlayer, 1) && !bActive)
						packUseItem(pPlayer, 1);

					if (!pPlayer->underwaterTime)
					{
						pPlayer->chokeEffect += 4;
						if (Chance(pPlayer->chokeEffect))
							actDamageSprite(actor, actor, kDamageDrown, 3 << 4);
					}
					else
						pPlayer->chokeEffect = 0;

					if (!actor->vel.XY().isZero())
						sfxPlay3DSound(actor, 709, 100, 2);

					pPlayer->bubbleTime = ClipLow(pPlayer->bubbleTime - 4, 0);
				}
				else if (gGameOptions.nGameType == 0)
				{
					if (pPlayer->GetActor()->xspr.health > 0 && pPlayer->restTime >= 1200 && Chance(0x200))
					{
						pPlayer->restTime = -1;
						sfxPlay3DSound(actor, 3100 + Random(11), 0, 2);
					}
				}
			}
			ProcessTouchObjects(actor);
		}
	}

	it.Reset(kStatDude);
	while (auto actor = it.Next())
	{
		if (actor->spr.flags & 32 || !actor->hasX()) continue;

		auto pSector = actor->sector();
		if (!actor->IsPlayerActor()) viewBackupSpriteLoc(actor);
		XSECTOR* pXSector = pSector->hasX() ? &pSector->xs() : nullptr;

		if (pXSector)
		{
			double top, bottom;
			GetActorExtents(actor, &top, &bottom);
			if (getflorzofslopeptr(pSector, actor->spr.pos) <= bottom)
			{
				DAngle angle = pXSector->panAngle;
				double speed = 0;
				if (pXSector->panAlways || pXSector->state || pXSector->busy)
				{
					speed = pXSector->panVel / 128.;
					if (!pXSector->panAlways && pXSector->busy)
						speed *= FixedToFloat(pXSector->busy);
				}
				if (pSector->floorstat & CSTAT_SECTOR_ALIGN)
					angle += pSector->walls[0].normalAngle();
				actor->vel += angle.ToVector() * speed;
			}
		}
		if (pXSector && pXSector->Underwater) actAirDrag(actor, 5376);
		else actAirDrag(actor, 128);

		if (actor->IsPlayerActor())
		{
			auto pPlayer = getPlayer(actor);
			double nDrag = FixedToFloat(gDudeDrag);
			if (actor->xspr.height > 0)
				nDrag -= Scale(nDrag, (double)actor->xspr.height, 256.);

			constexpr auto maxVel = (36211. / 3000.);
			pPlayer->Angles.doRollInput(&pPlayer->cmd.ucmd, actor->vel.XY(), maxVel, false);
			pPlayer->Angles.StrafeVel -= pPlayer->Angles.StrafeVel * nDrag;
		}

		if ((actor->spr.flags & 4) || !actor->vel.isZero() || actor->sector()->velFloor || actor->sector()->velCeil)
			MoveDude(actor);
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actCheckFlares()
{
	BloodStatIterator it(kStatFlare);
	while (auto actor = it.Next())
	{
		if ((actor->spr.flags & 32) || !actor->hasX()) continue;

		auto target = actor->GetTarget();
		if (!target) continue;

		viewBackupSpriteLoc(actor);
		if (target->spr.statnum == kMaxStatus)
		{
			GibSprite(actor, GIBTYPE_17, NULL, NULL);
			actPostSprite(actor, kStatFree);
		}
		if (target->hasX() && target->xspr.health > 0)
		{
			DVector3 pos = target->spr.pos;
			pos.XY() += (actor->xspr.goalAng + target->spr.Angles.Yaw).ToVector() * target->clipdist * 0.5;
			pos.Z += actor->xspr.TargetPos.Z;
			SetActor(actor, pos);
			actor->vel = target->vel;
		}
		else
		{
			GibSprite(actor, GIBTYPE_17, NULL, NULL);
			actPostSprite(actor, kStatFree);
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actProcessSprites(void)
{
#ifdef NOONE_EXTENSIONS
	if (gModernMap) nnExtProcessSuperSprites();
#endif

	actCheckProximity();
	actCheckThings();
	actCheckProjectiles();
	actCheckExplosion();
	actCheckTraps();
	actCheckDudes();
	actCheckFlares();
	aiProcessDudes();
	gFX.fxProcess();
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DBloodActor* actSpawnSprite(sectortype* pSector, const DVector3& pos, int nStat, bool setextra, PClass* cls, int type)
{
	DBloodActor* actor = InsertSprite(pSector, nStat, cls);
	if (type >= 0) actor->spr.lotag = type;	// we may still need this.

	SetActor(actor, pos);
	if (setextra && !actor->hasX())
	{
		actor->addX();
		actor->hit.florhit.setNone();
		actor->hit.ceilhit.setNone();
		if (!VanillaMode()) actor->SetTarget(nullptr);
	}
	return actor;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DBloodActor* actSpawnSprite(DBloodActor* source, int nStat, PClass* cls, int type)
{
	DBloodActor* actor = InsertSprite(source->sector(), nStat, cls);
	actor->spr.lotag = type;	// we still need this.

	actor->spr.pos = source->spr.pos;
	actor->vel = source->vel;
	actor->spr.flags = 0;
	actor->addX();
	actor->hit.florhit.setNone();
	actor->hit.ceilhit.setNone();
	if (!VanillaMode()) actor->SetTarget(nullptr);
	return actor;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DBloodActor* actSpawnDude(DBloodActor* source, int nType, double dist)
{
	auto cls = GetSpawnType(nType);
	auto spawned = actSpawnSprite(source, kStatDude, cls, nType);
	if (!spawned) return nullptr;
	DAngle angle = source->spr.Angles.Yaw;

	auto pos = source->spr.pos;

	if (dist >= 0)
	{
		pos.XY() += angle.ToVector() * dist;
	}
	spawned->originalType = cls;
	spawned->spr.Angles.Yaw = angle;
	SetActor(spawned, pos);

	spawned->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL | CSTAT_SPRITE_BLOOD_PUSHABLE;
	spawned->clipdist = spawned->fClipDist();
	spawned->xspr.health = spawned->startHealth() << 4;
	spawned->xspr.respawn = 1;
	seqSpawn(spawned->seqStartName(), spawned->seqStartID(), spawned);

#ifdef NOONE_EXTENSIONS
	// add a way to inherit some values of spawner type 18 by dude.
	// This way designer can count enemies via switches and do many other interesting things.
	if (gModernMap && source->spr.flags & kModernTypeFlag1)
	{
		// allow inheriting only for selected source types
		switch (source->GetType())
		{
		case kMarkerDudeSpawn:
			//inherit pal?
			if (spawned->spr.pal <= 0) spawned->spr.pal = source->spr.pal;

			// inherit spawn sprite trigger settings, so designer can count monsters.
			spawned->xspr.txID = source->xspr.txID;
			spawned->xspr.command = source->xspr.command;
			spawned->xspr.triggerOn = source->xspr.triggerOn;
			spawned->xspr.triggerOff = source->xspr.triggerOff;

			// inherit drop items
			spawned->xspr.dropMsg = source->xspr.dropMsg;

			// inherit dude flags
			spawned->xspr.dudeDeaf = source->xspr.dudeDeaf;
			spawned->xspr.dudeGuard = source->xspr.dudeGuard;
			spawned->xspr.dudeAmbush = source->xspr.dudeAmbush;
			spawned->xspr.dudeFlag4 = source->xspr.dudeFlag4;
			spawned->xspr.modernFlags = source->xspr.modernFlags;
			break;
		}
	}
#endif

	aiInitSprite(spawned);
	return spawned;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DBloodActor* actFireThing(DBloodActor* actor, double xyoff, double zoff, double zvel, int thingType, double nSpeed)
{
	assert(thingType >= kThingBase && thingType < kThingMax);

	DVector3 vect = actor->spr.pos.plusZ(zoff) + (actor->spr.Angles.Yaw + DAngle90).ToVector() * xyoff + actor->spr.Angles.Yaw.ToVector() * actor->clipdist;

	if (HitScan(actor, vect.Z, DVector3(vect.XY() - actor->spr.pos.XY(), 0), CLIPMASK0, actor->clipdist * 0.25) != -1)
	{
		vect.XY() = gHitInfo.hitpos.XY() - actor->spr.Angles.Yaw.ToVector() * actor->clipdist * 2;
	}
	auto fired = actSpawnThing(actor->sector(), vect, thingType);
	if (fired)
	{
		fired->SetOwner(actor);
		fired->spr.Angles.Yaw = actor->spr.Angles.Yaw;
		fired->vel = DVector3(fired->spr.Angles.Yaw.ToVector() * nSpeed, nSpeed * zvel * 4) + actor->vel * 0.5;
	}
	return fired;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool actCheckRespawn(DBloodActor* actor)
{
	if (actor->hasX())
	{
		int nRespawnTime = actGetRespawnTime(actor);
		if (nRespawnTime < 0) return 0;

		actor->xspr.respawnPending = 1;
		if (actor->GetType() >= kThingBase && actor->GetType() < kThingMax)
		{
			actor->xspr.respawnPending = 3;
			if (actor->GetType() == kThingTNTBarrel) actor->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		}
		if (nRespawnTime > 0)
		{
			if (actor->xspr.respawnPending == 1) nRespawnTime = MulScale(nRespawnTime, 0xa000, 16);
			actor->spr.intowner = actor->spr.statnum;
			actPostSprite(actor, kStatRespawn);
			actor->spr.flags |= kHitagRespawn;

			if (!(actor->IsDudeActor()))
			{
				actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
				actor->spr.pos = actor->basePoint;
			}
			evPostActor(actor, nRespawnTime, AF(Respawn));
		}
		return 1;
	}
	return  0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool actCanSplatWall(walltype* pWall)
{
	if (pWall->cstat & (CSTAT_WALL_MOVE_MASK)) return 0;

	int nType = pWall->type;
	if (nType >= kWallBase && nType < kWallMax) return 0;

	if (pWall->twoSided())
	{
		sectortype* pSector = pWall->nextSector();
		if (pSector->type >= kSectorBase && pSector->type < kSectorMax) return 0;
	}
	return 1;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actFireVector(DBloodActor* shooter, double offset, double zoffset, DVector3 dv, VECTOR_TYPE vectorType, double nRange)
{
	// this function expects a vector with unit length in XY. Let's not depend on all callers doing it.
	dv /= dv.XY().Length(); 
	assert(vectorType >= 0 && vectorType < kVectorMax);
	const VECTORDATA* pVectorData = &gVectorData[vectorType];
	if (nRange < 0) nRange = pVectorData->fMaxDist();
	// The vector for hitscan must be longer than what we got here as long as it works with integers.
	int hit = VectorScan(shooter, offset, zoffset, dv, nRange, 1);
	if (hit == 3)
	{
		auto hitactor = gHitInfo.actor();
		assert(hitactor != nullptr);
		if (!gGameOptions.bFriendlyFire && IsTargetTeammate(shooter, hitactor)) return;
		if (hitactor->IsPlayerActor())
		{
			auto pPlayer = getPlayer(hitactor);
			if (powerupCheck(pPlayer, kPwUpReflectShots))
			{
				gHitInfo.hitActor = shooter;
				gHitInfo.hitpos = shooter->spr.pos;
			}
		}
	}
	auto pos = gHitInfo.hitpos - dv;
	auto pSector = gHitInfo.hitSector;
	uint8_t nSurf = kSurfNone;
	if (nRange == 0 || (gHitInfo.hitpos.XY() - shooter->spr.pos.XY()).Length() < nRange)
	{
		switch (hit)
		{
		case 1:
		{
			if (pSector->ceilingstat & CSTAT_SECTOR_SKY)
				nSurf = kSurfNone;
			else
				nSurf = GetExtInfo(pSector->ceilingtexture).surftype;
			break;
		}
		case 2:
		{
			if (pSector->floorstat & CSTAT_SECTOR_SKY)
				nSurf = kSurfNone;
			else
				nSurf = GetExtInfo(pSector->floortexture).surftype;
			break;
		}
		case 0:
		{
			auto pWall = gHitInfo.hitWall;
			nSurf = GetExtInfo(pWall->walltexture).surftype;
			if (actCanSplatWall(pWall))
			{
				auto ppos = gHitInfo.hitpos - dv;
				int nnSurf = GetExtInfo(pWall->walltexture).surftype;
				assert(nnSurf < kSurfMax);
				if (pVectorData->surfHit[nnSurf].fx1 >= 0)
				{
					auto pFX = gFX.fxSpawnActor(pVectorData->surfHit[nnSurf].fx1, pSector, ppos);
					if (pFX)
					{
						pFX->spr.Angles.Yaw = pWall->normalAngle();
						pFX->spr.cstat |= CSTAT_SPRITE_ALIGNMENT_WALL;
						pFX->spr.cstat2 |= CSTAT2_SPRITE_DECAL;
					}
				}
			}
			break;
		}
		case 4:
		{
			auto pWall = gHitInfo.hitWall;
			nSurf = GetExtInfo(pWall->overtexture).surftype;
			if (pWall->hasX())
			{
				if (pWall->xw().triggerVector)
					trTriggerWall(pWall, kCmdWallImpact, shooter);
			}
			break;
		}
		case 3:
		{
			auto actor = gHitInfo.actor();
			nSurf = GetExtInfo(actor->spr.spritetexture()).surftype;
			pos -= 7 * dv;
			int shift = 4;
			if (vectorType == kVectorTine && !actor->IsPlayerActor()) shift = 3;

			actDamageSprite(shooter, actor, pVectorData->dmgType, pVectorData->dmg << shift);
			if (actor->hasX() && actor->xspr.Vector) trTriggerSprite(actor, kCmdSpriteImpact, shooter);
			int mass = actor->mass();

			if (actor->spr.statnum == kStatThing)
			{
				if (mass > 0 && pVectorData->impulse)
				{
					double thrust = double(pVectorData->impulse) / (mass * 1024);
					actor->vel += dv * thrust;
				}
				if (pVectorData->burnTime)
				{
					if (!actor->xspr.burnTime) evPostActor(actor, 0, AF(fxFlameLick));
					actBurnSprite(shooter->GetOwner(), actor, pVectorData->burnTime);
				}
			}
			else if (actor->spr.statnum == kStatDude && actor->hasX())
			{
				if (mass > 0 && pVectorData->impulse)
				{
					double thrust = double(pVectorData->impulse) / (mass * 1024);
					actor->vel += dv * thrust;
				}
				if (pVectorData->burnTime)
				{
					if (!actor->xspr.burnTime) evPostActor(actor, 0, AF(fxFlameLick));
					actBurnSprite(shooter->GetOwner(), actor, pVectorData->burnTime);
				}
				if (Chance(pVectorData->fxChance))
				{
					double tt = gVectorData[19].fMaxDist();
					dv.X += FixedToFloat<14>(Random3(4000)); // random messiness...
					dv.Y += FixedToFloat<14>(Random3(4000));
					dv.Z += FixedToFloat<14>(Random3(4000));
					if (HitScan(actor, gHitInfo.hitpos.Z, dv, CLIPMASK1, tt) == 0)
					{
						if ((gHitInfo.hitpos.XY() - actor->spr.pos.XY()).LengthSquared() <= tt * tt)
						{
							auto pWall = gHitInfo.hitWall;
							auto pSector1 = gHitInfo.hitSector;
							if (actCanSplatWall(pWall))
							{
								auto ppos = gHitInfo.hitpos - dv;
								int nnSurf = GetExtInfo(pWall->walltexture).surftype;
								const VECTORDATA* pVectorData1 = &gVectorData[19];
								FX_ID t2 = pVectorData1->surfHit[nnSurf].fx2;
								FX_ID t3 = pVectorData1->surfHit[nnSurf].fx3;

								DBloodActor* pFX = nullptr;
								if (t2 > FX_NONE && (t3 == FX_NONE || Chance(0x4000))) pFX = gFX.fxSpawnActor(t2, pSector1, ppos);
								else if (t3 > FX_NONE) pFX = gFX.fxSpawnActor(t3, pSector1, ppos);
								if (pFX)
								{
									pFX->vel.Z = FixedToFloat(0x2222);
									pFX->spr.Angles.Yaw = pWall->normalAngle();
									pFX->spr.cstat |= CSTAT_SPRITE_ALIGNMENT_WALL;
									pFX->spr.cstat2 |= CSTAT2_SPRITE_DECAL;
								}
							}
						}
					}
				}
				for (int i = 0; i < pVectorData->bloodSplats; i++)
					if (Chance(pVectorData->splatChance))
						fxSpawnBlood(actor, pVectorData->dmg << 4);
			}
#ifdef NOONE_EXTENSIONS
			// add impulse for sprites from physics list
			if (gPhysSpritesCount > 0 && pVectorData->impulse)
			{

				if (actor->hasX())
				{
					if (actor->xspr.physAttr & kPhysDebrisVector) 
					{
						int mass = max(actor->spriteMass.mass, 10);
						double thrust = double(pVectorData->impulse) / (mass * 4096);
						actor->vel += dv * thrust;

						if (pVectorData->burnTime != 0) {
							if (!actor->xspr.burnTime) evPostActor(actor, 0, AF(fxFlameLick));
							actBurnSprite(shooter->GetOwner(), actor, pVectorData->burnTime);
						}

						if (actor->GetType() >= kThingBase && actor->GetType() < kThingMax) {
							actor->spr.statnum = kStatThing; // temporary change statnum property
							actDamageSprite(shooter, actor, pVectorData->dmgType, pVectorData->dmg << 4);
							actor->spr.statnum = kStatDecoration; // return statnum property back
						}
					}
				}
			}
#endif
			break;
		}
		}
	}
	assert(nSurf < kSurfMax);
#ifdef NOONE_EXTENSIONS

	// let the patrol enemies hear surface hit sounds!

	if (pVectorData->surfHit[nSurf].fx2 >= 0) {

		auto pFX2 = gFX.fxSpawnActor(pVectorData->surfHit[nSurf].fx2, pSector, pos);
		if (pFX2 && gModernMap)
			pFX2->SetOwner(shooter);
	}

	if (pVectorData->surfHit[nSurf].fx3 >= 0) {

		auto pFX3 = gFX.fxSpawnActor(pVectorData->surfHit[nSurf].fx3, pSector, pos);
		if (pFX3 && gModernMap)
			pFX3->SetOwner(shooter);

	}

#else
	if (pVectorData->surfHit[nSurf].fx2 >= 0)
		gFX.fxSpawnActor(pVectorData->surfHit[nSurf].fx2, pSector, pos);
	if (pVectorData->surfHit[nSurf].fx3 >= 0)
		gFX.fxSpawnActor(pVectorData->surfHit[nSurf].fx3, pSector, pos);
#endif

	if (pVectorData->surfHit[nSurf].fxSnd >= 0)
		sfxPlay3DSectorSound(pos, pVectorData->surfHit[nSurf].fxSnd, pSector);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void FireballSeqCallback(DBloodActor* actor)
{
	auto pFX = gFX.fxSpawnActor(FX_11, actor->sector(), actor->spr.pos);
	if (pFX)
	{
		pFX->vel = actor->vel;
	}
}

void NapalmSeqCallback(DBloodActor* actor)
{
	auto pFX = gFX.fxSpawnActor(FX_12, actor->sector(), actor->spr.pos);
	if (pFX)
	{
		pFX->vel = actor->vel;
	}
}

void Fx32Callback(DBloodActor* actor)
{
	auto pFX = gFX.fxSpawnActor(FX_32, actor->sector(), actor->spr.pos);
	if (pFX)
	{
		pFX->vel = actor->vel;
	}
}

void Fx33Callback(DBloodActor* actor)
{
	auto pFX = gFX.fxSpawnActor(FX_33, actor->sector(), actor->spr.pos);
	if (pFX)
	{
		pFX->vel = actor->vel;
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void TreeToGibCallback(DBloodActor* actor)
{
	actor->ChangeType(kThingObjectExplode);
	actor->xspr.state = 1;
	actor->xspr.data1 = 15;
	actor->xspr.data2 = 0;
	actor->xspr.data3 = 0;
	actor->xspr.health = GetDefaultByType(actor->GetClass())->IntVar("defhealth");
	actor->xspr.data4 = 312;
	actor->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
}

void DudeToGibCallback1(DBloodActor* actor)
{
	actor->ChangeType(kThingBloodChunks);
	actor->xspr.data1 = 8;
	actor->xspr.data2 = 0;
	actor->xspr.data3 = 0;
	actor->xspr.health = GetDefaultByType(actor->GetClass())->IntVar("defhealth");
	actor->xspr.data4 = 319;
	actor->xspr.triggerOnce = 0;
	actor->xspr.isTriggered = 0;
	actor->xspr.locked = 0;
	actor->xspr.TargetPos.X = PlayClock;
	actor->xspr.state = 1;
}

void DudeToGibCallback2(DBloodActor* actor)
{
	actor->ChangeType(kThingBloodChunks);
	actor->xspr.data1 = 3;
	actor->xspr.data2 = 0;
	actor->xspr.data3 = 0;
	actor->xspr.health = GetDefaultByType(actor->GetClass())->IntVar("defhealth");
	actor->xspr.data4 = 319;
	actor->xspr.triggerOnce = 0;
	actor->xspr.isTriggered = 0;
	actor->xspr.locked = 0;
	actor->xspr.TargetPos.X = PlayClock;
	actor->xspr.state = 1;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void actPostSprite(DBloodActor* actor, int nStatus)
{
	assert(nStatus >= 0 && nStatus <= kStatFree);
	if (actor->spr.flags & 32)
	{
		for (auto& post : gPost)
			if (post.sprite == actor)
			{
				post.status = nStatus;
				return;
			}
	}
	else
	{
		actor->spr.flags |= 32;
		gPost.Push({ actor, nStatus });
	}
}

void actPostProcess(void)
{
	for (auto& p : gPost)
	{
		p.sprite->spr.flags &= ~32;
		int nStatus = p.status;
		if (nStatus == kStatFree)
		{
			if (p.sprite->spr.statnum != kStatFree)
			{
				evKillActor(p.sprite);
				if (p.sprite->hasX()) seqKill(p.sprite);
				DeleteSprite(p.sprite);
			}
		}
		else
			ChangeActorStat(p.sprite, nStatus);
	}
	gPost.Clear();
}

void actBurnSprite(DBloodActor* pSource, DBloodActor* pTarget, int nTime)
{
	pTarget->xspr.burnTime = ClipHigh(pTarget->xspr.burnTime + nTime, pTarget->spr.statnum == kStatDude ? 2400 : 1200);
	pTarget->SetBurnSource(pSource);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool ceilIsTooLow(DBloodActor* actor)
{
	if (actor != nullptr)
	{
		sectortype* pSector = actor->sector();
		double a = pSector->ceilingz - pSector->floorz;
		double top, bottom;
		GetActorExtents(actor, &top, &bottom);
		if (a > top - bottom) return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool isGrown(DBloodActor* actor)
{
	if (!(currentLevel->featureflags & kFeaturePlayerSize)) return false;
	if (powerupCheck(getPlayer(actor), kPwUpGrowShroom) > 0) return true;
	else if (actor->hasX() && actor->xspr.scale >= 512) return true;
	else return false;
}

bool isShrunk(DBloodActor* actor)
{
	if (!(currentLevel->featureflags & kFeaturePlayerSize)) return false;
	if (powerupCheck(getPlayer(actor), kPwUpShrinkShroom) > 0) return true;
	else if (actor->hasX() && actor->xspr.scale > 0 && actor->xspr.scale <= 128) return true;
	else return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

FSerializer& Serialize(FSerializer& arc, const char* keyname, SPRITEHIT& w, SPRITEHIT* def)
{
	if (arc.BeginObject(keyname))
	{
		arc("hit", w.hit)
			("ceilhit", w.ceilhit)
			("florhit", w.florhit)
			.EndObject();
	}
	return arc;
}


END_BLD_NS
