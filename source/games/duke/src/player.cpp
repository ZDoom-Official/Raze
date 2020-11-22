//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2000, 2003 - Matt Saettler (EDuke Enhancements)
Copyright (C) 2020 - Christoph Oelckers

This file is part of Enhanced Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1996 - Todd Replogle
Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms

EDuke enhancements integrated: 04/13/2003 - Matt Saettler

Note: EDuke source was in transition.  Changes are in-progress in the
source as it is released.

*/
//-------------------------------------------------------------------------


#include "ns.h"
#include "global.h"
#include "mapinfo.h"
#include "dukeactor.h"

BEGIN_DUKE_NS 

//---------------------------------------------------------------------------
//
// callback for playercolor CVAR
//
//---------------------------------------------------------------------------

int playercolor2lookup(int color)
{
	static int8_t player_pals[] = { 0, 9, 10, 11, 12, 13, 14, 15, 16, 21, 23, };
	if (color >= 0 && color < 10) return player_pals[color];
	return 0;
}

void PlayerColorChanged(void)
{
	if (ud.recstat != 0)
		return;

	auto& pp = ps[myconnectindex];
	if (ud.multimode > 1)
	{
		//Net_SendClientInfo();
	}
	else
	{
		pp.palookup = ud.user_pals[myconnectindex] = playercolor2lookup(playercolor);
	}
	if (pp.GetActor()->s.picnum == TILE_APLAYER && pp.GetActor()->s.pal != 1)
		pp.GetActor()->s.pal = ud.user_pals[myconnectindex];
}

//---------------------------------------------------------------------------
//
// Sync local player with CVARs.
//
//---------------------------------------------------------------------------

void setlocalplayerinput(player_struct* pp)
{
	pp->aim_mode = in_mousemode;
	pp->auto_aim = cl_autoaim;
}

//---------------------------------------------------------------------------
//
// calculates automatic view pitch for playing without a mouse
//
//---------------------------------------------------------------------------

void calcviewpitch(player_struct *p, double factor)
{
	int psect = p->cursectnum;
	int psectlotag = sector[psect].lotag;
	if (p->aim_mode == 0 && p->on_ground && psectlotag != ST_2_UNDERWATER && (sector[psect].floorstat & 2))
	{
		int x = p->posx + p->angle.ang.bcos(-5);
		int y = p->posy + p->angle.ang.bsin(-5);
		short tempsect = psect;
		updatesector(x, y, &tempsect);

		if (tempsect >= 0)
		{
			int k = getflorzofslope(psect, x, y);
			if (psect == tempsect || abs(getflorzofslope(tempsect, x, y) - k) <= (4 << 8))
			p->horizon.horizoff += q16horiz(FloatToFixed(factor * mulscale16(p->truefz - k, 160)));
		}
	}
	if (p->horizon.horizoff.asq16() > 0)
	{
		p->horizon.horizoff += q16horiz(xs_CRoundToInt(-factor * ((p->horizon.horizoff.asq16() >> 3) + FRACUNIT)));
		if (p->horizon.horizoff.asq16() < 0) p->horizon.horizoff = q16horiz(0);
	}
	else if (p->horizon.horizoff.asq16() < 0)
	{
		p->horizon.horizoff += q16horiz(xs_CRoundToInt(-factor * ((p->horizon.horizoff.asq16() >> 3) + FRACUNIT)));
		if (p->horizon.horizoff.asq16() > 0) p->horizon.horizoff = q16horiz(0);
	}
}

//---------------------------------------------------------------------------
//
// why is this such a mess?
//
//---------------------------------------------------------------------------

int setpal(struct player_struct* p)
{
	int palette;
	if (p->DrugMode) palette = DRUGPAL;
	else if (p->heat_on) palette = SLIMEPAL;
	else if (p->cursectnum < 0) palette = BASEPAL; // don't crash if out of range.
	else if (sector[p->cursectnum].ceilingpicnum >= TILE_FLOORSLIME && sector[p->cursectnum].ceilingpicnum <= TILE_FLOORSLIME + 2) palette = SLIMEPAL;
	else if (sector[p->cursectnum].lotag == ST_2_UNDERWATER) palette = WATERPAL;
	else palette = BASEPAL;
	return palette;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void quickkill(struct player_struct* p)
{
	SetPlayerPal(p, PalEntry(48, 48, 48, 48));

	auto pa = p->GetActor();
	pa->s.extra = 0;
	pa->s.cstat |= 32768;
	if (ud.god == 0) fi.guts(pa, TILE_JIBS6, 8, myconnectindex);
	return;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void forceplayerangle(int snum)
{
	player_struct* p = &ps[snum];
	int n;

	n = 128 - (krand() & 255);

	p->horizon.addadjustment(64);
	p->sync.actions |= SB_CENTERVIEW;
	p->angle.rotscrnang = p->angle.look_ang = buildlook(n >> 1);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void tracers(int x1, int y1, int z1, int x2, int y2, int z2, int n)
{
	int i, xv, yv, zv;
	short sect = -1;

	i = n + 1;
	xv = (x2 - x1) / i;
	yv = (y2 - y1) / i;
	zv = (z2 - z1) / i;

	if ((abs(x1 - x2) + abs(y1 - y2)) < 3084)
		return;

	for (i = n; i > 0; i--)
	{
		x1 += xv;
		y1 += yv;
		z1 += zv;
		updatesector(x1, y1, &sect);
		if (sect >= 0)
		{
			if (sector[sect].lotag == 2)
				EGS(sect, x1, y1, z1, TILE_WATERBUBBLE, -32, 4 + (krand() & 3), 4 + (krand() & 3), krand() & 2047, 0, 0, ps[0].GetActor(), 5);
			else
				EGS(sect, x1, y1, z1, TILE_SMALLSMOKE, -32, 14, 14, 0, 0, 0, ps[0].GetActor(), 5);
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int hits(DDukeActor* actor)
{
	auto sp = &actor->s;
	int sx, sy, sz;
	short sect;
	short hw;
	int zoff;
	DDukeActor* d;

	if (sp->picnum == TILE_APLAYER) zoff = (40 << 8);
	else zoff = 0;

	hitscan(sp->x, sp->y, sp->z - zoff, sp->sectnum, bcos(sp->ang), bsin(sp->ang), 0, &sect, &hw, &d, &sx, &sy, &sz, CLIPMASK1);

	return (FindDistance2D(sx - sp->x, sy - sp->y));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int hitasprite(DDukeActor* actor, DDukeActor** hitsp)
{
	auto sp = &actor->s;
	int sx, sy, sz, zoff;
	short sect, hw;

	if (badguy(actor))
		zoff = (42 << 8);
	else if (sp->picnum == TILE_APLAYER) zoff = (39 << 8);
	else zoff = 0;

	hitscan(sp->x, sp->y, sp->z - zoff, sp->sectnum, bcos(sp->ang), bsin(sp->ang), 0, &sect, &hw, hitsp, &sx, &sy, &sz, CLIPMASK1);

	if (hw >= 0 && (wall[hw].cstat & 16) && badguy(actor))
		return((1 << 30));

	return (FindDistance2D(sx - sp->x, sy - sp->y));
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int hitawall(struct player_struct* p, int* hitw)
{
	int sx, sy, sz;
	short sect, hitw1;
	DDukeActor* d;

	hitscan(p->posx, p->posy, p->posz, p->cursectnum,
		p->angle.ang.bcos(), p->angle.ang.bsin(), 0, &sect, &hitw1, &d, &sx, &sy, &sz, CLIPMASK0);
	*hitw = hitw1;

	return (FindDistance2D(sx - p->posx, sy - p->posy));
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

DDukeActor* aim(DDukeActor* actor, int aang)
{
	char gotshrinker, gotfreezer;
	int a, k, cans;
	int aimstats[] = { STAT_PLAYER, STAT_DUMMYPLAYER, STAT_ACTOR, STAT_ZOMBIEACTOR };
	int dx1, dy1, dx2, dy2, dx3, dy3, smax, sdist;
	int xv, yv;
	auto s = &actor->s;

	a = s->ang;

	// Autoaim from DukeGDX.
	if (s->picnum == TILE_APLAYER)
	{
		if (ps[s->yvel].auto_aim == 0)
		{
			// The chickens in RRRA are homing and must always autoaim.
			if (!isRRRA() || ps[s->yvel].curr_weapon != CHICKEN_WEAPON)
				return nullptr;
		}
		else if (ps[s->yvel].auto_aim == 2)
		{
			int weap;
			if (!isWW2GI())
			{
				weap = ps[s->yvel].curr_weapon;
			}
			else
			{
				weap = aplWeaponWorksLike[ps[s->yvel].curr_weapon][s->yvel];
			}
			if (weap > CHAINGUN_WEAPON || weap == KNEE_WEAPON)
			{
				return nullptr;
			}

		}
	}
	DDukeActor* aimed = nullptr;
	//	  if(s->picnum == TILE_APLAYER && ps[s->yvel].aim_mode) return -1;

	if (isRR())
	{
		gotshrinker = 0;
		gotfreezer = 0;
	}
	else if (isWW2GI())
	{
		gotshrinker = s->picnum == TILE_APLAYER && aplWeaponWorksLike[ps[s->yvel].curr_weapon][s->yvel] == SHRINKER_WEAPON;
		gotfreezer = s->picnum == TILE_APLAYER && aplWeaponWorksLike[ps[s->yvel].curr_weapon][s->yvel] == FREEZE_WEAPON;
	}
	else
	{
		gotshrinker = s->picnum == TILE_APLAYER && ps[s->yvel].curr_weapon == SHRINKER_WEAPON;
		gotfreezer = s->picnum == TILE_APLAYER && ps[s->yvel].curr_weapon == FREEZE_WEAPON;
	}

	smax = 0x7fffffff;

	dx1 = bcos(a - aang);
	dy1 = bsin(a - aang);
	dx2 = bcos(a + aang);
	dy2 = bsin(a + aang);

	dx3 = bcos(a);
	dy3 = bsin(a);

	for (k = 0; k < 4; k++)
	{
		if (aimed)
			break;

		DukeStatIterator it(aimstats[k]);
		while (auto act = it.Next())
		{
			auto sp = &act->s;
			if (sp->xrepeat > 0 && sp->extra >= 0 && (sp->cstat & (257 + 32768)) == 257)
				if (badguy(sp) || k < 2)
				{
					if (badguy(sp) || sp->picnum == TILE_APLAYER)
					{
						if (sp->picnum == TILE_APLAYER &&
							(isRR() && ud.ffire == 0) &&
							ud.coop == 1 &&
							s->picnum == TILE_APLAYER &&
							s != sp)
							continue;

						if (gotshrinker && sp->xrepeat < 30 && !(actorinfo[sp->picnum].flags & SFLAG_SHRINKAUTOAIM)) continue;
						if (gotfreezer && sp->pal == 1) continue;
					}

					xv = (sp->x - s->x);
					yv = (sp->y - s->y);

					if ((dy1 * xv) <= (dx1 * yv))
						if ((dy2 * xv) >= (dx2 * yv))
						{
							sdist = mulscale(dx3, xv, 14) + mulscale(dy3, yv, 14);
							if (sdist > 512 && sdist < smax)
							{
								if (s->picnum == TILE_APLAYER)
									a = (abs(scale(sp->z - s->z, 10, sdist) - ps[s->yvel].horizon.sum().asbuild()) < 100);
								else a = 1;

								cans = cansee(sp->x, sp->y, sp->z - (32 << 8) + actorinfo[sp->picnum].aimoffset, sp->sectnum, s->x, s->y, s->z - (32 << 8), s->sectnum);

								if (a && cans)
								{
									smax = sdist;
									aimed = act;
								}
							}
						}
				}
		}
	}

	return aimed;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void dokneeattack(int snum, const std::initializer_list<int> & respawnlist)
{
	auto p = &ps[snum];

	if (p->knee_incs > 0)
	{
		p->knee_incs++;
		p->horizon.addadjustment(-48);
		p->sync.actions |= SB_CENTERVIEW;
		if (p->knee_incs > 15)
		{
			p->knee_incs = 0;
			p->holster_weapon = 0;
			if (p->weapon_pos < 0)
				p->weapon_pos = -p->weapon_pos;
			if (p->actorsqu != nullptr && dist(p->GetActor(), p->actorsqu) < 1400)
			{
				fi.guts(p->actorsqu, TILE_JIBS6, 7, myconnectindex);
				spawn(p->actorsqu, TILE_BLOODPOOL);
				S_PlayActorSound(SQUISHED, p->actorsqu);
				if (isIn(p->actorsqu->s.picnum, respawnlist))
				{
					if (p->actorsqu->s.yvel)
						fi.operaterespawns(p->actorsqu->s.yvel);
				}

				if (p->actorsqu->s.picnum == TILE_APLAYER)
				{
					quickkill(&ps[p->actorsqu->s.yvel]);
					ps[p->actorsqu->s.yvel].frag_ps = snum;
				}
				else if (badguy(&p->actorsqu->s))
				{
					deletesprite(p->actorsqu);
					p->actors_killed++;
				}
				else deletesprite(p->actorsqu);
			}
			p->actorsqu = nullptr;
		}
	}

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int makepainsounds(int snum, int type)
{
	auto p = &ps[snum];
	auto actor = p->GetActor();
	auto s = &actor->s;
	int k = 0;

	switch (type)
	{
	case 0:
		if (rnd(32))
		{
			if (p->boot_amount > 0)
				k = 1;
			else
			{
				if (!S_CheckActorSoundPlaying(actor, DUKE_LONGTERM_PAIN))
					S_PlayActorSound(DUKE_LONGTERM_PAIN, actor);
				SetPlayerPal(p, PalEntry(32, 64, 64, 64));
				s->extra -= 1 + (krand() & 3);
				if (!S_CheckActorSoundPlaying(actor, SHORT_CIRCUIT))
					S_PlayActorSound(SHORT_CIRCUIT, actor);
			}
		}
		break;
	case 1:
		if (rnd(16))
		{
			if (p->boot_amount > 0)
				k = 1;
			else
			{
				if (!S_CheckActorSoundPlaying(actor, DUKE_LONGTERM_PAIN))
					S_PlayActorSound(DUKE_LONGTERM_PAIN, actor);
				SetPlayerPal(p, PalEntry(32, 0, 8, 0));
				s->extra -= 1 + (krand() & 3);
			}
		}
		break;
	case 2:
		if (rnd(32))
		{
			if (p->boot_amount > 0)
				k = 1;
			else
			{
				if (!S_CheckActorSoundPlaying(actor, DUKE_LONGTERM_PAIN))
					S_PlayActorSound(DUKE_LONGTERM_PAIN, actor);
				SetPlayerPal(p, PalEntry(32, 8, 0, 0));
				s->extra -= 1 + (krand() & 3);
			}
		}
		break;
	case 3:
		if ((krand() & 3) == 1)
			if (p->on_ground)
			{
				if (p->OnMotorcycle)
					s->extra -= 2;
				else
					s->extra -= 4;
				S_PlayActorSound(DUKE_LONGTERM_PAIN, actor);
			}
		break;
	}
	return k;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void footprints(int snum)
{
	auto p = &ps[snum];
	auto actor = p->GetActor();
	auto s = &actor->s;
	auto psect = s->sectnum;

	if (p->footprintcount > 0 && p->on_ground)
		if ((sector[p->cursectnum].floorstat & 2) != 2)
		{
			int j = -1;
			DukeSectIterator it(psect);
			while (auto act = it.Next())
			{
				if (act->s.picnum == TILE_FOOTPRINTS || act->s.picnum == TILE_FOOTPRINTS2 || act->s.picnum == TILE_FOOTPRINTS3 || act->s.picnum == TILE_FOOTPRINTS4)
					if (abs(act->s.x - p->posx) < 384)
						if (abs(act->s.y - p->posy) < 384)
						{
							j = 1;
							break;
						}
			}
			if (j < 0)
			{
				p->footprintcount--;
				if (sector[p->cursectnum].lotag == 0 && sector[p->cursectnum].hitag == 0)
				{
					DDukeActor* fprint;
					switch (krand() & 3)
					{
					case 0:	 fprint = spawn(actor, TILE_FOOTPRINTS); break;
					case 1:	 fprint = spawn(actor, TILE_FOOTPRINTS2); break;
					case 2:	 fprint = spawn(actor, TILE_FOOTPRINTS3); break;
					default: fprint = spawn(actor, TILE_FOOTPRINTS4); break;
					}
					fprint->s.pal = p->footprintpal;
					fprint->s.shade = p->footprintshade;
				}
			}
		}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

inline void backupplayer(player_struct* p)
{
	backuppos(p);
	p->angle.backup();
	p->horizon.backup();
}

void playerisdead(int snum, int psectlotag, int fz, int cz)
{
	auto p = &ps[snum];
	auto actor = p->GetActor();
	auto s = &actor->s;

	if (p->dead_flag == 0)
	{
		if (s->pal != 1)
		{
			SetPlayerPal(p, PalEntry(63, 63, 0, 0));
			p->posz -= (16 << 8);
			s->z -= (16 << 8);
		}
#if 0
		if (ud.recstat == 1 && ud.multimode < 2)
			closedemowrite();
#endif

		if (s->pal != 1)
			p->dead_flag = (512 - ((krand() & 1) << 10) + (krand() & 255) - 512) & 2047;

		p->jetpack_on = 0;
		p->holoduke_on = nullptr;

		if (!isRR())S_StopSound(DUKE_JETPACK_IDLE, actor);
		S_StopSound(-1, actor, CHAN_VOICE);


		if (s->pal != 1 && (s->cstat & 32768) == 0) s->cstat = 0;

		if (ud.multimode > 1 && (s->pal != 1 || (s->cstat & 32768)))
		{
			if (p->frag_ps != snum)
			{
				ps[p->frag_ps].frag++;
				frags[p->frag_ps][snum]++;

				auto pname = &ud.user_name[p->frag_ps][0];
				if (snum == screenpeek)
				{
					Printf(PRINT_NOTIFY, "Killed by %s", pname);
				}
				else
				{
					Printf(PRINT_NOTIFY, "Killed %s", pname);
				}

			}
			else p->fraggedself++;

#if 0
			if (myconnectindex == connecthead)
			{
				sprintf(tempbuf, "frag %d killed %d\n", p->frag_ps + 1, snum + 1);
				sendscore(tempbuf);
				//					  printf(tempbuf);
			}
#endif

			p->frag_ps = snum;
		}
	}

	if (psectlotag == ST_2_UNDERWATER)
	{
		if (p->on_warping_sector == 0)
		{
			if (abs(p->posz - fz) > (PHEIGHT >> 1))
				p->posz += 348;
		}
		else
		{
			s->z -= 512;
			s->zvel = -348;
		}

		clipmove(&p->posx, &p->posy, &p->posz, &p->cursectnum, 0, 0, 164, (4 << 8), (4 << 8), CLIPMASK0);
		//			  p->bobcounter += 32;
	}

	backupplayer(p);

	p->horizon.horizoff = p->horizon.horiz = q16horiz(0);

	updatesector(p->posx, p->posy, &p->cursectnum);

	pushmove(&p->posx, &p->posy, &p->posz, &p->cursectnum, 128L, (4 << 8), (20 << 8), CLIPMASK0);

	if (fz > cz + (16 << 8) && s->pal != 1)
		p->angle.rotscrnang = buildlook(p->dead_flag + ((fz + p->posz) >> 7));

	p->on_warping_sector = 0;

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int endoflevel(int snum)
{
	auto p = &ps[snum];

	// the fist puching the end-of-level thing...
	p->fist_incs++;
	if (p->fist_incs == 28)
	{
#if 0
		if (ud.recstat == 1) closedemowrite();
#endif
		S_PlaySound(PIPEBOMB_EXPLODE);
		SetPlayerPal(p, PalEntry(48, 64, 64, 64));
	}
	if (p->fist_incs > 42)
	{
		setnextmap(!!p->buttonpalette);
		return 1;
	}
	return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int timedexit(int snum)
{
	auto p = &ps[snum];
	p->timebeforeexit--;
	if (p->timebeforeexit == 26 * 5)
	{
		FX_StopAllSounds();
		if (p->customexitsound >= 0)
		{
			S_PlaySound(p->customexitsound);
			FTA(102, p);
		}
	}
	else if (p->timebeforeexit == 1)
	{
		setnextmap(false);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void playerCrouch(int snum)
{
	auto p = &ps[snum];
	// crouching
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_CROUCH, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) == 0)
	{
		p->posz += (2048 + 768);
		p->crack_time = CRACK_TIME;
	}
}

void playerJump(int snum, int fz, int cz)
{
	auto p = &ps[snum];
	if (p->jumping_toggle == 0 && p->jumping_counter == 0)
	{
		if ((fz - cz) > (56 << 8))
		{
			SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
			OnEvent(EVENT_JUMP, snum, p->GetActor(), -1);
			if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) == 0)
			{
				p->jumping_counter = 1;
				p->jumping_toggle = 1;
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void apply_seasick(player_struct* p, double factor)
{
	if (isRRRA() && p->SeaSick && p->dead_flag == 0)
	{
		if (p->SeaSick < 250)
		{
			if (p->SeaSick >= 180)
				p->angle.rotscrnang += bamlook(xs_CRoundToUInt(24 * factor * BAMUNIT));
			else if (p->SeaSick >= 130)
				p->angle.rotscrnang -= bamlook(xs_CRoundToUInt(24 * factor * BAMUNIT));
			else if (p->SeaSick >= 70)
				p->angle.rotscrnang += bamlook(xs_CRoundToUInt(24 * factor * BAMUNIT));
			else if (p->SeaSick >= 20)
				p->angle.rotscrnang -= bamlook(xs_CRoundToUInt(24 * factor * BAMUNIT));
		}
		if (p->SeaSick < 250)
			p->angle.look_ang = bamlook(xs_CRoundToUInt(((krand() & 255) - 128) * factor * BAMUNIT));
	}
}

//---------------------------------------------------------------------------
//
// split off because it can be called from multiple places.
//
//---------------------------------------------------------------------------

void processavel(player_struct* p, float* avel)
{
	// Taken from processinput() for use with applying look while cl_syncinput is 0.
	if (p->psectlotag == ST_2_UNDERWATER)
	{
		*avel = (*avel - (*avel / 8.f)) * sgn(TICSPERFRAME);
	}
	else
	{
		*avel = *avel * sgn(TICSPERFRAME);
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void backuppos(player_struct* p, bool noclipping)
{
	if (!noclipping)
	{
		p->oposx = p->posx;
		p->oposy = p->posy;
	}
	else
	{
		p->posx = p->oposx;
		p->posy = p->oposy;
	}

	p->oposz = p->posz;
	p->bobposx = p->posx;
	p->bobposy = p->posy;
	p->opyoff = p->pyoff;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void backupweapon(player_struct* p)
{
	p->oweapon_sway = p->weapon_sway;
	p->oweapon_pos = p->weapon_pos;
	p->okickback_pic = p->kickback_pic;
	p->orandom_club_frame = p->random_club_frame;
	p->ohard_landing = p->hard_landing;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void checkhardlanding(player_struct* p)
{
	if (p->hard_landing > 0)
	{
		p->horizon.addadjustment(-(p->hard_landing << 4));
		p->hard_landing--;
	}
}

void playerweaponsway(player_struct* p, spritetype* s)
{
	if (cl_weaponsway)
	{
		if (s->xvel < 32 || p->on_ground == 0 || p->bobcounter == 1024)
		{
			if ((p->weapon_sway & 2047) > (1024 + 96))
				p->weapon_sway -= 96;
			else if ((p->weapon_sway & 2047) < (1024 - 96))
				p->weapon_sway += 96;
			else p->oweapon_sway = p->weapon_sway = 1024;
		}
		else
		{
			p->weapon_sway = p->bobcounter;

			if ((p->bobcounter - p->oweapon_sway) > 256)
			{
				p->oweapon_sway = p->weapon_sway;
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void checklook(int snum, ESyncBits actions)
{
	auto p = &ps[snum];

	if ((actions & SB_LOOK_LEFT) && !p->OnMotorcycle)
	{
		SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
		OnEvent(EVENT_LOOKLEFT, snum, p->GetActor(), -1);
		if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) != 0)
		{
			actions &= ~SB_LOOK_LEFT;
		}
	}

	if ((actions & SB_LOOK_RIGHT) && !p->OnMotorcycle)
	{
		SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
		OnEvent(EVENT_LOOKRIGHT, snum, p->GetActor(), -1);
		if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) != 0)
		{
			actions &= ~SB_LOOK_RIGHT;
		}
	}
	p->angle.backup();
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void playerCenterView(int snum)
{
	auto p = &ps[snum];
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_RETURNTOCENTER, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) == 0)
	{
		p->sync.actions |= SB_CENTERVIEW;
	}
	else
	{
		p->sync.actions &= ~SB_CENTERVIEW;
	}
}

void playerLookUp(int snum, ESyncBits actions)
{
	auto p = &ps[snum];
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_LOOKUP, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) == 0)
	{
		p->sync.actions |= SB_CENTERVIEW;
	}
	else
	{
		p->sync.actions &= ~SB_LOOK_UP;
	}
}

void playerLookDown(int snum, ESyncBits actions)
{
	auto p = &ps[snum];
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_LOOKDOWN, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) == 0)
	{
		p->sync.actions |= SB_CENTERVIEW;
	}
	else
	{
		p->sync.actions &= ~SB_LOOK_DOWN;
	}
}

void playerAimUp(int snum, ESyncBits actions)
{
	auto p = &ps[snum];
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_AIMUP, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) != 0)
	{
		p->sync.actions &= ~SB_AIM_UP;
	}
}

void playerAimDown(int snum, ESyncBits actions)
{
	auto p = &ps[snum];
	SetGameVarID(g_iReturnVarID, 0, p->GetActor(), snum);
	OnEvent(EVENT_AIMDOWN, snum, p->GetActor(), -1);
	if (GetGameVarID(g_iReturnVarID, p->GetActor(), snum) != 0)
	{
		p->sync.actions &= ~SB_AIM_DOWN;
	}
}

//---------------------------------------------------------------------------
//
// split out so that the weapon check can be done right.
//
//---------------------------------------------------------------------------

bool movementBlocked(int snum)
{
	auto p = &ps[snum];

	auto blockingweapon = [=]()
	{
		if (isRR()) return false;
		if (isWW2GI()) return aplWeaponWorksLike[p->curr_weapon][snum] == TRIPBOMB_WEAPON;
		else return p->curr_weapon == TRIPBOMB_WEAPON;
	};

	auto weapondelay = [=]()
	{
		if (isWW2GI()) return aplWeaponFireDelay[p->curr_weapon][snum];
		else return 4;
	};

	return (p->fist_incs ||
		p->transporter_hold > 2 ||
		p->hard_landing ||
		p->access_incs > 0 ||
		p->knee_incs > 0 ||
		p->newOwner != nullptr ||
		(blockingweapon() && p->kickback_pic > 1 && p->kickback_pic < weapondelay()));
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int haskey(int sect, int snum)
{
	auto p = &ps[snum];
	if (!sectorextra[sect])
		return 1;
	if (sectorextra[sect] > 6)
		return 1;
	int wk = sectorextra[sect];
	if (wk > 3)
		wk -= 3;

	if (p->keys[wk] == 1)
	{
		sectorextra[sect] = 0;
		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void shootbloodsplat(DDukeActor* actor, int p, int sx, int sy, int sz, int sa, int atwith, int BIGFORCE, int OOZFILTER, int NEWBEAST)
{
	spritetype* const s = &actor->s;
	int sect = s->sectnum;
	int zvel;
	short hitsect, hitwall;
	int hitx, hity, hitz;
	DDukeActor* d;

	if (p >= 0)
		sa += 64 - (krand() & 127);
	else sa += 1024 + 64 - (krand() & 127);
	zvel = 1024 - (krand() & 2047);


	hitscan(sx, sy, sz, sect,
		bcos(sa),
		bsin(sa), zvel << 6,
		&hitsect, &hitwall, &d, &hitx, &hity, &hitz, CLIPMASK1);

	// oh my...
	if (FindDistance2D(sx - hitx, sy - hity) < 1024 &&
		(hitwall >= 0 && wall[hitwall].overpicnum != BIGFORCE) &&
		((wall[hitwall].nextsector >= 0 && hitsect >= 0 &&
			sector[wall[hitwall].nextsector].lotag == 0 &&
			sector[hitsect].lotag == 0 &&
			sector[wall[hitwall].nextsector].lotag == 0 &&
			(sector[hitsect].floorz - sector[wall[hitwall].nextsector].floorz) > (16 << 8)) ||
			(wall[hitwall].nextsector == -1 && sector[hitsect].lotag == 0)))
	{
		if ((wall[hitwall].cstat & 16) == 0)
		{
			if (wall[hitwall].nextsector >= 0)
			{
				DukeSectIterator it(wall[hitwall].nextsector);
				while (auto act2 = it.Next())
				{
					if (act2->s.statnum == STAT_EFFECTOR && act2->s.lotag == SE_13_EXPLOSIVE)
						return;
				}
			}

			if (wall[hitwall].nextwall >= 0 &&
				wall[wall[hitwall].nextwall].hitag != 0)
				return;

			if (wall[hitwall].hitag == 0)
			{
				auto spawned = spawn(actor, atwith);
				spawned->s.xvel = -12;
				spawned->s.ang = getangle(wall[hitwall].x - wall[wall[hitwall].point2].x, wall[hitwall].y - wall[wall[hitwall].point2].y) + 512;
				spawned->s.x = hitx;
				spawned->s.y = hity;
				spawned->s.z = hitz;
				spawned->s.cstat |= (krand() & 4);
				ssp(spawned, CLIPMASK0);
				setsprite(spawned, spawned->s.pos);
				if (s->picnum == OOZFILTER || s->picnum == NEWBEAST)
					spawned->s.pal = 6;
			}
		}
	}
}


//---------------------------------------------------------------------------
//
// view - as in third person view (stupid name for this function)
//
//---------------------------------------------------------------------------

bool view(struct player_struct* pp, int* vx, int* vy, int* vz, short* vsectnum, int ang, fixed_t q16horiz, double smoothratio)
{
	spritetype* sp;
	int i, nx, ny, nz, hx, hy, hitx, hity, hitz;
	short bakcstat, hitsect, hitwall, daang;
	DDukeActor* hitsprt;

	nx = -bcos(ang, -4);
	ny = -bsin(ang, -4);
	nz = q16horiz >> 9;

	sp = &pp->GetActor()->s;

	bakcstat = sp->cstat;
	sp->cstat &= (short)~0x101;

	updatesectorz(*vx, *vy, *vz, vsectnum);
	hitscan(*vx, *vy, *vz, *vsectnum, nx, ny, nz, &hitsect, &hitwall, &hitsprt, &hitx, &hity, &hitz, CLIPMASK1);

	if (*vsectnum < 0)
	{
		sp->cstat = bakcstat;
		return false;
	}

	hx = hitx - (*vx); hy = hity - (*vy);
	if (abs(nx) + abs(ny) > abs(hx) + abs(hy))
	{
		*vsectnum = hitsect;
		if (hitwall >= 0)
		{
			daang = getangle(wall[wall[hitwall].point2].x - wall[hitwall].x,
				wall[wall[hitwall].point2].y - wall[hitwall].y);

			i = nx * bsin(daang) + ny * -bcos(daang);
			if (abs(nx) > abs(ny)) hx -= mulscale28(nx, i);
			else hy -= mulscale28(ny, i);
		}
		else if (!hitsprt)
		{
			if (abs(nx) > abs(ny)) hx -= (nx >> 5);
			else hy -= (ny >> 5);
		}
		if (abs(nx) > abs(ny)) i = divscale16(hx, nx);
		else i = divscale16(hy, ny);
		if (i < cameradist) cameradist = i;
	}
	*vx = (*vx) + mulscale16(nx, cameradist);
	*vy = (*vy) + mulscale16(ny, cameradist);
	*vz = (*vz) + mulscale16(nz, cameradist);
	
	int myclock = ud.levelclock + int(TICSPERFRAME/65536. * smoothratio);
	if (cameraclock == INT_MIN) cameraclock = myclock;	// third person view was just started.

	cameradist = min(cameradist + ((myclock - cameraclock) << 10), 65536);
	cameraclock = myclock;

	updatesectorz(*vx, *vy, *vz, vsectnum);

	sp->cstat = bakcstat;
	return true;
}
	 

END_DUKE_NS
