#include <irrlicht.h>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cassert>

#include "player.h"
#include "level.h"
#include "device.h"

using namespace std;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

// Using Irrlicht's namespaces simplifies the code.
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void updateCamera(irr::scene::ICameraSceneNode *camera, 
	irr::scene::ISceneNode *lookAtNode,  
	vector3df offset) 
{ 
	matrix4 m; 
	m.setRotationDegrees(lookAtNode->getRotation()); 

	vector3df forwardVector = vector3df (0.0f, 0.0f, 1.0f); 
	m.transformVect(forwardVector); //Transform vec by matrix

	vector3df upVector = vector3df (0.0f, 1.0f, 0.0f); 
	m.transformVect(upVector); 

	m.transformVect(offset); 
	offset += lookAtNode->getPosition(); 
	camera->setPosition(offset); 

	camera->setUpVector(upVector); 

	offset += forwardVector; 
	camera->setTarget(offset); 
}

void printDebug(CDevice& device, int fps, bool debugCamera,
	vector3df& cameraRotation, vector3df& cameraPosition)
{
	core::stringw str = L"Alone [";
	str += device.getDriver()->getName();
	str += "] FPS:";
	str += fps;
	str += "\nCameraRotation: ";
	str += cameraRotation.X;
	str += ", ";
	str += cameraRotation.Y;
	str += ", ";
	str += cameraRotation.Z;
	str += "\nCameraPosition: ";
	str += cameraPosition.X;
	str += ", ";
	str += cameraPosition.Y;
	str += ", ";
	str += cameraPosition.Z;
	str += "\nDebug camera:";
	str += debugCamera;
	device.getFont()->draw(str,
		core::rect<s32>(30,10,300,90),
		video::SColor(255,255,255,255));
}

void updateTitleBar(int fps, int lastFPS, CDevice& device)
{
	if (lastFPS != fps)
	{
		core::stringw str = L"Ludum Dare 48 [";
		str += device.getDriver()->getName();
		str += "] FPS:";
		str += fps;

		device.getDevice()->setWindowCaption(str.c_str());
		lastFPS = fps;
	}
}

int main(int argc, char* argv[])
{
	MyEventReceiver receiver; 
	CDevice deviceSetup(receiver);

	//Get camera handle
	scene::ISceneNode *cameraLookAtNode = deviceSetup.getCameraLookAtNode();

	//Add the level and collision
	CLevel level(deviceSetup.getDriver(), deviceSetup.getSceneManager());

	//Creates the player
	CPlayer player = CPlayer(0, 0, deviceSetup.getDevice(), level.getTriangleSelector());

	/*******************
	  Creates the other players
	 *******************/
	/************
	 * Create various variables used in gameplay
	 ************/
	int lasttime = deviceSetup.getTimer()->getTime();
	bool quit;
	float frametime = 0;
	bool debugCamera = false;
	vector3df cameraPosition(0.0f,0.0f,-2.0f);
	vector3df cameraRotation(0.0f,0.0f,0.0f);

#ifdef IRRKLANG
	irrklang::vec3df* sndPos = new irrklang::vec3df();
#endif

	cerr <<"starting main rendering loop " <<endl;
	int lastFPS = -1;

	IrrlichtDevice* device = deviceSetup.getDevice();
	while(device->run())
	{
		if (device->isWindowActive())
		{
			quit = receiver.processInput(cameraPosition, cameraRotation, debugCamera);

			player.update(receiver.getKeys(), deviceSetup.getFrameTime());

			if (debugCamera)
			{
				cameraLookAtNode->setPosition(cameraPosition);
				cameraLookAtNode->setRotation(cameraRotation);
			}
			else
			{
				cameraLookAtNode->setPosition(player.getPos());
				cameraLookAtNode->setRotation(cameraRotation);
				cameraPosition = player.getPos();
			}
			updateCamera(cameraLookAtNode, deviceSetup.getCameraNode(), vector3df(0.0f,0.0f,25.0f));
			//updateCamera(deviceSetup.getCameraNode(), cameraLookAtNode, vector3df(0.0f,0.0f,25.0f));
			deviceSetup.getCameraNode()->updateAbsolutePosition();
			deviceSetup.getDriver()->beginScene(true, true, SColor(255,100,101,140));
			deviceSetup.getSceneManager()->drawAll();
			deviceSetup.getEnv()->drawAll();

			int fps = deviceSetup.getFPS();

			//draw some text
			printDebug(deviceSetup, fps, debugCamera, cameraRotation, cameraPosition);

			deviceSetup.getDriver()->endScene();
			updateTitleBar(fps, lastFPS, deviceSetup);

			if (quit)
				break; //Break the loop
		}
	}

	device->closeDevice();
	SDL_Quit();

	return 0;
}

/*
void createLight(ISceneManager* sceneMgr, ISceneNode* sceneNode, bool fog, bool zwrite, std::string texture)
{
	// Create light node
	scene::ILightSceneNode* light =
		sceneMgr->addLightSceneNode(sceneNode, core::vector3df(0,40,100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	// Create billboard object and attach light to it
	scene::ISceneNode* bill =
		sceneMgr->addBillboardSceneNode(light, core::dimension2d<f32>(30, 30));
	bill = sceneMgr->addBillboardSceneNode(light, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_FOG_ENABLE, fog);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, zwrite);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, m_driver->getTexture(texture));
}

void setupLighting(CDevice& device, CPlayer& player, CLevel& level)
{
	ISceneManager* sceneMgr = device.getSceneManager();
	// Add light and attach to player
	scene::IVolumeLightSceneNode* playerVolumeLight =
		sceneMgr->addVolumeLightSceneNode(player.getSceneNode(), -1);
	scene::ILightSceneNode* playerLight =
		sceneMgr->addLightSceneNode(player.getSceneNode(), player.getRot() + vector3df(0,0,-10.0f),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 50.0f);

	createLight(sceneMgr, &playerLight, true, true, "../media/particlered.bmp");
	createLight(sceneMgr, level.getSceneNode(), true, false, "../media/particlewhite.bmp");
	scene::ILightSceneNode* light3 =
		sceneMgr->addLightSceneNode(level.getSceneNode(), core::vector3df(0,40,-100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	bill = sceneMgr->addBillboardSceneNode(light3, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, m_driver->getTexture("../media/particlewhite.bmp"));
}
*/
