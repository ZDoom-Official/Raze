#include "ns.h"
#include "wh.h"

BEGIN_WH_NS

void spawnabaddy(int i, int monster);

static void chase(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];
	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0)
		spr.lotag = 250;

	short osectnum = spr.sectnum;

	if (krand() % 63 == 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, sprite[i].x, sprite[i].y,
			sprite[i].z - (tileHeight(sprite[i].picnum) << 7), sprite[i].sectnum))// && invisibletime < 0)
			newstatus(i, ATTACK);
	}
	else {
		checksight(plr, i);
		if (!checkdist(i, plr.x, plr.y, plr.z)) {
			int movestat = aimove(i);
			if ((movestat & kHitTypeMask) == kHitFloor)
			{
				spr.ang = (short)((spr.ang + 1024) & 2047);
				newstatus(i, FLEE);
				return;
			}
		}
		else {
			if (krand() % 8 == 0) // NEW
				newstatus(i, ATTACK); // NEW
			else { // NEW
				sprite[i].ang = (short)(((krand() & 512 - 256) + sprite[i].ang + 1024) & 2047); // NEW
				newstatus(i, FLEE); // NEW
			}
		}
	}

	getzrange(spr.x, spr.y, spr.z - 1, spr.sectnum, (spr.clipdist) << 2, CLIPMASK0);
	spr.z = zr_florz;

	if ((spr.sectnum != osectnum) && (sector[spr.sectnum].lotag == 10))
		warpsprite(i);

	if (checksector6(i))
		return;

	processfluid(i, zr_florhit, false);

	if (sector[osectnum].lotag == KILLSECTOR) {
		spr.hitag--;
		if (spr.hitag < 0)
			newstatus(i, DIE);
	}

	setsprite(i, spr.x, spr.y, spr.z);
}
	
static void resurect(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		newstatus(i, FACE);
		spr.picnum = KATIE;
		spr.hitag = (short)adjusthp(200);
		spr.lotag = 100;
		spr.cstat |= 1;
	}
}
	
static void search(PLAYER& plr, short i) {
	aisearch(plr, i, false);
	checksector6(i);
}
	
static void pain(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];
	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum = KATIE;
		spr.ang = plr.angle.ang.asbuild();
		newstatus(i, FLEE);
	}

	aimove(i);
	processfluid(i, zr_florhit, false);
	setsprite(i, spr.x, spr.y, spr.z);
}
	
static void face(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];

	spr.ang = (short)(getangle(plr.x - spr.x, plr.y - spr.y) & 2047);

	boolean cansee = ::cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y, spr.z - (tileHeight(spr.picnum) << 7),
		spr.sectnum);

	if (cansee && plr.invisibletime < 0) {
		if (plr.shadowtime > 0) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang + 1024) & 2047);
			newstatus(i, FLEE);
		}
		else {
			spr.owner = plr.spritenum;
			newstatus(i, CHASE);
		}
	}
	else { // get off the wall
		if (spr.owner == plr.spritenum) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang) & 2047);
			newstatus(i, FINDME);
		}
		else if (cansee) newstatus(i, FLEE);
	}

	if (checkdist(i, plr.x, plr.y, plr.z))
		newstatus(i, ATTACK);
}
	
static void attack(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];

	getzrange(spr.x, spr.y, spr.z - 1, spr.sectnum, (spr.clipdist) << 2, CLIPMASK0);
	spr.z = zr_florz;

	switch (checkfluid(i, zr_florhit)) {
	case TYPELAVA:
	case TYPEWATER:
		spr.z += tileHeight(spr.picnum) << 5;
		break;
	}

	setsprite(i, spr.x, spr.y, spr.z);

	sprite[i].lotag -= TICSPERFRAME;
	if (sprite[i].lotag < 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, sprite[i].x, sprite[i].y,
			sprite[i].z - (tileHeight(sprite[i].picnum) << 7), sprite[i].sectnum))
			newstatus(i, CAST);
		else
			newstatus(i, CHASE);
	}
	else
		sprite[i].ang = getangle(plr.x - sprite[i].x, plr.y - sprite[i].y);
}
	
static void flee(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];
	spr.lotag -= TICSPERFRAME;
	short osectnum = spr.sectnum;

	int movestat = aimove(i);
	if ((movestat & kHitTypeMask) != kHitFloor && movestat != 0) {
		if ((movestat & kHitTypeMask) == kHitWall) {
			int nWall = movestat & kHitIndexMask;
			int nx = -(wall[wall[nWall].point2].y - wall[nWall].y) >> 4;
			int ny = (wall[wall[nWall].point2].x - wall[nWall].x) >> 4;
			spr.ang = getangle(nx, ny);
		}
		else {
			spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
			newstatus(i, FACE);
		}
	}

	if (movestat != 0) {
		spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
		newstatus(i, FACE);
	}
	if (spr.lotag < 0)
		newstatus(i, FACE);

	if ((spr.sectnum != osectnum) && (sector[spr.sectnum].lotag == 10))
		warpsprite(i);

	if (checksector6(i))
		return;

	processfluid(i, zr_florhit, false);

	setsprite(i, spr.x, spr.y, spr.z);
}
	
static void cast(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum++;
		spr.lotag = 12;
	}

	if (spr.picnum == KATIEAT + 2) {
		if (spr.extra >= 2) {
			if (rand() % 100 > 50) {
				for (int j = 0; j < 4; j++) {
					spawnabaddy(i, GONZOGSH);
				}
			}
			else {
				for (int j = 0; j < 6; j++) {
					spawnabaddy(i, GRONSW);
				}
			}
			spr.picnum = KATIE;
			spr.extra--;
			spritesound(S_FIREBALL, &spr);
			newstatus(i, CHASE);
		}
	}

	if (spr.picnum == KATIEAT + 6) {
		if (spr.extra == 1) {
			for (short j = 0; j < MAXSPRITES; j++) {
				if (sprite[j].pal == 8) {
					sprite[j].picnum--;
					sprite[j].pal = 0;
					sprite[j].shade = 0;
					changespritestat(j, FACE);
				}
			}
			spr.picnum = KATIE;
			spritesound(S_FIREBALL, &spr);
			newstatus(i, CHASE);
			spr.extra--;
		}
	}

	if (spr.picnum == KATIEAT + 16) {
		spr.picnum = KATIE;
		spritesound(S_FIREBALL, &spr);
		castspell(plr, i);
		newstatus(i, CHASE);
		spr.extra++;
	}
	checksector6(i);
}
	
static void die(PLAYER& plr, short i) {
	SPRITE& spr = sprite[i];
	spr.lotag -= TICSPERFRAME;

	if (spr.lotag <= 0) {
		spr.picnum++;
		spr.lotag = 20;

		if (spr.picnum == KATIEDEAD) {
			if (difficulty == 4)
				newstatus(i, RESURECT);
			else {
				kills++;
				newstatus(i, DEAD);
			}
		}
	}
}

void createKatieAI() {
	auto& e = enemy[KATIETYPE];
	e.info.Init(35, 35, 2048, 120, 0, 64, false, 200, 0);
	e.chase = chase;
	e.resurect = resurect;
	e.search = search;
	e.pain = pain;
	e.face = face;
	e.attack = attack;
	e.flee = flee;
	e.cast = cast;
	e.die = die;
}

void premapKatie(short i) {
	SPRITE& spr = sprite[i];

	spr.detail = KATIETYPE;
	changespritestat(i, FACE);
	enemy[KATIETYPE].info.set(spr);
	spr.extra = 5;
}

END_WH_NS
