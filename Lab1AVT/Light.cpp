#include "Light.h"
#include "Vector.h"
#include "Game.h"
#include "Stack.h"
#include "Matrix.h"
#include <string>

Light::Light(int num, LightType type, Game *game)
{
	this->type = type;
	this->numLight = num;
	this->amb = new Vector(3);
	this->dif = new Vector(3);
	this->spec = new Vector(3);

	this->pos = new Vector(4);
	this->dir = new Vector(4);
	state = true; 
	this->game = game;
}

Light::~Light()
{
	delete amb;
	delete dif;
	delete spec;
	delete pos;
	delete dir;
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

void Light::setDirection(Vector dir)
{
	*(this->dir) = dir;
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
		break;
	case SPOT_LIGHT:
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
		
		// isSpot
		stringStream << "Lights[" << numLight << "].isSpot";
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1i(lightID, true);
		stringStream.str("");
		stringStream.clear();

		// cone direction
		stringStream << "Lights[" << numLight << "].coneDirection";
		point = game->getVM() * *dir;
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform3fv(lightID, 1, point.v);
		stringStream.str("");
		stringStream.clear();

		// spotCosCutoff
		stringStream << "Lights[" << numLight << "].spotCosCutoff";
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1f(lightID, cutOff);
		stringStream.str("");
		stringStream.clear();

		// spot exponent
		stringStream << "Lights[" << numLight << "].spotExponent";
		lightID = glGetUniformLocation(game->getShader(), stringStream.str().c_str());
		glUniform1f(lightID, exp);
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
		glUniform1f(lightID, 0.1f);
		stringStream.str("");
		stringStream.clear();
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
}

void Light::toggleState()
{
	state = !state;
}

bool Light::isPointLight()
{
	return (type == POINT_LIGHT);
}