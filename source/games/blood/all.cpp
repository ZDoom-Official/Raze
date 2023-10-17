
/*
#ifdef NOONE_EXTENSIONS
#undef NOONE_EXTENSIONS
#endif
*/

#ifndef NOONE_EXTENSIONS
#define NOONE_EXTENSIONS
#endif


#include "src/actor.cpp"
#include "src/ai.cpp"
#include "src/aibat.cpp"
#include "src/aibeast.cpp"
#include "src/aiboneel.cpp"
#include "src/aiburn.cpp"
#include "src/aicaleb.cpp"
#include "src/aicerber.cpp"
#include "src/aicult.cpp"
#include "src/aigarg.cpp"
#include "src/aighost.cpp"
#include "src/aigilbst.cpp"
#include "src/aihand.cpp"
#include "src/aihound.cpp"
#include "src/aiinnoc.cpp"
#include "src/aipod.cpp"
#include "src/airat.cpp"
#include "src/aispid.cpp"
#include "src/aitchern.cpp"
#include "src/aicdud.cpp"
#include "src/aizomba.cpp"
#include "src/aizombf.cpp"
#include "src/animatesprite.cpp"
#include "src/asound.cpp"
#include "src/barf.cpp"
#include "src/blood.cpp"
#include "src/callback.cpp"
#include "src/choke.cpp"
#include "src/db.cpp"
#include "src/dude.cpp"
#include "src/endgame.cpp"
#include "src/eventq.cpp"
#include "src/fire.cpp"
#include "src/fx.cpp"
#include "src/gameutil.cpp"
#include "src/gib.cpp"
#include "src/globals.cpp"
#include "src/hudsprites.cpp"
#include "src/inifile.cpp"
#include "src/levels.cpp"
#include "src/loadsave.cpp"
#include "src/messages.cpp"
#include "src/mirrors.cpp"
#include "src/misc.cpp"
#include "src/osdcmd.cpp"
#include "src/player.cpp"
#include "src/prediction.cpp"
#include "src/preload.cpp"
#include "src/qav.cpp"
#include "src/sbar.cpp"
#include "src/sectorfx.cpp"
#include "src/seq.cpp"
#include "src/sfx.cpp"
#include "src/sound.cpp"
#include "src/tile.cpp"
#include "src/view.cpp"
#include "src/warp.cpp"
#include "src/weapon.cpp"

#ifdef NOONE_EXTENSIONS
#include "src/nnsprinsect.cpp"
#include "src/nnexts.cpp"
#include "src/nnextsif.cpp"
#include "src/nnextstr.cpp"
#include "src/nnextcdud.cpp"
#include "src/nnextcdudserialize.cpp"
#endif

// after nnsprinsect
#include "src/triggers.cpp"

// This includes the VM so it is last
#include "src/vmexports.cpp"
#include "src/d_menu.cpp"

