#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <irrlicht.h>

using namespace irr::core;

class CProjectile
{
public:
	CProjectile(vector3df pos, vector3df dir);
	void update(double elapsedTime);
private:
	vector3df m_position;
	vector3df m_direction;
	ISceneNode* m_node;

};
#endif
