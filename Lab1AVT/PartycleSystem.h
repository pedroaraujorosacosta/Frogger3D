#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <vector>

class Particle;
class Game;

class ParticleSystem {


private:
	std::vector<Particle*> _liveParticles, _deadParticles;
	const unsigned int _MAXPARTICLES;
	const unsigned int _MAXPARTSPEREXPL;
	const unsigned int _MAXLIFE;
	Game *game;
	float position[3];

public:
	ParticleSystem(Game* game, unsigned int maxParticles, unsigned int nPartsPerExpl, unsigned int maxLife, float *pos);
	~ParticleSystem();
	void draw();
	void reset();
	void update(float dt);
	void explode(float posX, float posY, float posZ);

};

#endif