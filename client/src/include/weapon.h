#ifndef WEAPON_H
#define WEAPON_H

#include "projectile.h"
#include "particleSystem.h"

#include <irrlicht.h>
#include <vector>

using namespace std;

class CWeapon
{	
public:
	CWeapon(CDevice& device, ITriangleSelector* selector);
	bool tryShooting(vector3df direction);
	void updateProjectiles(float elapsedTime);
	bool testCollision(ITriangleSelector* selector);

private:
	void createExplosion();

	std::vector<CProjectile*> m_firedProjectiles;
	CDevice& m_device;
	CParticleSystem m_particleSystem;
	ITriangleSelector* m_terrainSelector;
	ISceneCollisionManager* m_collisionMgr;

	float m_rechargeTime;
};

#endif
