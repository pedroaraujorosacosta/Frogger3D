#include "PartycleSystem.h"
#include "Particle.h"
#include <algorithm>

ParticleSystem::ParticleSystem(Game* game, unsigned int maxParticles, unsigned int maxPartsPerExpl,
	unsigned int maxLife, float *pos) :_MAXPARTICLES(maxParticles), _MAXPARTSPEREXPL(maxPartsPerExpl), _MAXLIFE(maxLife)
{
	float faceDirection[3] = { 0.0f, -1.0f, 0.0f };

	for (int i = 0; i < 3; i++)
		position[i] = pos[i];
	this->game = game;
	
	for (int i = 0; i < maxParticles; i++) {
		_liveParticles.push_back(new Particle(pos, faceDirection, game, 1.0f, 1.0f));
	}
}

ParticleSystem::~ParticleSystem()
{
	_liveParticles.clear();
	_deadParticles.clear();
}

void ParticleSystem::draw()
{
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (std::vector<Particle*>::reverse_iterator it = _liveParticles.rbegin(); it != _liveParticles.rend(); it++)
		(*it)->draw();
	glDisable(GL_BLEND);
}

void ParticleSystem::reset()
{
	_liveParticles.clear();
	_deadParticles.clear();
	float faceDirection[3] = { 0.0f, -1.0f, 0.0f };

	for (int i = 0; i < _MAXPARTICLES; i++) {
		_liveParticles.push_back(new Particle(position, faceDirection, game, 1.0f, 1.0f));
	}
}

void ParticleSystem::update(float dt)
{
	for (std::vector<Particle*>::iterator it = _liveParticles.begin(); it != _liveParticles.end();)
	{
		(*it)->update(dt);
		if (!(*it)->isAlive())
		{
			_deadParticles.push_back((*it));
			it = _liveParticles.erase(it);
		}
		else
			it++;
	}
	std::sort(_liveParticles.begin(), _liveParticles.end(), comparatorObjects);
}

void ParticleSystem::explode(float posX, float posY, float posZ)
{
	unsigned int num_parts = rand() % _MAXPARTSPEREXPL;
	unsigned int lim;

	if (num_parts < _MAXPARTSPEREXPL / 4)
		num_parts = _MAXPARTSPEREXPL / 4;

	if (num_parts + _liveParticles.size() > _MAXPARTICLES)
		num_parts = _liveParticles.size() - _MAXPARTICLES;

	int liveLeft = num_parts - _deadParticles.size();
	if (liveLeft >= 0)
		lim = _deadParticles.size();
	else
		lim = num_parts;

	unsigned int i = 0;
	std::vector<Particle*>::iterator it = _deadParticles.begin();
	while (i < lim && it != _deadParticles.end())
	{
		(*it)->respawn(posX, posY, posZ);
		_liveParticles.push_back((*it));
		it = _deadParticles.erase(it);
		i++;
	}

	float faceDirection[3] = { 0.0f, -1.0f, 0.0f };
	int rest = num_parts - lim;
	while (rest > 0)
	{
		float pos[3] = { posX, posY, posZ };
		Particle *p = new Particle(pos, faceDirection, game, 1, 1);
		float difuse[4] = { 1.0f, 0.4f, 0.0f, 1.0f };
		p->setDiffuse(difuse);
		float specular[4] = { 1.0f, 0.4f, 0.0f, 1.0f };
		p->setSpecular(specular);
		p->setShininess(10.0f);
		_liveParticles.push_back(p);
		rest--;
	}
}
