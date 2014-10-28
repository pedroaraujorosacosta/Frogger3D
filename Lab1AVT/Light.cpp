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
	this->amb = new Vector(3);
	this->dif = new Vector(3);
	this->spec = new Vector(3);

	this->pos = new Vector(4);
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
	Vector point(3);
	float direction[3];
	GLint lightID;
	std::ostringstream stringStream;


	switch (type)
	{
	case POINT_LIGHT:
		// position
		stringStream << "Lights[" << numLight << "].position";
		point = game->getVM() * *pos;
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform3fv(lightID, 1, point.v);
		stringStream.str("");
		stringStream.clear();

		// isLocal
		stringStream << "Lights[" << numLight << "].isLocal";
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1i(lightID, true);
		stringStream.str("");
		stringStream.clear();
		break;
	case DIR_LIGHT:
		// position
		stringStream << "Lights[" << numLight << "].position";
		point = game->getVM() * *pos;
		for (int i = 0; i < 3; i++)	direction[i] = - point.v[i];
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform3fv(lightID, 1, direction);
		stringStream.str("");
		stringStream.clear();

		// isLocal
		stringStream << "Lights[" << numLight << "].isLocal";
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1i(lightID, false);
		stringStream.str("");
		stringStream.clear();
		break;
	case SPOT_LIGHT:
		break;
	}

	// isEnabled
	stringStream << "Lights[" << numLight << "].isEnabled";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform1i(lightID, state);
	stringStream.str("");
	stringStream.clear();

	// Ambient
	stringStream << "Lights[" << numLight << "].ambient";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	stringStream.str("");
	stringStream.clear();

	// Diffuse
	stringStream << "Lights[" << numLight << "].diffuse";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform3fv(lightID, 1, (*dif).v);
	stringStream.str("");
	stringStream.clear();

	// Specular
	stringStream << "Lights[" << numLight << "].specular";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform3fv(lightID, 1, (*spec).v);
	stringStream.str("");
	stringStream.clear();

	// Constant attenuation
	stringStream << "Lights[" << numLight << "].constantAttenuation";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform1f(lightID, 1.0f);
	stringStream.str("");
	stringStream.clear();

	// Linear attenuation
	stringStream << "Lights[" << numLight << "].linearAttenuation";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform1f(lightID, 0.0f);
	stringStream.str("");
	stringStream.clear();

	// Quadratic attenuation
	stringStream << "Lights[" << numLight << "].quadraticAttenuation";
	lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
	glUniform1f(lightID, 0.0f);
	stringStream.str("");
	stringStream.clear();
}

void Light::toggleState()
{
	state = !state;
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