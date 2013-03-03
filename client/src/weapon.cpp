#include "weapon.h"
#include "device.h"

#include <iostream>

using namespace std;

CWeapon::CWeapon(CDevice& device, ITriangleSelector* selector)
: m_device(device), m_terrainSelector(selector), m_rechargeTime(0)
	, m_particleSystem(device, selector)
{
}

bool CWeapon::tryShooting(vector3df direction)
{
	if (m_rechargeTime > 0)
		return false;

	auto newShot = new CProjectile(m_device, m_device.getCameraNode()->getPosition(), direction.normalize(), m_terrainSelector);
	m_firedProjectiles.push_back(newShot);
	m_rechargeTime = 1;

	return true;
}

void CWeapon::updateProjectiles(float elapsedTime)
{
	for (auto it = m_firedProjectiles.begin(); it != m_firedProjectiles.end(); ++it)
	{
		(*it)->update(elapsedTime);
	}
	m_rechargeTime -= elapsedTime;
}

bool CWeapon::testCollision(ITriangleSelector* selector)
{
	for(auto p = m_firedProjectiles.begin(); p != m_firedProjectiles.end(); )
	{
		vector3df collisionPoint;
		if ((*p)->testCollision(selector, collisionPoint))
		{
			cerr << "Collision detected" << endl;
			delete *p;
			p = m_firedProjectiles.erase(p);
			m_particleSystem.createExplosion(collisionPoint);
		}
		else
			++p;
	}
}
