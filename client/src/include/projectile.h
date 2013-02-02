#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <irrlicht.h>

class CDevice;

using namespace irr;
using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;

class CProjectile
{
public:
	CProjectile(CDevice& device, vector3df pos, vector3df dir, ITriangleSelector* selector);
	void update(float elapsedTime);
	bool testCollision(ITriangleSelector* selector);

private:
	vector3df m_position;
	vector3df m_direction;
	ISceneNode* m_node;
	ISceneCollisionManager* m_collisionMgr;

};
#endif
