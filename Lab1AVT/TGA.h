#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

#include <fstream>

typedef GLushort WORD;
typedef GLubyte byte;

typedef struct tImageTGA
{
	int channels;
	int size_x;	
	int size_y;				
	unsigned char *data;
} tImageTGA;


void TGA_Texture(unsigned int *textureArray, char *strFileName, int ID, bool mipMaps);

tImageTGA *Load_TGA(char *filename);
bool readFile(const char *filename, char **output);
unsigned int getStreamSize(std::ifstream &ifs);



// www.morrowland.com
// apron@morrowland.com