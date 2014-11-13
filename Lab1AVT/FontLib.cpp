/* --------------------------------------------------

Lighthouse3D

VSMathLib - Very Simple Font Library

http://www.lighthouse3d.com/very-simple-libs

----------------------------------------------------*/

#include "FontLib.h"
#include "Game.h"
#include "Stack.h"
#include "Matrix.h"

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2

#ifdef _WIN32 
#ifdef _DEBUG
#pragma comment(lib,"tinyxmld.lib")
#else
#pragma comment(lib, "tinyxml.lib")
#endif
#endif


// Constructor
// init Devil
// grab the singleton for VSMathLib
VSFontLib::VSFontLib(Game *game) :
		mHeight(0), 
		mNumChars(0), 
		mPrevDepth(false),
		mFixedSize(false)
{
	this->game = game;
	mMaterial.emissive[0] = 1.0f;
	mMaterial.emissive[1] = 1.0f;
	mMaterial.emissive[2] = 1.0f;
	mMaterial.emissive[3] = 1.0f;
	mMaterial.texCount = 1;
}


// Clear chars info, and sentences
VSFontLib::~VSFontLib()
{
	mChars.clear();

	// Free resources for each sentence
	std::vector<VSFLSentence>::iterator iter = mSentences.begin();

	for ( ; iter != mSentences.end(); ++iter) {
		(*iter).clear();
	}
}


// Generate slots for sentences
// If there are deleted slots use them, otherwise
// create a new slot
// returns the index of the new slot
unsigned int
VSFontLib::genSentence() 
{
	unsigned int index;
	VSFLSentence aSentence;

	// Are there deleted slots?
	if (mDeletedSentences.size()) {
		// use the last deleted slot
		index = mDeletedSentences[mDeletedSentences.size()-1];
		// remove the slot from the deleted list
		mDeletedSentences.pop_back();
	}
	// if not create a new slot
	else {
		index = mSentences.size();
		// add a slot
		mSentences.push_back(aSentence);
	}
	// return the index of the slot
	return index;
}


// Delete a Sentence
void 
VSFontLib::deleteSentence(unsigned int index)
{
	// if the index refers to a valid slot
	// i.e. the slot is within range and it has a sentence
	if (index < mSentences.size() && mSentences[index].getVAO()) {
		// clear deletes the VAO and buffers
		mSentences[index].clear();
		// add the index of the deleted slot to the list
		mDeletedSentences.push_back(index);
	}
}


// A font is specified by two files: a TGA file with the rendered 
// chars for the font, and a XML file which contains global info 
// about the font and the texture coordinates and width of each char
// The parameter fontName is the filename without extension. 
// It is assumed that the files are "fontName.xml" and "fontName.tga"
bool
VSFontLib::load(std::string fontName) 
{
	// Test if image file exists
	FILE *fp;
	std::string s;
	errno_t err = -1;

	s = fontName + ".tga";
	err = fopen_s(&fp, s.c_str(), "r");
	if (fp == NULL || err == -1)
	{
		printf("Unable to find font texture: %s", s.c_str());
		return false;
	}
	
	mFontTex = loadRGBATexture(s, true, false, GL_LINEAR, GL_REPEAT);

	s = fontName + ".xml";
	TiXmlDocument doc(s.c_str());
	bool loadOK = doc.LoadFile();

	if (!loadOK) {
		printf("Problem reading the XML font definition file: %s", s.c_str());
		return false;
	}
	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement *pElem;

	pElem = hDoc.FirstChildElement().Element();
	if (0 == pElem)
		return false;

	hRoot = TiXmlHandle(pElem);
	
	pElem->QueryIntAttribute("numchars",&mNumChars);

	if (mNumChars == 0)
		return false;

	hRoot = hRoot.FirstChild("characters");
	pElem = hRoot.FirstChild("chardata").Element();
	if (pElem)
		pElem->QueryIntAttribute("hgt",&mHeight);
	VSFLChar aChar;
	int charCode, numChars = 0;
	for(; 0 != pElem; pElem = pElem->NextSiblingElement(), ++numChars) {

		pElem->QueryIntAttribute("char",&charCode);
		pElem->QueryIntAttribute("wid",&(aChar.width));
		pElem->QueryFloatAttribute("X1", &(aChar.x1));
		pElem->QueryFloatAttribute("X2", &(aChar.x2));
		pElem->QueryFloatAttribute("Y1", &(aChar.y1));
		pElem->QueryFloatAttribute("Y2", &(aChar.y2));
		pElem->QueryIntAttribute("A", &(aChar.A));
		pElem->QueryIntAttribute("C", &(aChar.C));
		mChars[(unsigned char)charCode] = aChar;
	}
	printf("Font has %d chars", numChars);
	return true;
}

// helper function for derived classes
// loads an image and defines an 8-bit RGBA texture
unsigned int VSFontLib::loadRGBATexture(std::string filename, bool mipmap, bool compress, GLenum aFilter, GLenum aRepMode) 
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

// Matrix and OpenGL settings for rendering
void 
VSFontLib::prepareRender(float x, float y)
{
	// get previous depth test setting 
	glGetIntegerv(GL_DEPTH_TEST,&mPrevDepth);
	// disable depth testing
	glDisable(GL_DEPTH_TEST);

	// get previous blend settings
	glGetIntegerv(GL_BLEND, &mPrevBlend);
	glGetIntegerv(GL_BLEND_DST, &mPrevBlendDst);
	glGetIntegerv(GL_BLEND_SRC, &mPrevBlendSrc);
	// set blend for transparency
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// get viewport
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	// prepare projection matrix so that there is a 1:1 mapping
	// between window and vertex coordinates
	Stack *proj = game->getProjectionStack();
	proj->push();
	proj->loadIdentity();
	proj->orthogonal((float)vp[0], (float)vp[0] + (float)vp[2], (float)vp[1] + (float)vp[3], (float)vp[1], -1.0f, 1.0f);
	
	// set model and view = identity matrix
	Stack *mView = game->getModelViewStack();
	mView->push();
	mView->loadIdentity();

	//// translate to cursor position
	mView->translateMatrix((float)x, (float)y, 0.0f);

}


void
VSFontLib::restoreRender()
{
	// restore previous depth test settings
	if (mPrevDepth)
		glEnable(GL_DEPTH_TEST);

	// restore previous blend settings
	if (!mPrevBlend)
		glDisable(GL_BLEND);

	glBlendFunc(mPrevBlendSrc, mPrevBlendDst);

	Stack *proj = game->getProjectionStack();
	// restore previous projection matrix
	proj->pop();//LoadIdentity
	proj->pop();

	Stack *mView = game->getModelViewStack();
	// restore previous model and view matrices
	mView->pop();//LoadIdentity
	mView->pop();
}


void 
VSFontLib::prepareSentence(unsigned int index, std::string sentence)
{
	float *positions, *texCoords;
	float hDisp = 0.0f, vDisp = 0.0f;
	GLuint vao, buffer[2];

	// if index is not within range
	// this should never happen if using genSentence
	if (index >= mSentences.size())
		return;

	// clear previous sentence data if reusing
	mSentences[index].clear();

	// allocate temporary arrays for vertex and texture coordinates
	int size = sentence.length();
	positions = (float *)malloc(sizeof(float) * size * 6 * 3);
	texCoords = (float *)malloc(sizeof(float) * size * 6 * 2);

	int i = 0;
	for (int count = 0; count < size; count++) {
	
		// get char at position count
		char c = sentence[count];
		// if char exists in the font definition
		if (mChars.count(c)) {
			positions[18 * i + 0] = hDisp;
			positions[18 * i + 1] = vDisp + mHeight;
			positions[18 * i + 2] = 0.0f;

			positions[18 * i + 3] = hDisp + mChars[c].width;
			positions[18 * i + 4] = vDisp + 0.0f;
			positions[18 * i + 5] = 0.0f;

			positions[18 * i + 6] = hDisp;
			positions[18 * i + 7] = vDisp + 0.0f;
			positions[18 * i + 8] = 0.0f;

			positions[18 * i + 9] = hDisp + mChars[c].width;
			positions[18 * i + 10] = vDisp + 0.0f;
			positions[18 * i + 11] = 0.0f;

			positions[18 * i + 12] = hDisp;
			positions[18 * i + 13] = vDisp + mHeight;
			positions[18 * i + 14] = 0.0f;

			positions[18 * i + 15] = hDisp + mChars[c].width;
			positions[18 * i + 16] = vDisp + mHeight;
			positions[18 * i + 17] = 0.0f;

			texCoords[12 * i + 0] = mChars[c].x1;
			texCoords[12 * i + 1] = 1-mChars[c].y2;

			texCoords[12 * i + 2] = mChars[c].x2;
			texCoords[12 * i + 3] = 1-mChars[c].y1;

			texCoords[12 * i + 4] = mChars[c].x1;
			texCoords[12 * i + 5] = 1-mChars[c].y1;

			texCoords[12 * i + 6] = mChars[c].x2;
			texCoords[12 * i + 7] = 1-mChars[c].y1;

			texCoords[12 * i + 8] = mChars[c].x1;
			texCoords[12 * i + 9] = 1-mChars[c].y2;

			texCoords[12 * i + 10] = mChars[c].x2;
			texCoords[12 * i + 11] = 1-mChars[c].y2;

			if (mFixedSize)
				hDisp += mChars[c].C + mChars[c].A; 
			else
				hDisp += mChars[c].C;
			i++;
		}
		// newline
		else if (c == '\n') {
			vDisp += mHeight;
			hDisp = 0.0f;
		}
	}
	// real number of chars (excluding '\n')
	size = i;

	// create VAO
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// create vertex buffers
	glGenBuffers(2,buffer);
	
	// positions
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 6 * 3, positions,GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(MY_VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	// texCoords
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 6 * 2, texCoords,GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(MY_TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	glBindVertexArray(0);

	// init the sentence
	mSentences[index].initSentence(vao, buffer,size);

	// delete temporary arrays
	delete positions;
	delete texCoords;

}


// Render a previously prepared sentence at (x,y) window coords
// (0,0) is the top left corner of the window
void
VSFontLib::renderSentence(int x, int y, unsigned int index)
{
	if (mSentences[index].getVAO()) {

		prepareRender((float)x,(float)y);

		// set the material
		sendMaterials();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mFontTex);

		matricesToGL();		
		glBindVertexArray(mSentences[index].getVAO());
		glDrawArrays(GL_TRIANGLES, 0, mSentences[index].getSize()*6);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D,0);

		restoreRender();
	}
}

void VSFontLib::matricesToGL()
{
	GLuint pvmID = game->getPVMid();
	//GLuint vmID = game->getVMid();
	//GLuint iVmID = game->getIVMid();

	// send the VM matrix
	/*Matrix vm = game->getVM();
	float vmFloat[16];
	for (int i = 0; i < 16; i++)
		vmFloat[i] = vm.m[i];

	glUniformMatrix4fv(vmID, 1, GL_TRUE, vmFloat);*/


	// compute and send the normal matrix
	/*Matrix iVm = vm.invertMatrix();
	float iVmFloat[9];
	for (int i = 0; i < 9; i++)
		iVmFloat[i] = iVm.m[i];
	glUniformMatrix3fv(iVmID, 1, GL_TRUE, iVmFloat);*/


	// send the PVM matrix
	Matrix pvmM = game->getPVM();
	float pvmFloat[16];
	for (int i = 0; i < 16; i++)
		pvmFloat[i] = pvmM.m[i];
	glUniformMatrix4fv(pvmID, 1, GL_TRUE, pvmFloat);
}

void VSFontLib::sendMaterials()
{
	GLint loc;
	GLuint progID = game->getShader();

	loc = glGetUniformLocation(progID, "mat.ambient");
	glUniform4fv(loc, 1, mMaterial.ambient);
	loc = glGetUniformLocation(progID, "mat.diffuse");
	glUniform4fv(loc, 1, mMaterial.diffuse);
	loc = glGetUniformLocation(progID, "mat.specular");
	glUniform4fv(loc, 1, mMaterial.specular);
	loc = glGetUniformLocation(progID, "mat.shininess");
	glUniform1f(loc, mMaterial.shininess);
	loc = glGetUniformLocation(progID, "mat.emissive");
	glUniform4fv(loc, 1, mMaterial.emissive);
	loc = glGetUniformLocation(progID, "texMode");
	glUniform1i(loc, this->mMaterial.texCount);

	//Indicar aos samplers do GLSL quais os Texture Units a serem usados
	loc = glGetUniformLocation(progID, "texUnit");
	glUniform1i(loc, 0);
}

/* This is a shortcut to easily render a string once */
void
VSFontLib::renderAndDiscard(int x, int y, std::string sentence)
{
	unsigned int s = genSentence();
	prepareSentence(s, sentence);
	renderSentence(x,y,s);
	deleteSentence(s);
}


void 
VSFontLib::setFixedFont(bool fixed) {

	mFixedSize = fixed;
}


void 
VSFontLib::setColor(float *v) {

	mMaterial.emissive[0] = v[0];
	mMaterial.emissive[1] = v[1];
	mMaterial.emissive[2] = v[2];
	mMaterial.emissive[3] = v[3];
}


void 
VSFontLib::setColor(float r, float g, float b, float a) {

	mMaterial.emissive[0] = r;
	mMaterial.emissive[1] = g;
	mMaterial.emissive[2] = b;
	mMaterial.emissive[3] = a;
}

/* ---------------------------------------------------------------------------

	                        VSFLSentence (inner class)

----------------------------------------------------------------------------*/

// Init mVAO and mSize
VSFontLib::VSFLSentence::VSFLSentence()
{
	mVAO = 0;
	mSize = 0;
}


VSFontLib::VSFLSentence::~VSFLSentence()
{
	if (mVAO) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(2, mBuffers);
		mVAO = 0;
	}
}


void
VSFontLib::VSFLSentence::clear()
{
	if (mVAO) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(2, mBuffers);
		mVAO = 0;
	}
}


void
VSFontLib::VSFLSentence::initSentence(GLuint vao, GLuint *buffers, int size)
{
	mVAO = vao;
	mSize = size;
	mBuffers[0] = buffers[0];
	mBuffers[1] = buffers[1];
}


GLuint
VSFontLib::VSFLSentence::getVAO()
{
	return mVAO;
}


int
VSFontLib::VSFLSentence::getSize()
{
	return mSize;
}


GLuint
VSFontLib::VSFLSentence::getVertexBuffer()
{
	return mBuffers[0];
}


GLuint
VSFontLib::VSFLSentence::getTexCoordBuffer()
{
	return mBuffers[1];
}

bool VSFontLib::isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void VSFontLib::checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}