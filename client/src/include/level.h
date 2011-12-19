#ifndef PLAYER_H

#include <iostream>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

class CLevel
{
public:
	CLevel(IVideoDriver* driver, ISceneManager*);
	~CLevel();
	ITriangleSelector* getTriangleSelector() const { return mWallsBox->getTriangleSelector(); };
	ISceneNode* getSceneNode() const { return mGround; };
	void addCollisionDetection(ISceneNode*);

private:
	ISceneManager* mSmgr;
	ISceneNode* mGround;
	IMeshSceneNode* mWallsBox;

};

#endif
