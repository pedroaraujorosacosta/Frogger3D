#include <string>
#include <GL/glew.h>
#include <IL/il.h>

class TexLoader
{
public:
	static unsigned int loadRGBATexture(std::string filename, bool mipmap, bool compress, GLenum aFilter, GLenum aRepMode);
};