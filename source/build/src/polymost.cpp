/**************************************************************************************************
"POLYMOST" code originally written by Ken Silverman
Ken Silverman's official web site: http://www.advsys.net/ken

"POLYMOST2" changes Copyright (c) 2018, Alex Dawson
**************************************************************************************************/


#include "build.h"
#include "common.h"
#include "engine_priv.h"
#include "kplib.h"
#include "mdsprite.h"
#include "polymost.h"
#include "files.h"
#include "textures.h"
#include "bitmap.h"
#include "../../glbackend/glbackend.h"

void cleartexturecache();

extern char textfont[2048], smalltextfont[2048];

bool playing_rr;
bool playing_blood;

int32_t rendmode=0;
int32_t usemodels=1;
int32_t usehightile=1;

int fixpalette = 0, fixpalswap = 0;

typedef struct { float x, cy[2], fy[2]; int32_t tag; int16_t n, p, ctag, ftag; } vsptyp;
#define VSPMAX 2048 //<- careful!
static vsptyp vsp[VSPMAX];
static int32_t gtag, viewportNodeCount;
static float xbl, xbr, xbt, xbb;
int32_t domost_rejectcount;
#ifdef YAX_ENABLE
typedef struct { float x, cy[2]; int32_t tag; int16_t n, p, ctag; } yax_vsptyp;
static yax_vsptyp yax_vsp[YAX_MAXBUNCHES*2][VSPMAX];
typedef struct { float x0, x1, cy[2], fy[2]; } yax_hole_t;
static yax_hole_t yax_holecf[2][VSPMAX];
static int32_t yax_holencf[2];
static int32_t yax_drawcf = -1;
#endif

static float dxb1[MAXWALLSB], dxb2[MAXWALLSB];

//POGOTODO: the SCISDIST could be set to 0 now to allow close objects to render properly,
//          but there's a nasty rendering bug that needs to be dug into when setting SCISDIST lower than 1
#define SCISDIST 1.f  //close plane clipping distance

#define SOFTROTMAT 0

float shadescale = 1.0f;
int32_t shadescale_unbounded = 0;

int32_t r_pogoDebug = 0;
int32_t polymostcenterhoriz = 100;

static float gviewxrange;
static float ghoriz, ghoriz2;
static float ghorizcorrect;
double gxyaspect;
float gyxscale, ghalfx, grhalfxdown10, grhalfxdown10x, ghalfy;
float gcosang, gsinang, gcosang2, gsinang2;
float gchang, gshang, gctang, gstang, gvisibility;
float gtang = 0.f;
float gvrcorrection = 1.f;

static vec3d_t xtex, ytex, otex, xtex2, ytex2, otex2;

float fcosglobalang, fsinglobalang;
float fxdim, fydim, fydimen, fviewingrange;

float fsearchx, fsearchy, fsearchz;
int psectnum, pwallnum, pbottomwall, pisbottomwall, psearchstat;

static int32_t drawpoly_srepeat = 0, drawpoly_trepeat = 0;
#define MAX_DRAWPOLY_VERTS 8

struct glfiltermodes glfiltermodes[NUMGLFILTERMODES] =
{
    {"NEAREST"},
    {"LINEAR"},
    {"NEAREST_MIPMAP_NEAREST"},
    {"LINEAR_MIPMAP_NEAREST"},
    {"NEAREST_MIPMAP_LINEAR"},
    {"LINEAR_MIPMAP_LINEAR"}
};

int32_t glanisotropy = 0;            // 0 = maximum supported by card
int32_t gltexfiltermode = TEXFILTER_OFF;

int32_t r_polygonmode = 0;     // 0:GL_FILL,1:GL_LINE,2:GL_POINT //FUK
static int32_t lastglpolygonmode = 0; //FUK

#ifdef USE_GLEXT
int32_t glmultisample = 0, glnvmultisamplehint = 0;
int32_t r_detailmapping = 1;
int32_t r_glowmapping = 1;
#endif

int32_t glprojectionhacks = 2;
static FHardwareTexture *polymosttext = 0;
int32_t glrendmode = REND_POLYMOST;
int32_t r_shadeinterpolate = 1;

int32_t r_animsmoothing = 1;
int32_t r_downsize = 0;
int32_t r_downsizevar = -1;
int32_t r_scenebrightness = 0;

int32_t r_rortexture = 0;
int32_t r_rortexturerange = 0;
int32_t r_rorphase = 0;

int32_t r_yshearing = 0;

// used for fogcalc
static float fogresult, fogresult2;
coltypef fogcol, fogtable[MAXPALOOKUPS];

static GLuint quadVertsID = 0;


int32_t r_useindexedcolortextures = -1;
static FHardwareTexture *palswapTextureID = nullptr;

static inline float float_trans(uint32_t maskprops, uint8_t blend)
{
    switch (maskprops)
    {
    case DAMETH_TRANS1:
    case DAMETH_TRANS2:
        return glblend[blend].def[maskprops-2].alpha;
    default:
        return 1.0f;
    }
}

char ptempbuf[MAXWALLSB<<1];

// polymost ART sky control
int32_t r_parallaxskyclamping = 1;
int32_t r_parallaxskypanning = 1;

#define MIN_CACHETIME_PRINT 10



#define Bfabsf fabsf

int32_t mdtims, omdtims;
uint8_t alphahackarray[MAXTILES];
int32_t drawingskybox = 0;
int32_t hicprecaching = 0;

hitdata_t polymost_hitdata;

void polymost_outputGLDebugMessage(uint8_t severity, const char* format, ...)
{
}

void gltexapplyprops(void)
{
    if (videoGetRenderMode() == REND_CLASSIC)
        return;

	if (GLInterface.glinfo.maxanisotropy > 1.f)
	{
		if (glanisotropy <= 0 || glanisotropy > GLInterface.glinfo.maxanisotropy)
			glanisotropy = (int32_t)GLInterface.glinfo.maxanisotropy;
	}

#if 1
	GLInterface.mSamplers->SetTextureFilterMode(0, 1);
	r_useindexedcolortextures = true;// !gltexfiltermode;
#else
	GLInterface.mSamplers->SetTextureFilterMode(gltexfiltermode, glanisotropy);
	r_useindexedcolortextures = false;// !gltexfiltermode;
#endif
}

//--------------------------------------------------------------------------------------------------

float glox1, gloy1, glox2, gloy2, gloyxscale, gloxyaspect, glohoriz2, glohorizcorrect, glotang;

//Use this for both initialization and uninitialization of OpenGL.
static int32_t gltexcacnum = -1;

//in-place multiply m0=m0*m1
float* multiplyMatrix4f(float m0[4*4], const float m1[4*4])
{
    float mR[4*4];

#define multMatrix4RowCol(r, c) mR[r*4+c] = m0[r*4]*m1[c] + m0[r*4+1]*m1[c+4] + m0[r*4+2]*m1[c+8] + m0[r*4+3]*m1[c+12]

    multMatrix4RowCol(0, 0);
    multMatrix4RowCol(0, 1);
    multMatrix4RowCol(0, 2);
    multMatrix4RowCol(0, 3);

    multMatrix4RowCol(1, 0);
    multMatrix4RowCol(1, 1);
    multMatrix4RowCol(1, 2);
    multMatrix4RowCol(1, 3);

    multMatrix4RowCol(2, 0);
    multMatrix4RowCol(2, 1);
    multMatrix4RowCol(2, 2);
    multMatrix4RowCol(2, 3);

    multMatrix4RowCol(3, 0);
    multMatrix4RowCol(3, 1);
    multMatrix4RowCol(3, 2);
    multMatrix4RowCol(3, 3);

    Bmemcpy(m0, mR, sizeof(float)*4*4);

    return m0;

#undef multMatrix4RowCol
}

static void calcmat(vec3f_t a0, const vec2f_t *offset, float f, float mat[16], int16_t angle)
{
    float g;
    float k0, k1, k2, k3, k4, k5, k6, k7;

    k0 = a0.y;
    k1 = a0.x;
    a0.x += offset->x;
    a0.z += offset->y;
    f = gcosang2*gshang;
    g = gsinang2*gshang;
    k4 = (float)sintable[(angle+1024)&2047] * (1.f/16384.f);
    k5 = (float)sintable[(angle+512)&2047] * (1.f/16384.f);
    k2 = k0*(1-k4)+k1*k5;
    k3 = k1*(1-k4)-k0*k5;
    k6 = f*gstang - gsinang*gctang; k7 = g*gstang + gcosang*gctang;
    mat[0] = k4*k6 + k5*k7; mat[4] = gchang*gstang; mat[ 8] = k4*k7 - k5*k6; mat[12] = k2*k6 + k3*k7;
    k6 = f*gctang + gsinang*gstang; k7 = g*gctang - gcosang*gstang;
    mat[1] = k4*k6 + k5*k7; mat[5] = gchang*gctang; mat[ 9] = k4*k7 - k5*k6; mat[13] = k2*k6 + k3*k7;
    k6 =           gcosang2*gchang; k7 =           gsinang2*gchang;
    mat[2] = k4*k6 + k5*k7; mat[6] =-gshang;        mat[10] = k4*k7 - k5*k6; mat[14] = k2*k6 + k3*k7;

    mat[12] = (mat[12] + a0.y*mat[0]) + (a0.z*mat[4] + a0.x*mat[ 8]);
    mat[13] = (mat[13] + a0.y*mat[1]) + (a0.z*mat[5] + a0.x*mat[ 9]);
    mat[14] = (mat[14] + a0.y*mat[2]) + (a0.z*mat[6] + a0.x*mat[10]);
}

void polymost_glreset()
{
    for (bssize_t i=0; i<=MAXPALOOKUPS-1; i++)
    {
        fogtable[i].r = palookupfog[i].r * (1.f/255.f);
        fogtable[i].g = palookupfog[i].g * (1.f/255.f);
        fogtable[i].b = palookupfog[i].b * (1.f/255.f);
        fogtable[i].a = 0;
    }

    //Reset if this is -1 (meaning 1st texture call ever), or > 0 (textures in memory)
    if (gltexcacnum < 0)
    {
        gltexcacnum = 0;

        //Hack for polymost_dorotatesprite calls before 1st polymost_drawrooms()
        gcosang = gcosang2 = 16384.f/262144.f;
        gsinang = gsinang2 = 0.f;
    }
    else
    {
		cleartexturecache();
        clearskins(INVALIDATE_ALL);
    }

	if (polymosttext)
		delete polymosttext;
    polymosttext=nullptr;

    Bmemset(texcache.list,0,sizeof(texcache.list));
    glox1 = -1;

#ifdef DEBUGGINGAIDS
    OSD_Printf("polymost_glreset()\n");
#endif
}

static void polymost_bindPth(pthtyp const* const pPth, int sampler)
{
	Bassert(pPth);

	GLInterface.BindTexture(0, pPth->glpic, sampler);
}


FileReader GetBaseResource(const char* fn);

// one-time initialization of OpenGL for polymost
void polymost_glinit()
{
	for (int basepalnum = 0; basepalnum < MAXBASEPALS; ++basepalnum)
    {
        uploadbasepalette(basepalnum);
    }
    palswapTextureID = 0;
    for (int palookupnum = 0; palookupnum < MAXPALOOKUPS; ++palookupnum)
    {
		GLInterface.SetPalswapData(palookupnum, (uint8_t*)palookup[palookupnum], numshades+1);
	}
}

////////// VISIBILITY FOG ROUTINES //////////

// For GL_LINEAR fog:
#define GL_FOG_MAX 1.0e37f

void calc_and_apply_fog(int32_t shade, int32_t vis, int32_t pal)
{
    if (nofog) return;

    fogresult = 0.f;
    fogcol = fogtable[pal];

    if (((uint8_t)(vis + 16)) > 0 && globalvisibility > 0)
    {
        constexpr GLfloat glfogconstant = 262144.f;
        GLfloat fogrange = (frealmaxshade * glfogconstant) / (((uint8_t)(vis + 16)) * globalvisibility);

        fogresult = 0.f - (((min(shade, 0) - 0.5f) / frealmaxshade) * fogrange); // min() = subtract shades from fog
        fogresult2 = fogrange - (((shade - 0.5f) / frealmaxshade) * fogrange);
    }
    else
    {
        fogresult = 0.f;
        fogresult2 = -GL_FOG_MAX; // hide fog behind the camera
    }

	GLInterface.SetFogLinear((float*)&fogcol, fogresult, fogresult2);
}

void calc_and_apply_fog_factor(int32_t shade, int32_t vis, int32_t pal, float factor)
{
    if (nofog) return;

    fogcol = fogtable[pal];

    if (((uint8_t)(vis + 16)) > 0 && ((((uint8_t)(vis + 16)) / 8.f) + shade) > 0)
    {
        GLfloat normalizedshade = (shade - 0.5f) / frealmaxshade;
        GLfloat fogrange = (((uint8_t)(vis + 16)) / (8.f * frealmaxshade)) + normalizedshade;

        // subtract shades from fog
        if (normalizedshade > 0.f && normalizedshade < 1.f)
            fogrange = (fogrange - normalizedshade) / (1.f - normalizedshade);

        fogresult = -(GL_FOG_MAX * fogrange);
        fogresult2 = GL_FOG_MAX - (GL_FOG_MAX * fogrange);
    }
    else
    {
        fogresult = 0.f;
        fogresult2 = -GL_FOG_MAX; // hide fog behind the camera
    }

	GLInterface.SetFogLinear((float*)& fogcol, fogresult, fogresult2);
}
////////////////////


static float get_projhack_ratio(void)
{
    if ((glprojectionhacks == 1) && !r_yshearing)
    {
        // calculates the extend of the zenith glitch
        float verticalfovtan = (fviewingrange * (windowxy2.y-windowxy1.y) * 5.f) / ((float)yxaspect * (windowxy2.x-windowxy1.x) * 4.f);
        float verticalfov = atanf(verticalfovtan) * (2.f / fPI);
        static constexpr float const maxhorizangle = 0.6361136f; // horiz of 199 in degrees
        float zenglitch = verticalfov + maxhorizangle - 0.95f; // less than 1 because the zenith glitch extends a bit
        if (zenglitch <= 0.f)
            return 1.f;
        float const zenglitchtan = tanf((verticalfov - zenglitch) * (fPI / 2.f));
        static constexpr float const maxcoshoriz = 0.54097117f; // 128/sqrt(128^2+199^2) = cos of an horiz diff of 199
        return 1.f + (((verticalfovtan / zenglitchtan) - 1.f) * ((1.f - Bfabsf(gchang)) / maxcoshoriz ));
    }

    // No projection hacks (legacy or new-aspect)
    return 1.f;
}

static void resizeglcheck(void)
{
    //FUK
    if (lastglpolygonmode != r_polygonmode)
    {
        lastglpolygonmode = r_polygonmode;
		GLInterface.SetWireframe(r_polygonmode == 1);
    }
    if (r_polygonmode) //FUK
    {
		GLInterface.ClearScreen(1, 1, 1, true);
    }

    if ((glox1 != windowxy1.x) || (gloy1 != windowxy1.y) || (glox2 != windowxy2.x) || (gloy2 != windowxy2.y) || (gloxyaspect != gxyaspect) || (gloyxscale != gyxscale) || (glohoriz2 != ghoriz2) || (glohorizcorrect != ghorizcorrect) || (glotang != gtang))
    {
        const int32_t ourxdimen = (windowxy2.x-windowxy1.x+1);
        float ratio = get_projhack_ratio();
        const int32_t fovcorrect = (int32_t)(ourxdimen*ratio - ourxdimen);

        ratio = 1.f/ratio;

        glox1 = (float)windowxy1.x; gloy1 = (float)windowxy1.y;
        glox2 = (float)windowxy2.x; gloy2 = (float)windowxy2.y;

		GLInterface.SetViewport(windowxy1.x-(fovcorrect/2), ydim-(windowxy2.y+1),
                    ourxdimen+fovcorrect, windowxy2.y-windowxy1.y+1);

        float m[4][4];
        Bmemset(m,0,sizeof(m));

        float const nearclip = 4.0f / (gxyaspect * gyxscale * 1024.f);
        float const farclip = 64.f;

        gloxyaspect = gxyaspect;
        gloyxscale = gyxscale;
        glohoriz2 = ghoriz2;
        glohorizcorrect = ghorizcorrect;
        glotang = gtang;

        m[0][0] = 1.f;
        m[1][1] = fxdimen / (fydimen * ratio);
        m[2][0] = 2.f * ghoriz2 * gstang / fxdimen;
        m[2][1] = 2.f * (ghoriz2 * gctang + ghorizcorrect) / fydimen;
        m[2][2] = (farclip + nearclip) / (farclip - nearclip);
        m[2][3] = 1.f;
        m[3][2] = -(2.f * farclip * nearclip) / (farclip - nearclip);
		GLInterface.SetMatrix(Matrix_Projection, &m[0][0]);
		VSMatrix identity(0);
		GLInterface.SetMatrix(Matrix_ModelView, &identity);

        if (!nofog) GLInterface.SetFogEnabled(true);
    }
}


void uploadtexture(FHardwareTexture *tex, coltype* pic)
{
	tex->LoadTexture((uint8_t *)pic);
}

void uploadbasepalette(int32_t basepalnum, bool transient)	// transient palettes are used by movies and should not affect the engine state. All other palettes only get set at game startup.
{
    if (!basepaltable[basepalnum])
    {
        return;
    }

    uint8_t basepalWFullBrightInfo[4*256];
    for (int i = 0; i < 256; ++i)
    {
        basepalWFullBrightInfo[i*4] = basepaltable[basepalnum][i*3];
        basepalWFullBrightInfo[i*4+1] = basepaltable[basepalnum][i*3+1];
        basepalWFullBrightInfo[i*4+2] = basepaltable[basepalnum][i*3+2];
        basepalWFullBrightInfo[i*4+3] = 0-(IsPaletteIndexFullbright(i) != 0);
    }

	GLInterface.SetPaletteData(basepalnum, basepalWFullBrightInfo, transient);
}

// Used by RRRA fog hackery - the only place changing the palswaps at run time.
void uploadpalswaps(int count, int32_t* swaps)
{
	for (int i = 0; i < count; i++)
	{
		GLInterface.SetPalswapData(i, (uint8_t*)palookup[i], numshades + 1);
	}
}



#ifdef USE_GLEXT
void polymost_setupdetailtexture(const int32_t texunits, FHardwareTexture *tex)
{
	GLInterface.BindTexture(texunits, tex, SamplerRepeat);
}

void polymost_setupglowtexture(const int32_t texunits, FHardwareTexture* tex)
{
	GLInterface.BindTexture(texunits, tex, SamplerRepeat);
}
#endif


//(dpx,dpy) specifies an n-sided polygon. The polygon must be a convex clockwise loop.
//    n must be <= 8 (assume clipping can double number of vertices)
//method: 0:solid, 1:masked(255 is transparent), 2:transluscent #1, 3:transluscent #2
//    +4 means it's a sprite, so wraparound isn't needed

// drawpoly's hack globals
static int32_t pow2xsplit = 0, skyclamphack = 0, skyzbufferhack = 0, flatskyrender = 0;
static float drawpoly_alpha = 0.f;
static uint8_t drawpoly_blend = 0;

static inline pthtyp *our_texcache_fetch(int32_t dameth)
{
    return texcache_fetch(globalpicnum, globalpal, getpalookup(1, globalshade), dameth);
}

int32_t polymost_maskWallHasTranslucency(uwalltype const * const wall)
{
    if (wall->cstat & CSTAT_WALL_TRANSLUCENT)
        return true;

	auto tex = TileFiles.tiles[wall->picnum];
	auto si = tex->FindReplacement(wall->pal);
	if (si && usehightile) tex = si->faces[0];
	return tex && tex->GetTranslucency();
}

int32_t polymost_spriteHasTranslucency(uspritetype const * const tspr)
{
    if ((tspr->cstat & (CSTAT_SPRITE_TRANSLUCENT | CSTAT_SPRITE_RESERVED1)) ||
        ((unsigned)tspr->owner < MAXSPRITES && spriteext[tspr->owner].alpha))
        return true;

	auto tex = TileFiles.tiles[tspr->picnum];
	auto si = tex->FindReplacement(tspr->shade, hictinting[tspr->shade].f & HICTINT_ALWAYSUSEART);
	if (si && usehightile) tex = si->faces[0];
	return tex && tex->GetTranslucency();
}


static void polymost_updatePalette()
{
    if (videoGetRenderMode() != REND_POLYMOST)
    {
        return;
    }

	GLInterface.SetPalswap(fixpalswap >= 1? fixpalswap-1 : globalpal);
	GLInterface.SetShade(globalshade, numshades);
	GLInterface.SetPalette(fixpalette >= 1? fixpalette-1 : curbasepal);
}


static void polymost_updaterotmat(void)
{
    if (1)
    {
        float matrix[16] = {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
        };
#if !SOFTROTMAT
        //Up/down rotation
        float udmatrix[16] = {
            1.f, 0.f, 0.f, 0.f,
            0.f, gchang, -gshang*gvrcorrection, 0.f,
            0.f, gshang/gvrcorrection, gchang, 0.f,
            0.f, 0.f, 0.f, 1.f,
        };
        // Tilt rotation
        float tiltmatrix[16] = {
            gctang, -gstang, 0.f, 0.f,
            gstang, gctang, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
        };
        multiplyMatrix4f(matrix, udmatrix);
        multiplyMatrix4f(matrix, tiltmatrix);
#endif
		GLInterface.SetMatrix(Matrix_View, matrix);
    }
}

static void polymost_identityrotmat(void)
{
    if (1)
    {
		VSMatrix matrix(0);
		GLInterface.SetMatrix(Matrix_View, &matrix);
	}
}

static void polymost_flatskyrender(vec2f_t const* const dpxy, int32_t const n, int32_t method, const vec2_16_t& tilesiz);

static void polymost_drawpoly(vec2f_t const * const dpxy, int32_t const n, int32_t method, const vec2_16_t &tilesize)
{
    if (method == DAMETH_BACKFACECULL ||
#ifdef YAX_ENABLE
        g_nodraw ||
#endif
        (uint32_t)globalpicnum >= MAXTILES)
        return;

    const int32_t method_ = method;

    if (n == 3)
    {
        if ((dpxy[0].x-dpxy[1].x) * (dpxy[2].y-dpxy[1].y) >=
            (dpxy[2].x-dpxy[1].x) * (dpxy[0].y-dpxy[1].y)) return; //for triangle
    }
    else if (n > 3)
    {
        float f = 0; //f is area of polygon / 2

        for (bssize_t i=n-2, j=n-1,k=0; k<n; i=j,j=k,k++)
            f += (dpxy[i].x-dpxy[k].x)*dpxy[j].y;

        if (f <= 0) return;
    }

    static int32_t skyzbufferhack_pass = 0;
    if (flatskyrender && skyzbufferhack_pass == 0)
    {
        polymost_flatskyrender(dpxy, n, method, tilesize);
        return;
    }

    if (palookup[globalpal] == NULL)
        globalpal = 0;

    //Load texture (globalpicnum)
    setgotpic(globalpicnum);
	vec2_t tsiz = { tilesize.x, tilesize.y };

    Bassert(n <= MAX_DRAWPOLY_VERTS);

    int j = 0;
    float px[8], py[8], dd[8], uu[8], vv[8];
#if SOFTROTMAT
    float const ozgs = (ghalfx / gvrcorrection) * gshang,
                ozgc = (ghalfx / gvrcorrection) * gchang;
#endif

    for (bssize_t i=0; i<n; ++i)
    {
#if SOFTROTMAT
        //Up/down rotation
        vec3f_t const orot = { dpxy[i].x - ghalfx,
                              (dpxy[i].y - ghoriz) * gchang - ozgs,
                              (dpxy[i].y - ghoriz) * gshang + ozgc };

        // Tilt rotation
        float const r = (ghalfx / gvrcorrection) / orot.z;

        px[j] = ghalfx + (((orot.x * gctang) - (orot.y * gstang)) * r);
        py[j] = ghoriz + (((orot.x * gstang) + (orot.y * gctang)) * r);

        dd[j] = (dpxy[i].x * xtex.d + dpxy[i].y * ytex.d + otex.d) * r;
        if (dd[j] <= 0.f) // invalid polygon
            return;
        uu[j] = (dpxy[i].x * xtex.u + dpxy[i].y * ytex.u + otex.u) * r;
        vv[j] = (dpxy[i].x * xtex.v + dpxy[i].y * ytex.v + otex.v) * r;

        if ((!j) || (px[j] != px[j-1]) || (py[j] != py[j-1]))
            j++;
#else
        px[j] = dpxy[i].x;
        py[j] = dpxy[i].y;

        dd[j] = (dpxy[i].x * xtex.d + dpxy[i].y * ytex.d + otex.d);
        if (dd[j] <= 0.f) // invalid polygon
            return;
        uu[j] = (dpxy[i].x * xtex.u + dpxy[i].y * ytex.u + otex.u);
        vv[j] = (dpxy[i].x * xtex.v + dpxy[i].y * ytex.v + otex.v);
        j++;
#endif
    }

    while ((j >= 3) && (px[j-1] == px[0]) && (py[j-1] == py[0])) j--;

    if (j < 3)
        return;

    int const npoints = j;

    if (skyclamphack) method |= DAMETH_CLAMPED;

    polymost_outputGLDebugMessage(3, "polymost_drawpoly(dpxy:%p, n:%d, method_:%X), method: %X", dpxy, n, method_, method);

    pthtyp *pth = our_texcache_fetch(method | (videoGetRenderMode() == REND_POLYMOST && r_useindexedcolortextures ? PTH_INDEXED : 0));

    if (!pth)
    {
        return;
    }

    if (!tilePtr(globalpicnum))
    {
        tsiz.x = tsiz.y = 1;
		GLInterface.SetColorMask(false); //Hack to update Z-buffer for invalid mirror textures
    }

    Bassert(pth);

    // If we aren't rendmode 3, we're in Polymer, which means this code is
    // used for rotatesprite only. Polymer handles all the material stuff,
    // just submit the geometry and don't mess with textures.
    if (videoGetRenderMode() == REND_POLYMOST)
    {

		// The entire logic here is just one lousy hack.
		int mSampler = NoSampler;
		if (pth->glpic->GetSampler() != SamplerRepeat)
		{
			if (drawpoly_srepeat && drawpoly_trepeat) mSampler = SamplerRepeat;
			else if (drawpoly_srepeat) mSampler = SamplerClampY;
			else if (drawpoly_trepeat) mSampler = SamplerClampX;
			else mSampler = SamplerClampXY;
		}

		polymost_bindPth(pth, mSampler);

    }

	VSMatrix texmat;
    // texture scale by parkar request
    if (pth->hicr && !drawingskybox && ((pth->hicr->scale.x != 1.0f) || (pth->hicr->scale.y != 1.0f)))
    {
		texmat.loadIdentity();
        texmat.scale(pth->hicr->scale.x, pth->hicr->scale.y, 1.0f);
		GLInterface.SetMatrix(Matrix_Texture, &texmat);
    }

    if (videoGetRenderMode() == REND_POLYMOST)
    {
        polymost_updatePalette();
    }

    // detail texture
    if (r_detailmapping)
    {
        pthtyp *detailpth = NULL;

        if (usehightile && !drawingskybox && hicfindsubst(globalpicnum, DETAILPAL, 1) &&
            (detailpth = texcache_fetch(globalpicnum, DETAILPAL, 0, method & ~DAMETH_MASKPROPS)) &&
            detailpth->hicr && detailpth->hicr->palnum == DETAILPAL)
        {
			GLInterface.UseDetailMapping(true);
            polymost_setupdetailtexture(3, detailpth->glpic);

			texmat.loadIdentity();

            if (pth->hicr && ((pth->hicr->scale.x != 1.0f) || (pth->hicr->scale.y != 1.0f)))
                texmat.scale(pth->hicr->scale.x, pth->hicr->scale.y, 1.0f);

            if ((detailpth->hicr->scale.x != 1.0f) || (detailpth->hicr->scale.y != 1.0f))
                texmat.scale(detailpth->hicr->scale.x, detailpth->hicr->scale.y, 1.0f);

			GLInterface.SetMatrix(Matrix_Detail, &texmat);
        }
    }

    // glow texture
    if (r_glowmapping)
    {
        pthtyp *glowpth = NULL;

        if (usehightile && !drawingskybox && hicfindsubst(globalpicnum, GLOWPAL, 1) &&
            (glowpth = texcache_fetch(globalpicnum, GLOWPAL, 0, (method & ~DAMETH_MASKPROPS) | DAMETH_MASK)) &&
            glowpth->hicr && (glowpth->hicr->palnum == GLOWPAL))
        {
			GLInterface.UseGlowMapping(true);
            polymost_setupglowtexture(4, glowpth->glpic);
        }
    }

    if ((method & DAMETH_WALL) != 0)
    {
        int32_t size = tilesize.y;
        int32_t size2;
        for (size2 = 1; size2 < size; size2 += size2) {}
        if (size == size2)
			GLInterface.SetNpotEmulation(false, 1.f, 0.f); 
        else
        {
            float xOffset = 1.f / tilesize.x;
			GLInterface.SetNpotEmulation(true, (1.f*size2) / size, xOffset);
        }
    }
    else
    {
		GLInterface.SetNpotEmulation(false, 1.f, 0.f);
    }

    vec2f_t hacksc = { 1.f, 1.f };

    if (pth->flags & PTH_HIGHTILE)
    {
        hacksc = pth->scale;
        tsiz = pth->siz;
    }

    vec2_t tsiz2 = tsiz;


    if (method & DAMETH_MASKPROPS)
    {
		// Fixme: Alpha test on shaders must be done differently.
		// Also: Consider a texture's alpha threshold.
        float const al = alphahackarray[globalpicnum] != 0 ? alphahackarray[globalpicnum] * (1.f/255.f) :
                         (pth->hicr && pth->hicr->alphacut >= 0.f ? pth->hicr->alphacut : 0.f);

		GLInterface.SetAlphaThreshold(al);
        handle_blend((method & DAMETH_MASKPROPS) > DAMETH_MASK, drawpoly_blend, (method & DAMETH_MASKPROPS) == DAMETH_TRANS2);
    }

    float pc[4];

	polytint_t const& tint = hictinting[globalpal];
	pc[0] = (1.f - (tint.sr * (1.f / 255.f))) + (tint.sr * (1.f / 255.f));
	pc[1] = (1.f - (tint.sg * (1.f / 255.f))) + (tint.sg * (1.f / 255.f));
	pc[2] = (1.f - (tint.sb * (1.f / 255.f))) + (tint.sb * (1.f / 255.f));

    // spriteext full alpha control
    pc[3] = float_trans(method & DAMETH_MASKPROPS, drawpoly_blend) * (1.f - drawpoly_alpha);

    // tinting
	auto& h = hictinting[globalpal];
	polytintflags_t const tintflags = h.f;
    if (!(tintflags & HICTINT_PRECOMPUTED))
    {
        if (pth->flags & PTH_HIGHTILE)
        {
            if (pth->palnum != globalpal || (pth->effects & HICTINT_IN_MEMORY) || (tintflags & HICTINT_APPLYOVERALTPAL))
                hictinting_apply(pc, globalpal);
        }
        else if (tintflags & (HICTINT_USEONART|HICTINT_ALWAYSUSEART))
            hictinting_apply(pc, globalpal);
    }
	GLInterface.SetTinting(tintflags, PalEntry(h.sr, h.sg, h.sb));

    // global tinting
    if ((pth->flags & PTH_HIGHTILE) && have_basepal_tint())
        hictinting_apply(pc, MAXPALOOKUPS-1);

    globaltinting_apply(pc);

    if (skyzbufferhack_pass)
        pc[3] = 0.01f;

    GLInterface.SetColor(pc[0], pc[1], pc[2], pc[3]);

    vec2f_t const scale = { 1.f / tsiz2.x * hacksc.x, 1.f / tsiz2.y * hacksc.y };
	auto data = GLInterface.AllocVertices(npoints);
	auto vt = data.second;
	for (bssize_t i = 0; i < npoints; ++i, vt++)
    {
        float const r = 1.f / dd[i];

		//update texcoords
		vt->SetTexCoord(
			uu[i] * r * scale.x,
			vv[i] * r * scale.y);

        //update verts
		vt->SetVertex(
			(px[i] - ghalfx) * r * grhalfxdown10x,
			(ghalfy - py[i]) * r * grhalfxdown10,
			r * (1.f / 1024.f));

    }
	GLInterface.Draw(DT_TRIANGLE_FAN, data.first, npoints);

	GLInterface.SetTinting(0,0);
	GLInterface.UseDetailMapping(false);
	GLInterface.UseGlowMapping(false);
	GLInterface.SetNpotEmulation(false, 1.f, 0.f);

    if (pth->hicr)
    {
		VSMatrix identity(0);
		GLInterface.SetMatrix(Matrix_Texture, &identity);
		GLInterface.SetMatrix(Matrix_Detail, &identity);
    }

	if (skyzbufferhack && skyzbufferhack_pass == 0)
    {
        vec3d_t const bxtex = xtex, bytex = ytex, botex = otex;
        xtex = xtex2, ytex = ytex2, otex = otex2;
        skyzbufferhack_pass++;
		GLInterface.SetColorMask(false);
		polymost_drawpoly(dpxy, n, DAMETH_MASK, tilesize);
		GLInterface.SetColorMask(true);
		xtex = bxtex, ytex = bytex, otex = botex;
        skyzbufferhack_pass--;
    }

	if (!tilePtr(globalpicnum))
		GLInterface.SetColorMask(true);
}


static inline void vsp_finalize_init(int32_t const vcnt)
{
    for (bssize_t i=0; i<vcnt; ++i)
    {
        vsp[i].cy[1] = vsp[i+1].cy[0]; vsp[i].ctag = i;
        vsp[i].fy[1] = vsp[i+1].fy[0]; vsp[i].ftag = i;
        vsp[i].n = i+1; vsp[i].p = i-1;
//        vsp[i].tag = -1;
    }
    vsp[vcnt-1].n = 0; vsp[0].p = vcnt-1;

    //VSPMAX-1 is dummy empty node
    for (bssize_t i=vcnt; i<VSPMAX; i++) { vsp[i].n = i+1; vsp[i].p = i-1; }
    vsp[VSPMAX-1].n = vcnt; vsp[vcnt].p = VSPMAX-1;
}

#ifdef YAX_ENABLE
static inline void yax_vsp_finalize_init(int32_t const yaxbunch, int32_t const vcnt)
{
    for (bssize_t i=0; i<vcnt; ++i)
    {
        yax_vsp[yaxbunch][i].cy[1] = yax_vsp[yaxbunch][i+1].cy[0]; yax_vsp[yaxbunch][i].ctag = i;
        yax_vsp[yaxbunch][i].n = i+1; yax_vsp[yaxbunch][i].p = i-1;
//        vsp[i].tag = -1;
    }
    yax_vsp[yaxbunch][vcnt-1].n = 0; yax_vsp[yaxbunch][0].p = vcnt-1;

    //VSPMAX-1 is dummy empty node
    for (bssize_t i=vcnt; i<VSPMAX; i++) { yax_vsp[yaxbunch][i].n = i+1; yax_vsp[yaxbunch][i].p = i-1; }
    yax_vsp[yaxbunch][VSPMAX-1].n = vcnt; yax_vsp[yaxbunch][vcnt].p = VSPMAX-1;
}
#endif

#define COMBINE_STRIPS

#ifdef COMBINE_STRIPS

#define MERGE_NODES(i, ni)            \
    do                                \
    {                                 \
        vsp[i].cy[1] = vsp[ni].cy[1]; \
        vsp[i].fy[1] = vsp[ni].fy[1]; \
        vsdel(ni);                    \
    } while (0);

static inline void vsdel(int32_t const i)
{
    //Delete i
    int const pi = vsp[i].p;
    int const ni = vsp[i].n;

    vsp[ni].p = pi;
    vsp[pi].n = ni;

    //Add i to empty list
    vsp[i].n = vsp[VSPMAX-1].n;
    vsp[i].p = VSPMAX-1;
    vsp[vsp[VSPMAX-1].n].p = i;
    vsp[VSPMAX-1].n = i;
}
# ifdef YAX_ENABLE
static inline void yax_vsdel(int32_t const yaxbunch, int32_t const i)
{
    //Delete i
    int const pi = yax_vsp[yaxbunch][i].p;
    int const ni = yax_vsp[yaxbunch][i].n;

    yax_vsp[yaxbunch][ni].p = pi;
    yax_vsp[yaxbunch][pi].n = ni;

    //Add i to empty list
    yax_vsp[yaxbunch][i].n = yax_vsp[yaxbunch][VSPMAX - 1].n;
    yax_vsp[yaxbunch][i].p = VSPMAX - 1;
    yax_vsp[yaxbunch][yax_vsp[yaxbunch][VSPMAX - 1].n].p = i;
    yax_vsp[yaxbunch][VSPMAX - 1].n = i;
}
# endif
#endif

static inline int32_t vsinsaft(int32_t const i)
{
    //i = next element from empty list
    int32_t const r = vsp[VSPMAX-1].n;
    vsp[vsp[r].n].p = VSPMAX-1;
    vsp[VSPMAX-1].n = vsp[r].n;

    vsp[r] = vsp[i]; //copy i to r

    //insert r after i
    vsp[r].p = i; vsp[r].n = vsp[i].n;
    vsp[vsp[i].n].p = r; vsp[i].n = r;

    return r;
}

#ifdef YAX_ENABLE


static inline int32_t yax_vsinsaft(int32_t const yaxbunch, int32_t const i)
{
    //i = next element from empty list
    int32_t const r = yax_vsp[yaxbunch][VSPMAX - 1].n;
    yax_vsp[yaxbunch][yax_vsp[yaxbunch][r].n].p = VSPMAX - 1;
    yax_vsp[yaxbunch][VSPMAX - 1].n = yax_vsp[yaxbunch][r].n;

    yax_vsp[yaxbunch][r] = yax_vsp[yaxbunch][i]; //copy i to r

    //insert r after i
    yax_vsp[yaxbunch][r].p = i; yax_vsp[yaxbunch][r].n = yax_vsp[yaxbunch][i].n;
    yax_vsp[yaxbunch][yax_vsp[yaxbunch][i].n].p = r; yax_vsp[yaxbunch][i].n = r;

    return r;
}
#endif

static int32_t domostpolymethod = DAMETH_NOMASK;

#define DOMOST_OFFSET .01f

static void polymost_clipmost(vec2f_t *dpxy, int &n, float x0, float x1, float y0top, float y0bot, float y1top, float y1bot)
{
    if (y0bot < y0top || y1bot < y1top)
        return;

    //Clip to (x0,y0top)-(x1,y1top)

    vec2f_t dp2[8];

    float t0, t1;
    int n2 = 0;
    t1 = -((dpxy[0].x - x0) * (y1top - y0top) - (dpxy[0].y - y0top) * (x1 - x0));

    for (bssize_t i=0; i<n; i++)
    {
        int j = i + 1;

        if (j >= n)
            j = 0;

        t0 = t1;
        t1 = -((dpxy[j].x - x0) * (y1top - y0top) - (dpxy[j].y - y0top) * (x1 - x0));

        if (t0 >= 0)
            dp2[n2++] = dpxy[i];

        if ((t0 >= 0) != (t1 >= 0) && (t0 <= 0) != (t1 <= 0))
        {
            float const r = t0 / (t0 - t1);
            dp2[n2] = { (dpxy[j].x - dpxy[i].x) * r + dpxy[i].x,
                        (dpxy[j].y - dpxy[i].y) * r + dpxy[i].y };
            n2++;
        }
    }

    if (n2 < 3)
    {
        n = 0;
        return;
    }

    //Clip to (x1,y1bot)-(x0,y0bot)
    t1 = -((dp2[0].x - x1) * (y0bot - y1bot) - (dp2[0].y - y1bot) * (x0 - x1));
    n = 0;

    for (bssize_t i = 0, j = 1; i < n2; j = ++i + 1)
    {
        if (j >= n2)
            j = 0;

        t0 = t1;
        t1 = -((dp2[j].x - x1) * (y0bot - y1bot) - (dp2[j].y - y1bot) * (x0 - x1));

        if (t0 >= 0)
            dpxy[n++] = dp2[i];

        if ((t0 >= 0) != (t1 >= 0) && (t0 <= 0) != (t1 <= 0))
        {
            float const r = t0 / (t0 - t1);
            dpxy[n] = { (dp2[j].x - dp2[i].x) * r + dp2[i].x,
                        (dp2[j].y - dp2[i].y) * r + dp2[i].y };
            n++;
        }
    }

    if (n < 3)
    {
        n = 0;
        return;
    }
}

static void polymost_domost(float x0, float y0, float x1, float y1, float y0top = 0.f, float y0bot = -1.f, float y1top = 0.f, float y1bot = -1.f)
{
    int const dir = (x0 < x1);

    polymost_outputGLDebugMessage(3, "polymost_domost(x0:%f, y0:%f, x1:%f, y1:%f, y0top:%f, y0bot:%f, y1top:%f, y1bot:%f)",
                                  x0, y0, x1, y1, y0top, y0bot, y1top, y1bot);

    y0top -= DOMOST_OFFSET;
    y1top -= DOMOST_OFFSET;
    y0bot += DOMOST_OFFSET;
    y1bot += DOMOST_OFFSET;

    if (dir) //clip dmost (floor)
    {
        y0 -= DOMOST_OFFSET;
        y1 -= DOMOST_OFFSET;
    }
    else //clip umost (ceiling)
    {
        if (x0 == x1) return;
        swapfloat(&x0, &x1);
        swapfloat(&y0, &y1);
        swapfloat(&y0top, &y1top);
        swapfloat(&y0bot, &y1bot);
        y0 += DOMOST_OFFSET;
        y1 += DOMOST_OFFSET; //necessary?
    }

    // Test if span is outside screen bounds
    if (x1 < xbl || x0 > xbr)
    {
        domost_rejectcount++;
        return;
    }

    vec2f_t dm0 = { x0 - DOMOST_OFFSET, y0 };
    vec2f_t dm1 = { x1 + DOMOST_OFFSET, y1 };

    float const slop = (dm1.y - dm0.y) / (dm1.x - dm0.x);

    if (dm0.x < xbl)
    {
        dm0.y += slop*(xbl-dm0.x);
        dm0.x = xbl;
    }

    if (dm1.x > xbr)
    {
        dm1.y += slop*(xbr-dm1.x);
        dm1.x = xbr;
    }

    dm0.x -= DOMOST_OFFSET;
    dm1.x += DOMOST_OFFSET;

    drawpoly_alpha = 0.f;
    drawpoly_blend = 0;

    vec2f_t n0, n1;
    float spx[4];
    int32_t  spt[4];
    int firstnode = vsp[0].n;

    for (bssize_t newi, i=vsp[0].n; i; i=newi)
    {
        newi = vsp[i].n; n0.x = vsp[i].x; n1.x = vsp[newi].x;

        if (dm0.x >= n1.x)
        {
            firstnode = i;
            continue;
        }
        
        if (n0.x >= dm1.x)
            break;

        if (vsp[i].ctag <= 0) continue;

        float const dx = n1.x-n0.x;
        float const cy[2] = { vsp[i].cy[0], vsp[i].fy[0] },
                    cv[2] = { vsp[i].cy[1]-cy[0], vsp[i].fy[1]-cy[1] };

        int scnt = 0;

        //Test if left edge requires split (dm0.x,dm0.y) (nx0,cy(0)),<dx,cv(0)>
        if ((dm0.x > n0.x) && (dm0.x < n1.x))
        {
            float const t = (dm0.x-n0.x)*cv[dir] - (dm0.y-cy[dir])*dx;
            if (((!dir) && (t < 0.f)) || ((dir) && (t > 0.f)))
                { spx[scnt] = dm0.x; spt[scnt] = -1; scnt++; }
        }

        //Test for intersection on umost (0) and dmost (1)

        float const d[2] = { ((dm0.y - dm1.y) * dx) - ((dm0.x - dm1.x) * cv[0]),
                             ((dm0.y - dm1.y) * dx) - ((dm0.x - dm1.x) * cv[1]) };

        float const n[2] = { ((dm0.y - cy[0]) * dx) - ((dm0.x - n0.x) * cv[0]),
                             ((dm0.y - cy[1]) * dx) - ((dm0.x - n0.x) * cv[1]) };

        float const fnx[2] = { dm0.x + ((n[0] / d[0]) * (dm1.x - dm0.x)),
                               dm0.x + ((n[1] / d[1]) * (dm1.x - dm0.x)) };

        if ((Bfabsf(d[0]) > Bfabsf(n[0])) && (d[0] * n[0] >= 0.f) && (fnx[0] > n0.x) && (fnx[0] < n1.x))
            spx[scnt] = fnx[0], spt[scnt++] = 0;

        if ((Bfabsf(d[1]) > Bfabsf(n[1])) && (d[1] * n[1] >= 0.f) && (fnx[1] > n0.x) && (fnx[1] < n1.x))
            spx[scnt] = fnx[1], spt[scnt++] = 1;

        //Nice hack to avoid full sort later :)
        if ((scnt >= 2) && (spx[scnt-1] < spx[scnt-2]))
        {
            swapfloat(&spx[scnt-1], &spx[scnt-2]);
            swaplong(&spt[scnt-1], &spt[scnt-2]);
        }

        //Test if right edge requires split
        if ((dm1.x > n0.x) && (dm1.x < n1.x))
        {
            float const t = (dm1.x-n0.x)*cv[dir] - (dm1.y-cy[dir])*dx;
            if (((!dir) && (t < 0.f)) || ((dir) && (t > 0.f)))
                { spx[scnt] = dm1.x; spt[scnt] = -1; scnt++; }
        }

        vsp[i].tag = vsp[newi].tag = -1;

        float const rdx = 1.f/dx;

        for (bssize_t i = 0; i < scnt; i++)
        {
            if (spx[i] < x0)
                spx[i] = x0;
            else if (spx[i] > x1)
                spx[i] = x1;
        }

        for (bssize_t z=0, vcnt=0; z<=scnt; z++,i=vcnt)
        {
            float t;

            if (z == scnt)
                goto skip;

            t = (spx[z]-n0.x)*rdx;
            vcnt = vsinsaft(i);
            vsp[i].cy[1] = t*cv[0] + cy[0];
            vsp[i].fy[1] = t*cv[1] + cy[1];
            vsp[vcnt].x = spx[z];
            vsp[vcnt].cy[0] = vsp[i].cy[1];
            vsp[vcnt].fy[0] = vsp[i].fy[1];
            vsp[vcnt].tag = spt[z];

skip: ;
            int32_t const ni = vsp[i].n; if (!ni) continue; //this 'if' fixes many bugs!
            float const dx0 = vsp[i].x; if (dm0.x > dx0) continue;
            float const dx1 = vsp[ni].x; if (dm1.x < dx1) continue;
            n0.y = (dx0-dm0.x)*slop + dm0.y;
            n1.y = (dx1-dm0.x)*slop + dm0.y;

            //      dx0           dx1
            //       ~             ~
            //----------------------------
            //     t0+=0         t1+=0
            //   vsp[i].cy[0]  vsp[i].cy[1]
            //============================
            //     t0+=1         t1+=3
            //============================
            //   vsp[i].fy[0]    vsp[i].fy[1]
            //     t0+=2         t1+=6
            //
            //     ny0 ?         ny1 ?

            int k = 4;

            if ((vsp[i].tag == 0) || (n0.y <= vsp[i].cy[0]+DOMOST_OFFSET)) k--;
            if ((vsp[i].tag == 1) || (n0.y >= vsp[i].fy[0]-DOMOST_OFFSET)) k++;
            if ((vsp[ni].tag == 0) || (n1.y <= vsp[i].cy[1]+DOMOST_OFFSET)) k -= 3;
            if ((vsp[ni].tag == 1) || (n1.y >= vsp[i].fy[1]-DOMOST_OFFSET)) k += 3;

            if (!dir)
            {
                switch (k)
                {
                    case 4:
                    case 5:
                    case 7:
                    {
                        vec2f_t dpxy[8] = {
                            { dx0, vsp[i].cy[0] }, { dx1, vsp[i].cy[1] }, { dx1, n1.y }, { dx0, n0.y }
                        };

                        int n = 4;
                        polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                        if (g_nodraw)
                        {
                            if (yax_drawcf != -1)
                                yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].cy[0], vsp[i].cy[1], n0.y, n1.y };
                        }
                        else
#endif
                            polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                        vsp[i].cy[0] = n0.y;
                        vsp[i].cy[1] = n1.y;
                        vsp[i].ctag = gtag;
                    }
                    break;
                    case 1:
                    case 2:
                    {
                        vec2f_t dpxy[8] = { { dx0, vsp[i].cy[0] }, { dx1, vsp[i].cy[1] }, { dx0, n0.y } };

                        int n = 3;
                        polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                        if (g_nodraw)
                        {
                            if (yax_drawcf != -1)
                                yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].cy[0], vsp[i].cy[1], n0.y, vsp[i].cy[1] };
                        }
                        else
#endif
                            polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                        vsp[i].cy[0] = n0.y;
                        vsp[i].ctag = gtag;
                    }
                    break;
                    case 3:
                    case 6:
                    {
                        vec2f_t dpxy[8] = { { dx0, vsp[i].cy[0] }, { dx1, vsp[i].cy[1] }, { dx1, n1.y } };

                        int n = 3;
                        polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                        if (g_nodraw)
                        {
                            if (yax_drawcf != -1)
                                yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].cy[0], vsp[i].cy[1], vsp[i].cy[0], n1.y };
                        }
                        else
#endif
                            polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                        vsp[i].cy[1] = n1.y;
                        vsp[i].ctag = gtag;
                    }
                    break;
                    case 8:
                    {
                        vec2f_t dpxy[8] = {
                            { dx0, vsp[i].cy[0] }, { dx1, vsp[i].cy[1] }, { dx1, vsp[i].fy[1] }, { dx0, vsp[i].fy[0] }
                        };

                        int n = 4;
                        polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                        if (g_nodraw)
                        {
                            if (yax_drawcf != -1)
                                yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].cy[0], vsp[i].cy[1], vsp[i].fy[0], vsp[i].fy[1] };
                        }
                        else
#endif
                            polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                        vsp[i].ctag = vsp[i].ftag = -1;
                    }
                    default: break;
                }
            }
            else
            {
                switch (k)
                {
                case 4:
                case 3:
                case 1:
                {
                    vec2f_t dpxy[8] = {
                        { dx0, n0.y }, { dx1, n1.y }, { dx1, vsp[i].fy[1] }, { dx0, vsp[i].fy[0] }
                    };

                    int n = 4;
                    polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                    if (g_nodraw)
                    {
                        if (yax_drawcf != -1)
                            yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, n0.y, n1.y, vsp[i].fy[0], vsp[i].fy[1] };
                    }
                    else
#endif
                        polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                    vsp[i].fy[0] = n0.y;
                    vsp[i].fy[1] = n1.y;
                    vsp[i].ftag = gtag;
                }
                    break;
                case 7:
                case 6:
                {
                    vec2f_t dpxy[8] = { { dx0, n0.y }, { dx1, vsp[i].fy[1] }, { dx0, vsp[i].fy[0] } };

                    int n = 3;
                    polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                    if (g_nodraw)
                    {
                        if (yax_drawcf != -1)
                            yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, n0.y, vsp[i].fy[1], vsp[i].fy[0], vsp[i].fy[1] };
                    }
                    else
#endif
                        polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                    vsp[i].fy[0] = n0.y;
                    vsp[i].ftag = gtag;
                }
                    break;
                case 5:
                case 2:
                {
                    vec2f_t dpxy[8] = { { dx0, vsp[i].fy[0] }, { dx1, n1.y }, { dx1, vsp[i].fy[1] } };

                    int n = 3;
                    polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                    if (g_nodraw)
                    {
                        if (yax_drawcf != -1)
                            yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].fy[0], n1.y, vsp[i].fy[0], vsp[i].fy[1] };
                    }
                    else
#endif
                        polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                    vsp[i].fy[1] = n1.y;
                    vsp[i].ftag = gtag;
                }
                    break;
                case 0:
                {
                    vec2f_t dpxy[8] = { { dx0, vsp[i].cy[0] }, { dx1, vsp[i].cy[1] }, { dx1, vsp[i].fy[1] }, { dx0, vsp[i].fy[0] } };

                    int n = 4;
                    polymost_clipmost(dpxy, n, x0, x1, y0top, y0bot, y1top, y1bot);
#ifdef YAX_ENABLE
                    if (g_nodraw)
                    {
                        if (yax_drawcf != -1)
                            yax_holecf[yax_drawcf][yax_holencf[yax_drawcf]++] = { dx0, dx1, vsp[i].cy[0], vsp[i].cy[1], vsp[i].fy[0], vsp[i].fy[1] };
                    }
                    else
#endif
                        polymost_drawpoly(dpxy, n, domostpolymethod, tilesiz[globalpicnum]);

                    vsp[i].ctag = vsp[i].ftag = -1;
                }
                default:
                    break;
                }
            }
        }
    }

    gtag++;

    //Combine neighboring vertical strips with matching collinear top&bottom edges
    //This prevents x-splits from propagating through the entire scan
#ifdef COMBINE_STRIPS
    int i = firstnode;

    do
    {
        if (vsp[i].x >= dm1.x)
            break;

        if ((vsp[i].cy[0]+DOMOST_OFFSET*2 >= vsp[i].fy[0]) && (vsp[i].cy[1]+DOMOST_OFFSET*2 >= vsp[i].fy[1]))
            vsp[i].ctag = vsp[i].ftag = -1;

        int const ni = vsp[i].n;

        //POGO: specially treat the viewport nodes so that we will never end up in a situation where we accidentally access the sentinel node
        if (ni >= viewportNodeCount)
        {
            if ((vsp[i].ctag == vsp[ni].ctag) && (vsp[i].ftag == vsp[ni].ftag))
            {
                MERGE_NODES(i, ni);
                continue;
            }
            if (vsp[ni].x - vsp[i].x < DOMOST_OFFSET)
            {
                vsp[i].x = vsp[ni].x;
                vsp[i].cy[0] = vsp[ni].cy[0];
                vsp[i].fy[0] = vsp[ni].fy[0];
                vsp[i].ctag = vsp[ni].ctag;
                vsp[i].ftag = vsp[ni].ftag;
                MERGE_NODES(i, ni);
                continue;
            }
        }
        i = ni;
    }
    while (i);
#endif
}

#ifdef YAX_ENABLE
static void yax_polymost_domost(const int yaxbunch, float x0, float y0, float x1, float y1)
{
    int const dir = (x0 < x1);

    if (dir) //clip dmost (floor)
    {
        y0 -= DOMOST_OFFSET;
        y1 -= DOMOST_OFFSET;
    }
    else //clip umost (ceiling)
    {
        if (x0 == x1) return;
        swapfloat(&x0, &x1);
        swapfloat(&y0, &y1);
        y0 += DOMOST_OFFSET;
        y1 += DOMOST_OFFSET; //necessary?
    }

    // Test if span is outside screen bounds
    if (x1 < xbl || x0 > xbr)
    {
        domost_rejectcount++;
        return;
    }

    vec2f_t dm0 = { x0, y0 };
    vec2f_t dm1 = { x1, y1 };

    float const slop = (dm1.y - dm0.y) / (dm1.x - dm0.x);

    if (dm0.x < xbl)
    {
        dm0.y += slop*(xbl-dm0.x);
        dm0.x = xbl;
    }

    if (dm1.x > xbr)
    {
        dm1.y += slop*(xbr-dm1.x);
        dm1.x = xbr;
    }

    vec2f_t n0, n1;
    float spx[4];
    int32_t  spt[4];

    for (bssize_t newi, i=yax_vsp[yaxbunch][0].n; i; i=newi)
    {
        newi = yax_vsp[yaxbunch][i].n; n0.x = yax_vsp[yaxbunch][i].x; n1.x = yax_vsp[yaxbunch][newi].x;

        if ((dm0.x >= n1.x) || (n0.x >= dm1.x) || (yax_vsp[yaxbunch][i].ctag <= 0)) continue;

        double const dx = double(n1.x)-double(n0.x);
        double const cy = yax_vsp[yaxbunch][i].cy[0],
                     cv = yax_vsp[yaxbunch][i].cy[1]-cy;

        int scnt = 0;

        //Test if left edge requires split (dm0.x,dm0.y) (nx0,cy(0)),<dx,cv(0)>
        if ((dm0.x > n0.x) && (dm0.x < n1.x))
        {
            double const t = (dm0.x-n0.x)*cv - (dm0.y-cy)*dx;
            if (((!dir) && (t <= 0.0)) || ((dir) && (t >= 0.0)))
                { spx[scnt] = dm0.x; spt[scnt] = -1; scnt++; }
        }

        //Test for intersection on umost (0) and dmost (1)

        double const d = ((double(dm0.y) - double(dm1.y)) * dx) - ((double(dm0.x) - double(dm1.x)) * cv);

        double const n = ((double(dm0.y) - cy) * dx) - ((double(dm0.x) - double(n0.x)) * cv);

        double const fnx = double(dm0.x) + ((n / d) * (double(dm1.x) - double(dm0.x)));

        if ((fabs(d) > fabs(n)) && (d * n >= 0.0) && (fnx > n0.x) && (fnx < n1.x))
            spx[scnt] = fnx, spt[scnt++] = 0;

        //Nice hack to avoid full sort later :)
        if ((scnt >= 2) && (spx[scnt-1] < spx[scnt-2]))
        {
            swapfloat(&spx[scnt-1], &spx[scnt-2]);
            swaplong(&spt[scnt-1], &spt[scnt-2]);
        }

        //Test if right edge requires split
        if ((dm1.x > n0.x) && (dm1.x < n1.x))
        {
            double const t = (double(dm1.x)- double(n0.x))*cv - (double(dm1.y)- double(cy))*dx;
            if (((!dir) && (t <= 0.0)) || ((dir) && (t >= 0.0)))
                { spx[scnt] = dm1.x; spt[scnt] = -1; scnt++; }
        }

        yax_vsp[yaxbunch][i].tag = yax_vsp[yaxbunch][newi].tag = -1;

        float const rdx = 1.f/dx;

        for (bssize_t z=0, vcnt=0; z<=scnt; z++,i=vcnt)
        {
            float t;

            if (z == scnt)
                goto skip;

            t = (spx[z]-n0.x)*rdx;
            vcnt = yax_vsinsaft(yaxbunch, i);
            yax_vsp[yaxbunch][i].cy[1] = t*cv + cy;
            yax_vsp[yaxbunch][vcnt].x = spx[z];
            yax_vsp[yaxbunch][vcnt].cy[0] = yax_vsp[yaxbunch][i].cy[1];
            yax_vsp[yaxbunch][vcnt].tag = spt[z];

skip: ;
            int32_t const ni = yax_vsp[yaxbunch][i].n; if (!ni) continue; //this 'if' fixes many bugs!
            float const dx0 = yax_vsp[yaxbunch][i].x; if (dm0.x > dx0) continue;
            float const dx1 = yax_vsp[yaxbunch][ni].x; if (dm1.x < dx1) continue;
            n0.y = (dx0-dm0.x)*slop + dm0.y;
            n1.y = (dx1-dm0.x)*slop + dm0.y;

            //      dx0           dx1
            //       ~             ~
            //----------------------------
            //     t0+=0         t1+=0
            //   vsp[i].cy[0]  vsp[i].cy[1]
            //============================
            //     t0+=1         t1+=3
            //============================
            //   vsp[i].fy[0]    vsp[i].fy[1]
            //     t0+=2         t1+=6
            //
            //     ny0 ?         ny1 ?

            int k = 4;

            if (!dir)
            {
                if ((yax_vsp[yaxbunch][i].tag == 0) || (n0.y <= yax_vsp[yaxbunch][i].cy[0]+DOMOST_OFFSET)) k--;
                if ((yax_vsp[yaxbunch][ni].tag == 0) || (n1.y <= yax_vsp[yaxbunch][i].cy[1]+DOMOST_OFFSET)) k -= 3;
                switch (k)
                {
                    case 4:
                    {
                        yax_vsp[yaxbunch][i].cy[0] = n0.y;
                        yax_vsp[yaxbunch][i].cy[1] = n1.y;
                        yax_vsp[yaxbunch][i].ctag = gtag;
                    }
                    break;
                    case 1:
                    case 2:
                    {
                        yax_vsp[yaxbunch][i].cy[0] = n0.y;
                        yax_vsp[yaxbunch][i].ctag = gtag;
                    }
                    break;
                    case 3:
                    {
                        yax_vsp[yaxbunch][i].cy[1] = n1.y;
                        yax_vsp[yaxbunch][i].ctag = gtag;
                    }
                    break;
                    default: break;
                }
            }
            else
            {
                if ((yax_vsp[yaxbunch][i].tag == 0) || (n0.y >= yax_vsp[yaxbunch][i].cy[0]-DOMOST_OFFSET)) k++;
                if ((yax_vsp[yaxbunch][ni].tag == 0) || (n1.y >= yax_vsp[yaxbunch][i].cy[1]-DOMOST_OFFSET)) k += 3;
                switch (k)
                {
                case 4:
                {
                    yax_vsp[yaxbunch][i].cy[0] = n0.y;
                    yax_vsp[yaxbunch][i].cy[1] = n1.y;
                    yax_vsp[yaxbunch][i].ctag = gtag;
                }
                    break;
                case 7:
                case 6:
                {
                    yax_vsp[yaxbunch][i].cy[0] = n0.y;
                    yax_vsp[yaxbunch][i].ctag = gtag;
                }
                    break;
                case 5:
                {
                    yax_vsp[yaxbunch][i].cy[1] = n1.y;
                    yax_vsp[yaxbunch][i].ctag = gtag;
                }
                    break;
                default:
                    break;
                }
            }
        }
    }

    gtag++;

    //Combine neighboring vertical strips with matching collinear top&bottom edges
    //This prevents x-splits from propagating through the entire scan
#ifdef COMBINE_STRIPS
    int i = yax_vsp[yaxbunch][0].n;

    do
    {
        int const ni = yax_vsp[yaxbunch][i].n;

        if ((yax_vsp[yaxbunch][i].ctag == yax_vsp[yaxbunch][ni].ctag))
        {
            yax_vsp[yaxbunch][i].cy[1] = yax_vsp[yaxbunch][ni].cy[1];
            yax_vsdel(yaxbunch, ni);
        }
        else i = ni;
    }
    while (i);
#endif
}

static int32_t should_clip_cfwall(float x0, float y0, float x1, float y1)
{
    int const dir = (x0 < x1);

    if (dir && yax_globallev >= YAX_MAXDRAWS)
        return 1;

    if (!dir && yax_globallev <= YAX_MAXDRAWS)
        return 1;

    if (dir) //clip dmost (floor)
    {
        y0 -= DOMOST_OFFSET;
        y1 -= DOMOST_OFFSET;
    }
    else //clip umost (ceiling)
    {
        if (x0 == x1) return 1;
        swapfloat(&x0, &x1);
        swapfloat(&y0, &y1);
        y0 += DOMOST_OFFSET;
        y1 += DOMOST_OFFSET; //necessary?
    }

    x0 -= DOMOST_OFFSET;
    x1 += DOMOST_OFFSET;

    // Test if span is outside screen bounds
    if (x1 < xbl || x0 > xbr)
        return 1;

    vec2f_t dm0 = { x0, y0 };
    vec2f_t dm1 = { x1, y1 };

    float const slop = (dm1.y - dm0.y) / (dm1.x - dm0.x);

    if (dm0.x < xbl)
    {
        dm0.y += slop*(xbl-dm0.x);
        dm0.x = xbl;
    }

    if (dm1.x > xbr)
    {
        dm1.y += slop*(xbr-dm1.x);
        dm1.x = xbr;
    }

    vec2f_t n0, n1;
    float spx[6], spcy[6], spfy[6];
    int32_t spt[6];

    for (bssize_t newi, i=vsp[0].n; i; i=newi)
    {
        newi = vsp[i].n; n0.x = vsp[i].x; n1.x = vsp[newi].x;

        if ((dm0.x >= n1.x) || (n0.x >= dm1.x) || (vsp[i].ctag <= 0)) continue;

        float const dx = n1.x-n0.x;
        float const cy[2] = { vsp[i].cy[0], vsp[i].fy[0] },
                    cv[2] = { vsp[i].cy[1]-cy[0], vsp[i].fy[1]-cy[1] };

        int scnt = 0;

        spx[scnt] = n0.x; spt[scnt] = -1; scnt++;

        //Test if left edge requires split (dm0.x,dm0.y) (nx0,cy(0)),<dx,cv(0)>
        if ((dm0.x > n0.x) && (dm0.x < n1.x))
        {
            float const t = (dm0.x-n0.x)*cv[dir] - (dm0.y-cy[dir])*dx;
            if (((!dir) && (t < 0.f)) || ((dir) && (t > 0.f)))
                { spx[scnt] = dm0.x; spt[scnt] = -1; scnt++; }
        }

        //Test for intersection on umost (0) and dmost (1)

        float const d[2] = { ((dm0.y - dm1.y) * dx) - ((dm0.x - dm1.x) * cv[0]),
                             ((dm0.y - dm1.y) * dx) - ((dm0.x - dm1.x) * cv[1]) };

        float const n[2] = { ((dm0.y - cy[0]) * dx) - ((dm0.x - n0.x) * cv[0]),
                             ((dm0.y - cy[1]) * dx) - ((dm0.x - n0.x) * cv[1]) };

        float const fnx[2] = { dm0.x + ((n[0] / d[0]) * (dm1.x - dm0.x)),
                               dm0.x + ((n[1] / d[1]) * (dm1.x - dm0.x)) };

        if ((Bfabsf(d[0]) > Bfabsf(n[0])) && (d[0] * n[0] >= 0.f) && (fnx[0] > n0.x) && (fnx[0] < n1.x))
            spx[scnt] = fnx[0], spt[scnt++] = 0;

        if ((Bfabsf(d[1]) > Bfabsf(n[1])) && (d[1] * n[1] >= 0.f) && (fnx[1] > n0.x) && (fnx[1] < n1.x))
            spx[scnt] = fnx[1], spt[scnt++] = 1;

        //Nice hack to avoid full sort later :)
        if ((scnt >= 2) && (spx[scnt-1] < spx[scnt-2]))
        {
            swapfloat(&spx[scnt-1], &spx[scnt-2]);
            swaplong(&spx[scnt-1], &spx[scnt-2]);
        }

        //Test if right edge requires split
        if ((dm1.x > n0.x) && (dm1.x < n1.x))
        {
            float const t = (dm1.x-n0.x)*cv[dir] - (dm1.y-cy[dir])*dx;
            if (((!dir) && (t < 0.f)) || ((dir) && (t > 0.f)))
                { spx[scnt] = dm1.x; spt[scnt] = -1; scnt++; }
        }

        spx[scnt] = n1.x; spt[scnt] = -1; scnt++;

        float const rdx = 1.f/dx;
        for (bssize_t z=0; z<scnt; z++)
        {
            float const t = (spx[z]-n0.x)*rdx;
            spcy[z] = t*cv[0]+cy[0];
            spfy[z] = t*cv[1]+cy[1];
        }

        for (bssize_t z=0; z<scnt-1; z++)
        {
            float const dx0 = spx[z];
            float const dx1 = spx[z+1];
            n0.y = (dx0-dm0.x)*slop + dm0.y;
            n1.y = (dx1-dm0.x)*slop + dm0.y;

            //      dx0           dx1
            //       ~             ~
            //----------------------------
            //     t0+=0         t1+=0
            //   vsp[i].cy[0]  vsp[i].cy[1]
            //============================
            //     t0+=1         t1+=3
            //============================
            //   vsp[i].fy[0]    vsp[i].fy[1]
            //     t0+=2         t1+=6
            //
            //     ny0 ?         ny1 ?

            int k = 4;
            if (dir)
            {
                if ((spt[z] == 0) || (n0.y <= spcy[z]+DOMOST_OFFSET)) k--;
                if ((spt[z+1] == 0) || (n1.y <= spcy[z+1]+DOMOST_OFFSET)) k -= 3;
                if (k != 0)
                    return 1;
            }
            else
            {
                if ((spt[z] == 1) || (n0.y >= spfy[z]-DOMOST_OFFSET)) k++;
                if ((spt[z+1] == 1) || (n1.y >= spfy[z+1]-DOMOST_OFFSET)) k += 3;
                if (k != 8)
                    return 1;
            }
        }
    }
    return 0;
}

#endif


// variables that are set to ceiling- or floor-members, depending
// on which one is processed right now
static int32_t global_cf_z;
static float global_cf_xpanning, global_cf_ypanning, global_cf_heinum;
static int32_t global_cf_shade, global_cf_pal, global_cf_fogpal;
static float (*global_getzofslope_func)(usectorptr_t, float, float);

static void polymost_internal_nonparallaxed(vec2f_t n0, vec2f_t n1, float ryp0, float ryp1, float x0, float x1,
                                            float y0, float y1, int32_t sectnum)
{
    int const have_floor = sectnum & MAXSECTORS;
    sectnum &= ~MAXSECTORS;
    usectortype const * const sec = (usectortype *)&sector[sectnum];

    // comments from floor code:
            //(singlobalang/-16384*(sx-ghalfx) + 0*(sy-ghoriz) + (cosviewingrangeglobalang/16384)*ghalfx)*d + globalposx    = u*16
            //(cosglobalang/ 16384*(sx-ghalfx) + 0*(sy-ghoriz) + (sinviewingrangeglobalang/16384)*ghalfx)*d + globalposy    = v*16
            //(                  0*(sx-ghalfx) + 1*(sy-ghoriz) + (                             0)*ghalfx)*d + globalposz/16 = (sec->floorz/16)

    float ft[4] = { fglobalposx, fglobalposy, fcosglobalang, fsinglobalang };

    polymost_outputGLDebugMessage(3, "polymost_internal_nonparallaxed(n0:{x:%f, y:%f}, n1:{x:%f, y:%f}, ryp0:%f, ryp1:%f, x0:%f, x1:%f, y0:%f, y1:%f, sectnum:%d)",
                                  n0.x, n0.y, n1.x, n1.y, ryp0, ryp1, x0, x1, y0, y1, sectnum);

    if (globalorientation & 64)
    {
        //relative alignment
        vec2_t const xy = { wall[wall[sec->wallptr].point2].x - wall[sec->wallptr].x,
                            wall[wall[sec->wallptr].point2].y - wall[sec->wallptr].y };
        float r;

        if (globalorientation & 2)
        {
            int i = krecipasm(nsqrtasm(uhypsq(xy.x,xy.y)));
            r = i * (1.f/1073741824.f);
        }
        else
        {
            int i = nsqrtasm(uhypsq(xy.x,xy.y)); if (i == 0) i = 1024; else i = tabledivide32(1048576, i);
            r = i * (1.f/1048576.f);
        }

        vec2f_t const fxy = { xy.x*r, xy.y*r };

        ft[0] = ((float)(globalposx - wall[sec->wallptr].x)) * fxy.x + ((float)(globalposy - wall[sec->wallptr].y)) * fxy.y;
        ft[1] = ((float)(globalposy - wall[sec->wallptr].y)) * fxy.x - ((float)(globalposx - wall[sec->wallptr].x)) * fxy.y;
        ft[2] = fcosglobalang * fxy.x + fsinglobalang * fxy.y;
        ft[3] = fsinglobalang * fxy.x - fcosglobalang * fxy.y;

        globalorientation ^= (!(globalorientation & 4)) ? 32 : 16;
    }

    xtex.d = 0;
    ytex.d = gxyaspect;

    if (!(globalorientation&2) && global_cf_z-globalposz)  // PK 2012: don't allow div by zero
            ytex.d /= (double)(global_cf_z-globalposz);

    otex.d = -ghoriz * ytex.d;

    if (globalorientation & 8)
    {
        ft[0] *=  (1.f / 8.f);
        ft[1] *= -(1.f / 8.f);
        ft[2] *=  (1.f / 2097152.f);
        ft[3] *=  (1.f / 2097152.f);
    }
    else
    {
        ft[0] *=  (1.f / 16.f);
        ft[1] *= -(1.f / 16.f);
        ft[2] *=  (1.f / 4194304.f);
        ft[3] *=  (1.f / 4194304.f);
    }

    xtex.u = ft[3] * -(1.f / 65536.f) * (double)viewingrange;
    xtex.v = ft[2] * -(1.f / 65536.f) * (double)viewingrange;
    ytex.u = ft[0] * ytex.d;
    ytex.v = ft[1] * ytex.d;
    otex.u = ft[0] * otex.d;
    otex.v = ft[1] * otex.d;
    otex.u += (ft[2] - xtex.u) * ghalfx;
    otex.v -= (ft[3] + xtex.v) * ghalfx;

    //Texture flipping
    if (globalorientation&4)
    {
        swapdouble(&xtex.u, &xtex.v);
        swapdouble(&ytex.u, &ytex.v);
        swapdouble(&otex.u, &otex.v);
    }

    if (globalorientation&16) { xtex.u = -xtex.u; ytex.u = -ytex.u; otex.u = -otex.u; }
    if (globalorientation&32) { xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; }

    //Texture panning
    vec2f_t fxy = { global_cf_xpanning * ((float)(1 << (picsiz[globalpicnum] & 15))) * (1.0f / 256.f),
                    global_cf_ypanning * ((float)(1 << (picsiz[globalpicnum] >> 4))) * (1.0f / 256.f) };

    if ((globalorientation&(2+64)) == (2+64)) //Hack for panning for slopes w/ relative alignment
    {
        float r = global_cf_heinum * (1.0f / 4096.f);
        r = polymost_invsqrt_approximation(r * r + 1);

        if (!(globalorientation & 4))
            fxy.y *= r;
        else
            fxy.x *= r;
    }
    ytex.u += ytex.d*fxy.x; otex.u += otex.d*fxy.x;
    ytex.v += ytex.d*fxy.y; otex.v += otex.d*fxy.y;

    if (globalorientation&2) //slopes
    {
        //Pick some point guaranteed to be not collinear to the 1st two points
        vec2f_t dxy = { n1.y - n0.y, n0.x - n1.x };

        float const dxyr = polymost_invsqrt_approximation(dxy.x * dxy.x + dxy.y * dxy.y);

        dxy.x *= dxyr * 4096.f;
        dxy.y *= dxyr * 4096.f;

        vec2f_t const oxy = { n0.x + dxy.x, n0.y + dxy.y };

        float const ox2 = (oxy.y - fglobalposy) * gcosang - (oxy.x - fglobalposx) * gsinang;
        float oy2 = 1.f / ((oxy.x - fglobalposx) * gcosang2 + (oxy.y - fglobalposy) * gsinang2);

        double const px[3] = { x0, x1, (double)ghalfx * ox2 * oy2 + ghalfx };

        oy2 *= gyxscale;

        double py[3] = { ryp0 + (double)ghoriz, ryp1 + (double)ghoriz, oy2 + (double)ghoriz };

        vec3d_t const duv[3] = {
            { (px[0] * xtex.d + py[0] * ytex.d + otex.d),
              (px[0] * xtex.u + py[0] * ytex.u + otex.u),
              (px[0] * xtex.v + py[0] * ytex.v + otex.v)
            },
            { (px[1] * xtex.d + py[1] * ytex.d + otex.d),
              (px[1] * xtex.u + py[1] * ytex.u + otex.u),
              (px[1] * xtex.v + py[1] * ytex.v + otex.v)
            },
            { (px[2] * xtex.d + py[2] * ytex.d + otex.d),
              (px[2] * xtex.u + py[2] * ytex.u + otex.u),
              (px[2] * xtex.v + py[2] * ytex.v + otex.v)
            }
        };

        py[0] = y0;
        py[1] = y1;
        py[2] = double(global_getzofslope_func((usectorptr_t)&sector[sectnum], oxy.x, oxy.y) - globalposz) * oy2 + ghoriz;

        vec3f_t oxyz[2] = { { (float)(py[1] - py[2]), (float)(py[2] - py[0]), (float)(py[0] - py[1]) },
                            { (float)(px[2] - px[1]), (float)(px[0] - px[2]), (float)(px[1] - px[0]) } };

        float const r = 1.f / (oxyz[0].x * px[0] + oxyz[0].y * px[1] + oxyz[0].z * px[2]);

        xtex.d = (oxyz[0].x * duv[0].d + oxyz[0].y * duv[1].d + oxyz[0].z * duv[2].d) * r;
        xtex.u = (oxyz[0].x * duv[0].u + oxyz[0].y * duv[1].u + oxyz[0].z * duv[2].u) * r;
        xtex.v = (oxyz[0].x * duv[0].v + oxyz[0].y * duv[1].v + oxyz[0].z * duv[2].v) * r;

        ytex.d = (oxyz[1].x * duv[0].d + oxyz[1].y * duv[1].d + oxyz[1].z * duv[2].d) * r;
        ytex.u = (oxyz[1].x * duv[0].u + oxyz[1].y * duv[1].u + oxyz[1].z * duv[2].u) * r;
        ytex.v = (oxyz[1].x * duv[0].v + oxyz[1].y * duv[1].v + oxyz[1].z * duv[2].v) * r;

        otex.d = duv[0].d - px[0] * xtex.d - py[0] * ytex.d;
        otex.u = duv[0].u - px[0] * xtex.u - py[0] * ytex.u;
        otex.v = duv[0].v - px[0] * xtex.v - py[0] * ytex.v;

        if (globalorientation&64) //Hack for relative alignment on slopes
        {
            float r = global_cf_heinum * (1.0f / 4096.f);
            r = Bsqrtf(r*r+1);
            if (!(globalorientation&4)) { xtex.v *= r; ytex.v *= r; otex.v *= r; }
            else { xtex.u *= r; ytex.u *= r; otex.u *= r; }
        }
    }

    domostpolymethod = (globalorientation>>7) & DAMETH_MASKPROPS;

    pow2xsplit = 0;
    drawpoly_alpha = 0.f;
    drawpoly_blend = 0;

	calc_and_apply_fog(fogshade(global_cf_shade, global_cf_pal), sec->visibility, POLYMOST_CHOOSE_FOG_PAL(global_cf_fogpal, global_cf_pal));

    if (have_floor)
    {
        if (globalposz > getflorzofslope(sectnum, globalposx, globalposy))
            domostpolymethod = DAMETH_BACKFACECULL; //Back-face culling

        if (domostpolymethod & DAMETH_MASKPROPS)
            GLInterface.EnableBlend(true);

        polymost_domost(x0, y0, x1, y1); //flor
    }
    else
    {
        if (globalposz < getceilzofslope(sectnum, globalposx, globalposy))
            domostpolymethod = DAMETH_BACKFACECULL; //Back-face culling

        if (domostpolymethod & DAMETH_MASKPROPS)
            GLInterface.EnableBlend(true);

        polymost_domost(x1, y1, x0, y0); //ceil
    }

    if (domostpolymethod & DAMETH_MASKPROPS)
        GLInterface.EnableBlend(false);

    domostpolymethod = DAMETH_NOMASK;
}

static void calc_ypanning(int32_t refposz, float ryp0, float ryp1,
                          float x0, float x1, uint8_t ypan, uint8_t yrepeat,
                          int32_t dopancor, const vec2_16_t &tilesize)
{
    float const t0 = ((float)(refposz-globalposz))*ryp0 + ghoriz;
    float const t1 = ((float)(refposz-globalposz))*ryp1 + ghoriz;
    float t = (float(xtex.d*x0 + otex.d) * (float)yrepeat) / ((x1-x0) * ryp0 * 2048.f);
    int i = (1<<(picsiz[globalpicnum]>>4));
    if (i < tilesize.y) i <<= 1;


    float const fy = (float)(ypan * i) * (1.f / 256.f);

    xtex.v = double(t0 - t1) * t;
    ytex.v = double(x1 - x0) * t;
    otex.v = -xtex.v * x0 - ytex.v * t0 + fy * otex.d;
    xtex.v += fy * xtex.d;
    ytex.v += fy * ytex.d;
}

static inline int32_t testvisiblemost(float const x0, float const x1)
{
    for (bssize_t i=vsp[0].n, newi; i; i=newi)
    {
        newi = vsp[i].n;
        if ((x0 < vsp[newi].x) && (vsp[i].x < x1) && (vsp[i].ctag >= 0))
            return 1;
    }
    return 0;
}

static inline int polymost_getclosestpointonwall(vec2_t const * const pos, int32_t dawall, vec2_t * const n)
{
    vec2_t const w = { wall[dawall].x, wall[dawall].y };
    vec2_t const d = { POINT2(dawall).x - w.x, POINT2(dawall).y - w.y };
    int64_t i = d.x * ((int64_t)pos->x - w.x) + d.y * ((int64_t)pos->y - w.y);

    if (i < 0)
        return 1;

    int64_t const j = (int64_t)d.x * d.x + (int64_t)d.y * d.y;

    if (i > j)
        return 1;

    i = tabledivide64((i << 15), j) << 15;

    n->x = w.x + ((d.x * i) >> 30);
    n->y = w.y + ((d.y * i) >> 30);

    return 0;
}

float fgetceilzofslope(usectorptr_t sec, float dax, float day)
{
    if (!(sec->ceilingstat&2))
        return float(sec->ceilingz);

    auto const wal  = (uwallptr_t)&wall[sec->wallptr];
    auto const wal2 = (uwallptr_t)&wall[wal->point2];

    vec2_t const w = *(vec2_t const *)wal;
    vec2_t const d = { wal2->x - w.x, wal2->y - w.y };

    int const i = nsqrtasm(uhypsq(d.x,d.y))<<5;
    if (i == 0) return sec->ceilingz;

    float const j = (d.x*(day-w.y)-d.y*(dax-w.x))*(1.f/8.f);
    return float(sec->ceilingz) + (sec->ceilingheinum*j)/i;
}

float fgetflorzofslope(usectorptr_t sec, float dax, float day)
{
    if (!(sec->floorstat&2))
        return float(sec->floorz);

    auto const wal  = (uwallptr_t)&wall[sec->wallptr];
    auto const wal2 = (uwallptr_t)&wall[wal->point2];

    vec2_t const w = *(vec2_t const *)wal;
    vec2_t const d = { wal2->x - w.x, wal2->y - w.y };

    int const i = nsqrtasm(uhypsq(d.x,d.y))<<5;
    if (i == 0) return sec->floorz;

    float const j = (d.x*(day-w.y)-d.y*(dax-w.x))*(1.f/8.f);
    return float(sec->floorz) + (sec->floorheinum*j)/i;
}

void fgetzsofslope(usectorptr_t sec, float dax, float day, float* ceilz, float *florz)
{
    *ceilz = float(sec->ceilingz); *florz = float(sec->floorz);

    if (((sec->ceilingstat|sec->floorstat)&2) != 2)
        return;

    auto const wal  = (uwallptr_t)&wall[sec->wallptr];
    auto const wal2 = (uwallptr_t)&wall[wal->point2];

    vec2_t const d = { wal2->x - wal->x, wal2->y - wal->y };

    int const i = nsqrtasm(uhypsq(d.x,d.y))<<5;
    if (i == 0) return;
    
    float const j = (d.x*(day-wal->y)-d.y*(dax-wal->x))*(1.f/8.f);
    if (sec->ceilingstat&2)
        *ceilz += (sec->ceilingheinum*j)/i;
    if (sec->floorstat&2)
        *florz += (sec->floorheinum*j)/i;
}

static void polymost_flatskyrender(vec2f_t const* const dpxy, int32_t const n, int32_t method, const vec2_16_t &tilesiz)
{
    flatskyrender = 0;
    vec2f_t xys[8];

    // Transform polygon to sky coordinates
    for (int i = 0; i < n; i++)
    {
        vec3f_t const o = { dpxy[i].x-ghalfx, dpxy[i].y-ghalfy, ghalfx / gvrcorrection };

        //Up/down rotation
        vec3d_t v = { o.x, o.y * gchang - o.z * gshang, o.z * gchang + o.y * gshang };
        float const r = (ghalfx / gvrcorrection) / v.z;
        xys[i].x = v.x * r + ghalfx;
        xys[i].y = v.y * r + ghalfy;
    }
    
    float const fglobalang = fix16_to_float(qglobalang);
    int32_t dapyscale, dapskybits, dapyoffs, daptileyscale;
    int8_t const * dapskyoff = getpsky(globalpicnum, &dapyscale, &dapskybits, &dapyoffs, &daptileyscale);

    ghoriz = (qglobalhoriz*(1.f/65536.f)-float(ydimen>>1))*dapyscale*(1.f/65536.f)+float(ydimen>>1)+ghorizcorrect;

    float const dd = fxdimen*.0000001f; //Adjust sky depth based on screen size!
    float vv[2];
    float t = (float)((1<<(picsiz[globalpicnum]&15))<<dapskybits);
    vv[1] = dd*((float)xdimscale*fviewingrange) * (1.f/(daptileyscale*65536.f));
    vv[0] = dd*((float)((tilesiz.y>>1)+dapyoffs)) - vv[1]*ghoriz;
    int ti = (1<<(picsiz[globalpicnum]>>4)); if (ti != tilesiz.y) ti += ti;
    vec3f_t o;

    skyclamphack = 0;

    xtex.d = xtex.v = 0;
    ytex.d = ytex.u = 0;
    otex.d = dd;
    xtex.u = otex.d * (t * double(((uint64_t)xdimscale * yxaspect) * viewingrange)) *
                        (1.0 / (16384.0 * 65536.0 * 65536.0 * 5.0 * 1024.0));
    ytex.v = vv[1];
    otex.v = r_parallaxskypanning ? vv[0] + dd*(float)global_cf_ypanning*(float)ti*(1.f/256.f) : vv[0];

    float x0 = xys[0].x, x1 = xys[0].x;

    for (bssize_t i=n-1; i>=1; i--)
    {
        if (xys[i].x < x0) x0 = xys[i].x;
        if (xys[i].x > x1) x1 = xys[i].x;
    }

    int const npot = (1<<(picsiz[globalpicnum]&15)) != tilesiz.x;
    int const xpanning = (r_parallaxskypanning?global_cf_xpanning:0);

	GLInterface.SetClamp((npot || xpanning != 0) ? 0 : 2);

    int picnumbak = globalpicnum;
    ti = globalpicnum;
    o.y = fviewingrange/(ghalfx*256.f); o.z = 1.f/o.y;

    int y = ((int32_t)(((x0-ghalfx)*o.y)+fglobalang)>>(11-dapskybits));
    float fx = x0;

    do
    {
        globalpicnum = dapskyoff[y&((1<<dapskybits)-1)]+ti;
        if (npot)
        {
            fx = ((float)((y<<(11-dapskybits))-fglobalang))*o.z+ghalfx;
            int tang = (y<<(11-dapskybits))&2047;
            otex.u = otex.d*(t*((float)(tang)) * (1.f/2048.f) + xpanning) - xtex.u*fx;
        }
        else
            otex.u = otex.d*(t*((float)(fglobalang-(y<<(11-dapskybits)))) * (1.f/2048.f) + xpanning) - xtex.u*ghalfx;
        y++;
        o.x = fx; fx = ((float)((y<<(11-dapskybits))-fglobalang))*o.z+ghalfx;

        if (fx > x1) { fx = x1; ti = -1; }

        vec3d_t otexbak = otex, xtexbak = xtex, ytexbak = ytex;

        // Transform texture mapping factors
        vec2f_t fxy[3] = { { ghalfx * (1.f - 0.25f), ghalfy * (1.f - 0.25f) },
                          { ghalfx, ghalfy * (1.f + 0.25f) },
                          { ghalfx * (1.f + 0.25f), ghalfy * (1.f - 0.25f) } };

        vec3d_t duv[3] = {
            { (fxy[0].x * xtex.d + fxy[0].y * ytex.d + otex.d),
              (fxy[0].x * xtex.u + fxy[0].y * ytex.u + otex.u),
              (fxy[0].x * xtex.v + fxy[0].y * ytex.v + otex.v)
            },
            { (fxy[1].x * xtex.d + fxy[1].y * ytex.d + otex.d),
              (fxy[1].x * xtex.u + fxy[1].y * ytex.u + otex.u),
              (fxy[1].x * xtex.v + fxy[1].y * ytex.v + otex.v)
            },
            { (fxy[2].x * xtex.d + fxy[2].y * ytex.d + otex.d),
              (fxy[2].x * xtex.u + fxy[2].y * ytex.u + otex.u),
              (fxy[2].x * xtex.v + fxy[2].y * ytex.v + otex.v)
            }
        };
        vec2f_t fxyt[3];
        vec3d_t duvt[3];

        for (int i = 0; i < 3; i++)
        {
            vec2f_t const o = { fxy[i].x-ghalfx, fxy[i].y-ghalfy };
            vec3f_t const o2 = { o.x, o.y, ghalfx / gvrcorrection };

            //Up/down rotation (backwards)
            vec3d_t v = { o2.x, o2.y * gchang + o2.z * gshang, o2.z * gchang - o2.y * gshang };
            float const r = (ghalfx / gvrcorrection) / v.z;
            fxyt[i].x = v.x * r + ghalfx;
            fxyt[i].y = v.y * r + ghalfy;
            duvt[i].d = duv[i].d*r;
            duvt[i].u = duv[i].u*r;
            duvt[i].v = duv[i].v*r;
        }

        vec3f_t oxyz[2] = { { (float)(fxyt[1].y - fxyt[2].y), (float)(fxyt[2].y - fxyt[0].y), (float)(fxyt[0].y - fxyt[1].y) },
                            { (float)(fxyt[2].x - fxyt[1].x), (float)(fxyt[0].x - fxyt[2].x), (float)(fxyt[1].x - fxyt[0].x) } };

        float const rr = 1.f / (oxyz[0].x * fxyt[0].x + oxyz[0].y * fxyt[1].x + oxyz[0].z * fxyt[2].x);

        xtex.d = (oxyz[0].x * duvt[0].d + oxyz[0].y * duvt[1].d + oxyz[0].z * duvt[2].d) * rr;
        xtex.u = (oxyz[0].x * duvt[0].u + oxyz[0].y * duvt[1].u + oxyz[0].z * duvt[2].u) * rr;
        xtex.v = (oxyz[0].x * duvt[0].v + oxyz[0].y * duvt[1].v + oxyz[0].z * duvt[2].v) * rr;

        ytex.d = (oxyz[1].x * duvt[0].d + oxyz[1].y * duvt[1].d + oxyz[1].z * duvt[2].d) * rr;
        ytex.u = (oxyz[1].x * duvt[0].u + oxyz[1].y * duvt[1].u + oxyz[1].z * duvt[2].u) * rr;
        ytex.v = (oxyz[1].x * duvt[0].v + oxyz[1].y * duvt[1].v + oxyz[1].z * duvt[2].v) * rr;

        otex.d = duvt[0].d - fxyt[0].x * xtex.d - fxyt[0].y * ytex.d;
        otex.u = duvt[0].u - fxyt[0].x * xtex.u - fxyt[0].y * ytex.u;
        otex.v = duvt[0].v - fxyt[0].x * xtex.v - fxyt[0].y * ytex.v;

        vec2f_t cxy[8];
        vec2f_t cxy2[8];
        int n2 = 0, n3 = 0;

        // Clip to o.x
        for (bssize_t i=0; i<n; i++)
        {
            int const j = i < n-1 ? i + 1 : 0;

            if (xys[i].x >= o.x)
                cxy[n2++] = xys[i];

            if ((xys[i].x >= o.x) != (xys[j].x >= o.x))
            {
                float const r = (o.x - xys[i].x) / (xys[j].x - xys[i].x);
                cxy[n2++] = { o.x, (xys[j].y - xys[i].y) * r + xys[i].y };
            }
        }

        // Clip to fx
        for (bssize_t i=0; i<n2; i++)
        {
            int const j = i < n2-1 ? i + 1 : 0;

            if (cxy[i].x <= fx)
                cxy2[n3++] = cxy[i];

            if ((cxy[i].x <= fx) != (cxy[j].x <= fx))
            {
                float const r = (fx - cxy[i].x) / (cxy[j].x - cxy[i].x);
                cxy2[n3++] = { fx, (cxy[j].y - cxy[i].y) * r + cxy[i].y };
            }
        }

        // Transform back to polymost coordinates
        for (int i = 0; i < n3; i++)
        {
            vec3f_t const o = { cxy2[i].x-ghalfx, cxy2[i].y-ghalfy, ghalfx / gvrcorrection };

            //Up/down rotation
            vec3d_t v = { o.x, o.y * gchang + o.z * gshang, o.z * gchang - o.y * gshang };
            float const r = (ghalfx / gvrcorrection) / v.z;
            cxy[i].x = v.x * r + ghalfx;
            cxy[i].y = v.y * r + ghalfy;
        }

        polymost_drawpoly(cxy, n3, method|DAMETH_WALL, tilesiz);

        otex = otexbak, xtex = xtexbak, ytex = ytexbak;
    }
    while (ti >= 0);

    globalpicnum = picnumbak;

	GLInterface.SetClamp(0);

    flatskyrender = 1;
}

static void polymost_drawalls(int32_t const bunch)
{
    drawpoly_alpha = 0.f;
    drawpoly_blend = 0;

    int32_t const sectnum = thesector[bunchfirst[bunch]];
    auto const sec = (usectorptr_t)&sector[sectnum];
    float const fglobalang = fix16_to_float(qglobalang);

    polymost_outputGLDebugMessage(3, "polymost_drawalls(bunch:%d)", bunch);

    //DRAW WALLS SECTION!
    for (bssize_t z=bunchfirst[bunch]; z>=0; z=bunchp2[z])
    {
        int32_t const wallnum = thewall[z];

        auto const wal = (uwallptr_t)&wall[wallnum];
        auto const wal2 = (uwallptr_t)&wall[wal->point2];
        int32_t const nextsectnum = wal->nextsector;
        auto const nextsec = nextsectnum>=0 ? (usectorptr_t)&sector[nextsectnum] : NULL;

        //Offset&Rotate 3D coordinates to screen 3D space
        vec2f_t walpos = { (float)(wal->x-globalposx), (float)(wal->y-globalposy) };

        vec2f_t p0 = { walpos.y * gcosang - walpos.x * gsinang, walpos.x * gcosang2 + walpos.y * gsinang2 };
        vec2f_t const op0 = p0;

        walpos = { (float)(wal2->x - globalposx),
                   (float)(wal2->y - globalposy) };

        vec2f_t p1 = { walpos.y * gcosang - walpos.x * gsinang, walpos.x * gcosang2 + walpos.y * gsinang2 };

        //Clip to close parallel-screen plane

        vec2f_t n0, n1;
        float t0, t1;

        if (p0.y < SCISDIST)
        {
            if (p1.y < SCISDIST) continue;
            t0 = (SCISDIST-p0.y)/(p1.y-p0.y);
            p0 = { (p1.x-p0.x)*t0+p0.x, SCISDIST };
            n0 = { (wal2->x-wal->x)*t0+wal->x,
                   (wal2->y-wal->y)*t0+wal->y };
        }
        else
        {
            t0 = 0.f;
            n0 = { (float)wal->x, (float)wal->y };
        }

        if (p1.y < SCISDIST)
        {
            t1 = (SCISDIST-op0.y)/(p1.y-op0.y);
            p1 = { (p1.x-op0.x)*t1+op0.x, SCISDIST };
            n1 = { (wal2->x-wal->x)*t1+wal->x,
                   (wal2->y-wal->y)*t1+wal->y };
        }
        else
        {
            t1 = 1.f;
            n1 = { (float)wal2->x, (float)wal2->y };
        }

        float ryp0 = 1.f/p0.y, ryp1 = 1.f/p1.y;

        //Generate screen coordinates for front side of wall
        float const x0 = ghalfx*p0.x*ryp0 + ghalfx, x1 = ghalfx*p1.x*ryp1 + ghalfx;

        if (x1 <= x0) continue;

        ryp0 *= gyxscale; ryp1 *= gyxscale;

        float cz, fz;

        fgetzsofslope((usectorptr_t)&sector[sectnum],n0.x,n0.y,&cz,&fz);
        float const cy0 = (cz-globalposz)*ryp0 + ghoriz, fy0 = (fz-globalposz)*ryp0 + ghoriz;

        fgetzsofslope((usectorptr_t)&sector[sectnum],n1.x,n1.y,&cz,&fz);
        float const cy1 = (cz-globalposz)*ryp1 + ghoriz, fy1 = (fz-globalposz)*ryp1 + ghoriz;

        xtex2.d = (ryp0 - ryp1)*gxyaspect / (x0 - x1);
        ytex2.d = 0;
        otex2.d = ryp0 * gxyaspect - xtex2.d*x0;

        xtex2.u = ytex2.u = otex2.u = 0;
        xtex2.v = ytex2.v = otex2.v = 0;

#ifdef YAX_ENABLE
        yax_holencf[YAX_FLOOR] = 0;
        yax_drawcf = YAX_FLOOR;
#endif

        // Floor

        globalpicnum = sec->floorpicnum;
        globalshade = sec->floorshade;
        globalpal = sec->floorpal;
        globalorientation = sec->floorstat;
        globvis = (sector[sectnum].visibility != 0) ?
                  mulscale4(globalcisibility, (uint8_t)(sector[sectnum].visibility + 16)) :
                  globalcisibility;
        globvis2 = (sector[sectnum].visibility != 0) ?
                  mulscale4(globalcisibility2, (uint8_t)(sector[sectnum].visibility + 16)) :
                  globalcisibility2;
		GLInterface.SetVisibility(globvis2, fviewingrange);

        tileUpdatePicnum(&globalpicnum, sectnum);

        int32_t dapyscale, dapskybits, dapyoffs, daptileyscale;
        int8_t const * dapskyoff = getpsky(globalpicnum, &dapyscale, &dapskybits, &dapyoffs, &daptileyscale);

        global_cf_fogpal = sec->fogpal;
        global_cf_shade = sec->floorshade, global_cf_pal = sec->floorpal; global_cf_z = sec->floorz;  // REFACT
        global_cf_xpanning = sec->floorxpanning; global_cf_ypanning = sec->floorypanning, global_cf_heinum = sec->floorheinum;
        global_getzofslope_func = &fgetflorzofslope;

        if (globalpicnum >= r_rortexture && globalpicnum < r_rortexture + r_rortexturerange && r_rorphase == 0)
        {
            xtex.d = (ryp0-ryp1)*gxyaspect / (x0-x1);
            ytex.d = 0;
            otex.d = ryp0*gxyaspect - xtex.d*x0;
        
            xtex.u = ytex.u = otex.u = 0;
            xtex.v = ytex.v = otex.v = 0;
            polymost_domost(x0, fy0, x1, fy1);
        }
        else if (!(globalorientation&1))
        {
            int32_t fz = getflorzofslope(sectnum, globalposx, globalposy);
            if (globalposz <= fz)
                polymost_internal_nonparallaxed(n0, n1, ryp0, ryp1, x0, x1, fy0, fy1, sectnum | MAXSECTORS);
        }
        else if ((nextsectnum < 0) || (!(sector[nextsectnum].floorstat&1)))
        {
			calc_and_apply_fog_factor(sec->floorshade, sec->visibility, sec->floorpal, 0.005f);

            globvis2 = globalpisibility;
            if (sec->visibility != 0)
                globvis2 = mulscale4(globvis2, (uint8_t)(sec->visibility + 16));
            float viscale = xdimscale*fxdimen*(.0000001f/256.f);
			GLInterface.SetVisibility(globvis2*viscale, fviewingrange);

            //Use clamping for tiled sky textures
            //(don't wrap around edges if the sky use multiple panels)
            for (bssize_t i=(1<<dapskybits)-1; i>0; i--)
                if (dapskyoff[i] != dapskyoff[i-1])
                    { skyclamphack = r_parallaxskyclamping; break; }

            skyzbufferhack = 1;

            if (!usehightile || !hicfindskybox(globalpicnum, globalpal))
            {
                float const ghorizbak = ghoriz;
				pow2xsplit = 0;
                skyclamphack = 0;
                flatskyrender = 1;
                globalshade += globvis2*xdimscale*fviewingrange*(1.f / (64.f * 65536.f * 256.f * 1024.f));
				GLInterface.SetVisibility(0.f, fviewingrange);
                polymost_domost(x0,fy0,x1,fy1);
                flatskyrender = 0;
                ghoriz = ghorizbak;
            }
            else  //NOTE: code copied from ceiling code... lots of duplicated stuff :/
            {
                //Skybox code for parallax floor!
                float sky_t0, sky_t1; // _nx0, _ny0, _nx1, _ny1;
                float sky_ryp0, sky_ryp1, sky_x0, sky_x1, sky_cy0, sky_fy0, sky_cy1, sky_fy1, sky_ox0, sky_ox1;
                static vec2f_t const skywal[4] = { { -512, -512 }, { 512, -512 }, { 512, 512 }, { -512, 512 } };

                pow2xsplit = 0;
                skyclamphack = 1;

                for (bssize_t i=0; i<4; i++)
                {
                    walpos = skywal[i&3];
                    vec2f_t skyp0 = { walpos.y * gcosang - walpos.x * gsinang,
                                      walpos.x * gcosang2 + walpos.y * gsinang2 };

                    walpos = skywal[(i + 1) & 3];
                    vec2f_t skyp1 = { walpos.y * gcosang - walpos.x * gsinang,
                                      walpos.x * gcosang2 + walpos.y * gsinang2 };

                    vec2f_t const oskyp0 = skyp0;

                    //Clip to close parallel-screen plane
                    if (skyp0.y < SCISDIST)
                    {
                        if (skyp1.y < SCISDIST) continue;
                        sky_t0 = (SCISDIST - skyp0.y) / (skyp1.y - skyp0.y);
                        skyp0  = { (skyp1.x - skyp0.x) * sky_t0 + skyp0.x, SCISDIST };
                    }
                    else { sky_t0 = 0.f; }

                    if (skyp1.y < SCISDIST)
                    {
                        sky_t1  = (SCISDIST - oskyp0.y) / (skyp1.y - oskyp0.y);
                        skyp1 = { (skyp1.x - oskyp0.x) * sky_t1 + oskyp0.x, SCISDIST };
                    }
                    else { sky_t1 = 1.f; }

                    sky_ryp0 = 1.f/skyp0.y; sky_ryp1 = 1.f/skyp1.y;

                    //Generate screen coordinates for front side of wall
                    sky_x0 = ghalfx*skyp0.x*sky_ryp0 + ghalfx;
                    sky_x1 = ghalfx*skyp1.x*sky_ryp1 + ghalfx;
                    if ((sky_x1 <= sky_x0) || (sky_x0 >= x1) || (x0 >= sky_x1)) continue;

                    sky_ryp0 *= gyxscale; sky_ryp1 *= gyxscale;

                    sky_cy0 = -8192.f*sky_ryp0 + ghoriz;
                    sky_fy0 =  8192.f*sky_ryp0 + ghoriz;
                    sky_cy1 = -8192.f*sky_ryp1 + ghoriz;
                    sky_fy1 =  8192.f*sky_ryp1 + ghoriz;

                    sky_ox0 = sky_x0; sky_ox1 = sky_x1;

                    //Make sure: x0<=_x0<_x1<=x1
                    float nfy[2] = { fy0, fy1 };

                    if (sky_x0 < x0)
                    {
                        float const t = (x0-sky_x0)/(sky_x1-sky_x0);
                        sky_cy0 += (sky_cy1-sky_cy0)*t;
                        sky_fy0 += (sky_fy1-sky_fy0)*t;
                        sky_x0 = x0;
                    }
                    else if (sky_x0 > x0) nfy[0] += (sky_x0-x0)*(fy1-fy0)/(x1-x0);

                    if (sky_x1 > x1)
                    {
                        float const t = (x1-sky_x1)/(sky_x1-sky_x0);
                        sky_cy1 += (sky_cy1-sky_cy0)*t;
                        sky_fy1 += (sky_fy1-sky_fy0)*t;
                        sky_x1 = x1;
                    }
                    else if (sky_x1 < x1) nfy[1] += (sky_x1-x1)*(fy1-fy0)/(x1-x0);

                    //   (skybox floor)
                    //(_x0,_fy0)-(_x1,_fy1)
                    //   (skybox wall)
                    //(_x0,_cy0)-(_x1,_cy1)
                    //   (skybox ceiling)
                    //(_x0,nfy0)-(_x1,nfy1)

                    //floor of skybox
                    drawingskybox = 6; //floor/6th texture/index 5 of skybox
                    float const ft[4] = { 512 / 16, 512 / -16, fcosglobalang * (1.f / 2147483648.f),
                                          fsinglobalang * (1.f / 2147483648.f) };

                    xtex.d = 0;
                    ytex.d = gxyaspect*(1.0/4194304.0);
                    otex.d = -ghoriz*ytex.d;
                    xtex.u = ft[3]*fviewingrange*(-1.0/65536.0);
                    xtex.v = ft[2]*fviewingrange*(-1.0/65536.0);
                    ytex.u = ft[0]*ytex.d; ytex.v = ft[1]*ytex.d;
                    otex.u = ft[0]*otex.d; otex.v = ft[1]*otex.d;
                    otex.u += (ft[2]-xtex.u)*ghalfx;
                    otex.v -= (ft[3]+xtex.v)*ghalfx;
                    xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; //y-flip skybox floor

                    if ((sky_fy0 > nfy[0]) && (sky_fy1 > nfy[1]))
                        polymost_domost(sky_x0,sky_fy0,sky_x1,sky_fy1);
                    else if ((sky_fy0 > nfy[0]) != (sky_fy1 > nfy[1]))
                    {
                        //(ox,oy) is intersection of: (_x0,_fy0)-(_x1,_fy1)
                        //                            (_x0,nfy0)-(_x1,nfy1)
                        float const t = (sky_fy0-nfy[0])/(nfy[1]-nfy[0]-sky_fy1+sky_fy0);
                        vec2f_t const o = { sky_x0 + (sky_x1-sky_x0)*t, sky_fy0 + (sky_fy1-sky_fy0)*t };
                        if (nfy[0] > sky_fy0)
                        {
                            polymost_domost(sky_x0,nfy[0],o.x,o.y);
                            polymost_domost(o.x,o.y,sky_x1,sky_fy1);
                        }
                        else
                        {
                            polymost_domost(sky_x0,sky_fy0,o.x,o.y);
                            polymost_domost(o.x,o.y,sky_x1,nfy[1]);
                        }
                    }
                    else
                        polymost_domost(sky_x0,nfy[0],sky_x1,nfy[1]);

                    //wall of skybox
                    drawingskybox = i+1; //i+1th texture/index i of skybox
                    xtex.d = (sky_ryp0-sky_ryp1)*gxyaspect*(1.0/512.0) / (sky_ox0-sky_ox1);
                    ytex.d = 0;
                    otex.d = sky_ryp0*gxyaspect*(1.0/512.0) - xtex.d*sky_ox0;
                    xtex.u = (sky_t0*sky_ryp0 - sky_t1*sky_ryp1)*gxyaspect*(64.0/512.0) / (sky_ox0-sky_ox1);
                    otex.u = sky_t0*sky_ryp0*gxyaspect*(64.0/512.0) - xtex.u*sky_ox0;
                    ytex.u = 0;
                    sky_t0 = -8192.f*sky_ryp0 + ghoriz;
                    sky_t1 = -8192.f*sky_ryp1 + ghoriz;
                    float const t = ((xtex.d*sky_ox0 + otex.d)*8.f) / ((sky_ox1-sky_ox0) * sky_ryp0 * 2048.f);
                    xtex.v = (sky_t0-sky_t1)*t;
                    ytex.v = (sky_ox1-sky_ox0)*t;
                    otex.v = -xtex.v*sky_ox0 - ytex.v*sky_t0;

                    if ((sky_cy0 > nfy[0]) && (sky_cy1 > nfy[1]))
                        polymost_domost(sky_x0,sky_cy0,sky_x1,sky_cy1);
                    else if ((sky_cy0 > nfy[0]) != (sky_cy1 > nfy[1]))
                    {
                        //(ox,oy) is intersection of: (_x0,_fy0)-(_x1,_fy1)
                        //                            (_x0,nfy0)-(_x1,nfy1)
                        float const t = (sky_cy0-nfy[0])/(nfy[1]-nfy[0]-sky_cy1+sky_cy0);
                        vec2f_t const o = { sky_x0 + (sky_x1 - sky_x0) * t, sky_cy0 + (sky_cy1 - sky_cy0) * t };
                        if (nfy[0] > sky_cy0)
                        {
                            polymost_domost(sky_x0,nfy[0],o.x,o.y);
                            polymost_domost(o.x,o.y,sky_x1,sky_cy1);
                        }
                        else
                        {
                            polymost_domost(sky_x0,sky_cy0,o.x,o.y);
                            polymost_domost(o.x,o.y,sky_x1,nfy[1]);
                        }
                    }
                    else
                        polymost_domost(sky_x0,nfy[0],sky_x1,nfy[1]);
                }

                //Ceiling of skybox
                drawingskybox = 5; //ceiling/5th texture/index 4 of skybox
                float const ft[4] = { 512 / 16, -512 / -16, fcosglobalang * (1.f / 2147483648.f),
                                      fsinglobalang * (1.f / 2147483648.f) };

                xtex.d = 0;
                ytex.d = gxyaspect*(-1.0/4194304.0);
                otex.d = -ghoriz*ytex.d;
                xtex.u = ft[3]*fviewingrange*(-1.0/65536.0);
                xtex.v = ft[2]*fviewingrange*(-1.0/65536.0);
                ytex.u = ft[0]*ytex.d; ytex.v = ft[1]*ytex.d;
                otex.u = ft[0]*otex.d; otex.v = ft[1]*otex.d;
                otex.u += (ft[2]-xtex.u)*ghalfx;
                otex.v -= (ft[3]+xtex.v)*ghalfx;

                polymost_domost(x0,fy0,x1,fy1);

                skyclamphack = 0;
                drawingskybox = 0;
            }

            skyclamphack = 0;
            skyzbufferhack = 0;
            if (!nofog)
				GLInterface.SetFogEnabled(true);
        }

        // Ceiling

#ifdef YAX_ENABLE
        yax_holencf[YAX_CEILING] = 0;
        yax_drawcf = YAX_CEILING;
#endif

        globalpicnum = sec->ceilingpicnum;
        globalshade = sec->ceilingshade;
        globalpal = sec->ceilingpal;
        globalorientation = sec->ceilingstat;
        globvis = (sector[sectnum].visibility != 0) ?
                  mulscale4(globalcisibility, (uint8_t)(sector[sectnum].visibility + 16)) :
                  globalcisibility;
        globvis2 = (sector[sectnum].visibility != 0) ?
                  mulscale4(globalcisibility2, (uint8_t)(sector[sectnum].visibility + 16)) :
                  globalcisibility2;
		GLInterface.SetVisibility(globvis2, fviewingrange);

        tileUpdatePicnum(&globalpicnum, sectnum);


        dapskyoff = getpsky(globalpicnum, &dapyscale, &dapskybits, &dapyoffs, &daptileyscale);

        global_cf_fogpal = sec->fogpal;
        global_cf_shade = sec->ceilingshade, global_cf_pal = sec->ceilingpal; global_cf_z = sec->ceilingz;  // REFACT
        global_cf_xpanning = sec->ceilingxpanning; global_cf_ypanning = sec->ceilingypanning, global_cf_heinum = sec->ceilingheinum;
        global_getzofslope_func = &fgetceilzofslope;
        
        if (globalpicnum >= r_rortexture && globalpicnum < r_rortexture + r_rortexturerange && r_rorphase == 0)
        {
            xtex.d = (ryp0-ryp1)*gxyaspect / (x0-x1);
            ytex.d = 0;
            otex.d = ryp0*gxyaspect - xtex.d*x0;
        
            xtex.u = ytex.u = otex.u = 0;
            xtex.v = ytex.v = otex.v = 0;
            polymost_domost(x1, cy1, x0, cy0);
        }
        else if (!(globalorientation&1))
        {
            int32_t cz = getceilzofslope(sectnum, globalposx, globalposy);
            if (globalposz >= cz)
                polymost_internal_nonparallaxed(n0, n1, ryp0, ryp1, x0, x1, cy0, cy1, sectnum);
        }
        else if ((nextsectnum < 0) || (!(sector[nextsectnum].ceilingstat&1)))
        {
			calc_and_apply_fog_factor(sec->ceilingshade, sec->visibility, sec->ceilingpal, 0.005f);

            globvis2 = globalpisibility;
            if (sec->visibility != 0)
                globvis2 = mulscale4(globvis2, (uint8_t)(sec->visibility + 16));
            float viscale = xdimscale*fxdimen*(.0000001f/256.f);
			GLInterface.SetVisibility(globvis2*viscale, fviewingrange);

            //Use clamping for tiled sky textures
            //(don't wrap around edges if the sky use multiple panels)
            for (bssize_t i=(1<<dapskybits)-1; i>0; i--)
                if (dapskyoff[i] != dapskyoff[i-1])
                    { skyclamphack = r_parallaxskyclamping; break; }

            skyzbufferhack = 1;

			if (!usehightile || !hicfindskybox(globalpicnum, globalpal))
			{
				float const ghorizbak = ghoriz;
				pow2xsplit = 0;
				skyclamphack = 0;
				flatskyrender = 1;
				globalshade += globvis2 * xdimscale * fviewingrange * (1.f / (64.f * 65536.f * 256.f * 1024.f));
				GLInterface.SetVisibility(0.f, fviewingrange);
				polymost_domost(x1, cy1, x0, cy0);
				flatskyrender = 0;
                ghoriz = ghorizbak;
			}
            else
            {
                //Skybox code for parallax ceiling!
                float sky_t0, sky_t1; // _nx0, _ny0, _nx1, _ny1;
                float sky_ryp0, sky_ryp1, sky_x0, sky_x1, sky_cy0, sky_fy0, sky_cy1, sky_fy1, sky_ox0, sky_ox1;
                static vec2f_t const skywal[4] = { { -512, -512 }, { 512, -512 }, { 512, 512 }, { -512, 512 } };

                pow2xsplit = 0;
                skyclamphack = 1;

                for (bssize_t i=0; i<4; i++)
                {
                    walpos = skywal[i&3];
                    vec2f_t skyp0 = { walpos.y * gcosang - walpos.x * gsinang,
                                      walpos.x * gcosang2 + walpos.y * gsinang2 };

                    walpos = skywal[(i + 1) & 3];
                    vec2f_t skyp1 = { walpos.y * gcosang - walpos.x * gsinang,
                                      walpos.x * gcosang2 + walpos.y * gsinang2 };

                    vec2f_t const oskyp0 = skyp0;

                    //Clip to close parallel-screen plane
                    if (skyp0.y < SCISDIST)
                    {
                        if (skyp1.y < SCISDIST) continue;
                        sky_t0 = (SCISDIST - skyp0.y) / (skyp1.y - skyp0.y);
                        skyp0  = { (skyp1.x - skyp0.x) * sky_t0 + skyp0.x, SCISDIST };
                    }
                    else { sky_t0 = 0.f; }

                    if (skyp1.y < SCISDIST)
                    {
                        sky_t1 = (SCISDIST - oskyp0.y) / (skyp1.y - oskyp0.y);
                        skyp1  = { (skyp1.x - oskyp0.x) * sky_t1 + oskyp0.x, SCISDIST };
                    }
                    else { sky_t1 = 1.f; }

                    sky_ryp0 = 1.f/skyp0.y; sky_ryp1 = 1.f/skyp1.y;

                    //Generate screen coordinates for front side of wall
                    sky_x0 = ghalfx*skyp0.x*sky_ryp0 + ghalfx;
                    sky_x1 = ghalfx*skyp1.x*sky_ryp1 + ghalfx;
                    if ((sky_x1 <= sky_x0) || (sky_x0 >= x1) || (x0 >= sky_x1)) continue;

                    sky_ryp0 *= gyxscale; sky_ryp1 *= gyxscale;

                    sky_cy0 = -8192.f*sky_ryp0 + ghoriz;
                    sky_fy0 =  8192.f*sky_ryp0 + ghoriz;
                    sky_cy1 = -8192.f*sky_ryp1 + ghoriz;
                    sky_fy1 =  8192.f*sky_ryp1 + ghoriz;

                    sky_ox0 = sky_x0; sky_ox1 = sky_x1;

                    //Make sure: x0<=_x0<_x1<=x1
                    float ncy[2] = { cy0, cy1 };

                    if (sky_x0 < x0)
                    {
                        float const t = (x0-sky_x0)/(sky_x1-sky_x0);
                        sky_cy0 += (sky_cy1-sky_cy0)*t;
                        sky_fy0 += (sky_fy1-sky_fy0)*t;
                        sky_x0 = x0;
                    }
                    else if (sky_x0 > x0) ncy[0] += (sky_x0-x0)*(cy1-cy0)/(x1-x0);

                    if (sky_x1 > x1)
                    {
                        float const t = (x1-sky_x1)/(sky_x1-sky_x0);
                        sky_cy1 += (sky_cy1-sky_cy0)*t;
                        sky_fy1 += (sky_fy1-sky_fy0)*t;
                        sky_x1 = x1;
                    }
                    else if (sky_x1 < x1) ncy[1] += (sky_x1-x1)*(cy1-cy0)/(x1-x0);

                    //   (skybox ceiling)
                    //(_x0,_cy0)-(_x1,_cy1)
                    //   (skybox wall)
                    //(_x0,_fy0)-(_x1,_fy1)
                    //   (skybox floor)
                    //(_x0,ncy0)-(_x1,ncy1)

                    //ceiling of skybox
                    drawingskybox = 5; //ceiling/5th texture/index 4 of skybox
                    float const ft[4] = { 512 / 16, -512 / -16, fcosglobalang * (1.f / 2147483648.f),
                                          fsinglobalang * (1.f / 2147483648.f) };

                    xtex.d = 0;
                    ytex.d = gxyaspect*(-1.0/4194304.0);
                    otex.d = -ghoriz*ytex.d;
                    xtex.u = ft[3]*fviewingrange*(-1.0/65536.0);
                    xtex.v = ft[2]*fviewingrange*(-1.0/65536.0);
                    ytex.u = ft[0]*ytex.d; ytex.v = ft[1]*ytex.d;
                    otex.u = ft[0]*otex.d; otex.v = ft[1]*otex.d;
                    otex.u += (ft[2]-xtex.u)*ghalfx;
                    otex.v -= (ft[3]+xtex.v)*ghalfx;


                    if ((sky_cy0 < ncy[0]) && (sky_cy1 < ncy[1]))
                        polymost_domost(sky_x1,sky_cy1,sky_x0,sky_cy0);
                    else if ((sky_cy0 < ncy[0]) != (sky_cy1 < ncy[1]))
                    {
                        //(ox,oy) is intersection of: (_x0,_cy0)-(_x1,_cy1)
                        //                            (_x0,ncy0)-(_x1,ncy1)
                        float const t = (sky_cy0-ncy[0])/(ncy[1]-ncy[0]-sky_cy1+sky_cy0);
                        vec2f_t const o = { sky_x0 + (sky_x1-sky_x0)*t, sky_cy0 + (sky_cy1-sky_cy0)*t };
                        if (ncy[0] < sky_cy0)
                        {
                            polymost_domost(o.x,o.y,sky_x0,ncy[0]);
                            polymost_domost(sky_x1,sky_cy1,o.x,o.y);
                        }
                        else
                        {
                            polymost_domost(o.x,o.y,sky_x0,sky_cy0);
                            polymost_domost(sky_x1,ncy[1],o.x,o.y);
                        }
                    }
                    else
                        polymost_domost(sky_x1,ncy[1],sky_x0,ncy[0]);

                    //wall of skybox
                    drawingskybox = i+1; //i+1th texture/index i of skybox
                    xtex.d = (sky_ryp0-sky_ryp1)*gxyaspect*(1.0/512.0) / (sky_ox0-sky_ox1);
                    ytex.d = 0;
                    otex.d = sky_ryp0*gxyaspect*(1.0/512.0) - xtex.d*sky_ox0;
                    xtex.u = (sky_t0*sky_ryp0 - sky_t1*sky_ryp1)*gxyaspect*(64.0/512.0) / (sky_ox0-sky_ox1);
                    otex.u = sky_t0*sky_ryp0*gxyaspect*(64.0/512.0) - xtex.u*sky_ox0;
                    ytex.u = 0;
                    sky_t0 = -8192.f*sky_ryp0 + ghoriz;
                    sky_t1 = -8192.f*sky_ryp1 + ghoriz;
                    float const t = ((xtex.d*sky_ox0 + otex.d)*8.f) / ((sky_ox1-sky_ox0) * sky_ryp0 * 2048.f);
                    xtex.v = (sky_t0-sky_t1)*t;
                    ytex.v = (sky_ox1-sky_ox0)*t;
                    otex.v = -xtex.v*sky_ox0 - ytex.v*sky_t0;

                    if ((sky_fy0 < ncy[0]) && (sky_fy1 < ncy[1]))
                        polymost_domost(sky_x1,sky_fy1,sky_x0,sky_fy0);
                    else if ((sky_fy0 < ncy[0]) != (sky_fy1 < ncy[1]))
                    {
                        //(ox,oy) is intersection of: (_x0,_fy0)-(_x1,_fy1)
                        //                            (_x0,ncy0)-(_x1,ncy1)
                        float const t = (sky_fy0-ncy[0])/(ncy[1]-ncy[0]-sky_fy1+sky_fy0);
                        vec2f_t const o = { sky_x0 + (sky_x1 - sky_x0) * t, sky_fy0 + (sky_fy1 - sky_fy0) * t };
                        if (ncy[0] < sky_fy0)
                        {
                            polymost_domost(o.x,o.y,sky_x0,ncy[0]);
                            polymost_domost(sky_x1,sky_fy1,o.x,o.y);
                        }
                        else
                        {
                            polymost_domost(o.x,o.y,sky_x0,sky_fy0);
                            polymost_domost(sky_x1,ncy[1],o.x,o.y);
                        }
                    }
                    else
                        polymost_domost(sky_x1,ncy[1],sky_x0,ncy[0]);
                }

                //Floor of skybox
                drawingskybox = 6; //floor/6th texture/index 5 of skybox
                float const ft[4] = { 512 / 16, 512 / -16, fcosglobalang * (1.f / 2147483648.f),
                                      fsinglobalang * (1.f / 2147483648.f) };

                xtex.d = 0;
                ytex.d = gxyaspect*(1.0/4194304.0);
                otex.d = -ghoriz*ytex.d;
                xtex.u = ft[3]*fviewingrange*(-1.0/65536.0);
                xtex.v = ft[2]*fviewingrange*(-1.0/65536.0);
                ytex.u = ft[0]*ytex.d; ytex.v = ft[1]*ytex.d;
                otex.u = ft[0]*otex.d; otex.v = ft[1]*otex.d;
                otex.u += (ft[2]-xtex.u)*ghalfx;
                otex.v -= (ft[3]+xtex.v)*ghalfx;
                xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; //y-flip skybox floor
                polymost_domost(x1,cy1,x0,cy0);

                skyclamphack = 0;
                drawingskybox = 0;
            }

            skyclamphack = 0;
            skyzbufferhack = 0;
            if (!nofog)
				GLInterface.SetFogEnabled(true);
        }

#ifdef YAX_ENABLE
        if (g_nodraw)
        {
            int32_t baselevp, checkcf, i, j;
            int16_t bn[2];
            baselevp = (yax_globallev == YAX_MAXDRAWS);

            yax_getbunches(sectnum, &bn[0], &bn[1]);
            checkcf = (bn[0]>=0) + ((bn[1]>=0)<<1);
            if (!baselevp)
                checkcf &= (1<<yax_globalcf);

            for (i=0; i<2; i++)
                if (checkcf&(1<<i))
                {
                    if ((haveymost[bn[i]>>3]&pow2char[bn[i]&7])==0)
                    {
                        // init yax *most arrays for that bunch
                        haveymost[bn[i]>>3] |= pow2char[bn[i]&7];
                        yax_vsp[bn[i]*2][1].x = xbl;
                        yax_vsp[bn[i]*2][2].x = xbr;
                        yax_vsp[bn[i]*2][1].cy[0] = xbb;
                        yax_vsp[bn[i]*2][2].cy[0] = xbb;
                        yax_vsp_finalize_init(bn[i]*2, 3);
                        yax_vsp[bn[i]*2+1][1].x = xbl;
                        yax_vsp[bn[i]*2+1][2].x = xbr;
                        yax_vsp[bn[i]*2+1][1].cy[0] = xbt;
                        yax_vsp[bn[i]*2+1][2].cy[0] = xbt;
                        yax_vsp_finalize_init(bn[i]*2+1, 3);
                    }

                    for (j = 0; j < yax_holencf[i]; j++)
                    {
                        yax_hole_t *hole = &yax_holecf[i][j];
                        yax_polymost_domost(bn[i]*2, hole->x0, hole->cy[0], hole->x1, hole->cy[1]);
                        yax_polymost_domost(bn[i]*2+1, hole->x1, hole->fy[1], hole->x0, hole->fy[0]);
                    }
                }
        }
#endif

        // Wall

#ifdef YAX_ENABLE
        yax_drawcf = -1;
#endif

        xtex.d = (ryp0-ryp1)*gxyaspect / (x0-x1);
        ytex.d = 0;
        otex.d = ryp0*gxyaspect - xtex.d*x0;

        xtex.u = (t0*ryp0 - t1*ryp1)*gxyaspect*(float)wal->xrepeat*8.f / (x0-x1);
        otex.u = t0*ryp0*gxyaspect*wal->xrepeat*8.0 - xtex.u*x0;
        otex.u += (float)wal->xpanning*otex.d;
        xtex.u += (float)wal->xpanning*xtex.d;
        ytex.u = 0;

        float const ogux = xtex.u, oguy = ytex.u, oguo = otex.u;

        Bassert(domostpolymethod == DAMETH_NOMASK);
        domostpolymethod = DAMETH_WALL;

#ifdef YAX_ENABLE
        if (yax_nomaskpass==0 || !yax_isislandwall(wallnum, !yax_globalcf) || (yax_nomaskdidit=1, 0))
#endif
        if (nextsectnum >= 0)
        {
            fgetzsofslope((usectorptr_t)&sector[nextsectnum],n0.x,n0.y,&cz,&fz);
            float const ocy0 = (cz-globalposz)*ryp0 + ghoriz;
            float const ofy0 = (fz-globalposz)*ryp0 + ghoriz;
            fgetzsofslope((usectorptr_t)&sector[nextsectnum],n1.x,n1.y,&cz,&fz);
            float const ocy1 = (cz-globalposz)*ryp1 + ghoriz;
            float const ofy1 = (fz-globalposz)*ryp1 + ghoriz;

            if ((wal->cstat&48) == 16) maskwall[maskwallcnt++] = z;

            if (((cy0 < ocy0) || (cy1 < ocy1)) && (!((sec->ceilingstat&sector[nextsectnum].ceilingstat)&1)))
            {
                globalpicnum = wal->picnum; globalshade = wal->shade; globalpal = (int32_t)((uint8_t)wal->pal);
                globvis = globalvisibility;
                if (sector[sectnum].visibility != 0) globvis = mulscale4(globvis, (uint8_t)(sector[sectnum].visibility+16));
                globvis2 = globalvisibility2;
                if (sector[sectnum].visibility != 0) globvis2 = mulscale4(globvis2, (uint8_t)(sector[sectnum].visibility+16));
				GLInterface.SetVisibility(globvis2, fviewingrange);
                globalorientation = wal->cstat;
                tileUpdatePicnum(&globalpicnum, wallnum+16384);

                int i = (!(wal->cstat&4)) ? sector[nextsectnum].ceilingz : sec->ceilingz;

                // over
                calc_ypanning(i, ryp0, ryp1, x0, x1, wal->ypanning, wal->yrepeat, wal->cstat&4, tilesiz[globalpicnum]);

                if (wal->cstat&8) //xflip
                {
                    float const t = (float)(wal->xrepeat*8 + wal->xpanning*2);
                    xtex.u = xtex.d*t - xtex.u;
                    ytex.u = ytex.d*t - ytex.u;
                    otex.u = otex.d*t - otex.u;
                }
                if (wal->cstat&256) { xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; } //yflip

				calc_and_apply_fog(fogshade(wal->shade, wal->pal), sec->visibility, get_floor_fogpal(sec));

                pow2xsplit = 1;
#ifdef YAX_ENABLE
                if (should_clip_cfwall(x1,cy1,x0,cy0))
#endif
                polymost_domost(x1,ocy1,x0,ocy0,cy1,ocy1,cy0,ocy0);
                if (wal->cstat&8) { xtex.u = ogux; ytex.u = oguy; otex.u = oguo; }
            }
            if (((ofy0 < fy0) || (ofy1 < fy1)) && (!((sec->floorstat&sector[nextsectnum].floorstat)&1)))
            {
                uwallptr_t nwal;

                if (!(wal->cstat&2)) nwal = wal;
                else
                {
                    nwal = (uwallptr_t)&wall[wal->nextwall];
                    otex.u += (float)(nwal->xpanning - wal->xpanning) * otex.d;
                    xtex.u += (float)(nwal->xpanning - wal->xpanning) * xtex.d;
                    ytex.u += (float)(nwal->xpanning - wal->xpanning) * ytex.d;
                }
                globalpicnum = nwal->picnum; globalshade = nwal->shade; globalpal = (int32_t)((uint8_t)nwal->pal);
                globvis = globalvisibility;
                if (sector[sectnum].visibility != 0) globvis = mulscale4(globvis, (uint8_t)(sector[sectnum].visibility+16));
                globvis2 = globalvisibility2;
                if (sector[sectnum].visibility != 0) globvis2 = mulscale4(globvis2, (uint8_t)(sector[sectnum].visibility+16));
				GLInterface.SetVisibility(globvis2, fviewingrange);
                globalorientation = nwal->cstat;
                tileUpdatePicnum(&globalpicnum, wallnum+16384);

                int i = (!(nwal->cstat&4)) ? sector[nextsectnum].floorz : sec->ceilingz;

                // under
                calc_ypanning(i, ryp0, ryp1, x0, x1, nwal->ypanning, wal->yrepeat, !(nwal->cstat&4), tilesiz[globalpicnum]);

                if (wal->cstat&8) //xflip
                {
                    float const t = (float)(wal->xrepeat*8 + nwal->xpanning*2);
                    xtex.u = xtex.d*t - xtex.u;
                    ytex.u = ytex.d*t - ytex.u;
                    otex.u = otex.d*t - otex.u;
                }
                if (nwal->cstat&256) { xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; } //yflip

				calc_and_apply_fog(fogshade(nwal->shade, nwal->pal), sec->visibility, get_floor_fogpal(sec));

                pow2xsplit = 1;
#ifdef YAX_ENABLE
                if (should_clip_cfwall(x0,fy0,x1,fy1))
#endif
                polymost_domost(x0,ofy0,x1,ofy1,ofy0,fy0,ofy1,fy1);
                if (wal->cstat&(2+8)) { otex.u = oguo; xtex.u = ogux; ytex.u = oguy; }
            }
        }

        if ((nextsectnum < 0) || (wal->cstat&32))   //White/1-way wall
        {
            do
            {
                const int maskingOneWay = (nextsectnum >= 0 && (wal->cstat&32));

                if (maskingOneWay)
                {
                    vec2_t n, pos = { globalposx, globalposy };
                    if (!polymost_getclosestpointonwall(&pos, wallnum, &n) && klabs(pos.x - n.x) + klabs(pos.y - n.y) <= 128)
                        break;
                }

                globalpicnum = (nextsectnum < 0) ? wal->picnum : wal->overpicnum;

                globalshade = wal->shade;
                globalpal = wal->pal;
                globvis = (sector[sectnum].visibility != 0) ?
                          mulscale4(globalvisibility, (uint8_t)(sector[sectnum].visibility + 16)) :
                          globalvisibility;
                globvis2 = (sector[sectnum].visibility != 0) ?
                          mulscale4(globalvisibility2, (uint8_t)(sector[sectnum].visibility + 16)) :
                          globalvisibility2;
				GLInterface.SetVisibility(globvis2, fviewingrange);
                globalorientation = wal->cstat;
                tileUpdatePicnum(&globalpicnum, wallnum+16384);

                int i;
                int const nwcs4 = !(wal->cstat & 4);

                if (nextsectnum >= 0) { i = nwcs4 ? nextsec->ceilingz : sec->ceilingz; }
                else { i = nwcs4 ? sec->ceilingz : sec->floorz; }

                // white / 1-way
                calc_ypanning(i, ryp0, ryp1, x0, x1, wal->ypanning, wal->yrepeat, nwcs4 && !maskingOneWay, tilesiz[globalpicnum]);

                if (wal->cstat&8) //xflip
                {
                    float const t = (float) (wal->xrepeat*8 + wal->xpanning*2);
                    xtex.u = xtex.d*t - xtex.u;
                    ytex.u = ytex.d*t - ytex.u;
                    otex.u = otex.d*t - otex.u;
                }
                if (wal->cstat&256) { xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; } //yflip

				calc_and_apply_fog(fogshade(wal->shade, wal->pal), sec->visibility, get_floor_fogpal(sec));

                pow2xsplit = 1;

#ifdef YAX_ENABLE
                // TODO: slopes?

                if (globalposz > sec->floorz && yax_isislandwall(wallnum, YAX_FLOOR))
                    polymost_domost(x1, fy1, x0, fy0, cy1, fy1, cy0, fy0);
                else
#endif
                    polymost_domost(x0, cy0, x1, cy1, cy0, fy0, cy1, fy1);
            } while (0);
        }

        domostpolymethod = DAMETH_NOMASK;

        if (nextsectnum >= 0)
            if ((!(gotsector[nextsectnum>>3]&pow2char[nextsectnum&7])) && testvisiblemost(x0,x1))
                polymost_scansector(nextsectnum);
    }
}

static int32_t polymost_bunchfront(const int32_t b1, const int32_t b2)
{
    int b1f = bunchfirst[b1];
    const double x2b2 = dxb2[bunchlast[b2]];
    const double x1b1 = dxb1[b1f];

    if (nexttowardf(x1b1, x2b2) >= x2b2)
        return -1;

    int b2f = bunchfirst[b2];
    const double x1b2 = dxb1[b2f];

    if (nexttowardf(x1b2, dxb2[bunchlast[b1]]) >= dxb2[bunchlast[b1]])
        return -1;

    if (nexttowardf(x1b1, x1b2) > x1b2)
    {
        while (nexttowardf(dxb2[b2f], x1b1) <= x1b1) b2f=bunchp2[b2f];
        return wallfront(b1f, b2f);
    }

    while (nexttowardf(dxb2[b1f], x1b2) <= x1b2) b1f=bunchp2[b1f];
    return wallfront(b1f, b2f);
}

void polymost_scansector(int32_t sectnum)
{
    if (sectnum < 0) return;

    if (automapping)
        show2dsector[sectnum>>3] |= pow2char[sectnum&7];

    sectorborder[0] = sectnum;
    int sectorbordercnt = 1;
    do
    {
        sectnum = sectorborder[--sectorbordercnt];

#ifdef YAX_ENABLE
        if (scansector_collectsprites)
#endif
        for (bssize_t z=headspritesect[sectnum]; z>=0; z=nextspritesect[z])
        {
            auto const spr = (uspriteptr_t)&sprite[z];

            if ((spr->cstat & 0x8000 && !showinvisibility) || spr->xrepeat == 0 || spr->yrepeat == 0)
                continue;

            vec2_t const s = { spr->x-globalposx, spr->y-globalposy };

            if ((spr->cstat&48) ||
                (usemodels && tile2model[spr->picnum].modelid>=0) ||
                ((s.x * gcosang) + (s.y * gsinang) > 0))
            {
                if ((spr->cstat&(64+48))!=(64+16) ||
                    (usevoxels && tiletovox[spr->picnum] >= 0 && voxmodels[tiletovox[spr->picnum]]) ||
                    dmulscale6(sintable[(spr->ang+512)&2047],-s.x, sintable[spr->ang&2047],-s.y) > 0)
                    if (renderAddTsprite(z, sectnum))
                        break;
            }
        }

        gotsector[sectnum>>3] |= pow2char[sectnum&7];

        int const bunchfrst = numbunches;
        int const onumscans = numscans;
        int const startwall = sector[sectnum].wallptr;
        int const endwall   = sector[sectnum].wallnum + startwall;

        int scanfirst = numscans;

        vec2d_t p2 = { 0, 0 };

        uwallptr_t wal;
        int z;

        for (z=startwall,wal=(uwallptr_t)&wall[z]; z<endwall; z++,wal++)
        {
            auto const wal2 = (uwallptr_t)&wall[wal->point2];

            vec2d_t const fp1 = { double(wal->x - globalposx), double(wal->y - globalposy) };
            vec2d_t const fp2 = { double(wal2->x - globalposx), double(wal2->y - globalposy) };

            int const nextsectnum = wal->nextsector; //Scan close sectors

            if (nextsectnum >= 0 && !(wal->cstat&32) && sectorbordercnt < ARRAY_SSIZE(sectorborder))
#ifdef YAX_ENABLE
            if (yax_nomaskpass==0 || !yax_isislandwall(z, !yax_globalcf) || (yax_nomaskdidit=1, 0))
#endif
            if ((gotsector[nextsectnum>>3]&pow2char[nextsectnum&7]) == 0)
            {
                double const d = fp1.x*fp2.y - fp2.x*fp1.y;
                vec2d_t const p1 = { fp2.x-fp1.x, fp2.y-fp1.y };

                // this said (SCISDIST*SCISDIST*260.f), but SCISDIST is 1 and the significance of 260 isn't obvious to me
                // is 260 fudged to solve a problem, and does the problem still apply to our version of the renderer?
                if (d*d < (p1.x*p1.x + p1.y*p1.y) * 256.f)
                {
                    sectorborder[sectorbordercnt++] = nextsectnum;
                    gotsector[nextsectnum>>3] |= pow2char[nextsectnum&7];
                }
            }

            vec2d_t p1;

            if ((z == startwall) || (wall[z-1].point2 != z))
            {
                p1 = { (((fp1.y * fcosglobalang) - (fp1.x * fsinglobalang)) * (1.0/64.0)),
                       (((fp1.x * cosviewingrangeglobalang) + (fp1.y * sinviewingrangeglobalang)) * (1.0/64.0)) };
            }
            else { p1 = p2; }

            p2 = { (((fp2.y * fcosglobalang) - (fp2.x * fsinglobalang)) * (1.0/64.0)),
                   (((fp2.x * cosviewingrangeglobalang) + (fp2.y * sinviewingrangeglobalang)) * (1.0/64.0)) };

            if (numscans >= MAXWALLSB-1)
            {
                OSD_Printf("!!numscans\n");
                return;
            }

            //if wall is facing you...
            if ((p1.y >= SCISDIST || p2.y >= SCISDIST) && (nexttoward(p1.x*p2.y, p2.x*p1.y) < p2.x*p1.y))
            {
                dxb1[numscans] = (p1.y >= SCISDIST) ? float(p1.x*ghalfx/p1.y + ghalfx) : -1e32f;
                dxb2[numscans] = (p2.y >= SCISDIST) ? float(p2.x*ghalfx/p2.y + ghalfx) : 1e32f;

                if (dxb1[numscans] < xbl)
                    dxb1[numscans] = xbl;
                else if (dxb1[numscans] > xbr)
                    dxb1[numscans] = xbr;
                if (dxb2[numscans] < xbl)
                    dxb2[numscans] = xbl;
                else if (dxb2[numscans] > xbr)
                    dxb2[numscans] = xbr;

                if (nexttowardf(dxb1[numscans], dxb2[numscans]) < dxb2[numscans])
                {
                    thesector[numscans] = sectnum;
                    thewall[numscans] = z;
                    bunchp2[numscans] = numscans + 1;
                    numscans++;
                }
            }

            if ((wall[z].point2 < z) && (scanfirst < numscans))
            {
                bunchp2[numscans-1] = scanfirst;
                scanfirst = numscans;
            }
        }

        for (bssize_t z=onumscans; z<numscans; z++)
        {
            if ((wall[thewall[z]].point2 != thewall[bunchp2[z]]) || (dxb2[z] > nexttowardf(dxb1[bunchp2[z]], dxb2[z])))
            {
                bunchfirst[numbunches++] = bunchp2[z];
                bunchp2[z] = -1;
#ifdef YAX_ENABLE
                if (scansector_retfast)
                    return;
#endif
            }
        }

        for (bssize_t z=bunchfrst; z<numbunches; z++)
        {
            int zz;
            for (zz=bunchfirst[z]; bunchp2[zz]>=0; zz=bunchp2[zz]) { }
            bunchlast[z] = zz;
        }
    }
    while (sectorbordercnt > 0);
}

/*Init viewport boundary (must be 4 point convex loop):
//      (px[0],py[0]).----.(px[1],py[1])
//                  /      \
//                /          \
// (px[3],py[3]).--------------.(px[2],py[2])
*/

static void polymost_initmosts(const float * px, const float * py, int const n)
{
    if (n < 3) return;

    int32_t imin = (px[1] < px[0]);

    for (bssize_t i=n-1; i>=2; i--)
        if (px[i] < px[imin]) imin = i;

    int32_t vcnt = 1; //0 is dummy solid node

    vsp[vcnt].x = px[imin];
    vsp[vcnt].cy[0] = vsp[vcnt].fy[0] = py[imin];
    vcnt++;

    int i = imin+1, j = imin-1;
    if (i >= n) i = 0;
    if (j < 0) j = n-1;

    do
    {
        if (px[i] < px[j])
        {
            if (px[i] <= vsp[vcnt-1].x) vcnt--;
            vsp[vcnt].x = px[i];
            vsp[vcnt].cy[0] = py[i];
            int k = j+1; if (k >= n) k = 0;
            //(px[k],py[k])
            //(px[i],?)
            //(px[j],py[j])
            vsp[vcnt].fy[0] = (px[i]-px[k])*(py[j]-py[k])/(px[j]-px[k]) + py[k];
            vcnt++;
            i++; if (i >= n) i = 0;
        }
        else if (px[j] < px[i])
        {
            if (px[j] <= vsp[vcnt-1].x) vcnt--;
            vsp[vcnt].x = px[j];
            vsp[vcnt].fy[0] = py[j];
            int k = i-1; if (k < 0) k = n-1;
            //(px[k],py[k])
            //(px[j],?)
            //(px[i],py[i])
            vsp[vcnt].cy[0] = (px[j]-px[k])*(py[i]-py[k])/(px[i]-px[k]) + py[k];
            vcnt++;
            j--; if (j < 0) j = n-1;
        }
        else
        {
            if (px[i] <= vsp[vcnt-1].x) vcnt--;
            vsp[vcnt].x = px[i];
            vsp[vcnt].cy[0] = py[i];
            vsp[vcnt].fy[0] = py[j];
            vcnt++;
            i++; if (i >= n) i = 0; if (i == j) break;
            j--; if (j < 0) j = n-1;
        }
    } while (i != j);

    if (px[i] > vsp[vcnt-1].x)
    {
        vsp[vcnt].x = px[i];
        vsp[vcnt].cy[0] = vsp[vcnt].fy[0] = py[i];
        vcnt++;
    }

    domost_rejectcount = 0;

    vsp_finalize_init(vcnt);

    xbl = px[0];
    xbr = px[0];
    xbt = py[0];
    xbb = py[0];

    for (bssize_t i=n-1; i>=1; i--)
    {
        if (xbl > px[i]) xbl = px[i];
        if (xbr < px[i]) xbr = px[i];
        if (xbt > py[i]) xbt = py[i];
        if (xbb < py[i]) xbb = py[i];
    }

    gtag = vcnt;
    viewportNodeCount = vcnt;
}

void polymost_drawrooms()
{
    if (videoGetRenderMode() == REND_CLASSIC) return;

    polymost_outputGLDebugMessage(3, "polymost_drawrooms()");

    videoBeginDrawing();
    frameoffset = frameplace + windowxy1.y*bytesperline + windowxy1.x;

#ifdef YAX_ENABLE
	if (yax_polymostclearzbuffer)
#endif
	{
		GLInterface.ClearDepth();
	}
    GLInterface.EnableBlend(false);
    GLInterface.EnableAlphaTest(false);
    GLInterface.EnableDepthTest(true);
	GLInterface.SetDepthFunc(Depth_Always);

	GLInterface.SetBrightness(r_scenebrightness);

    gvrcorrection = viewingrange*(1.f/65536.f);
    if (glprojectionhacks == 2)
    {
        // calculates the extend of the zenith glitch
        float verticalfovtan = (fviewingrange * (windowxy2.y-windowxy1.y) * 5.f) / ((float)yxaspect * (windowxy2.x-windowxy1.x) * 4.f);
        float verticalfov = atanf(verticalfovtan) * (2.f / fPI);
        static constexpr float const maxhorizangle = 0.6361136f; // horiz of 199 in degrees
        float zenglitch = verticalfov + maxhorizangle - 0.95f; // less than 1 because the zenith glitch extends a bit
        if (zenglitch > 0.f)
            gvrcorrection /= (zenglitch * 2.5f) + 1.f;
    }

    //Polymost supports true look up/down :) Here, we convert horizon to angle.
    //gchang&gshang are cos&sin of this angle (respectively)
    gyxscale = ((float)xdimenscale)*(1.0f/131072.f);
    gxyaspect = ((double)xyaspect*fviewingrange)*(5.0/(65536.0*262144.0));
    gviewxrange = fviewingrange * fxdimen * (1.f/(32768.f*1024.f));
    gcosang = fcosglobalang*(1.0f/262144.f);
    gsinang = fsinglobalang*(1.0f/262144.f);
    gcosang2 = gcosang * (fviewingrange * (1.0f/65536.f));
    gsinang2 = gsinang * (fviewingrange * (1.0f/65536.f));
    ghalfx = (float)(xdimen>>1);
    ghalfy = (float)(ydimen>>1);
    grhalfxdown10 = 1.f/(ghalfx*1024.f);
    ghoriz = fix16_to_float(qglobalhoriz);
    ghorizcorrect = fix16_to_float((100-polymostcenterhoriz)*divscale16(xdimenscale, viewingrange));

    gvisibility = ((float)globalvisibility)*FOGSCALE;

	GLInterface.SetShadeInterpolate(r_shadeinterpolate);

    //global cos/sin height angle
    if (r_yshearing)
    {
        gshang  = 0.f;
        gchang  = 1.f;
        ghoriz2 = (float)(ydimen >> 1) - (ghoriz + ghorizcorrect);
    }
    else
    {
        float r = (float)(ydimen >> 1) - (ghoriz + ghorizcorrect);
        gshang  = r / Bsqrtf(r * r + ghalfx * ghalfx / (gvrcorrection * gvrcorrection));
        gchang  = Bsqrtf(1.f - gshang * gshang);
        ghoriz2 = 0.f;
    }

    ghoriz = (float)(ydimen>>1);

    resizeglcheck();
    float const ratio = 1.f/get_projhack_ratio();

    //global cos/sin tilt angle
    gctang = cosf(gtang);
    gstang = sinf(gtang);

    if (Bfabsf(gstang) < .001f)  // This hack avoids nasty precision bugs in domost()
    {
        gstang = 0.f;
        gctang = (gctang > 0.f) ? 1.f : -1.f;
    }

    if (inpreparemirror)
        gstang = -gstang;

    //Generate viewport trapezoid (for handling screen up/down)
    vec3f_t p[4] = {  { 0-1,                                        0-1+ghorizcorrect,                                  0 },
                      { (float)(windowxy2.x + 1 - windowxy1.x + 2), 0-1+ghorizcorrect,                                  0 },
                      { (float)(windowxy2.x + 1 - windowxy1.x + 2), (float)(windowxy2.y + 1 - windowxy1.y + 2)+ghorizcorrect, 0 },
                      { 0-1,                                        (float)(windowxy2.y + 1 - windowxy1.y + 2)+ghorizcorrect, 0 } };

    for (auto & v : p)
    {
        //Tilt rotation (backwards)
        vec2f_t const o = { (v.x-ghalfx)*ratio, (v.y-ghoriz)*ratio };
        vec3f_t const o2 = { o.x*gctang + o.y*gstang, o.y*gctang - o.x*gstang + ghoriz2, ghalfx / gvrcorrection };

        //Up/down rotation (backwards)
        v = { o2.x, o2.y * gchang + o2.z * gshang, o2.z * gchang - o2.y * gshang };
    }

#if !SOFTROTMAT
    if (inpreparemirror)
        gstang = -gstang;
#endif

    //Clip to SCISDIST plane
    int n = 0;

    vec3f_t p2[6];

    for (bssize_t i=0; i<4; i++)
    {
        int const j = i < 3 ? i + 1 : 0;

        if (p[i].z >= SCISDIST)
            p2[n++] = p[i];

        if ((p[i].z >= SCISDIST) != (p[j].z >= SCISDIST))
        {
            float const r = (SCISDIST - p[i].z) / (p[j].z - p[i].z);
            p2[n++] = { (p[j].x - p[i].x) * r + p[i].x, (p[j].y - p[i].y) * r + p[i].y, SCISDIST };
        }
    }

    if (n < 3) 
	{
		GLInterface.SetDepthFunc(Depth_LessEqual);
		videoEndDrawing(); 
		return; 
	}

    float sx[6], sy[6];

    for (bssize_t i = 0; i < n; i++)
    {
        float const r = (ghalfx / gvrcorrection) / p2[i].z;
        sx[i] = p2[i].x * r + ghalfx;
        sy[i] = p2[i].y * r + ghoriz;
    }

    polymost_initmosts(sx, sy, n);

#ifdef YAX_ENABLE
    if (yax_globallev != YAX_MAXDRAWS)
    {
        int i, newi;
        int32_t nodrawbak = g_nodraw;
        g_nodraw = 1;
        for (i = yax_vsp[yax_globalbunch*2][0].n; i; i=newi)
        {
            newi = yax_vsp[yax_globalbunch*2][i].n;
            if (!newi)
                break;
            polymost_domost(yax_vsp[yax_globalbunch*2][newi].x, yax_vsp[yax_globalbunch*2][i].cy[1]-DOMOST_OFFSET, yax_vsp[yax_globalbunch*2][i].x, yax_vsp[yax_globalbunch*2][i].cy[0]-DOMOST_OFFSET);
        }
        for (i = yax_vsp[yax_globalbunch*2+1][0].n; i; i=newi)
        {
            newi = yax_vsp[yax_globalbunch*2+1][i].n;
            if (!newi)
                break;
            polymost_domost(yax_vsp[yax_globalbunch*2+1][i].x, yax_vsp[yax_globalbunch*2+1][i].cy[0]+DOMOST_OFFSET, yax_vsp[yax_globalbunch*2+1][newi].x, yax_vsp[yax_globalbunch*2+1][i].cy[1]+DOMOST_OFFSET);
        }
        g_nodraw = nodrawbak;

#ifdef COMBINE_STRIPS
        i = vsp[0].n;

        do
        {
            int const ni = vsp[i].n;

            //POGO: specially treat the viewport nodes so that we will never end up in a situation where we accidentally access the sentinel node
            if (ni >= viewportNodeCount)
            {
                if (Bfabsf(vsp[i].cy[1]-vsp[ni].cy[0]) < 0.1f && Bfabsf(vsp[i].fy[1]-vsp[ni].fy[0]) < 0.1f)
                {
                    float const dx = 1.f/(vsp[ni].x-vsp[i].x);
                    float const dx2 = 1.f/(vsp[vsp[ni].n].x-vsp[i].x);
                    float const cslop[2] = { vsp[i].cy[1]-vsp[i].cy[0], vsp[ni].cy[1]-vsp[i].cy[0] };
                    float const fslop[2] = { vsp[i].fy[1]-vsp[i].fy[0], vsp[ni].fy[1]-vsp[i].fy[0] };

                    if (Bfabsf(cslop[0]*dx-cslop[1]*dx2) < 0.001f && Bfabsf(fslop[0]*dx-fslop[1]*dx2) < 0.001f)
                    {
                        MERGE_NODES(i, ni);
                        continue;
                    }
                }
            }
            i = ni;
        }
        while (i);
#undef MERGE_NODES
#endif
    }
    //else if (!g_nodraw) { videoEndDrawing(); return; }
#endif

    polymost_updaterotmat();

    numscans = numbunches = 0;

    // MASKWALL_BAD_ACCESS
    // Fixes access of stale maskwall[maskwallcnt] (a "scan" index, in BUILD lingo):
    maskwallcnt = 0;

    // NOTE: globalcursectnum has been already adjusted in ADJUST_GLOBALCURSECTNUM.
    Bassert((unsigned)globalcursectnum < MAXSECTORS);
    polymost_scansector(globalcursectnum);

    grhalfxdown10x = grhalfxdown10;

    if (inpreparemirror)
    {
        // see engine.c: INPREPAREMIRROR_NO_BUNCHES
        if (numbunches > 0)
        {
            grhalfxdown10x = -grhalfxdown10;
            polymost_drawalls(0);
            numbunches--;
            bunchfirst[0] = bunchfirst[numbunches];
            bunchlast[0] = bunchlast[numbunches];
        } else
        {
            inpreparemirror = 0;
        }
    }

    while (numbunches > 0)
    {
        Bmemset(ptempbuf,0,numbunches+3); ptempbuf[0] = 1;

        int32_t closest = 0;              //Almost works, but not quite :(

        for (bssize_t i=1; i<numbunches; ++i)
        {
            int const bnch = polymost_bunchfront(i,closest); if (bnch < 0) continue;
            ptempbuf[i] = 1;
            if (!bnch) { ptempbuf[closest] = 1; closest = i; }
        }
        for (bssize_t i=0; i<numbunches; ++i) //Double-check
        {
            if (ptempbuf[i]) continue;
            int const bnch = polymost_bunchfront(i,closest); if (bnch < 0) continue;
            ptempbuf[i] = 1;
            if (!bnch) { ptempbuf[closest] = 1; closest = i; i = 0; }
        }

        polymost_drawalls(closest);

        if (automapping)
        {
            for (int z=bunchfirst[closest]; z>=0; z=bunchp2[z])
                show2dwall[thewall[z]>>3] |= pow2char[thewall[z]&7];
        }

        numbunches--;
        bunchfirst[closest] = bunchfirst[numbunches];
        bunchlast[closest] = bunchlast[numbunches];
    }

	GLInterface.SetDepthFunc(Depth_LessEqual);
	polymost_identityrotmat();

    videoEndDrawing();
}

static void polymost_drawmaskwallinternal(int32_t wallIndex)
{
    auto const wal = (uwallptr_t)&wall[wallIndex];
    auto const wal2 = (uwallptr_t)&wall[wal->point2];
    int32_t const sectnum = wall[wal->nextwall].nextsector;
    auto const sec = (usectorptr_t)&sector[sectnum];

//    if (wal->nextsector < 0) return;
    // Without MASKWALL_BAD_ACCESS fix:
    // wal->nextsector is -1, WGR2 SVN Lochwood Hollow (Til' Death L1)  (or trueror1.map)

    auto const nsec = (usectorptr_t)&sector[wal->nextsector];

    polymost_outputGLDebugMessage(3, "polymost_drawmaskwallinternal(wallIndex:%d)", wallIndex);

    globalpicnum = wal->overpicnum;
    if ((uint32_t)globalpicnum >= MAXTILES)
        globalpicnum = 0;

    globalorientation = (int32_t)wal->cstat;
    tileUpdatePicnum(&globalpicnum, (int16_t)wallIndex+16384);

    globvis = globalvisibility;
    globvis = (sector[sectnum].visibility != 0) ? mulscale4(globvis, (uint8_t)(sector[sectnum].visibility + 16)) : globalvisibility;

    globvis2 = globalvisibility2;
    globvis2 = (sector[sectnum].visibility != 0) ? mulscale4(globvis2, (uint8_t)(sector[sectnum].visibility + 16)) : globalvisibility2;
	GLInterface.SetVisibility(globvis2, fviewingrange);

    globalshade = (int32_t)wal->shade;
    globalpal = (int32_t)((uint8_t)wal->pal);

    vec2f_t s0 = { (float)(wal->x-globalposx), (float)(wal->y-globalposy) };
    vec2f_t p0 = { s0.y*gcosang - s0.x*gsinang, s0.x*gcosang2 + s0.y*gsinang2 };

    vec2f_t s1 = { (float)(wal2->x-globalposx), (float)(wal2->y-globalposy) };
    vec2f_t p1 = { s1.y*gcosang - s1.x*gsinang, s1.x*gcosang2 + s1.y*gsinang2 };

    if ((p0.y < SCISDIST) && (p1.y < SCISDIST)) return;

    //Clip to close parallel-screen plane
    vec2f_t const op0 = p0;

    float t0 = 0.f;

    if (p0.y < SCISDIST)
    {
        t0 = (SCISDIST - p0.y) / (p1.y - p0.y);
        p0 = { (p1.x - p0.x) * t0 + p0.x, SCISDIST };
    }

    float t1 = 1.f;

    if (p1.y < SCISDIST)
    {
        t1 = (SCISDIST - op0.y) / (p1.y - op0.y);
        p1 = { (p1.x - op0.x) * t1 + op0.x, SCISDIST };
    }

    int32_t m0 = (int32_t)((wal2->x - wal->x) * t0 + wal->x);
    int32_t m1 = (int32_t)((wal2->y - wal->y) * t0 + wal->y);
    int32_t cz[4], fz[4];
    getzsofslope(sectnum, m0, m1, &cz[0], &fz[0]);
    getzsofslope(wal->nextsector, m0, m1, &cz[1], &fz[1]);
    m0 = (int32_t)((wal2->x - wal->x) * t1 + wal->x);
    m1 = (int32_t)((wal2->y - wal->y) * t1 + wal->y);
    getzsofslope(sectnum, m0, m1, &cz[2], &fz[2]);
    getzsofslope(wal->nextsector, m0, m1, &cz[3], &fz[3]);

    float ryp0 = 1.f/p0.y;
    float ryp1 = 1.f/p1.y;

    //Generate screen coordinates for front side of wall
    float const x0 = ghalfx*p0.x*ryp0 + ghalfx;
    float const x1 = ghalfx*p1.x*ryp1 + ghalfx;
    if (x1 <= x0) return;

    ryp0 *= gyxscale; ryp1 *= gyxscale;

    xtex.d = (ryp0-ryp1)*gxyaspect / (x0-x1);
    ytex.d = 0;
    otex.d = ryp0*gxyaspect - xtex.d*x0;

    //gux*x0 + guo = t0*wal->xrepeat*8*yp0
    //gux*x1 + guo = t1*wal->xrepeat*8*yp1
    xtex.u = (t0*ryp0 - t1*ryp1)*gxyaspect*(float)wal->xrepeat*8.f / (x0-x1);
    otex.u = t0*ryp0*gxyaspect*(float)wal->xrepeat*8.f - xtex.u*x0;
    otex.u += (float)wal->xpanning*otex.d;
    xtex.u += (float)wal->xpanning*xtex.d;
    ytex.u = 0;

    // mask
    calc_ypanning((!(wal->cstat & 4)) ? max(nsec->ceilingz, sec->ceilingz) : min(nsec->floorz, sec->floorz), ryp0, ryp1,
                  x0, x1, wal->ypanning, wal->yrepeat, 0, tilesiz[globalpicnum]);

    if (wal->cstat&8) //xflip
    {
        float const t = (float)(wal->xrepeat*8 + wal->xpanning*2);
        xtex.u = xtex.d*t - xtex.u;
        ytex.u = ytex.d*t - ytex.u;
        otex.u = otex.d*t - otex.u;
    }
    if (wal->cstat&256) { xtex.v = -xtex.v; ytex.v = -ytex.v; otex.v = -otex.v; } //yflip

    int method = DAMETH_MASK | DAMETH_WALL;

    if (wal->cstat & 128)
        method = DAMETH_WALL | (((wal->cstat & 512)) ? DAMETH_TRANS2 : DAMETH_TRANS1);

#ifdef NEW_MAP_FORMAT
    uint8_t const blend = wal->blend;
#else
    uint8_t const blend = wallext[wallIndex].blend;
#endif
    handle_blend(!!(wal->cstat & 128), blend, !!(wal->cstat & 512));

    drawpoly_alpha = 0.f;
    drawpoly_blend = blend;

	calc_and_apply_fog(fogshade(wal->shade, wal->pal), sec->visibility, get_floor_fogpal(sec));

    float const csy[4] = { ((float)(cz[0] - globalposz)) * ryp0 + ghoriz,
                           ((float)(cz[1] - globalposz)) * ryp0 + ghoriz,
                           ((float)(cz[2] - globalposz)) * ryp1 + ghoriz,
                           ((float)(cz[3] - globalposz)) * ryp1 + ghoriz };

    float const fsy[4] = { ((float)(fz[0] - globalposz)) * ryp0 + ghoriz,
                           ((float)(fz[1] - globalposz)) * ryp0 + ghoriz,
                           ((float)(fz[2] - globalposz)) * ryp1 + ghoriz,
                           ((float)(fz[3] - globalposz)) * ryp1 + ghoriz };

    //Clip 2 quadrilaterals
    //               /csy3
    //             /   |
    // csy0------/----csy2
    //   |     /xxxxxxx|
    //   |   /xxxxxxxxx|
    // csy1/xxxxxxxxxxx|
    //   |xxxxxxxxxxx/fsy3
    //   |xxxxxxxxx/   |
    //   |xxxxxxx/     |
    // fsy0----/------fsy2
    //   |   /
    // fsy1/

    vec2f_t dpxy[16] = { { x0, csy[1] }, { x1, csy[3] }, { x1, fsy[3] }, { x0, fsy[1] } };

    //Clip to (x0,csy[0])-(x1,csy[2])

    vec2f_t dp2[8];

    int n2 = 0;
    t1 = -((dpxy[0].x - x0) * (csy[2] - csy[0]) - (dpxy[0].y - csy[0]) * (x1 - x0));

    for (bssize_t i=0; i<4; i++)
    {
        int j = i + 1;

        if (j >= 4)
            j = 0;

        t0 = t1;
        t1 = -((dpxy[j].x - x0) * (csy[2] - csy[0]) - (dpxy[j].y - csy[0]) * (x1 - x0));

        if (t0 >= 0)
            dp2[n2++] = dpxy[i];

        if ((t0 >= 0) != (t1 >= 0) && (t0 <= 0) != (t1 <= 0))
        {
            float const r = t0 / (t0 - t1);
            dp2[n2++] = { (dpxy[j].x - dpxy[i].x) * r + dpxy[i].x, (dpxy[j].y - dpxy[i].y) * r + dpxy[i].y };
        }
    }

    if (n2 < 3)
        return;

    //Clip to (x1,fsy[2])-(x0,fsy[0])
    t1 = -((dp2[0].x - x1) * (fsy[0] - fsy[2]) - (dp2[0].y - fsy[2]) * (x0 - x1));
    int n = 0;

    for (bssize_t i = 0, j = 1; i < n2; j = ++i + 1)
    {
        if (j >= n2)
            j = 0;

        t0 = t1;
        t1 = -((dp2[j].x - x1) * (fsy[0] - fsy[2]) - (dp2[j].y - fsy[2]) * (x0 - x1));

        if (t0 >= 0)
            dpxy[n++] = dp2[i];

        if ((t0 >= 0) != (t1 >= 0) && (t0 <= 0) != (t1 <= 0))
        {
            float const r = t0 / (t0 - t1);
            dpxy[n++] = { (dp2[j].x - dp2[i].x) * r + dp2[i].x, (dp2[j].y - dp2[i].y) * r + dp2[i].y };
        }
    }

    if (n < 3)
        return;

    pow2xsplit = 0;
    skyclamphack = 0;

    polymost_updaterotmat();
    polymost_drawpoly(dpxy, n, method, tilesiz[globalpicnum]);
    polymost_identityrotmat();
}

void polymost_drawmaskwall(int32_t damaskwallcnt)
{
    int const z = maskwall[damaskwallcnt];
    polymost_drawmaskwallinternal(thewall[z]);
}

void polymost_prepareMirror(int32_t dax, int32_t day, int32_t daz, fix16_t daang, fix16_t dahoriz, int16_t mirrorWall)
{
    polymost_outputGLDebugMessage(3, "polymost_prepareMirror(%u)", mirrorWall);

    //POGO: prepare necessary globals for drawing, as we intend to call this outside of drawrooms
    gvrcorrection = viewingrange*(1.f/65536.f);
    if (glprojectionhacks == 2)
    {
        // calculates the extend of the zenith glitch
        float verticalfovtan = (fviewingrange * (windowxy2.y-windowxy1.y) * 5.f) / ((float)yxaspect * (windowxy2.x-windowxy1.x) * 4.f);
        float verticalfov = atanf(verticalfovtan) * (2.f / fPI);
        static constexpr float const maxhorizangle = 0.6361136f; // horiz of 199 in degrees
        float zenglitch = verticalfov + maxhorizangle - 0.95f; // less than 1 because the zenith glitch extends a bit
        if (zenglitch > 0.f)
            gvrcorrection /= (zenglitch * 2.5f) + 1.f;
    }

    set_globalpos(dax, day, daz);
    set_globalang(daang);
    globalhoriz = mulscale16(fix16_to_int(dahoriz)-100,divscale16(xdimenscale,viewingrange))+(ydimen>>1);
    qglobalhoriz = mulscale16(dahoriz-F16(100), divscale16(xdimenscale, viewingrange))+fix16_from_int(ydimen>>1);
    gyxscale = ((float)xdimenscale)*(1.0f/131072.f);
    gxyaspect = ((double)xyaspect*fviewingrange)*(5.0/(65536.0*262144.0));
    gviewxrange = fviewingrange * fxdimen * (1.f/(32768.f*1024.f));
    gcosang = fcosglobalang*(1.0f/262144.f);
    gsinang = fsinglobalang*(1.0f/262144.f);
    gcosang2 = gcosang * (fviewingrange * (1.0f/65536.f));
    gsinang2 = gsinang * (fviewingrange * (1.0f/65536.f));
    ghalfx = (float)(xdimen>>1);
    ghalfy = (float)(ydimen>>1);
    grhalfxdown10 = 1.f/(ghalfx*1024.f);
    ghoriz = fix16_to_float(qglobalhoriz);
    ghorizcorrect = fix16_to_float((100-polymostcenterhoriz)*divscale16(xdimenscale, viewingrange));
    gvisibility = ((float)globalvisibility)*FOGSCALE;
    resizeglcheck();
    if (r_yshearing)
    {
        gshang  = 0.f;
        gchang  = 1.f;
        ghoriz2 = (float)(ydimen >> 1) - (ghoriz+ghorizcorrect);
    }
    else
    {
        float r = (float)(ydimen >> 1) - (ghoriz+ghorizcorrect);
        gshang  = r / Bsqrtf(r * r + ghalfx * ghalfx / (gvrcorrection * gvrcorrection));
        gchang  = Bsqrtf(1.f - gshang * gshang);
        ghoriz2 = 0.f;
    }
    ghoriz = (float)(ydimen>>1);
    gctang = cosf(gtang);
    gstang = sinf(gtang);
    if (Bfabsf(gstang) < .001f)
    {
        gstang = 0.f;
        gctang = (gctang > 0.f) ? 1.f : -1.f;
    }
    grhalfxdown10x = grhalfxdown10;

    //POGO: write the mirror region to the stencil buffer to allow showing mirrors & skyboxes at the same time
	GLInterface.EnableStencilWrite(1);
    GLInterface.EnableAlphaTest(false);
    GLInterface.EnableDepthTest(false);
    polymost_drawmaskwallinternal(mirrorWall);
    GLInterface.EnableAlphaTest(true);
    GLInterface.EnableDepthTest(true);

    //POGO: render only to the mirror region
	GLInterface.EnableStencilTest(1);
}

void polymost_completeMirror()
{
    polymost_outputGLDebugMessage(3, "polymost_completeMirror()");
	GLInterface.DisableStencil();
}

typedef struct
{
    uint32_t wrev;
    uint32_t srev;
    int16_t wall;
    int8_t wdist;
    int8_t filler;
} wallspriteinfo_t;

wallspriteinfo_t wsprinfo[MAXSPRITES];

void Polymost_prepare_loadboard(void)
{
    Bmemset(wsprinfo, 0, sizeof(wsprinfo));
}

static inline int32_t polymost_findwall(uspriteptr_t const tspr, vec2_t const * const tsiz, int32_t * rd)
{
    int32_t dist = 4, closest = -1;
    auto const sect = (usectortype  * )&sector[tspr->sectnum];
    vec2_t n;

    for (bssize_t i=sect->wallptr; i<sect->wallptr + sect->wallnum; i++)
    {
        if ((wall[i].nextsector == -1 || ((sector[wall[i].nextsector].ceilingz > (tspr->z - ((tsiz->y * tspr->yrepeat) << 2))) ||
             sector[wall[i].nextsector].floorz < tspr->z)) && !polymost_getclosestpointonwall((const vec2_t *) tspr, i, &n))
        {
            int const dst = klabs(tspr->x - n.x) + klabs(tspr->y - n.y);

            if (dst <= dist)
            {
                dist = dst;
                closest = i;
            }
        }
    }

    *rd = dist;

    return closest;
}

int32_t polymost_lintersect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                            int32_t x3, int32_t y3, int32_t x4, int32_t y4)
{
    // p1 to p2 is a line segment
    int32_t const x21 = x2 - x1, x34 = x3 - x4;
    int32_t const y21 = y2 - y1, y34 = y3 - y4;
    int32_t const bot = x21 * y34 - y21 * x34;

    if (!bot)
        return 0;

    int32_t const x31 = x3 - x1, y31 = y3 - y1;
    int32_t const topt = x31 * y34 - y31 * x34;

    int rv = 1;

    if (bot > 0)
    {
        if ((unsigned)topt >= (unsigned)bot)
            rv = 0;

        int32_t topu = x21 * y31 - y21 * x31;

        if ((unsigned)topu >= (unsigned)bot)
            rv = 0;
    }
    else
    {
        if ((unsigned)topt <= (unsigned)bot)
            rv = 0;

        int32_t topu = x21 * y31 - y21 * x31;

        if ((unsigned)topu <= (unsigned)bot)
            rv = 0;
    }

    return rv;
}

#define TSPR_OFFSET_FACTOR .000008f
#define TSPR_OFFSET(tspr) ((TSPR_OFFSET_FACTOR + ((tspr->owner != -1 ? tspr->owner & 63 : 1) * TSPR_OFFSET_FACTOR)) * (float)sepdist(globalposx - tspr->x, globalposy - tspr->y, globalposz - tspr->z) * 0.025f)


void polymost_drawsprite(int32_t snum)
{
    auto const tspr = tspriteptr[snum];

    if (EDUKE32_PREDICT_FALSE(bad_tspr(tspr)))
        return;

    usectorptr_t sec;

    int32_t spritenum = tspr->owner;

    polymost_outputGLDebugMessage(3, "polymost_drawsprite(snum:%d)", snum);

    if ((tspr->cstat&48) != 48)
        tileUpdatePicnum(&tspr->picnum, spritenum + 32768);

    globalpicnum = tspr->picnum;
    globalshade = tspr->shade;
    globalpal = tspr->pal;
    globalorientation = tspr->cstat;
    globvis = globalvisibility;

    if (sector[tspr->sectnum].visibility != 0)
        globvis = mulscale4(globvis, (uint8_t)(sector[tspr->sectnum].visibility + 16));

    globvis2 = globalvisibility2;
    if (sector[tspr->sectnum].visibility != 0)
        globvis2 = mulscale4(globvis2, (uint8_t)(sector[tspr->sectnum].visibility + 16));
	GLInterface.SetVisibility(globvis2, fviewingrange);

    vec2_t off = { 0, 0 };

    if ((globalorientation & 48) != 48)  // only non-voxel sprites should do this
    {
        int const flag = usehightile && h_xsize[globalpicnum];
        off = { (int32_t)tspr->xoffset + (flag ? h_xoffs[globalpicnum] : picanm[globalpicnum].xofs),
                (int32_t)tspr->yoffset + (flag ? h_yoffs[globalpicnum] : picanm[globalpicnum].yofs) };
    }

    int32_t method = DAMETH_MASK | DAMETH_CLAMPED;

    if (tspr->cstat & 2)
        method = DAMETH_CLAMPED | ((tspr->cstat & 512) ? DAMETH_TRANS2 : DAMETH_TRANS1);

    handle_blend(!!(tspr->cstat & 2), tspr->blend, !!(tspr->cstat & 512));

    drawpoly_alpha = spriteext[spritenum].alpha;
    drawpoly_blend = tspr->blend;

    sec = (usectorptr_t)&sector[tspr->sectnum];

	calc_and_apply_fog(fogshade(globalshade, globalpal), sec->visibility, get_floor_fogpal(sec));

    while (!(spriteext[spritenum].flags & SPREXT_NOTMD))
    {
        if (usemodels && tile2model[Ptile2tile(tspr->picnum, tspr->pal)].modelid >= 0 &&
            tile2model[Ptile2tile(tspr->picnum, tspr->pal)].framenum >= 0)
        {
            if (polymost_mddraw(tspr)) return;
            break;  // else, render as flat sprite
        }

        if (usevoxels && (tspr->cstat & 48) != 48 && tiletovox[tspr->picnum] >= 0 && voxmodels[tiletovox[tspr->picnum]])
        {
            if (polymost_voxdraw(voxmodels[tiletovox[tspr->picnum]], tspr)) return;
            break;  // else, render as flat sprite
        }

        if ((tspr->cstat & 48) == 48 && voxmodels[tspr->picnum])
        {
            polymost_voxdraw(voxmodels[tspr->picnum], tspr);
            return;
        }

        break;
    }

    vec2_t pos = tspr->pos.vec2;

    if (spriteext[spritenum].flags & SPREXT_AWAY1)
    {
        pos.x += (sintable[(tspr->ang + 512) & 2047] >> 13);
        pos.y += (sintable[(tspr->ang) & 2047] >> 13);
    }
    else if (spriteext[spritenum].flags & SPREXT_AWAY2)
    {
        pos.x -= (sintable[(tspr->ang + 512) & 2047] >> 13);
        pos.y -= (sintable[(tspr->ang) & 2047] >> 13);
    }

    vec2_16_t const oldsiz = tilesiz[globalpicnum];
    vec2_t tsiz = { oldsiz.x, oldsiz.y };

    if (usehightile && h_xsize[globalpicnum])
        tsiz = { h_xsize[globalpicnum], h_ysize[globalpicnum] };

    if (tsiz.x <= 0 || tsiz.y <= 0)
        return;

    polymost_updaterotmat();

    vec2f_t const ftsiz = { (float) tsiz.x, (float) tsiz.y };

    switch ((globalorientation >> 4) & 3)
    {
        case 0:  // Face sprite
        {
            // Project 3D to 2D
            if (globalorientation & 4)
                off.x = -off.x;
            // NOTE: yoff not negated not for y flipping, unlike wall and floor
            // aligned sprites.

            int const ang = (getangle(tspr->x - globalposx, tspr->y - globalposy) + 1024) & 2047;

            float const foffs = TSPR_OFFSET(tspr);

            vec2f_t const offs = { (float) (sintable[(ang + 512) & 2047] >> 6) * foffs,
                (float) (sintable[(ang) & 2047] >> 6) * foffs };

            vec2f_t s0 = { (float)(tspr->x - globalposx) + offs.x,
                           (float)(tspr->y - globalposy) + offs.y};

            vec2f_t p0 = { s0.y * gcosang - s0.x * gsinang, s0.x * gcosang2 + s0.y * gsinang2 };

            if (p0.y <= SCISDIST)
                goto _drawsprite_return;

            float const ryp0 = 1.f / p0.y;
            s0 = { ghalfx * p0.x * ryp0 + ghalfx, ((float)(tspr->z - globalposz)) * gyxscale * ryp0 + ghoriz };

            float const f = ryp0 * fxdimen * (1.0f / 160.f);

            vec2f_t ff = { ((float)tspr->xrepeat) * f,
                           ((float)tspr->yrepeat) * f * ((float)yxaspect * (1.0f / 65536.f)) };

            if (tsiz.x & 1)
                s0.x += ff.x * 0.5f;
            if (globalorientation & 128 && tsiz.y & 1)
                s0.y += ff.y * 0.5f;

            s0.x -= ff.x * (float) off.x;
            s0.y -= ff.y * (float) off.y;

            ff.x *= ftsiz.x;
            ff.y *= ftsiz.y;

            vec2f_t pxy[4];

            pxy[0].x = pxy[3].x = s0.x - ff.x * 0.5f;
            pxy[1].x = pxy[2].x = s0.x + ff.x * 0.5f;
            if (!(globalorientation & 128))
            {
                pxy[0].y = pxy[1].y = s0.y - ff.y;
                pxy[2].y = pxy[3].y = s0.y;
            }
            else
            {
                pxy[0].y = pxy[1].y = s0.y - ff.y * 0.5f;
                pxy[2].y = pxy[3].y = s0.y + ff.y * 0.5f;
            }

            xtex.d = ytex.d = ytex.u = xtex.v = 0;
            otex.d = ryp0 * gviewxrange;

            if (!(globalorientation & 4))
            {
                xtex.u = ftsiz.x * otex.d / (pxy[1].x - pxy[0].x + .002f);
                otex.u = -xtex.u * (pxy[0].x - .001f);
            }
            else
            {
                xtex.u = ftsiz.x * otex.d / (pxy[0].x - pxy[1].x - .002f);
                otex.u = -xtex.u * (pxy[1].x + .001f);
            }

            if (!(globalorientation & 8))
            {
                ytex.v = ftsiz.y * otex.d / (pxy[3].y - pxy[0].y + .002f);
                otex.v = -ytex.v * (pxy[0].y - .001f);
            }
            else
            {
                ytex.v = ftsiz.y * otex.d / (pxy[0].y - pxy[3].y - .002f);
                otex.v = -ytex.v * (pxy[3].y + .001f);
            }

            // sprite panning
            if (spriteext[spritenum].xpanning)
            {
                ytex.u -= ytex.d * ((float) (spriteext[spritenum].xpanning) * (1.0f / 255.f)) * ftsiz.x;
                otex.u -= otex.d * ((float) (spriteext[spritenum].xpanning) * (1.0f / 255.f)) * ftsiz.x;
                drawpoly_srepeat = 1;
            }

            if (spriteext[spritenum].ypanning)
            {
                ytex.v -= ytex.d * ((float) (spriteext[spritenum].ypanning) * (1.0f / 255.f)) * ftsiz.y;
                otex.v -= otex.d * ((float) (spriteext[spritenum].ypanning) * (1.0f / 255.f)) * ftsiz.y;
                drawpoly_trepeat = 1;
            }

            // Clip sprites to ceilings/floors when no parallaxing and not sloped
            if (!(sector[tspr->sectnum].ceilingstat & 3))
            {
                s0.y = ((float) (sector[tspr->sectnum].ceilingz - globalposz)) * gyxscale * ryp0 + ghoriz;
                if (pxy[0].y < s0.y)
                    pxy[0].y = pxy[1].y = s0.y;
            }

            if (!(sector[tspr->sectnum].floorstat & 3))
            {
                s0.y = ((float) (sector[tspr->sectnum].floorz - globalposz)) * gyxscale * ryp0 + ghoriz;
                if (pxy[2].y > s0.y)
                    pxy[2].y = pxy[3].y = s0.y;
            }

            vec2_16_t tempsiz = { (int16_t)tsiz.x, (int16_t)tsiz.y };
            pow2xsplit = 0;
            polymost_drawpoly(pxy, 4, method, tempsiz);

            drawpoly_srepeat = 0;
            drawpoly_trepeat = 0;
        }
        break;

        case 1:  // Wall sprite
        {
            // Project 3D to 2D
            if (globalorientation & 4)
                off.x = -off.x;

            if (globalorientation & 8)
                off.y = -off.y;

            vec2f_t const extent = { (float)tspr->xrepeat * (float)sintable[(tspr->ang) & 2047] * (1.0f / 65536.f),
                                     (float)tspr->xrepeat * (float)sintable[(tspr->ang + 1536) & 2047] * (1.0f / 65536.f) };

            float f = (float)(tsiz.x >> 1) + (float)off.x;

            vec2f_t const vf = { extent.x * f, extent.y * f };

            vec2f_t vec0 = { (float)(pos.x - globalposx) - vf.x,
                             (float)(pos.y - globalposy) - vf.y };

            int32_t const s = tspr->owner;
            int32_t walldist = 1;
            int32_t w = (s == -1) ? -1 : wsprinfo[s].wall;

            // find the wall most likely to be what the sprite is supposed to be ornamented against
            // this is really slow, so cache the result
            if (s == -1 || !wsprinfo[s].wall || (spritechanged[s] != wsprinfo[s].srev) ||
                (w != -1 && wallchanged[w] != wsprinfo[s].wrev))
            {
                w = polymost_findwall(tspr, &tsiz, &walldist);

                if (s != -1)
                {
                    wallspriteinfo_t *ws = &wsprinfo[s];
                    ws->wall = w;

                    if (w != -1)
                    {
                        ws->wdist = walldist;
                        ws->wrev = wallchanged[w];
                        ws->srev = spritechanged[s];
                    }
                }
            }
            else if (s != -1)
                walldist = wsprinfo[s].wdist;

            // detect if the sprite is either on the wall line or the wall line and sprite intersect
            if (w != -1)
            {
                vec2_t v = { /*Blrintf(vf.x)*/(int)vf.x, /*Blrintf(vf.y)*/(int)vf.y };

                if (walldist <= 2 || ((pos.x - v.x) + (pos.x + v.x)) == (wall[w].x + POINT2(w).x) ||
                    ((pos.y - v.y) + (pos.y + v.y)) == (wall[w].y + POINT2(w).y) ||
                    polymost_lintersect(pos.x - v.x, pos.y - v.y, pos.x + v.x, pos.y + v.y, wall[w].x, wall[w].y,
                                        POINT2(w).x, POINT2(w).y))
                {
                    int32_t const ang = getangle(wall[w].x - POINT2(w).x, wall[w].y - POINT2(w).y);
                    float const foffs = TSPR_OFFSET(tspr);
                    vec2f_t const offs = { (float)(sintable[(ang + 1024) & 2047] >> 6) * foffs,
                                     (float)(sintable[(ang + 512) & 2047] >> 6) * foffs};

                    vec0.x -= offs.x;
                    vec0.y -= offs.y;
                }
            }

            vec2f_t p0 = { vec0.y * gcosang - vec0.x * gsinang,
                           vec0.x * gcosang2 + vec0.y * gsinang2 };

            vec2f_t const pp = { extent.x * ftsiz.x + vec0.x,
                                 extent.y * ftsiz.x + vec0.y };

            vec2f_t p1 = { pp.y * gcosang - pp.x * gsinang,
                           pp.x * gcosang2 + pp.y * gsinang2 };

            if ((p0.y <= SCISDIST) && (p1.y <= SCISDIST))
                goto _drawsprite_return;

            // Clip to close parallel-screen plane
            vec2f_t const op0 = p0;

            float t0 = 0.f, t1 = 1.f;

            if (p0.y < SCISDIST)
            {
                t0 = (SCISDIST - p0.y) / (p1.y - p0.y);
                p0 = { (p1.x - p0.x) * t0 + p0.x, SCISDIST };
            }

            if (p1.y < SCISDIST)
            {
                t1 = (SCISDIST - op0.y) / (p1.y - op0.y);
                p1 = { (p1.x - op0.x) * t1 + op0.x, SCISDIST };
            }

            f = 1.f / p0.y;
            const float ryp0 = f * gyxscale;
            float sx0 = ghalfx * p0.x * f + ghalfx;

            f = 1.f / p1.y;
            const float ryp1 = f * gyxscale;
            float sx1 = ghalfx * p1.x * f + ghalfx;

            tspr->z -= ((off.y * tspr->yrepeat) << 2);

            if (globalorientation & 128)
            {
                tspr->z += ((tsiz.y * tspr->yrepeat) << 1);

                if (tsiz.y & 1)
                    tspr->z += (tspr->yrepeat << 1);  // Odd yspans
            }

            xtex.d = (ryp0 - ryp1) * gxyaspect / (sx0 - sx1);
            ytex.d = 0;
            otex.d = ryp0 * gxyaspect - xtex.d * sx0;

            if (globalorientation & 4)
            {
                t0 = 1.f - t0;
                t1 = 1.f - t1;
            }

            // sprite panning
            if (spriteext[spritenum].xpanning)
            {
                float const xpan = ((float)(spriteext[spritenum].xpanning) * (1.0f / 255.f));
                t0 -= xpan;
                t1 -= xpan;
                drawpoly_srepeat = 1;
            }

            xtex.u = (t0 * ryp0 - t1 * ryp1) * gxyaspect * ftsiz.x / (sx0 - sx1);
            ytex.u = 0;
            otex.u = t0 * ryp0 * gxyaspect * ftsiz.x - xtex.u * sx0;

            f = ((float) tspr->yrepeat) * ftsiz.y * 4;

            float sc0 = ((float) (tspr->z - globalposz - f)) * ryp0 + ghoriz;
            float sc1 = ((float) (tspr->z - globalposz - f)) * ryp1 + ghoriz;
            float sf0 = ((float) (tspr->z - globalposz)) * ryp0 + ghoriz;
            float sf1 = ((float) (tspr->z - globalposz)) * ryp1 + ghoriz;

            // gvx*sx0 + gvy*sc0 + gvo = 0
            // gvx*sx1 + gvy*sc1 + gvo = 0
            // gvx*sx0 + gvy*sf0 + gvo = tsizy*(gdx*sx0 + gdo)
            f = ftsiz.y * (xtex.d * sx0 + otex.d) / ((sx0 - sx1) * (sc0 - sf0));

            if (!(globalorientation & 8))
            {
                xtex.v = (sc0 - sc1) * f;
                ytex.v = (sx1 - sx0) * f;
                otex.v = -xtex.v * sx0 - ytex.v * sc0;
            }
            else
            {
                xtex.v = (sf1 - sf0) * f;
                ytex.v = (sx0 - sx1) * f;
                otex.v = -xtex.v * sx0 - ytex.v * sf0;
            }

            // sprite panning
            if (spriteext[spritenum].ypanning)
            {
                float const ypan = ((float)(spriteext[spritenum].ypanning) * (1.0f / 255.f)) * ftsiz.y;
                xtex.v -= xtex.d * ypan;
                ytex.v -= ytex.d * ypan;
                otex.v -= otex.d * ypan;
                drawpoly_trepeat = 1;
            }

            // Clip sprites to ceilings/floors when no parallaxing
            if (!(sector[tspr->sectnum].ceilingstat & 1))
            {
                if (sector[tspr->sectnum].ceilingz > tspr->z - (float)((tspr->yrepeat * tsiz.y) << 2))
                {
                    sc0 = (float)(sector[tspr->sectnum].ceilingz - globalposz) * ryp0 + ghoriz;
                    sc1 = (float)(sector[tspr->sectnum].ceilingz - globalposz) * ryp1 + ghoriz;
                }
            }
            if (!(sector[tspr->sectnum].floorstat & 1))
            {
                if (sector[tspr->sectnum].floorz < tspr->z)
                {
                    sf0 = (float)(sector[tspr->sectnum].floorz - globalposz) * ryp0 + ghoriz;
                    sf1 = (float)(sector[tspr->sectnum].floorz - globalposz) * ryp1 + ghoriz;
                }
            }

            if (sx0 > sx1)
            {
                if (globalorientation & 64)
                    goto _drawsprite_return;  // 1-sided sprite

                swapfloat(&sx0, &sx1);
                swapfloat(&sc0, &sc1);
                swapfloat(&sf0, &sf1);
            }

            vec2f_t const pxy[4] = { { sx0, sc0 }, { sx1, sc1 }, { sx1, sf1 }, { sx0, sf0 } };

			vec2_16_t tempsiz = { (int16_t)tsiz.x, (int16_t)tsiz.y };
			pow2xsplit = 0;
            polymost_drawpoly(pxy, 4, method, tempsiz);

            drawpoly_srepeat = 0;
            drawpoly_trepeat = 0;
        }
        break;

        case 2:  // Floor sprite
            globvis2 = globalhisibility2;
            if (sector[tspr->sectnum].visibility != 0)
                globvis2 = mulscale4(globvis2, (uint8_t)(sector[tspr->sectnum].visibility + 16));
			GLInterface.SetVisibility(globvis2, fviewingrange);

            if ((globalorientation & 64) != 0 && (globalposz > tspr->z) == (!(globalorientation & 8)))
                goto _drawsprite_return;
            else
            {
                if ((globalorientation & 4) > 0)
                    off.x = -off.x;
                if ((globalorientation & 8) > 0)
                    off.y = -off.y;

                vec2f_t const p0 = { (float)(((tsiz.x + 1) >> 1) - off.x) * tspr->xrepeat,
                                     (float)(((tsiz.y + 1) >> 1) - off.y) * tspr->yrepeat },
                              p1 = { (float)((tsiz.x >> 1) + off.x) * tspr->xrepeat,
                                     (float)((tsiz.y >> 1) + off.y) * tspr->yrepeat };

                float const c = sintable[(tspr->ang + 512) & 2047] * (1.0f / 65536.f);
                float const s = sintable[tspr->ang & 2047] * (1.0f / 65536.f);

                vec2f_t pxy[6];

                // Project 3D to 2D
                for (bssize_t j = 0; j < 4; j++)
                {
                    vec2f_t s0 = { (float)(tspr->x - globalposx), (float)(tspr->y - globalposy) };

                    if ((j + 0) & 2)
                    {
                        s0.y -= s * p0.y;
                        s0.x -= c * p0.y;
                    }
                    else
                    {
                        s0.y += s * p1.y;
                        s0.x += c * p1.y;
                    }
                    if ((j + 1) & 2)
                    {
                        s0.x -= s * p0.x;
                        s0.y += c * p0.x;
                    }
                    else
                    {
                        s0.x += s * p1.x;
                        s0.y -= c * p1.x;
                    }

                    pxy[j] = { s0.y * gcosang - s0.x * gsinang, s0.x * gcosang2 + s0.y * gsinang2 };
                }

                if (tspr->z < globalposz)  // if floor sprite is above you, reverse order of points
                {
                    EDUKE32_STATIC_ASSERT(sizeof(uint64_t) == sizeof(vec2f_t));

                    swap64bit(&pxy[0], &pxy[1]);
                    swap64bit(&pxy[2], &pxy[3]);
                }

                // Clip to SCISDIST plane
                int32_t npoints = 0;
                vec2f_t p2[6];

                for (bssize_t i = 0, j = 1; i < 4; j = ((++i + 1) & 3))
                {
                    if (pxy[i].y >= SCISDIST)
                        p2[npoints++] = pxy[i];

                    if ((pxy[i].y >= SCISDIST) != (pxy[j].y >= SCISDIST))
                    {
                        float const f = (SCISDIST - pxy[i].y) / (pxy[j].y - pxy[i].y);
                        vec2f_t const t = { (pxy[j].x - pxy[i].x) * f + pxy[i].x,
                                            (pxy[j].y - pxy[i].y) * f + pxy[i].y };
                        p2[npoints++] = t;
                    }
                }

                if (npoints < 3)
                    goto _drawsprite_return;

                // Project rotated 3D points to screen

                int fadjust = 0;

                // unfortunately, offsetting by only 1 isn't enough on most Android devices
                if (tspr->z == sec->ceilingz || tspr->z == sec->ceilingz + 1)
                    tspr->z = sec->ceilingz + 2, fadjust = (tspr->owner & 31);

                if (tspr->z == sec->floorz || tspr->z == sec->floorz - 1)
                    tspr->z = sec->floorz - 2, fadjust = -((tspr->owner & 31));

                float f = (float)(tspr->z - globalposz + fadjust) * gyxscale;

                for (bssize_t j = 0; j < npoints; j++)
                {
                    float const ryp0 = 1.f / p2[j].y;
                    pxy[j] = { ghalfx * p2[j].x * ryp0 + ghalfx, f * ryp0 + ghoriz };
                }

                // gd? Copied from floor rendering code

                xtex.d = 0;
                ytex.d = gxyaspect / (double)(tspr->z - globalposz + fadjust);
                otex.d = -ghoriz * ytex.d;

                // copied&modified from relative alignment
                vec2f_t const vv = { (float)tspr->x + s * p1.x + c * p1.y, (float)tspr->y + s * p1.y - c * p1.x };
                vec2f_t ff = { -(p0.x + p1.x) * s, (p0.x + p1.x) * c };

                f = polymost_invsqrt_approximation(ff.x * ff.x + ff.y * ff.y);

                ff.x *= f;
                ff.y *= f;

                float const ft[4] = { ((float)(globalposy - vv.y)) * ff.y + ((float)(globalposx - vv.x)) * ff.x,
                                      ((float)(globalposx - vv.x)) * ff.y - ((float)(globalposy - vv.y)) * ff.x,
                                      fsinglobalang * ff.y + fcosglobalang * ff.x,
                                      fsinglobalang * ff.x - fcosglobalang * ff.y };

                f = fviewingrange * -(1.f / (65536.f * 262144.f));
                xtex.u = (float)ft[3] * f;
                xtex.v = (float)ft[2] * f;
                ytex.u = ft[0] * ytex.d;
                ytex.v = ft[1] * ytex.d;
                otex.u = ft[0] * otex.d;
                otex.v = ft[1] * otex.d;
                otex.u += (ft[2] * (1.0f / 262144.f) - xtex.u) * ghalfx;
                otex.v -= (ft[3] * (1.0f / 262144.f) + xtex.v) * ghalfx;

                f = 4.f / (float)tspr->xrepeat;
                xtex.u *= f;
                ytex.u *= f;
                otex.u *= f;

                f = -4.f / (float)tspr->yrepeat;
                xtex.v *= f;
                ytex.v *= f;
                otex.v *= f;

                if (globalorientation & 4)
                {
                    xtex.u = ftsiz.x * xtex.d - xtex.u;
                    ytex.u = ftsiz.x * ytex.d - ytex.u;
                    otex.u = ftsiz.x * otex.d - otex.u;
                }

                // sprite panning
                if (spriteext[spritenum].xpanning)
                {
                    float const f = ((float)(spriteext[spritenum].xpanning) * (1.0f / 255.f)) * ftsiz.x;
                    ytex.u -= ytex.d * f;
                    otex.u -= otex.d * f;
                    drawpoly_srepeat = 1;
                }

                if (spriteext[spritenum].ypanning)
                {
                    float const f = ((float)(spriteext[spritenum].ypanning) * (1.0f / 255.f)) * ftsiz.y;
                    ytex.v -= ytex.d * f;
                    otex.v -= otex.d * f;
                    drawpoly_trepeat = 1;
                }

				vec2_16_t tempsiz = { (int16_t)tsiz.x, (int16_t)tsiz.y };
				pow2xsplit = 0;

                polymost_drawpoly(pxy, npoints, method, tempsiz);

                drawpoly_srepeat = 0;
                drawpoly_trepeat = 0;
            }

            break;

        case 3:  // Voxel sprite
            break;
    }

    if (automapping == 1 && (unsigned)spritenum < MAXSPRITES)
        show2dsprite[spritenum>>3] |= pow2char[spritenum&7];

_drawsprite_return:
    polymost_identityrotmat();
}

EDUKE32_STATIC_ASSERT((int)RS_YFLIP == (int)HUDFLAG_FLIPPED);

//sx,sy       center of sprite; screen coords*65536
//z           zoom*65536. > is zoomed in
//a           angle (0 is default)
//dastat&1    1:translucence
//dastat&2    1:auto-scale mode (use 320*200 coordinates)
//dastat&4    1:y-flip
//dastat&8    1:don't clip to startumost/startdmost
//dastat&16   1:force point passed to be top-left corner, 0:Editart center
//dastat&32   1:reverse translucence
//dastat&64   1:non-masked, 0:masked
//dastat&128  1:draw all pages (permanent)
//cx1,...     clip window (actual screen coords)

void polymost_dorotatespritemodel(int32_t sx, int32_t sy, int32_t z, int16_t a, int16_t picnum,
    int8_t dashade, uint8_t dapalnum, int32_t dastat, uint8_t daalpha, uint8_t dablend, int32_t uniqid)
{
    float d, cosang, sinang, cosang2, sinang2;
    float m[4][4];

    const int32_t tilenum = Ptile2tile(picnum, dapalnum);

    if (tile2model[tilenum].modelid == -1 || tile2model[tilenum].framenum == -1)
        return;

    vec3f_t vec1;

    uspritetype tspr;
    Bmemset(&tspr, 0, sizeof(spritetype));

    hudtyp const * const hud = tile2model[tilenum].hudmem[(dastat&4)>>2];

    if (!hud || hud->flags & HUDFLAG_HIDE)
        return;

    polymost_outputGLDebugMessage(3, "polymost_dorotatespritemodel(sx:%d, sy:%d, z:%d, a:%hd, picnum:%hd, dashade:%hhd, dapalnum:%hhu, dastat:%d, daalpha:%hhu, dablend:%hhu, uniqid:%d)",
                                  sx, sy, z, a, picnum, dashade, dapalnum, dastat, daalpha, dablend, uniqid);

    float const ogchang = gchang; gchang = 1.f;
    float const ogshang = gshang; gshang = 0.f; d = (float) z*(1.0f/(65536.f*16384.f));
    float const ogctang = gctang; gctang = (float) sintable[(a+512)&2047]*d;
    float const ogstang = gstang; gstang = (float) sintable[a&2047]*d;
    int const ogshade  = globalshade;  globalshade  = dashade;
    int const ogpal    = globalpal;    globalpal    = (int32_t) ((uint8_t) dapalnum);
    double const ogxyaspect = gxyaspect; gxyaspect = 1.f;
    int const oldviewingrange = viewingrange; viewingrange = 65536;
    float const oldfviewingrange = fviewingrange; fviewingrange = 65536.f;
    float const ogvrcorrection = gvrcorrection; gvrcorrection = 1.f;

    polymost_updaterotmat();

    vec1 = hud->add;

    if (!(hud->flags & HUDFLAG_NOBOB))
    {
        vec2f_t f = { (float)sx * (1.f / 65536.f), (float)sy * (1.f / 65536.f) };

        if (dastat & RS_TOPLEFT)
        {
            vec2_16_t siz = tilesiz[picnum];
            vec2_16_t off = { (int16_t)((siz.x >> 1) + picanm[picnum].xofs), (int16_t)((siz.y >> 1) + picanm[picnum].yofs) };

            d = (float)z * (1.0f / (65536.f * 16384.f));
            cosang2 = cosang = (float)sintable[(a + 512) & 2047] * d;
            sinang2 = sinang = (float)sintable[a & 2047] * d;

            if ((dastat & RS_AUTO) || (!(dastat & RS_NOCLIP)))  // Don't aspect unscaled perms
            {
                d = (float)xyaspect * (1.0f / 65536.f);
                cosang2 *= d;
                sinang2 *= d;
            }

            vec2f_t const foff = { (float)off.x, (float)off.y };
            f.x += -foff.x * cosang2 + foff.y * sinang2;
            f.y += -foff.x * sinang  - foff.y * cosang;
        }

        if (!(dastat & RS_AUTO))
        {
            vec1.x += f.x / ((float)(xdim << 15)) - 1.f;  //-1: left of screen, +1: right of screen
            vec1.y += f.y / ((float)(ydim << 15)) - 1.f;  //-1: top of screen, +1: bottom of screen
        }
        else
        {
            vec1.x += f.x * (1.0f / 160.f) - 1.f;  //-1: left of screen, +1: right of screen
            vec1.y += f.y * (1.0f / 100.f) - 1.f;  //-1: top of screen, +1: bottom of screen
        }
    }
    tspr.ang = hud->angadd+globalang;


    if (dastat & RS_YFLIP) { vec1.x = -vec1.x; vec1.y = -vec1.y; }

    // In Polymost, we don't care if the model is very big
    {
        tspr.xrepeat = tspr.yrepeat = 32;

        tspr.x = globalposx + Blrintf((gcosang*vec1.z - gsinang*vec1.x)*16384.f);
        tspr.y = globalposy + Blrintf((gsinang*vec1.z + gcosang*vec1.x)*16384.f);
        tspr.z = globalposz + Blrintf(vec1.y * (16384.f * 0.8f));
    }

    tspr.picnum = picnum;
    tspr.shade = dashade;
    tspr.pal = dapalnum;
    tspr.owner = uniqid+MAXSPRITES;
    // 1 -> 1
    // 32 -> 32*16 = 512
    // 4 -> 8
    tspr.cstat = globalorientation = (dastat&RS_TRANS1) | ((dastat&RS_TRANS2)<<4) | ((dastat&RS_YFLIP)<<1);

    if ((dastat&(RS_AUTO|RS_NOCLIP)) == RS_AUTO)
    {
		GLInterface.SetViewport(windowxy1.x, ydim-(windowxy2.y+1), windowxy2.x-windowxy1.x+1, windowxy2.y-windowxy1.y+1);
        glox1 = -1;
    }
    else
    {
		GLInterface.SetViewport(0, 0, xdim, ydim);
        glox1 = -1; //Force fullscreen (glox1=-1 forces it to restore)
    }

    if (videoGetRenderMode() < REND_POLYMER)
    {
        Bmemset(m, 0, sizeof(m));

        if ((dastat&(RS_AUTO|RS_NOCLIP)) == RS_AUTO)
        {
            float f = 1.f;
            int32_t fov = hud->fov;
            if (fov != -1)
                f = 1.f/tanf(((float)fov * 2.56f) * ((.5f * fPI) * (1.0f/2048.f)));

            m[0][0] = f*fydimen; m[0][2] = 1.f;
            m[1][1] = f*fxdimen; m[1][2] = 1.f;
            m[2][2] = 1.f; m[2][3] = fydimen;
            m[3][2] =-1.f;
        }
        else
        {
            m[0][0] = m[2][3] = 1.f;
            m[1][1] = fxdim/fydim;
            m[2][2] = 1.0001f;
            m[3][2] = 1-m[2][2];
        }
			
		GLInterface.SetMatrix(Matrix_Projection, &m[0][0]);
		VSMatrix identity(0);
		GLInterface.SetMatrix(Matrix_ModelView, &identity);
    }

    if (hud->flags & HUDFLAG_NODEPTH)
        GLInterface.EnableDepthTest(false);
    else
    {
        static int32_t onumframes = 0;

        GLInterface.EnableDepthTest(true);

        if (onumframes != numframes)
        {
            onumframes = numframes;
			GLInterface.ClearDepth();
		}
    }

    spriteext[tspr.owner].alpha = daalpha * (1.0f / 255.0f);
    tspr.blend = dablend;

	GLInterface.SetFogEnabled(false);

    if (videoGetRenderMode() == REND_POLYMOST)
        polymost_mddraw(&tspr);

    if (!nofog) GLInterface.SetFogEnabled(true);

    gvrcorrection = ogvrcorrection;
    viewingrange = oldviewingrange;
    fviewingrange = oldfviewingrange;
    gxyaspect = ogxyaspect;
    globalshade  = ogshade;
    globalpal    = ogpal;
    gchang = ogchang;
    gshang = ogshang;
    gctang = ogctang;
    gstang = ogstang;
    polymost_identityrotmat();
}

void polymost_dorotatesprite(int32_t sx, int32_t sy, int32_t z, int16_t a, int16_t picnum,
                             int8_t dashade, uint8_t dapalnum, int32_t dastat, uint8_t daalpha, uint8_t dablend,
                             int32_t cx1, int32_t cy1, int32_t cx2, int32_t cy2, int32_t uniqid)
{
    if (usemodels && tile2model[picnum].hudmem[(dastat&4)>>2])
    {
        polymost_dorotatespritemodel(sx, sy, z, a, picnum, dashade, dapalnum, dastat, daalpha, dablend, uniqid);
        return;
    }

    polymost_outputGLDebugMessage(3, "polymost_dorotatesprite(sx:%d, sy:%d, z:%d, a:%hd, picnum:%hd, dashade:%hhd, dapalnum:%hhu, dastat:%d, daalpha:%hhu, dablend:%hhu, cx1:%d, cy1:%d, cx2:%d, cy2:%d, uniqid:%d)",
                                  sx, sy, z, a, picnum, dashade, dapalnum, dastat, daalpha, dablend, cx1, cy1, cx2, cy2, uniqid);

	GLInterface.SetViewport(0,0,xdim,ydim); glox1 = -1; //Force fullscreen (glox1=-1 forces it to restore)
	auto oldproj = GLInterface.GetMatrix(Matrix_Projection);
	auto oldmv = GLInterface.GetMatrix(Matrix_ModelView);
	
    globvis = 0;
    globvis2 = 0;
	GLInterface.SetClamp(1+2);
	GLInterface.SetVisibility(globvis2, fviewingrange);

    int32_t const ogpicnum = globalpicnum;
    globalpicnum = picnum;
    int32_t const  ogshade = globalshade;
    globalshade = dashade;
    int32_t const  ogpal = globalpal;
    globalpal = (int32_t)((uint8_t)dapalnum);
    float const  oghalfx = ghalfx;
    ghalfx = fxdim * .5f;
    float const  oghalfy = ghalfy;
    ghalfy = fydim * .5f;
    float const  ogrhalfxdown10 = grhalfxdown10;
    grhalfxdown10 = 1.f / (ghalfx * 1024.f);
    float const  ogrhalfxdown10x = grhalfxdown10x;
    grhalfxdown10x = grhalfxdown10;
    float const  oghoriz = ghoriz;
    ghoriz = fydim * .5f;
    int32_t const  ofoffset = frameoffset;
    frameoffset = frameplace;
    float const  ogchang = gchang;
    gchang = 1.f;
    float const  ogshang = gshang;
    gshang = 0.f;
    float const  ogctang = gctang;
    gctang = 1.f;
    float const  ogstang = gstang;
    gstang = 0.f;
    float const  ogvrcorrection = gvrcorrection;
    gvrcorrection = 1.f;

    polymost_updaterotmat();

    float m[4][4];

    Bmemset(m,0,sizeof(m));
    m[0][0] = m[2][3] = 1.0f;
    m[1][1] = fxdim / fydim;
    m[2][2] = 1.0001f;
    m[3][2] = 1 - m[2][2];
	
	GLInterface.SetMatrix(Matrix_Projection, &m[0][0]);
	VSMatrix identity(0);
	GLInterface.SetMatrix(Matrix_ModelView, &identity);
    GLInterface.EnableDepthTest(false);

    int32_t method = DAMETH_CLAMPED; //Use OpenGL clamping - dorotatesprite never repeats

    if (!(dastat & RS_NOMASK))
    {
        GLInterface.EnableAlphaTest(true);
        GLInterface.EnableBlend(true);

        if (dastat & RS_TRANS1)
            method |= (dastat & RS_TRANS2) ? DAMETH_TRANS2 : DAMETH_TRANS1;
        else
            method |= DAMETH_MASK;
    }
    else
    {
        GLInterface.EnableAlphaTest(false);
        GLInterface.EnableBlend(false);
    }

    handle_blend(!!(dastat & RS_TRANS1), dablend, !!(dastat & RS_TRANS2));


    drawpoly_alpha = daalpha * (1.0f / 255.0f);
    drawpoly_blend = dablend;

    vec2_16_t const siz = tilesiz[globalpicnum];
    vec2_16_t ofs = { 0, 0 };

    if (!(dastat & RS_TOPLEFT))
    {
        ofs = { int16_t(picanm[globalpicnum].xofs + (siz.x>>1)),
                int16_t(picanm[globalpicnum].yofs + (siz.y>>1)) };
    }

    if (dastat & RS_YFLIP)
        ofs.y = siz.y - ofs.y;

    int32_t ourxyaspect, ouryxaspect;
    dorotspr_handle_bit2(&sx, &sy, &z, dastat, cx1 + cx2, cy1 + cy2, &ouryxaspect, &ourxyaspect);

    int32_t cosang = mulscale14(sintable[(a + 512) & 2047], z);
    int32_t cosang2 = cosang;
    int32_t sinang = mulscale14(sintable[a & 2047], z);
    int32_t sinang2 = sinang;

    if ((dastat & RS_AUTO) || (!(dastat & RS_NOCLIP)))  // Don't aspect unscaled perms
    {
        cosang2 = mulscale16(cosang2,ourxyaspect);
        sinang2 = mulscale16(sinang2,ourxyaspect);
    }

    int32_t const cx = sx - ofs.x * cosang2 + ofs.y * sinang2;
    int32_t const cy = sy - ofs.x * sinang  - ofs.y * cosang;

    vec2_t pxy[8] = { { cx, cy },
                      { cx + siz.x * cosang2, cy + siz.x * sinang },
                      { 0, 0 },
                      { cx - siz.y * sinang2, cy + siz.y * cosang } };

    pxy[2]= { pxy[1].x + pxy[3].x - pxy[0].x,
              pxy[1].y + pxy[3].y - pxy[0].y };

    vec2_t const gxy = pxy[0];

    //Clippoly4

    int32_t n = 4, nn = 0, nz = 0;
    int32_t px2[8], py2[8];

    cx2++;
    cy2++;

    cx1 <<= 16;
    cy1 <<= 16;
    cx2 <<= 16;
    cy2 <<= 16;

    do
    {
        int32_t zz = nz+1; if (zz == n) zz = 0;
        int32_t const x1 = pxy[nz].x, x2 = pxy[zz].x-x1;
        if ((cx1 <= x1) && (x1 <= cx2)) { px2[nn] = x1; py2[nn] = pxy[nz].y; nn++; }
        int32_t fx = (x2 <= 0 ? cx2 : cx1), t = fx-x1;
        if ((t < x2) != (t < 0)) { px2[nn] = fx; py2[nn] = scale(pxy[zz].y-pxy[nz].y,t,x2) + pxy[nz].y; nn++; }
        fx = (x2 <= 0 ? cx1 : cx2); t = fx-x1;
        if ((t < x2) != (t < 0)) { px2[nn] = fx; py2[nn] = scale(pxy[zz].y-pxy[nz].y,t,x2) + pxy[nz].y; nn++; }
        nz = zz;
    }
    while (nz);

    n = 0;

    if (nn >= 3)
    {
        nz = 0;
        do
        {
            int32_t zz = nz+1; if (zz == nn) zz = 0;
            int32_t const y1 = py2[nz], y2 = py2[zz]-y1;
            if ((cy1 <= y1) && (y1 <= cy2)) { pxy[n].y = y1; pxy[n].x = px2[nz]; n++; }
            int32_t fy = (y2 <= 0 ? cy2 : cy1), t = fy - y1;
            if ((t < y2) != (t < 0)) { pxy[n].y = fy; pxy[n].x = scale(px2[zz]-px2[nz],t,y2) + px2[nz]; n++; }
            fy = (y2 <= 0 ? cy1 : cy2); t = fy - y1;
            if ((t < y2) != (t < 0)) { pxy[n].y = fy; pxy[n].x = scale(px2[zz]-px2[nz],t,y2) + px2[nz]; n++; }
            nz = zz;
        }
        while (nz);
    }

    if (n >= 3)
    {
        int32_t i = divscale32(1,z);
        int32_t xv = mulscale14(sintable[a&2047],i);
        int32_t yv = mulscale14(sintable[(a+512)&2047],i);
        int32_t xv2, yv2;
        if ((dastat&RS_AUTO) || (dastat&RS_NOCLIP)==0) //Don't aspect unscaled perms
        {
            yv2 = mulscale16(-xv,ouryxaspect);
            xv2 = mulscale16(yv,ouryxaspect);
        }
        else
        {
            yv2 = -xv;
            xv2 = yv;
        }

        int32_t lx = pxy[0].x;
        for (int v=n-1; v>0; v--)
            if (pxy[v].x < lx) lx = pxy[v].x;

        vec2_t oxy = { (lx>>16), 0 };
        int32_t x = (oxy.x<<16)-1-gxy.x;
        int32_t y = (oxy.y<<16)+65535-gxy.y;
        int32_t bx = dmulscale16(x,xv2,y,xv);
        int32_t by = dmulscale16(x,yv2,y,yv);

        if (dastat & RS_YFLIP)
        {
            yv = -yv;
            yv2 = -yv2;
            by = (siz.y<<16)-1-by;
        }

        vec2f_t fpxy[8];
        for (int v=0; v<n; v++)
            fpxy[v] = { float((pxy[v].x+8192)>>16), float((pxy[v].y+8192)>>16) };

        xtex.d = 0; ytex.d = 0; otex.d = 1.0;
        otex.u = (bx-(oxy.x-1+0.7)*xv2-(oxy.y+0.7)*xv)*(1.0/65536.0);
        xtex.u = xv2*(1.0/65536.0);
        ytex.u = xv*(1.0/65536.0);
        otex.v = (by-(oxy.x-1+0.7)*yv2-(oxy.y+0.7)*yv)*(1.0/65536.0);
        xtex.v = yv2*(1.0/65536.0);
        ytex.v = yv*(1.0/65536.0);

		GLInterface.SetFogEnabled(false);
        pow2xsplit = 0; polymost_drawpoly(fpxy,n,method, tilesiz[globalpicnum]);
        if (!nofog) GLInterface.SetFogEnabled(true);
    }

    GLInterface.EnableAlphaTest(false);
    GLInterface.EnableBlend(false);
	GLInterface.SetClamp(0);

	GLInterface.SetMatrix(Matrix_Projection, &oldproj);
	GLInterface.SetMatrix(Matrix_ModelView, &oldmv);

    globalpicnum = ogpicnum;
    globalshade  = ogshade;
    globalpal    = ogpal;
    ghalfx       = oghalfx;
    ghalfy       = oghalfy;
    grhalfxdown10 = ogrhalfxdown10;
    grhalfxdown10x = ogrhalfxdown10x;
    ghoriz       = oghoriz;
    frameoffset  = ofoffset;
    gchang = ogchang;
    gshang = ogshang;
    gctang = ogctang;
    gstang = ogstang;
    gvrcorrection = ogvrcorrection;

    polymost_identityrotmat();
}

static float trapextx[2];
static void drawtrap(float x0, float x1, float y0, float x2, float x3, float y1)
{
    if (y0 == y1) return;

    float px[4], py[4];
    int32_t n = 3;

    px[0] = x0; py[0] = y0;  py[2] = y1;
    if (x0 == x1) { px[1] = x3; py[1] = y1; px[2] = x2; }
    else if (x2 == x3) { px[1] = x1; py[1] = y0; px[2] = x3; }
    else               { px[1] = x1; py[1] = y0; px[2] = x3; px[3] = x2; py[3] = y1; n = 4; }

	auto data = GLInterface.AllocVertices(n);
	auto vt = data.second;
    for (bssize_t i=0; i<n; i++, vt++)
    {
        px[i] = min(max(px[i],trapextx[0]),trapextx[1]);
        vt->SetTexCoord(px[i]*xtex.u + py[i]*ytex.u + otex.u,
                      px[i]*xtex.v + py[i]*ytex.v + otex.v);
        vt->SetVertex(px[i],py[i]);
    }
	GLInterface.Draw(DT_TRIANGLE_FAN, data.first, n);
}

static void tessectrap(const float *px, const float *py, const int32_t *point2, int32_t numpoints)
{
    float x0, x1, m0, m1;
    int32_t i, j, k, z, i0, i1, i2, i3, npoints, gap, numrst;

    static int32_t allocpoints = 0, *slist = 0, *npoint2 = 0;
    typedef struct { float x, y, xi; int32_t i; } raster;
    static raster *rst = 0;
    if (numpoints+16 > allocpoints) //16 for safety
    {
        allocpoints = numpoints+16;
        rst = (raster *)Xrealloc(rst,allocpoints*sizeof(raster));
        slist = (int32_t *)Xrealloc(slist,allocpoints*sizeof(int32_t));
        npoint2 = (int32_t *)Xrealloc(npoint2,allocpoints*sizeof(int32_t));
    }

    //Remove unnecessary collinear points:
    for (i=0; i<numpoints; i++) npoint2[i] = point2[i];
    npoints = numpoints; z = 0;
    for (i=0; i<numpoints; i++)
    {
        j = npoint2[i]; if ((i < numpoints-1) && (point2[i] < i)) z = 3;
        if (j < 0) continue;
        k = npoint2[j];
        m0 = (px[j]-px[i])*(py[k]-py[j]);
        m1 = (py[j]-py[i])*(px[k]-px[j]);
        if (m0 < m1) { z |= 1; continue; }
        if (m0 > m1) { z |= 2; continue; }
        npoint2[i] = k; npoint2[j] = -1; npoints--; i--; //collinear
    }
    if (!z) return;
    trapextx[0] = trapextx[1] = px[0];
    for (i=j=0; i<numpoints; i++)
    {
        if (npoint2[i] < 0) continue;
        if (px[i] < trapextx[0]) trapextx[0] = px[i];
        if (px[i] > trapextx[1]) trapextx[1] = px[i];
        slist[j++] = i;
    }
    if (z != 3) //Simple polygon... early out
    {
		auto data = GLInterface.AllocVertices(npoints);
		auto vt = data.second;

        for (i=0; i<npoints; i++, vt++)
        {
            j = slist[i];
            vt->SetTexCoord(px[j]*xtex.u + py[j]*ytex.u + otex.u,
                          px[j]*xtex.v + py[j]*ytex.v + otex.v);
            vt->SetVertex(px[j],py[j]);
        }
		GLInterface.Draw(DT_TRIANGLE_FAN, data.first, npoints);
        return;
    }

    //Sort points by y's
    for (gap=(npoints>>1); gap; gap>>=1)
        for (i=0; i<npoints-gap; i++)
            for (j=i; j>=0; j-=gap)
            {
                if (py[npoint2[slist[j]]] <= py[npoint2[slist[j+gap]]]) break;
                k = slist[j]; slist[j] = slist[j+gap]; slist[j+gap] = k;
            }

    numrst = 0;
    for (z=0; z<npoints; z++)
    {
        i0 = slist[z]; i1 = npoint2[i0]; if (py[i0] == py[i1]) continue;
        i2 = i1; i3 = npoint2[i1];
        if (py[i1] == py[i3]) { i2 = i3; i3 = npoint2[i3]; }

        //i0        i3
        //  \      /
        //   i1--i2
        //  /      \ ~
        //i0        i3

        if ((py[i1] < py[i0]) && (py[i2] < py[i3])) //Insert raster
        {
            for (i=numrst; i>0; i--)
            {
                if (rst[i-1].xi*(py[i1]-rst[i-1].y) + rst[i-1].x < px[i1]) break;
                rst[i+1] = rst[i-1];
            }
            numrst += 2;

            if (i&1) //split inside area
            {
                j = i-1;

                x0 = (py[i1] - rst[j  ].y)*rst[j  ].xi + rst[j  ].x;
                x1 = (py[i1] - rst[j+1].y)*rst[j+1].xi + rst[j+1].x;
                drawtrap(rst[j].x,rst[j+1].x,rst[j].y,x0,x1,py[i1]);
                rst[j  ].x = x0; rst[j  ].y = py[i1];
                rst[j+3].x = x1; rst[j+3].y = py[i1];
            }

            m0 = (px[i0]-px[i1]) / (py[i0]-py[i1]);
            m1 = (px[i3]-px[i2]) / (py[i3]-py[i2]);
            j = ((px[i1] > px[i2]) || ((i1 == i2) && (m0 >= m1))) + i;
            k = (i<<1)+1 - j;
            rst[j].i = i0; rst[j].xi = m0; rst[j].x = px[i1]; rst[j].y = py[i1];
            rst[k].i = i3; rst[k].xi = m1; rst[k].x = px[i2]; rst[k].y = py[i2];
        }
        else
        {
            //NOTE:don't count backwards!
            if (i1 == i2) { for (i=0; i<numrst; i++) if (rst[i].i == i1) break; }
            else { for (i=0; i<numrst; i++) if ((rst[i].i == i1) || (rst[i].i == i2)) break; }
            j = i&~1;

            if ((py[i1] > py[i0]) && (py[i2] > py[i3])) //Delete raster
            {
                for (; j<=i+1; j+=2)
                {
                    x0 = (py[i1] - rst[j  ].y)*rst[j  ].xi + rst[j  ].x;
                    if ((i == j) && (i1 == i2)) x1 = x0; else x1 = (py[i1] - rst[j+1].y)*rst[j+1].xi + rst[j+1].x;
                    drawtrap(rst[j].x,rst[j+1].x,rst[j].y,x0,x1,py[i1]);
                    rst[j  ].x = x0; rst[j  ].y = py[i1];
                    rst[j+1].x = x1; rst[j+1].y = py[i1];
                }
                numrst -= 2; for (; i<numrst; i++) rst[i] = rst[i+2];
            }
            else
            {
                x0 = (py[i1] - rst[j  ].y)*rst[j  ].xi + rst[j  ].x;
                x1 = (py[i1] - rst[j+1].y)*rst[j+1].xi + rst[j+1].x;
                drawtrap(rst[j].x,rst[j+1].x,rst[j].y,x0,x1,py[i1]);
                rst[j  ].x = x0; rst[j  ].y = py[i1];
                rst[j+1].x = x1; rst[j+1].y = py[i1];

                if (py[i0] < py[i3]) { rst[i].x = px[i2]; rst[i].y = py[i2]; rst[i].i = i3; }
                else { rst[i].x = px[i1]; rst[i].y = py[i1]; rst[i].i = i0; }
                rst[i].xi = (px[rst[i].i] - rst[i].x) / (py[rst[i].i] - py[i1]);
            }
        }
    }
}

void polymost_fillpolygon(int32_t npoints)
{
    polymost_outputGLDebugMessage(3, "polymost_fillpolygon(npoints:%d)", npoints);

    globvis2 = 0;
	GLInterface.SetVisibility(globvis2, fviewingrange);

    globalx1 = mulscale16(globalx1,xyaspect);
    globaly2 = mulscale16(globaly2,xyaspect);
    xtex.u = ((float)asm1) * (1.f / 4294967296.f);
    xtex.v = ((float)asm2) * (1.f / 4294967296.f);
    ytex.u = ((float)globalx1) * (1.f / 4294967296.f);
    ytex.v = ((float)globaly2) * (-1.f / 4294967296.f);
    otex.u = (fxdim * xtex.u + fydim * ytex.u) * -0.5f + fglobalposx * (1.f / 4294967296.f);
    otex.v = (fxdim * xtex.v + fydim * ytex.v) * -0.5f - fglobalposy * (1.f / 4294967296.f);

    //Convert int32_t to float (in-place)
    for (bssize_t i=0; i<npoints; ++i)
    {
        ((float *)rx1)[i] = ((float)rx1[i])*(1.0f/4096.f);
        ((float *)ry1)[i] = ((float)ry1[i])*(1.0f/4096.f);
    }

    if (gloy1 != -1) polymostSet2dView(); //disables blending, texturing, and depth testing
    GLInterface.EnableAlphaTest(true);
    pthtyp const * const pth = our_texcache_fetch(DAMETH_NOMASK | (videoGetRenderMode() == REND_POLYMOST && r_useindexedcolortextures ? PTH_INDEXED : 0));

    if (pth)
    {
        polymost_bindPth(pth, -1);
    }

    polymost_updatePalette();

    uint8_t const maskprops = (globalorientation>>7)&DAMETH_MASKPROPS;
    handle_blend(maskprops > DAMETH_MASK, 0, maskprops == DAMETH_TRANS2);
    if (maskprops > DAMETH_MASK)
    {
        GLInterface.EnableBlend(true);
        GLInterface.SetColor(1.f, 1.f, 1.f, float_trans(maskprops, 0));
    }
    else
    {
        GLInterface.EnableBlend(false);
		GLInterface.SetColor(1.f, 1.f, 1.f);
    }

    tessectrap((float *)rx1,(float *)ry1,xb1,npoints);
}

static int32_t gen_font_glyph_tex(void)
{
    // construct a 256x128 texture for the font glyph matrix

	polymosttext = GLInterface.NewTexture();
    if (!polymosttext) return -1;

    char * const tbuf = (char *)Xmalloc(256*128*4);

    Bmemset(tbuf, 0, 256*128*4);

    char * cptr = (char *)textfont;

    for (bssize_t h=0; h<256; h++)
    {
        char *tptr = tbuf + (h%32)*8*4 + (h/32)*256*8*4;
        for (bssize_t i=0; i<8; i++)
        {
            for (bssize_t j=0; j<8; j++)
            {
                if (cptr[h*8+i] & pow2char[7-j])
                {
                    Bmemset(tptr+4*j, 255, 4);
                }
            }
            tptr += 256*4;
        }
    }

    cptr = (char *)smalltextfont;

    for (bssize_t h=0; h<256; h++)
    {
        char *tptr = tbuf + 256*64*4 + (h%32)*8*4 + (h/32)*256*8*4;
        for (bssize_t i=1; i<7; i++)
        {
            for (bssize_t j=2; j<6; j++)
            {
                if (cptr[h*8+i] & pow2char[7-j])
                {
                    Bmemset(tptr+4*(j-2), 255, 4);
                }
            }
            tptr += 256*4;
        }
    }

	polymosttext->CreateTexture(256, 128, false, false);
	polymosttext->LoadTexture((uint8_t*)tbuf); // RGBA
	polymosttext->SetSampler(Sampler2DNoFilter);
    Xfree(tbuf);

    return 0;
}

// These are used by the automap drawers
void renderDisableFog(void)
{
	GLInterface.SetFogEnabled(false);
}

void renderEnableFog(void)
{
	if (!nofog) GLInterface.SetFogEnabled(true);
}

int32_t polymost_printext256(int32_t xpos, int32_t ypos, int16_t col, int16_t backcol, const char *name, char fontsize)
{
    int const arbackcol = (unsigned)backcol < 256 ? backcol : 0;

    polymost_outputGLDebugMessage(3, "polymost_printtext256(xpos:%d, ypos:%d, col:%hd, backcol:%hd, name:%p, fontsize:%hhu)",
                                      xpos, ypos, col, backcol, name, fontsize);

    // FIXME?
    if (col < 0)
        col = 0;

    palette_t p, b;

    bricolor(&p, col);
    bricolor(&b, arbackcol);

    if (videoGetRenderMode() < REND_POLYMOST || !in3dmode() || (!polymosttext && gen_font_glyph_tex() < 0))
        return -1;

	GLInterface.BindTexture(0, polymosttext);
    
    polymostSet2dView();	// disables blending, texturing, and depth testing

    GLInterface.EnableAlphaTest(false);
	GLInterface.SetDepthMask(false);

	GLInterface.SetFogEnabled(false);
    // We want to have readable text in wireframe mode, too:
	GLInterface.SetWireframe(false);
	lastglpolygonmode = 0;

    if (backcol >= 0)
    {
        int const c = Bstrlen(name);

        GLInterface.SetColorub(b.r,b.g,b.b,255);

		auto data = GLInterface.AllocVertices(4);
		auto vt = data.second;

		vt->SetTexCoord(); vt->SetVertex(xpos, ypos); vt++;
		vt->SetTexCoord(); vt->SetVertex(xpos, ypos + (fontsize ? 6 : 8)); vt++;
		vt->SetTexCoord(); vt->SetVertex(xpos + (c << (3 - fontsize)), ypos + (fontsize ? 6 : 8)); vt++;
		vt->SetTexCoord(); vt->SetVertex(xpos+(c<<(3-fontsize)), ypos);

		GLInterface.Draw(DT_TRIANGLE_FAN, data.first, 4);
    }

    GLInterface.EnableBlend(true);
    GLInterface.SetColorub(p.r,p.g,p.b,255);

    vec2f_t const tc = { fontsize ? (4.f / 256.f) : (8.f / 256.f),
                         fontsize ? (6.f / 128.f) : (8.f / 128.f) };


    for (bssize_t c=0; name[c]; ++c)
    {
        if (name[c] == '^' && isdigit(name[c+1]))
        {
            char smallbuf[8];
            int bi = 0;

            while (isdigit(name[c+1]) && bi<3)
            {
                smallbuf[bi++]=name[c+1];
                c++;
            }

            smallbuf[bi++] = 0;

            if (col)
            {
                col = Batol(smallbuf);

                if ((unsigned) col >= 256)
                    col = 0;
            }

            bricolor(&p, col);

            GLInterface.SetColorub(p.r, p.g, p.b, 255);

            continue;
        }

        vec2f_t const t = { (float)(name[c] % 32) * (1.0f / 32.f),
                            (float)((name[c] / 32) + (fontsize * 8)) * (1.0f / 16.f) };

		auto data = GLInterface.AllocVertices(4);
		auto vt = data.second;

        vt->SetTexCoord(t.x, t.y);
        vt->SetVertex(xpos, ypos);
		vt++;

		vt->SetTexCoord(t.x + tc.x, t.y);
		vt->SetVertex(xpos + (8 >> fontsize), ypos);
		vt++;

		vt->SetTexCoord(t.x + tc.x, t.y + tc.y);
		vt->SetVertex(xpos + (8 >> fontsize), ypos + (fontsize ? 6 : 8));
		vt++;

		vt->SetTexCoord(t.x, t.y + tc.y);
		vt->SetVertex(xpos, ypos + (fontsize ? 6 : 8));
		vt++;

		GLInterface.Draw(DT_TRIANGLE_FAN, data.first, 4);

        xpos += (8>>fontsize);
    }


	GLInterface.SetDepthMask(true);

    if (!nofog) GLInterface.SetFogEnabled(true);

    return 0;
}

// Console commands by JBF
static int32_t gltexturemode(osdcmdptr_t parm)
{
    int32_t m;
    char *p;

    if (parm->numparms != 1)
    {
        OSD_Printf("Current texturing mode is %s\n", glfiltermodes[gltexfiltermode].name);
        OSD_Printf("  Vaild modes are:\n");
        for (m = 0; m < NUMGLFILTERMODES; m++)
            OSD_Printf("     %d - %s\n", m, glfiltermodes[m].name);

        return OSDCMD_OK;
    }

    m = Bstrtoul(parm->parms[0], &p, 10);
    if (p == parm->parms[0])
    {
        // string
        for (m = 0; m < NUMGLFILTERMODES; m++)
        {
            if (!Bstrcasecmp(parm->parms[0], glfiltermodes[m].name))
                break;
        }

        if (m == NUMGLFILTERMODES)
            m = gltexfiltermode;   // no change
    }
    else
    {
        m = clamp(m, 0, NUMGLFILTERMODES-1);
    }

    gltexfiltermode = m;
    gltexapplyprops();

    OSD_Printf("Texture filtering mode changed to %s\n", glfiltermodes[gltexfiltermode].name);

    return OSDCMD_OK;
}

static int osdcmd_cvar_set_polymost(osdcmdptr_t parm)
{
    int32_t r = osdcmd_cvar_set(parm);

    if (xdim == 0 || ydim == 0 || bpp == 0) // video not set up yet
    {

        return r;
    }

    if (r == OSDCMD_OK)
    {
        if (!Bstrcasecmp(parm->name, "r_swapinterval"))
            vsync = videoSetVsync(vsync);
        else if (!Bstrcasecmp(parm->name, "r_downsize"))
        {
            if (r_downsizevar == -1)
                r_downsizevar = r_downsize;

            if (in3dmode() && r_downsize != r_downsizevar)
            {
                videoResetMode();
                if (videoSetGameMode(fullscreen,xres,yres,bpp,upscalefactor))
                    OSD_Printf("restartvid: Reset failed...\n");
            }

            r_downsizevar = r_downsize;
        }
        else if (!Bstrcasecmp(parm->name, "r_anisotropy"))
            gltexapplyprops();
        else if (!Bstrcasecmp(parm->name, "r_texfilter"))
            gltexturemode(parm);
    }

    return r;
}

void polymost_initosdfuncs(void)
{
    uint32_t i;

    static osdcvardata_t cvars_polymost[] =
    {
#ifdef USE_GLEXT
        { "r_detailmapping","enable/disable detail mapping",(void *) &r_detailmapping, CVAR_BOOL, 0, 1 },
        { "r_glowmapping","enable/disable glow mapping",(void *) &r_glowmapping, CVAR_BOOL, 0, 1 },
#endif
        { "r_polygonmode","debugging feature",(void *) &r_polygonmode, CVAR_INT | CVAR_NOSAVE, 0, 3 },

        { "r_animsmoothing","enable/disable model animation smoothing",(void *) &r_animsmoothing, CVAR_BOOL, 0, 1 },
        { "r_anisotropy", "changes the OpenGL texture anisotropy setting", (void *) &glanisotropy, CVAR_INT|CVAR_FUNCPTR, 0, 16 },
        { "r_hightile","enable/disable hightile texture rendering",(void *) &usehightile, CVAR_BOOL, 0, 1 },
        { "r_models", "enable/disable model rendering", (void *)&usemodels, CVAR_BOOL, 0, 1 },
        { "r_nofog", "enable/disable GL fog", (void *)&nofog, CVAR_BOOL, 0, 1},
        { "r_parallaxskyclamping","enable/disable parallaxed floor/ceiling sky texture clamping", (void *) &r_parallaxskyclamping, CVAR_BOOL, 0, 1 },
        { "r_parallaxskypanning","enable/disable parallaxed floor/ceiling panning when drawing a parallaxing sky", (void *) &r_parallaxskypanning, CVAR_BOOL, 0, 1 },
        { "r_projectionhack", "enable/disable projection hack", (void *) &glprojectionhacks, CVAR_INT, 0, 2 },
        { "r_shadeinterpolate", "enable/disable shade interpolation", (void *) &r_shadeinterpolate, CVAR_BOOL, 0, 1 },
        { "r_shadescale","multiplier for shading",(void *) &shadescale, CVAR_FLOAT, 0, 10 },
        { "r_shadescale_unbounded","enable/disable allowance of complete blackness",(void *) &shadescale_unbounded, CVAR_BOOL, 0, 1 },
        { "r_swapinterval","sets the GL swap interval (VSync)",(void *) &vsync, CVAR_INT|CVAR_FUNCPTR, -1, 1 },
        { "r_texfilter", "changes the texture filtering settings (may require restart)", (void *) &gltexfiltermode, CVAR_INT|CVAR_FUNCPTR, 0, 5 },
        { "r_useindexedcolortextures", "enable/disable indexed color texture rendering", (void *) &r_useindexedcolortextures, CVAR_INT, 0, 1 },

        { "r_yshearing", "enable/disable y-shearing", (void*) &r_yshearing, CVAR_BOOL, 0, 1 },
		{ "fixpalette", "", (void*)& fixpalette, CVAR_INT, 0, 256 },
		{ "fixpalswap", "", (void*)& fixpalswap, CVAR_INT, 0, 256 },

    };

    for (i=0; i<ARRAY_SIZE(cvars_polymost); i++)
        OSD_RegisterCvar(&cvars_polymost[i], (cvars_polymost[i].flags & CVAR_FUNCPTR) ? osdcmd_cvar_set_polymost : osdcmd_cvar_set);
}

void polymost_precache(int32_t dapicnum, int32_t dapalnum, int32_t datype)
{
    // dapicnum and dapalnum are like you'd expect
    // datype is 0 for a wall/floor/ceiling and 1 for a sprite
    //    basically this just means walls are repeating
    //    while sprites are clamped

    if (videoGetRenderMode() < REND_POLYMOST) return;
   if ((dapalnum < (MAXPALOOKUPS - RESERVEDPALS)) && (palookup[dapalnum] == NULL)) return;//dapalnum = 0;

    //OSD_Printf("precached %d %d type %d\n", dapicnum, dapalnum, datype);
    hicprecaching = 1;
    texcache_fetch(dapicnum, dapalnum, 0, (datype & 1)*(DAMETH_MASK));
    hicprecaching = 0;

    if (datype == 0 || !usemodels) return;

    int const mid = md_tilehasmodel(dapicnum, dapalnum);

    if (mid < 0 || models[mid]->mdnum < 2) return;

    int const surfaces = (models[mid]->mdnum == 3) ? ((md3model_t *)models[mid])->head.numsurfs : 0;

    for (int i = 0; i <= surfaces; i++)
        mdloadskin((md2model_t *)models[mid], 0, dapalnum, i);
}

void PrecacheHardwareTextures(int nTile)
{
	// PRECACHE
	// This really *really* needs improvement on the game side - the entire precaching logic has no clue about the different needs of a hardware renderer.

	polymost_precache(nTile, 0, 1);

	if (r_detailmapping)
		polymost_precache(nTile, DETAILPAL, 1);

	if (r_glowmapping)
		polymost_precache(nTile, GLOWPAL, 1);

}
