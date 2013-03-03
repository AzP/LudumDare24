#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "device.h"

#include <irrlicht.h>
#include <vector>

using namespace std;
using namespace irr;

class CParticleSystem
{	
public:
	CParticleSystem(CDevice& device, ITriangleSelector* selector);
	void createExplosion(const vector3df&);

private:

	//std::vector<CParticleSystem*> m_explosions;
	CDevice& m_device;
	ITriangleSelector* m_terrainSelector;
	ISceneCollisionManager* m_collisionMgr;
};

#endif
