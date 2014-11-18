#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include <string.h>

#include "Flare.h"
#include "Quad.h"
#include "Game.h"
#include "Matrix.h"

#define NORMALIZED_X(x) (x) * 2.0f / glutGet(GLUT_WINDOW_WIDTH)
#define NORMALIZED_Y(y) (y) * 2.0f / glutGet(GLUT_WINDOW_HEIGHT)
#define FLARE_ERROR -1

struct TexturedQuads 
{
	GLuint texID;
	int tUnit;
	Quad *quad;
	
	TexturedQuads(GLuint texID, Quad *quad, int tUnit) { this->texID = texID; this->quad = quad; this->tUnit = tUnit; }
};

std::vector<TexturedQuads> quads;

/*typedef struct TEXTURE_DEF
{
	char    *filename;
	int     width;
	int     height;
	void    *pixels;
	unsigned char   *memory;        // file buffer; free this when done with texture
}
TEXTURE_DEF;*/

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

GLuint Flare::TM_setTexture(TEXTURE_DEF *tex)
{
	GLuint texID;

	if (tex)
	{
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D,
			0,                  // level
			GL_RGBA,			// components
			tex->width,
			tex->height,
			0,                  // border
			GL_RGBA,            // format
			GL_UNSIGNED_BYTE,   //type
			tex->pixels);

		checkOpenGLError("Error loading flare texture");
		return texID;
	}
	else
		return FLARE_ERROR;
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
			if (sscanf_s(buf, "%s %lf %lf ( %d %d %d %d )", name, _countof(name), &dDist, &dSize, &a, &r, &g, &b) && !feof(f))
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

Flare::Flare(Game *game) {

	this->game = game;
	xFlare = 10;
	yFlare = 10;

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
	
	// TODO
	
	float pos[3] = { 0.0f, 0.0f, 0.0f };
	// first, load all the textures and store their names
	int numQuads = renderFlare.nPieces;
	GLuint texID;
	int tUnit = 0;
	for (int i = 0; i < numQuads; i++)
	{
		if (!hasBeenLoaded(renderFlare.element[i].texture->filename))
		{
			texID = TM_setTexture(renderFlare.element[i].texture);
			NamedTexture *nTex = new NamedTexture(renderFlare.element[i].texture->filename, texID, tUnit++);
			texs.push_back(nTex);
		}
	}

	// now create quads that reference those textures
	for (int i = 0; i < numQuads; i++)
	{
		if (hasBeenLoaded(renderFlare.element[i].texture->filename))
		{
			getNamedTexture(renderFlare.element[i].texture->filename, &texID, &tUnit);
			if (tUnit != FLARE_ERROR && texID != FLARE_ERROR)
				quads.push_back(TexturedQuads(texID, new Quad(pos, game), tUnit));
		}
	}
}

void Flare::getNamedTexture(char *filename, GLuint *texID, int *tUnit)
{
	for (NamedTexture *tex : texs)
		if (!strncmp(tex->filename, filename, strlen(tex->filename)))
		{
			*texID = tex->texID;
			*tUnit = tex->tUnit;
			return;
		}
	*tUnit = *texID = FLARE_ERROR;
}

bool Flare::hasBeenLoaded(char *filename)
{
	if (texs.empty())
		return false;
	else
	{
		for (NamedTexture *e : texs)
		{
			if (!strncmp(e->filename, filename, strlen(e->filename)))
				return true;
		}
		return false;
	}
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

void drawQuad(Game *game, int index, float normOriginX, float normOriginY, float normWidth, float normHeight)
{
	GLint loc;
	GLuint progID = game->getShader();
	MatrixStack* modelview = game->getModelViewStack();
	GLuint texID = quads[index].texID;
	int tUnit = quads[index].tUnit;

	//Indicar aos samplers do GLSL quais os Texture Units a serem usados
	loc = glGetUniformLocation(progID, "texUnit");
	glUniform1i(loc, tUnit);

	glActiveTexture(GL_TEXTURE0+tUnit);
	glBindTexture(GL_TEXTURE_2D, texID);

	modelview->push();
	quads[index].quad->drawCustomQuad(normOriginX, normOriginY, normWidth, normHeight);
	modelview->pop();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void    FLARE_render(FLARE_DEF *flare, int lx, int ly, int cx, int cy, Game *game)
{
	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height;    // Piece parameters;
	float   alpha;
	int     i;
	FLARE_ELEMENT_DEF    *element;

	// Compute how far off-center the flare source is.
	maxflaredist = floor(sqrt(cx*cx + cy*cy));
	flaredist = floor(sqrt((lx - cx)*(lx - cx) +
		(ly - cy)*(ly - cy)));
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
		alpha = (flaredist*((element->argb >> 24)/255.0f)) / maxflaredist;

		MatrixStack* modelview = game->getModelViewStack();

		if (width > 1)
		{
			/* */
			float centerOfQuadX = NORMALIZED_X(px);
			float centerOfQuadY = NORMALIZED_Y(py);
			float normOriginX = NORMALIZED_X(px - width / 2) - 1.0f;
			float normOriginY = NORMALIZED_Y(py - height / 2) - 1.0f;
			float normWidth = NORMALIZED_X(width);
			float normHeight = NORMALIZED_Y(height);
			/* */
			//unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);
			
			float color[4] = { 1.0, 1.0, 1.0, alpha};
			quads[i].quad->setAmbient(color);
			quads[i].quad->setDiffuse(color);
			quads[i].quad->setSpecular(color);
			quads[i].quad->setTexCount(0);

			drawQuad(game, i, normOriginX, normOriginY, normWidth, normHeight);
		}
	}
}

int     xFlare = 400;
int     yFlare = 300;

void render(Game* game, int xFlare, int yFlare)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FLARE_render(&renderFlare, xFlare, yFlare, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, game);
	
	glDisable(GL_BLEND);
}

void Flare::draw()
{
	render(game, xFlare, yFlare);
}

void Flare::update(float dt)
{

}

void Flare::reset()
{

}

void Flare::setXYFlare(int xFlare, int yFlare)
{
	this->xFlare = xFlare;
	this->yFlare = yFlare;
}

void Flare::getXYFlare(int *xFlare, int *yFlare)
{
	*xFlare = this->xFlare;
	*yFlare = this->yFlare;
}