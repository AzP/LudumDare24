#ifndef PLAYER_H

#include <irrlicht.h>
#include <string>

using namespace irr;
using namespace irr::video;
using namespace irr::core;

class CPlayer {
public:
	CPlayer();
	CPlayer(int posX, int posY, IrrlichtDevice*, scene::ITriangleSelector*);
	~CPlayer();
	ITexture* getSprite() const { return mSprite; };
	int getPosX() const { return mPosX; };
	int getPosY() const { return mPosY; };
	vector3df getPos() const { return mNode->getPosition(); };
	vector3df getRot() const { return mNode->getRotation(); };
	void update(const bool* keys, const float frametime);
	void rotate(core::vector3df rot);
	void move(core::vector3df dir);

	scene::ISceneNode* const getSceneNode() const { return mNode; };
	scene::ITriangleSelector* getTriangleSelector() const { return mNode->getTriangleSelector(); };

private:
	ITexture* mSprite;
	scene::ISceneNode* mNode;
	int mPosX, mPosY;
	vector3df mDirection;
	static const float SPEED = 300.0f;

	//Handles to Irrlicht device
	IVideoDriver* driver;
};

#endif
