#ifndef LEVEL_H
#define LEVEL_H

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
	ITriangleSelector* getTriangleSelector() const { return m_towerNode->getTriangleSelector(); };
	ISceneNode* getSceneNode() const { return m_terrain; };
	void addCollisionDetection(ISceneNode*);

private:
	void setupSkyBox(IVideoDriver* driver);

	ISceneManager* m_smgr;
	ITerrainSceneNode* m_terrain;
	IMeshSceneNode* m_towerNode;

};

#endif
