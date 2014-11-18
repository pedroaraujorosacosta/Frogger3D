#include "TexLoader.h"

// loads an image and defines an 8-bit RGBA texture
unsigned int TexLoader::loadRGBATexture(std::string filename, bool mipmap, bool compress, GLenum aFilter, GLenum aRepMode)
{
	ILboolean success;
	unsigned int imageID;
	GLuint textureID = 0;

	ilInit();
	// Load Texture Map
	ilGenImages(1, &imageID);

	ilBindImage(imageID); /* Binding of DevIL image name */
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	success = ilLoadImage((ILstring)filename.c_str());

	if (!success) {
		printf("Couldn't load texture: %s",
			filename.c_str());
		// The operation was not sucessfull 
		// hence free image and texture 
		ilDeleteImages(1, &imageID);
		return 0;
	}

	// add information to the log

	printf("Texture Loaded: %s", filename.c_str());
	printf("Width: %d, Height %d, Bytes per Pixel %d",
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

	std::string s;
	switch (ilGetInteger(IL_IMAGE_FORMAT)) {
	case IL_COLOR_INDEX: s = "IL_COLOR_INDEX"; break;
	case IL_ALPHA: s = "IL_ALPHA"; break;
	case IL_RGB: s = "IL_RGB"; break;
	case IL_RGBA: s = "IL_RGBA"; break;
	case IL_BGR: s = "IL_BGR"; break;
	case IL_BGRA: s = "IL_BGRA"; break;
	case IL_LUMINANCE: s = "IL_LUMINANCE"; break;
	case  IL_LUMINANCE_ALPHA: s = "IL_LUMINANCE_ALPHA"; break;
	}
	printf(" Format %s", s.c_str());

	switch (ilGetInteger(IL_IMAGE_TYPE)) {
	case IL_BYTE: s = "IL_BYTE"; break;
	case IL_UNSIGNED_BYTE: s = "IL_UNSIGNED_BYTE"; break;
	case IL_SHORT: s = "IL_SHORT"; break;
	case IL_UNSIGNED_SHORT: s = "IL_UNSIGNED_SHORT"; break;
	case IL_INT: s = "IL_INT"; break;
	case IL_UNSIGNED_INT: s = "IL_UNSIGNED_INT"; break;
	case IL_FLOAT: s = "IL_FLOAT"; break;
	case IL_DOUBLE: s = "IL_DOUBLE"; break;
	case IL_HALF: s = "IL_HALF"; break;
	}
	printf(" Data type:  %s\n", s.c_str());

	/* Convert image to RGBA */
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	// Set filters
	GLenum minFilter = aFilter;
	if (aFilter == GL_LINEAR && mipmap) {
		minFilter = GL_LINEAR_MIPMAP_LINEAR;
	}
	else if (aFilter == GL_NEAREST && mipmap){
		minFilter = GL_NEAREST_MIPMAP_LINEAR;
	}
	GLenum type;
	if (compress)
		type = GL_RGBA;
	else
		type = GL_COMPRESSED_RGBA;


	/* Create and load textures to OpenGL */
	glGenTextures(1, &textureID); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, aFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aRepMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aRepMode);
	glTexImage2D(GL_TEXTURE_2D, 0, type,
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData());

	// Mipmapping?
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(1, &imageID);


	return textureID;
}