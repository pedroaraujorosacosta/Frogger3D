#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "Flare.h"



typedef struct TEXTURE_DEF
{
	char    *filename;
	int     width;
	int     height;
	void    *pixels;
	unsigned char   *memory;        // file buffer; free this when done with texture
}
TEXTURE_DEF;

enum
{
	TEX_CRCL,
	TEX_HXGN,
	TEX_RING,
	TEX_FLAR,

	NPIECETEXTURES,

	TEX_SUN = NPIECETEXTURES,
	TEX_BACK,

	NTEXTURES
};

enum
{
	BACK_TEXTURED,
	BACK_COLOUR0,
	BACK_COLOUR1,

	NBACK
};



TEXTURE_DEF *texBack = NULL;
TEXTURE_DEF *texSun = NULL;
int     nBackground = BACK_TEXTURED;
int     bShowSun = 0;


TEXTURE_DEF gTextures[NTEXTURES] =
{
	{ "crcl.raw" },
	{ "hxgn.raw" },
	{ "ring.raw" },
	{ "flar.raw" },
	{ "sun.raw" },
	{ "back.raw" }
};


void TM_loadTextures(void)
{
	int i;
	

	TEXTURE_DEF *tex = &gTextures[0];

	for (i = 0; i < NTEXTURES; ++i, ++tex)
	{
		//FILE    *f = fopen(tex->filename, "rb");
		errno_t err = -1;
		FILE    *f;
		err = fopen_s(&f, tex->filename, "rb");
		if (f == NULL || err == -1)
		{
			printf("Error loading tex file.\n");
		}

		int     nBytes;

		tex->height = 0;
		tex->width = 0;
		tex->pixels = NULL;
		tex->memory = NULL;

		if (f)
		{
			int             j;
			unsigned char   *ppix;

			// Find file length by seeking to end of file.
			fseek(f, 0, SEEK_END);
			nBytes = ftell(f);
			fseek(f, 0, SEEK_SET);

			tex->memory = (unsigned char*)malloc(nBytes);
			fread(tex->memory, 1, nBytes, f);
			fclose(f);

			// Read little-endian texture sizes in machine-independent way.
			tex->width = (tex->memory[1] << 8) | (tex->memory[0]);
			tex->height = (tex->memory[3] << 8) | (tex->memory[2]);
			tex->pixels = tex->memory + 4;

			// Fix ARGB --> RGBA pixel byte order to suit OpenGL pixel format.
			j = tex->width * tex->height;
			ppix = (unsigned char*)tex->pixels;
			while (j--)
			{
				unsigned char b = ppix[0];
				unsigned char g = ppix[1];
				unsigned char r = ppix[2];
				unsigned char a = ppix[3];

				ppix[0] = r;
				ppix[1] = g;
				ppix[2] = b;
				ppix[3] = a;

				ppix += 4;
			}
		}
	}
}



TEXTURE_DEF* TM_getNamedTexture(char *name)
{
	int     i;

	for (i = 0; i < NTEXTURES; ++i)
	{
		if (strncmp(name, gTextures[i].filename, strlen(name)) == 0)
			return &gTextures[i];
	}

	return NULL;

}

typedef struct FLARE_ELEMENT_DEF
{
	struct TEXTURE_DEF  *texture;

	float           fDistance;        // Distance along ray from source (0.0-1.0)
	float           fSize;            // Size relative to flare envelope (0.0-1.0)
	unsigned int    argb;            // ARGB for intensity mapping
}
FLARE_ELEMENT_DEF;

typedef struct FLARE_DEF
{
	float           fScale;     // Scale factor for adjusting overall size of flare elements.
	float           fMaxSize;   // Max size of largest element, as proportion of screen width (0.0-1.0)
	int             nPieces;    // Number of elements in use


	FLARE_ELEMENT_DEF    element[FLARE_MAXELEMENTSPERFLARE];
} FLARE_DEF;

FLARE_DEF       renderFlare;



float flare_range(){
	return ((rand() % (FLARE_MAXELEMENTSPERFLARE-FLARE_MINELEMENTSPERFLARE+1)) + (FLARE_MINELEMENTSPERFLARE));
}


TEXTURE_DEF *TM_getNthTexture(int n)
{
	if ((n < 0) || (n >= NTEXTURES))
		return NULL;

	return &gTextures[n];
}

void    FLARE_randomize(FLARE_DEF *flare,
	int nTextures,
	int nPieces,
	float fMaxSize,
	unsigned int minColour,
	unsigned int maxColour)
{
	int             i;
	float           fFracDist;
	float           fEnvelopeSize;
	FLARE_ELEMENT_DEF *element;

	// Initialize flare with requested number of elements, with
	// random placement, colour, and size of elements.
	flare->nPieces = nPieces;
	flare->fScale = fMaxSize;
	flare->fMaxSize = fMaxSize;
	fFracDist = 1.0f / (float)(flare->nPieces - 1);

	for (i = 0; i < flare->nPieces; ++i)
	{
		element = &flare->element[i];
		element->fDistance = (fFracDist*i) + FLARE_FRANGE(0, fFracDist);

		// Envelope size is maximum at ends of line, minimum in the middle (i.e. two
		// cones, touching at the tips).
		fEnvelopeSize = (float)fabs(1.0f - 2 * element->fDistance);

		element->fSize = FLARE_FRANGE(0.6f, 1.0f) * fEnvelopeSize;
		element->argb = FLARE_RANGE(minColour & 0xff000000, maxColour & 0xff000000) |
			FLARE_RANGE(minColour & 0x00ff0000, maxColour & 0x00ff0000) |
			FLARE_RANGE(minColour & 0x0000ff00, maxColour & 0x0000ff00) |
			FLARE_RANGE(minColour & 0x000000ff, maxColour & 0x000000ff);
		element->texture = TM_getNthTexture(FLARE_RANGE(0, nTextures - 1));
	}
}



void loadFlareFile(FLARE_DEF *flare, char *filename)
{
	int     n = 0;
	FILE    *f;
	char    buf[256];

	memset(flare, 0, sizeof(FLARE_DEF));

	//f = fopen(filename, "r");
	errno_t err = -1;
	err = fopen_s(&f, filename, "r");
	if (f == NULL || err == -1)
	{
		printf("Error loading tex file.\n");
		exit(-1);
	}
	if (f)
	{
		fgets(buf, sizeof(buf), f);
		sscanf_s(buf, "%f %f", &flare->fScale, &flare->fMaxSize);

		while (!feof(f))
		{
			char            name[8] = { '\0', };
			double          dDist = 0.0, dSize = 0.0;
			unsigned int    a = 0, r = 0, g = 0, b = 0;

			fgets(buf, sizeof(buf), f);
			if (sscanf_s(buf, "%s %lf %lf ( %d %d %d %d )",
				name, &dDist, &dSize, &a, &r, &g, &b))
			{
				flare->element[n].texture = TM_getNamedTexture(name);
				flare->element[n].fDistance = (float)dDist;
				flare->element[n].fSize = (float)dSize;
				flare->element[n].argb = (a << 24) | (r << 16) | (g << 8) | (b << 0);

				++n;
			}
		}

		flare->nPieces = n;

		fclose(f);
	}
}

void newFlare(int bFromFile)
{
	if (bFromFile)
		loadFlareFile(&renderFlare, "flare.txt");
	else
		FLARE_randomize(&renderFlare, NPIECETEXTURES, flare_range(), FLARE_MAXSIZE, FLARE_MINCOLOUR, FLARE_MAXCOLOUR);
}

Flare::Flare() {

	TM_loadTextures();
	texSun = TM_getNamedTexture("sun");
	if (texSun)
		bShowSun = 1;
	texBack = TM_getNamedTexture("back");
	if (texBack)
		nBackground = BACK_TEXTURED;
	else
		nBackground = BACK_COLOUR0;

	newFlare(0);
}

void        TM_purgeTextures(void)
{
	int         i;
	TEXTURE_DEF *tex = &gTextures[0];

	for (i = 0; i < NTEXTURES; ++i, ++tex)
	{
		if (tex->memory)
			free(tex->memory);
	}
}

Flare::~Flare() {
	TM_purgeTextures();
}