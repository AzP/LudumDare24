#include "include/player.h"
#include "include/weapon.h"

#include <irrlicht.h>
#include <string>
#include <cassert>

using namespace irr;
using namespace std;

CPlayer::CPlayer(vector3df position, CDevice& device, scene::ITriangleSelector* selector)
	: m_terrainSelector(selector), m_weapon(device, selector)
{
	IAnimatedMesh* mesh = device.getSceneManager()->getMesh("../media/cannon.3ds");
	m_cannonTexture = device.getDriver()->getTexture("../media/cannon.png");
	assert(m_cannonTexture!=NULL);

	mNode = device.getSceneManager()->addAnimatedMeshSceneNode(mesh);
	mNode->setParent(device.getCameraNode()); 
	if (mNode)
	{
		mNode->setMaterialTexture(0, m_cannonTexture);
		mNode->setMaterialFlag(video::EMF_LIGHTING, false);
		mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
		mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
		mNode->setMaterialFlag(video::EMF_FOG_ENABLE, false);
		mNode->setPosition(vector3df(0,-1.5,1.7));
		mNode->setRotation(vector3df(0,90,0));
		mNode->setScale ( core::vector3df(0.5,0.5,0.5) );
	}
}
#if 0
void CPlayer::createNode(IAnimatedMesh* mesh )
{
	node = smgr->addAnimatedMeshSceneNode( mesh );
	colors * shipAttributes = theColors;
	video::SColor shipColorMax = shipAttributes->getParticleColor(id);
	video::SColor shipColorMin;  

	shipColorMin.setRed(shipColorMax.getRed()/(s32)5);
	shipColorMin.setGreen(shipColorMax.getGreen()/(s32)5);
	shipColorMin.setBlue(shipColorMax.getBlue()/(s32)5);
	shipColorMin.setAlpha(shipColorMax.getAlpha());

	if (node) 
	{

		node->setMaterialFlag(EMF_LIGHTING, false);	
		node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
		node->setMaterialTexture( 0, theColors->getTexture(id));
		node->setScale(vector3df(0.1,0.1,0.1));
		node->setVisible(false);

		shipSelector = smgr->createOctreeTriangleSelector(mesh->getMesh(0), node);
		node->setTriangleSelector(shipSelector);

		// add particle system
		particleNode2= smgr->addEmptySceneNode(node);
		particleNode2->setPosition(core::vector3df(0,15,-1010));
		scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false, particleNode2);

		// create and set emitter
		scene::IParticleEmitter* em2 = ps2->createBoxEmitter(
			core::aabbox3d<f32>(-3,0,-3,3,1,3), 
			core::vector3df(0.0f,0.0f,0.0f),
			10,15, // min and max particles per second
			shipColorMin, //min color
			shipColorMax, //max color
			15000,20000, //min lifetime and max lifetime
			45,           // Max angles degrees
			core::dimension2df( 5.0f, 5.0f), //Minimum particle size
			core::dimension2df( 10.0f, 10.0f) //Maximum particle size
			);
		ps2->setEmitter(em2);
		em2->drop();

		// create and set affector
		scene::IParticleAffector* paf2 = ps2->createFadeOutParticleAffector(video::SColor(0,0,0,0), 15000);
		ps2->addAffector(paf2);
		paf2->drop();

		// adjust some material settings
		ps2->setMaterialFlag(video::EMF_LIGHTING, false);
		ps2->setMaterialTexture(0, driver->getTexture("../media/particle/smoke.bmp"));
		ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

		node->setPosition( vector3df(0,0,50*id));
		node->setRotation( vector3df(0,0,45*id));
	}   
}
#endif
CPlayer::~CPlayer()
{
	delete m_cannonTexture;
	m_cannonTexture = 0;
}

void CPlayer::update(CDevice& device, const bool* keys, const bool fire, const float frametime)
{
	if(keys[KEY_KEY_X])
		move(vector3df(frametime*SPEED,0,0));
	if(keys[KEY_KEY_S])
		move(vector3df(-frametime*SPEED,0,0));
	if(keys[KEY_KEY_C])
		move(vector3df(0, 0, frametime*SPEED));
	if(keys[KEY_KEY_Z])
		move(vector3df(0, 0, -frametime*SPEED));
	if(keys[KEY_NEXT])
		move(vector3df(0,-frametime*SPEED,0));
	if(keys[KEY_PRIOR])
		move(vector3df(0,frametime*SPEED,0));
	if(keys[KEY_HOME])
		rotate(vector3df(0,frametime*SPEED,0));
	if(keys[KEY_END])
		rotate(vector3df(0,-frametime*SPEED,0));
	if(keys[KEY_KEY_0])
		mNode->getParent()->setPosition(vector3df(0,0,0));

	short shotId = -1;
	if(fire)
	{   
		shotId = m_weapon.tryShooting(device.getCameraNode()->getTarget());
		if (shotId >= 0)
		{
#ifdef IRRKLANG
	//		audioIKSystem.playShotSound();
#else
	//		audioSystem.playShotSound(1,128,0,0);
#endif
		}
	} 

	//update all projectiles fired from the weapon 
	m_weapon.updateProjectiles(frametime);

	//check collision with the level
	short collided = m_weapon.testCollision(m_terrainSelector);
	//hitPlayer = NOHIT;
/*
	if (collided >= 0) 
	{
		hitPlayer = BUILDINGHIT;
		hitShot = collided;
		//set the hitting shot to shotId
	}  

	//Checks collisions with ships
	short colShotId = -1;
	for (short i = 0; i< otherShips->NUMBER_OF_SHIPS; i++)
	{
		if(otherShips->getActive(i)) 
		{
			colShotId = projectile->testCollision(otherShips->getSelector(i));
			if(colShotId >= 0)
			{
				otherShips->playerDied(i);
				playerScore->addPoint(playerid,1);
				theHUD->updateScore(playerScore->retrievePoints(playerid));
#ifdef IRRKLANG
				audioIKSystem.playExplosionSound();
#else	
				audioSystem.playExplosionSound(3,128);
#endif
				hitPlayer = i; // hits a target, gives targets 20+ id numbers
				hitShot = colShotId; //Set the colliding shot to shotId
			}
		}
	}

	return shotId;*/
}

void CPlayer::move(core::vector3df vel)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(mNode->getRotation());
	m.transformVect(vel);
	mNode->setPosition(mNode->getPosition() + vel);
}

void CPlayer::setRotation(vector3df rot)
{
	mNode->setRotation(rot);
}

void CPlayer::setPosition(vector3df pos)
{
	mNode->setPosition(pos);
}

void CPlayer::rotate(core::vector3df rot)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(mNode->getRotation());
	irr::core::matrix4 n;
	n.setRotationDegrees(rot);
	m *= n;
	mNode->setRotation(m.getRotationDegrees());
}

