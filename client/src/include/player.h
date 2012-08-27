#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <string>

#include "device.h"
#include "weapon.h"

using namespace irr;
using namespace irr::video;
using namespace irr::core;

class CPlayer {
public:
	CPlayer();
	CPlayer(vector3df position, CDevice& device, scene::ITriangleSelector*);
	~CPlayer();
	ITexture* getCannonTexture() const { return m_cannonTexture; };
	int getPosX() const { return mPosX; };
	int getPosY() const { return mPosY; };
	vector3df getPosition() const { return mNode->getPosition(); };
	vector3df getRotation() const { return mNode->getRotation(); };
	void setRotation(vector3df rot);
	void setPosition(vector3df pos);
	void update(CDevice& device, const bool* keys, const bool fire, const float frametime);
	void rotate(core::vector3df rot);
	void move(core::vector3df dir);

	scene::ISceneNode* const getSceneNode() const { return mNode; };
	scene::ITriangleSelector* getTriangleSelector() const { return mNode->getTriangleSelector(); };

private:
	ITexture* m_cannonTexture;
	scene::ISceneNode* mNode;
	CWeapon m_weapon;
	int mPosX, mPosY;
	vector3df mDirection;
	static constexpr float SPEED = 300.0f;
};

#endif
