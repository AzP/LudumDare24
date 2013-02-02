#include <irrlicht.h>

#include <iostream>
#include "projectile.h"
#include "device.h"

using namespace std;

CProjectile::CProjectile(CDevice& device, vector3df pos, vector3df dir, ITriangleSelector* selector)
: m_position(pos), m_direction(dir)
{
	IAnimatedMesh* mesh = device.getSceneManager()->getMesh("../media/cannonball.3ds");
	m_node = device.getSceneManager()->addAnimatedMeshSceneNode(mesh);
	m_node->setMaterialFlag(video::EMF_LIGHTING, false);
	//mesh->setMaterialTexture(0, theColors->getBulletTexture(playerId));
	m_node->setScale ( core::vector3df(0.2,0.2,0.2) );
	m_node->setVisible(true);  

	scene::ISceneNodeAnimator* anim = device.getSceneManager()->createCollisionResponseAnimator(
		selector, m_node, core::vector3df(1.0,1.0,1.0), //Size of collision sphere
		core::vector3df(0,9,0), //Gravity vector
		core::vector3df(0,0,0)); //Translation of sphere
	m_node->addAnimator(anim);
	anim->drop();

	m_position += m_direction.normalize() * 4.5;
	m_node->setPosition(m_position);
	m_node->setRotation(m_direction);
}

void CProjectile::update(float elapsedTime)
{
	//Nice flight!
	m_position += m_direction * elapsedTime * 120.0;
	m_node->setPosition(m_position);
	//std::cerr << "Updated position: " << m_position.X << ", " << m_position.Y << ", " << m_position.Z << std::endl;
}

bool CProjectile::testCollision(ITriangleSelector* selector)
{
	vector3df collisionPoint;
	core::triangle3df tri;
	const ISceneNode* hitSceneNode = 0;
	short collidedShot = -1;

	//Create a line in front of the bullet and 
	//check for collision between it and the selector
	core::line3d<f32> line;
	line.start = m_position;
	line.end =  m_position + m_direction*5;

	bool collided = m_collisionMgr->getCollisionPoint(line, selector, collisionPoint, tri, hitSceneNode);
	return collided;
}
