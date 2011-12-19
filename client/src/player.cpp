#include "include/player.h"

#include <irrlicht.h>
#include <string>
#include <cassert>

using namespace irr;
using namespace std;

CPlayer::CPlayer()
{

}

CPlayer::CPlayer(int posX, int posY, IrrlichtDevice *device, scene::ITriangleSelector* selector)
	: mPosX(posX), mPosY(posY)
{
	mSprite = device->getVideoDriver()->getTexture("../media/player.png");
	assert(mSprite!=NULL);

	mNode = device->getSceneManager()->addCubeSceneNode(6.0f);
	if (mNode)
	{
		mNode->setMaterialTexture(0, mSprite);
		mNode->setMaterialFlag(video::EMF_LIGHTING, true);
		mNode->setMaterialFlag(video::EMF_FOG_ENABLE, false);
		mNode->setPosition(vector3df(mPosX, mPosY, 0));
	}

	scene::ISceneNodeAnimator* anim = device->getSceneManager()->createCollisionResponseAnimator(
		selector, mNode, core::vector3df(2.6,2.6,3.8), //Size of collision sphere
		core::vector3df(0,0,10), //Gravity vector
		core::vector3df(0,0,0)); //Translation
	mNode->addAnimator(anim);
	anim->drop();
}

CPlayer::~CPlayer()
{
	driver = 0;
	delete mSprite;
	mSprite = 0;
}

void CPlayer::update(const bool* keys, const float frametime)
{
	if(keys[KEY_UP])
		move(vector3df(0, frametime*SPEED,0));
	if(keys[KEY_DOWN])
		move(vector3df(0, -frametime*SPEED,0));
	if(keys[KEY_RIGHT])
		move(vector3df(frametime*SPEED,0,0));
	if(keys[KEY_LEFT])
		move(vector3df(-frametime*SPEED,0,0));
	if(keys[KEY_PRIOR])
		move(vector3df(0,0,frametime*SPEED));
	if(keys[KEY_SPACE])
		move(vector3df(0,0,-frametime*SPEED));
}

void CPlayer::move(core::vector3df vel)
{
    irr::core::matrix4 m;
    m.setRotationDegrees(mNode->getRotation());
    m.transformVect(vel);
    mNode->setPosition(mNode->getPosition() + vel);
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

