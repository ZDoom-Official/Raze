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
#include "baselayer.h"
#include "osd.h"
#include "gamecvars.h"

#include "settings.h"
#include "mytypes.h"
#include "scriplib.h"
#include "gamedefs.h"
#include "keyboard.h"
#include "gamecontrol.h"
#include "control.h"
#include "fx_man.h"
#include "sounds.h"
#include "config.h"
#include "common_game.h"

// we load this in to get default button and key assignments
// as well as setting up function mappings

#include "_functio.h"
#include "_config.h"

#if defined RENDERTYPESDL && defined SDL_TARGET && SDL_TARGET > 1
# include "sdl_inc.h"
#endif

BEGIN_SW_NS

extern void ReadGameSetup(int32_t scripthandle);

//
// Comm variables
//

char CommPlayerName[32];
int32_t NumberPlayers,CommPort,PortSpeed,IrqNumber,UartAddress;

//
// Sound variables
//


int32_t UseMouse = 1, UseJoystick = 0;


//
// Screen variables
//

int32_t ScreenMode = 1;
int32_t ScreenWidth = 640;
int32_t ScreenHeight = 480;
int32_t ScreenBPP = 8;
int32_t ForceSetup = 1;

extern char WangBangMacro[10][64];
char  RTSName[MAXRTSNAMELENGTH];
static int32_t scripthandle = -1;




/*
===================
=
= CONFIG_SetDefaults
=
===================
*/

void CONFIG_SetDefaults(void)
{
    // JBF 20031211
	int32_t i;// , f;
    //uint8_t k1,k2;

    ScreenMode = 1;

#if defined RENDERTYPESDL && SDL_MAJOR_VERSION > 1
    uint32_t inited = SDL_WasInit(SDL_INIT_VIDEO);
    if (inited == 0)
        SDL_Init(SDL_INIT_VIDEO);
    else if (!(inited & SDL_INIT_VIDEO))
        SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) == 0)
    {
        ScreenWidth = dm.w;
        ScreenHeight = dm.h;
    }
    else
#endif
    {
        ScreenWidth = 1024;
        ScreenHeight = 768;
    }

    ScreenBPP = 32;
    memcpy(&gs, &gs_defaults, sizeof(gs));

    Bstrcpy(RTSName, DEFAULTRTSFILE);
    Bstrcpy(CommPlayerName, DEFAULTPLAYERNAME);

    Bstrcpy(WangBangMacro[0], MACRO1);
    Bstrcpy(WangBangMacro[1], MACRO2);
    Bstrcpy(WangBangMacro[2], MACRO3);
    Bstrcpy(WangBangMacro[3], MACRO4);
    Bstrcpy(WangBangMacro[4], MACRO5);
    Bstrcpy(WangBangMacro[5], MACRO6);
    Bstrcpy(WangBangMacro[6], MACRO7);
    Bstrcpy(WangBangMacro[7], MACRO8);
    Bstrcpy(WangBangMacro[8], MACRO9);
    Bstrcpy(WangBangMacro[9], MACRO10);

    //SetMouseDefaults(0);

}


void SetDefaultKeyDefinitions(int style)
{
	CONFIG_SetDefaultKeys(style ? "demolition/defbinds.txt" : "demolition/origbinds.txt");
}

/*
===================
=
= CONFIG_ReadSetup
=
===================
*/

int32_t CONFIG_ReadSetup(void)
{
    //char ret;
    extern char ds[];
    extern char PlayerNameArg[32];

    char waveformtrackname[MAXWAVEFORMTRACKLENGTH] = {0};

    CONFIG_SetDefaults();

    if (buildvfs_exists(setupfilename))
        scripthandle = SCRIPT_Load(setupfilename);

    if (scripthandle < 0) return -1;

	// What was here is no longer needed with a globally stored config that's being read automatically.
    ReadGameSetup(scripthandle);

    if (PlayerNameArg[0] != '\0')
    {
        strcpy(CommPlayerName, PlayerNameArg);
    }
    return 0;
}

END_SW_NS
