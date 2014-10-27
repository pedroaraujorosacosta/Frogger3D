#include "Light.h"
#include "Vector.h"
#include "Game.h"
#include "Stack.h"
#include "Matrix.h"
#include <string>

struct Material{
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};

enum MaterialSemantics {
	DIFFUSE,
	AMBIENT,
	SPECULAR,
	EMISSIVE,
	SHININESS,
	TEX_COUNT
} MaterialComponent;

Light::Light(int num, LightType type, Game *game)
{
	this->type = type;
	this->numLight = num;
	this->amb = new Vector(4);
	this->dif = new Vector(4);
	this->spec = new Vector(4);

	this->pos = new Vector(4);
	this->dir = new Vector(3);
	state = true; 
	this->game = game;
}

void Light::setAmbient(Vector amb)
{
	*(this->amb) = amb;
}

void Light::setDiffuse(Vector dif)
{
	*(this->dif) = dif;
}

void Light::setSpecular(Vector spec)
{
	*(this->spec) = spec;
}

void Light::setPosition(Vector pos)
{
	*(this->pos) = pos;
}

void Light::setDirection(Vector dir)
{
	*(this->dir) = dir;
}

void Light::setCutoff(float cutOff)
{
	this->cutOff = cutOff;
}

void Light::setExponent(float exp)
{
	this->exp = exp;
}

void Light::update(float dt)
{

}
void Light::draw()
{

}
void Light::reset()
{

}

void Light::illuminate()
{
	Vector point(4);
	Vector direction(3);
	GLint lightID;
	GLint lightIsEnabledID;
	std::ostringstream stringStream;

	switch (type)
	{
	case POINT_LIGHT:
		point = game->getVM() * *pos;
		lightID = glGetUniformLocation(game->getShader(), "l_pos");
		glUniform4fv(lightID, 1, point.v);
		stringStream << "light[" << numLight << "].isEnabled";
		checkOpenGLError("ERROR: Could not destroy shaders.");
		lightIsEnabledID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1i(lightIsEnabledID, state);
		break;
	case DIR_LIGHT:
		direction = game->getVM() * *dir;
		lightID = glGetUniformLocation(game->getShader(), "l_dir");
		glUniform3fv(lightID, 1, direction.v);
		stringStream << "dirLight.isEnabled";
		lightIsEnabledID = glGetUniformLocation(game->getShader(), stringStream.str().c_str()); 
		glUniform1i(lightIsEnabledID, state);
		break;
	case SPOT_LIGHT:
		break;
	}
}

/*
void VSResourceLib::setMaterial(Material &aMat) {

	// use named block
	if (mMaterialBlockName != "" && mMatSemanticMap.size() == 0) {
		VSShaderLib::setBlock(mMaterialBlockName, &aMat);
	}
	// use uniforms in named block
	else if (mMaterialBlockName != "" && mMatSemanticMap.size() != 0) {

		std::map<std::string, MaterialSemantics>::iterator iter;
		for (iter = mMatSemanticMap.begin(); iter != mMatSemanticMap.end(); ++iter) {
			void *value;
			switch ((*iter).second) {
			case DIFFUSE: value = (void *)aMat.diffuse;
				break;
			case AMBIENT: value = (void *)aMat.ambient;
				break;
			case SPECULAR: value = (void *)aMat.specular;
				break;
			case EMISSIVE: value = (void *)aMat.emissive;
				break;
			case SHININESS: value = (void *)&aMat.shininess;
				break;
			case TEX_COUNT: value = (void *)&aMat.texCount;
				break;
			}
			VSShaderLib::setBlockUniform(mMaterialBlockName,
				(*iter).first, value);
		}
	}
}


void VSResSurfRevLib::setColor(VSResourceLib::MaterialSemantics m, float *values) {

	if (m == TEX_COUNT)
		return;

	switch (m) {
	case SHININESS:
		mMyMesh.mat.shininess = *values;
		break;
	case DIFFUSE:
		memcpy(mMyMesh.mat.diffuse, values, sizeof(float) * 4);
		break;
	case AMBIENT:
		memcpy(mMyMesh.mat.ambient, values, sizeof(float) * 4);
		break;
	case SPECULAR:
		memcpy(mMyMesh.mat.specular, values, sizeof(float) * 4);
		break;
	case EMISSIVE:
		memcpy(mMyMesh.mat.emissive, values, sizeof(float) * 4);
		break;
	}
}
*/