#ifndef WEAPON_H
#define WEAPON_H

#include "projectile.h"

#include <irrlicht.h>
#include <vector>

using namespace std;

class CWeapon
{	
public:
	CWeapon(CDevice& device, ITriangleSelector* selector);
	bool tryShooting(vector3df direction);
	void updateProjectiles(float elapsedTime);

private:
	std::vector<CProjectile*> m_firedProjectiles;
	CDevice& m_device;
	ITriangleSelector* m_terrainSelector;

};

#endif
