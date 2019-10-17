/*
 * High-colour textures support for Polymost
 * by Jonathon Fowler
 * See the included license file "BUILDLIC.TXT" for license info.
 */

#include "build.h"
#include "compat.h"
#include "kplib.h"
#include "hightile.h"
#include "baselayer.h"


polytint_t hictinting[MAXPALOOKUPS];


//
// hicinit()
//   Initialize the high-colour stuff to default.
//
void hicinit(void)
{
    int32_t i;

    for (i=0; i<MAXPALOOKUPS; i++)  	// all tints should be 100%
    {
        polytint_t & tint = hictinting[i];
        tint.r = tint.g = tint.b = 0xff;
        tint.f = 0;
    }
}


//
// hicsetpalettetint(pal,r,g,b,sr,sg,sb,effect)
//   The tinting values represent a mechanism for emulating the effect of global sector
//   palette shifts on true-colour textures and only true-colour textures.
//   effect bitset: 1 = greyscale, 2 = invert
//
void hicsetpalettetint(int32_t palnum, char r, char g, char b, char sr, char sg, char sb, polytintflags_t effect)
{
    if ((uint32_t)palnum >= (uint32_t)MAXPALOOKUPS) return;

    polytint_t & tint = hictinting[palnum];
    tint.r = r;
    tint.g = g;
    tint.b = b;
    tint.sr = sr;
    tint.sg = sg;
    tint.sb = sb;
    tint.f = effect;
}



