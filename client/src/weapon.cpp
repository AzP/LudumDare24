#include "weapon.h"
#include "device.h"

CWeapon::CWeapon(CDevice& device, ITriangleSelector* selector)
	: m_device(device), m_terrainSelector(selector)
{
}

bool CWeapon::tryShooting(vector3df direction)
{
	auto newShot = new CProjectile(m_device, m_device.getCameraNode()->getPosition(), direction.normalize(), m_terrainSelector);
	m_firedProjectiles.push_back(newShot);
}

void CWeapon::updateProjectiles(float elapsedTime)
{
	for (auto it = m_firedProjectiles.begin(); it != m_firedProjectiles.end(); ++it)
	{
		(*it)->update(elapsedTime);
	}


}
