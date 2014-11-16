#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include <string.h>

#include "Flare.h"
#include "Quad.h"
#include "Game.h"


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
			if (sscanf_s(buf, "%s %lf %lf ( %d %d %d %d )", name, _countof(name), &dDist, &dSize, &a, &r, &g, &b))
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

	newFlare(1);
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


void    FLARE_render(FLARE_DEF *flare, int lx, int ly, int cx, int cy, Game* game)
{
	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	FLARE_ELEMENT_DEF    *element;

	// Compute how far off-center the flare source is.
	maxflaredist = isqrt(cx*cx + cy*cy);
	flaredist = isqrt((lx - cx)*(lx - cx) +
		(ly - cy)*(ly - cy));
	flaredist = (maxflaredist - flaredist);
	flaremaxsize = (int)(glutGet(GLUT_WINDOW_WIDTH)* flare->fMaxSize);
	flarescale = (int)(glutGet(GLUT_WINDOW_WIDTH) * flare->fScale);

	// Destination is opposite side of centre from source
	dx = cx + (cx - lx);
	dy = cy + (cy - ly);

	// Render each element.
	for (i = 0; i < flare->nPieces; ++i)
	{
		element = &flare->element[i];

		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - element->fDistance)*lx + element->fDistance*dx);
		py = (int)((1.0f - element->fDistance)*ly + element->fDistance*dy);

		// Piece size are 0 to 1; flare size is proportion of
		// screen width; scale by flaredist/maxflaredist.
		width = (int)((flaredist*flarescale*element->fSize) / maxflaredist);

		// Width gets clamped, to allows the off-axis flares
		// to keep a good size without letting the elements get
		// too big when centered.
		if (width > flaremaxsize)
		{
			width = flaremaxsize;
		}

		// Flare elements are square (round) so height is just
		// width scaled by aspect ratio.
		height = HEIGHTFROMWIDTH(width);
		alpha = (flaredist*(element->argb >> 24)) / maxflaredist;

		Stack* modelview = game->getModelViewStack();

		if (width > 1)
		{
			//unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);
			float pos[3] = { px - width / 2, py - height / 2, 0.0 };
			Quad* quad = new Quad(pos, game);
			float color[4] = { 1.0, 1.0, 1.0, 0.0};
			quad->setAmbient(color);
			quad->setDiffuse(color);
			quad->setSpecular(color);
			quad->setTexCount(2);
			//element->texture!!!
			modelview->push();
				quad->draw();
			modelview->pop();
			//drawQuad(px - width / 2, py - height / 2, width, height, element->texture, argb);
		}
	}
}

int     xFlare = 10;
int     yFlare = 10;

void render(Game* game)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float pos[3] = { 0.0f, 0.0f, 0.0f };
	Quad *quad = new Quad(pos, game);

	float color1[4] = { 1.0, 1.0, 1.0, 1.0 };
	float color2[4] = { 1.0, 0.4, 0.4, 1.0 };
	float color3[4] = { 1.0, 0.0, 0.0, 0.0 };
	float color4[4] = { 1.0, 1.0, 1.0, 0.8 };

	Stack* modelview = game->getModelViewStack();

	switch (nBackground)
	{
	case BACK_TEXTURED:
		//texBack
		quad->setTexCount(1);
		quad->setAmbient(color1);
		quad->setDiffuse(color1);
		quad->setSpecular(color1);
		modelview->push();
			quad->draw();
		modelview->pop();
		//drawQuad( 0, 0, SCREENwidth, SCREENheight, texBack, 0xffffffff );
		break;
	case BACK_COLOUR0:
		quad->setTexCount(0);
		quad->setAmbient(color2);
		quad->setDiffuse(color2);
		quad->setSpecular(color2);
		modelview->push();
			quad->draw();
		modelview->pop();
		// drawQuad( 0, 0, SCREENwidth, SCREENheight, NULL, 0xff6060ff );
		break;
	case BACK_COLOUR1:
		quad->setTexCount(0);
		quad->setAmbient(color3);
		quad->setDiffuse(color3);
		quad->setSpecular(color3);
		modelview->push();
			quad->draw();
		modelview->pop();
		//drawQuad( 0, 0, SCREENwidth, SCREENheight, NULL, 0xff000000 );
		break;
	}

	Quad* quad2 = new Quad(pos, game);
	if (bShowSun)
		//texSun
		quad2->setTexCount(2);
		quad2->setAmbient(color4);
		quad2->setDiffuse(color4);
		quad2->setSpecular(color4);
		modelview->push();
			quad->draw();
		modelview->pop();
		//drawQuad(xFlare - SUNWIDTH / 2, yFlare - SUNHEIGHT / 2, SUNWIDTH, SUNHEIGHT, texSun, 0xffffffe0);

		FLARE_render(&renderFlare, xFlare, yFlare, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, game);
}

void Flare::draw(Game* game)
{
	render(game);
}
