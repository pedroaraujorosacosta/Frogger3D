#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <list>

class Particle;

class ParticleSystem {


private:
	std::list<Particle*> _liveParticles, _deadParticles;
	const unsigned int _MAXPARTICLES;
	const unsigned int _MAXPARTSPEREXPL;
	const unsigned int _MAXLIFE;

public:
	ParticleSystem(unsigned int maxParticles, unsigned int nPartsPerExpl, unsigned int maxLife);
	~ParticleSystem();
	void draw();
	void reset();
	void update(float dt);
	void explode(float posX, float posY, float posZ);

};

#endif