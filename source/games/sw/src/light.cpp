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

#include "build.h"

#include "names2.h"
#include "game.h"
#include "light.h"

BEGIN_SW_NS


void SectorLightShade(DSWActor* actor, short intensity)
{
	auto u = actor->hasU()? actor->u() : nullptr;
    auto sp = &actor->s();
    int8_t* wall_shade;
    short base_shade;

    if (TEST_BOOL8(sp))
        intensity = -intensity;

    if (!TEST_BOOL2(sp))
    {
        if (!TEST_BOOL6(sp))
            sp->sector()->floorpal = sp->pal;
        sp->sector()->floorshade = LIGHT_FloorShade(sp) + intensity;     // floor change
    }

    if (!TEST_BOOL3(sp))
    {
        if (!TEST_BOOL6(sp))
            sp->sector()->ceilingpal = sp->pal;
        sp->sector()->ceilingshade = LIGHT_CeilingShade(sp) + intensity;   // ceiling change
    }

    // change wall
    if (!TEST_BOOL4(sp))
    {
        ASSERT(u && u->WallShade.Data());
        wall_shade = u->WallShade.Data();
        int wallcount = 0;

        for(auto &wal : wallsofsector(sp->sector()))
        {
            base_shade = wall_shade[wallcount];
            wal.shade = base_shade + intensity;
            if (!TEST_BOOL6(sp))
                wal.pal = sp->pal;
            wallcount++;

            if (TEST(sp->extra, SPRX_BOOL5))
            {
                if (wal.twoSided())
                {
                    auto nextWall = wal.nextWall();
                    base_shade = wall_shade[wallcount];
                    nextWall->shade = base_shade + intensity;
                    if (!TEST_BOOL6(sp))
                        nextWall->pal = sp->pal;
                    wallcount++;
                }
            }
        }
    }
}


void DiffuseLighting(DSWActor* actor)
{
    auto sp = &actor->s();
    int i;
    short count;
    short shade;
    SPRITEp dsp;

    // diffused lighting
    count = 0;
    SWStatIterator it(STAT_LIGHTING_DIFFUSE);
    while (auto itActor = it.Next())
    {
        dsp = &itActor->s();

        // make sure matchs match
        if (LIGHT_Match(itActor) != LIGHT_Match(actor))
            continue;

        shade = sp->shade + ((LIGHT_DiffuseNum(dsp) + 1) * LIGHT_DiffuseMult(dsp));

        if (shade > LIGHT_MaxDark(sp))
            shade = LIGHT_MaxDark(sp);

        if (!TEST_BOOL6(dsp))
            dsp->pal = sp->pal;

        SectorLightShade(itActor, shade);

        count++;
    }
}

void DoLightingMatch(short match, short state)
{
    SPRITEp sp;

    SWStatIterator it(STAT_LIGHTING);
    while (auto itActor = it.Next())
    {
        auto u = itActor->u();
        sp = &itActor->s();

        if (LIGHT_Match(itActor) != match)
            continue;

        switch (LIGHT_Type(sp))
        {
        case LIGHT_CONSTANT:

            // initialized
            SET_BOOL9(sp);

            // toggle
            if (state == -1)
                state = !TEST_BOOL1(sp);

            if (state == ON)
            {
                SET_BOOL1(sp);
                sp->shade = -LIGHT_MaxBright(sp);
                sp->pal = u->spal; // on
                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
            }
            else
            {
                RESET_BOOL1(sp);
                sp->shade = LIGHT_MaxDark(sp);
                sp->pal = 0; // off
                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
            }
            break;

        case LIGHT_FLICKER:
        case LIGHT_FADE:
            // initialized
            SET_BOOL9(sp);

            // toggle
            if (state == -1)
                state = !TEST_BOOL1(sp);

            if (state == ON)
            {
                // allow fade or flicker
                SET_BOOL1(sp);
            }
            else
            {
                RESET_BOOL1(sp);
                sp->shade = LIGHT_MaxDark(sp);
                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
            }
            break;

        case LIGHT_FADE_TO_ON_OFF:

            // initialized
            SET_BOOL9(sp);

            // toggle
            //if (state == -1)
            //    state = !TEST_BOOL1(sp);

            if (state == ON)
            {
                if (LIGHT_Dir(sp) == 1)
                {
                    LIGHT_DirChange(sp);
                }
            }
            else if (state == OFF)
            {
                if (LIGHT_Dir(sp) == 0)
                {
                    LIGHT_DirChange(sp);
                }
            }

            // allow fade or flicker
            SET_BOOL1(sp);
            break;

        case LIGHT_FLICKER_ON:

            // initialized
            SET_BOOL9(sp);

            // toggle
            if (state == -1)
                state = !TEST_BOOL1(sp);

            if (state == ON)
            {
                // allow fade or flicker
                SET_BOOL1(sp);
            }
            else
            {
                // turn it off till next switch
                auto spal = sp->pal;
                RESET_BOOL1(sp);
                sp->pal = 0;
                sp->shade = LIGHT_MaxDark(sp);
                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
                sp->pal = spal;
            }
            break;
        }
    }
}

void InitLighting(void)
{
    SPRITEp sp;

    // processed on level startup
    // puts lights in correct state
    SWStatIterator it(STAT_LIGHTING);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (!TEST_BOOL9(sp))
        {
            DoLightingMatch(LIGHT_Match(actor), !!TEST_BOOL1(sp));
        }
    }
}

void DoLighting(void)
{
    SPRITEp sp;

    SWStatIterator it(STAT_LIGHTING);
    while (auto itActor = it.Next())
    {
        auto u = itActor->u();
        sp = &itActor->s();

        // on/off test
        if (TEST_BOOL1(sp) == OFF)
            continue;

        switch (LIGHT_Type(sp))
        {
        case LIGHT_CONSTANT:
            break;

        case LIGHT_FLICKER:

            LIGHT_Tics(sp) += synctics;
            while (LIGHT_Tics(sp) >= LIGHT_MaxTics(sp))
            {
                LIGHT_Tics(sp) -= LIGHT_MaxTics(sp);

                if ((RANDOM_P2(128 << 8) >> 8) > 64)
                {
                    sp->shade = -LIGHT_MaxBright(sp) + RandomRange(LIGHT_MaxBright(sp) + LIGHT_MaxDark(sp));
                    SectorLightShade(itActor, sp->shade);
                    DiffuseLighting(itActor);
                }
                else
                {
                    // turn off lighting - even colored lighting
                    auto spal = sp->pal;
                    sp->pal = 0;
                    sp->shade = LIGHT_MaxDark(sp);
                    SectorLightShade(itActor, sp->shade);
                    DiffuseLighting(itActor);
                    sp->pal = spal;
                }
            }

            break;

        case LIGHT_FADE:

            LIGHT_Tics(sp) += synctics;

            while (LIGHT_Tics(sp) >= LIGHT_MaxTics(sp))
            {
                LIGHT_Tics(sp) -= LIGHT_MaxTics(sp);

                if (LIGHT_Dir(sp) == 1)
                {
                    sp->shade += LIGHT_ShadeInc(sp);
                    if (sp->shade >= LIGHT_MaxDark(sp))
                        LIGHT_DirChange(sp);
                }
                else
                {
                    sp->shade -= LIGHT_ShadeInc(sp);
                    if (sp->shade <= -LIGHT_MaxBright(sp))
                        LIGHT_DirChange(sp);
                }

                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
            }

            break;

        case LIGHT_FADE_TO_ON_OFF:

            LIGHT_Tics(sp) += synctics;

            while (LIGHT_Tics(sp) >= LIGHT_MaxTics(sp))
            {
                LIGHT_Tics(sp) -= LIGHT_MaxTics(sp);

                if (LIGHT_Dir(sp) == 1)
                {
                    sp->shade += LIGHT_ShadeInc(sp);
                    if (sp->shade >= LIGHT_MaxDark(sp))
                    {
                        sp->pal = 0; // off
                        LIGHT_DirChange(sp);
                        // stop it until switch is hit
                        RESET_BOOL1(sp);
                    }
                }
                else
                {
                    sp->shade -= LIGHT_ShadeInc(sp);
                    sp->pal = u->spal; // on
                    if (sp->shade <= -LIGHT_MaxBright(sp))
                    {
                        LIGHT_DirChange(sp);
                        // stop it until switch is hit
                        RESET_BOOL1(sp);
                    }
                }

                SectorLightShade(itActor, sp->shade);
                DiffuseLighting(itActor);
            }

            break;

        case LIGHT_FLICKER_ON:

            LIGHT_Tics(sp) += synctics;

            while (LIGHT_Tics(sp) >= LIGHT_MaxTics(sp))
            {
                LIGHT_Tics(sp) -= LIGHT_MaxTics(sp);

                if ((RANDOM_P2(128 << 8) >> 8) > 64)
                {
                    sp->shade = -LIGHT_MaxBright(sp) + RandomRange(LIGHT_MaxBright(sp) + LIGHT_MaxDark(sp));
                    SectorLightShade(itActor, sp->shade);
                    DiffuseLighting(itActor);
                }
                else
                {
                    // turn off lighting - even colored lighting
                    auto spal = sp->pal;
                    sp->pal = 0;
                    sp->shade = LIGHT_MaxDark(sp);
                    SectorLightShade(itActor, sp->shade);
                    DiffuseLighting(itActor);
                    sp->pal = spal;
                }

                if ((RANDOM_P2(128 << 8) >> 8) < 8)
                {
                    // set to full brightness
                    sp->shade = -LIGHT_MaxBright(sp);
                    SectorLightShade(itActor, sp->shade);
                    DiffuseLighting(itActor);
                    // turn it off until a swith happens
                    RESET_BOOL1(sp);
                }
            }
            break;
        }
    }
}
END_SW_NS
