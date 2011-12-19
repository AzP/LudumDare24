#include <irrlicht.h>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cassert>

#include "player.h"
#include "level.h"
#include "eventreceiver.h"

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

static const int windowX = 800;
static const int windowY = 600;

void updateCamera(irr::scene::ICameraSceneNode *camera, 
	irr::scene::ISceneNode *node,  
	irr::core::vector3df offset) 
{ 
	irr::core::matrix4 m; 
	m.setRotationDegrees(node->getRotation()); 

	irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f); 
	m.transformVect(frv); 

	irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f); 
	m.transformVect(upv); 

	m.transformVect(offset); 

	offset += node->getPosition(); 
	camera->setPosition(offset); 

	camera->setUpVector(upv); 

	offset += frv; 
	camera->setTarget(offset); 
} 

int main(int argc, char* argv[])
{
	MyEventReceiver receiver; 

	// Create device
	const dimension2d<u32> windowDimensions(windowX, windowY);
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, windowDimensions, 
		32, false, true, true, &receiver); 
	device->setResizable(false); 

	// Check if device could be created. If not, terminate.
	if (device == 0) 
	{
		cerr << "Could not create device" << endl;
		return 1; 
	}

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	gui::IGUIEnvironment* env = device->getGUIEnvironment();
	gui::IGUIFont* font = env->getFont("../media/fonthaettenschweiler.bmp");
	assert(font);

	/*************
	  Create the camera
	 *************/
	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setFOV(PI/2.0f);
	scene::ISceneNode *cameraLookAtNode = smgr->addEmptySceneNode(); //node the camera is set to look at
	camera->setPosition(core::vector3df(0.0f,0.0f,-15.0f));

	/************
	 * Set up fog
	 ************/
	driver->setFog(video::SColor(30,125,125,138), video::EFT_FOG_EXP, 50.f, 100.f, .01f, true, true);

	/***********************
	  Add the level and collision
	 ***********************/
	CLevel level(driver, smgr);

	//Creates the player
	CPlayer player = CPlayer(0, 0, device, level.getTriangleSelector());

	/*******************
	  Creates the other players
	 *******************/

	/************
	  Add the gui
	 ************/
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	//Create a timer to keep track of time
	ITimer *timekeeper = device->getTimer(); 
	int lasttime = timekeeper->getTime();

	/************
	  Add light and attatch to player
	 ************/
	scene::IVolumeLightSceneNode* playerVolumeLight =
		smgr->addVolumeLightSceneNode(player.getSceneNode(), -1);
	scene::ILightSceneNode* playerLight =
		smgr->addLightSceneNode(player.getSceneNode(), player.getRot() + vector3df(0,0,-10.0f),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 50.0f);
	// attach billboard to the light
	scene::ISceneNode* bill =
		smgr->addBillboardSceneNode(playerLight, core::dimension2d<f32>(30, 30));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlered.bmp"));

	//vector3d(horiz, height, vert);
	scene::ILightSceneNode* light2 =
		smgr->addLightSceneNode(level.getSceneNode(), core::vector3df(0,40,100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	bill = smgr->addBillboardSceneNode(light2, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlewhite.bmp"));

	scene::ILightSceneNode* light3 =
		smgr->addLightSceneNode(level.getSceneNode(), core::vector3df(0,40,-100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	bill = smgr->addBillboardSceneNode(light3, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlewhite.bmp"));
	/************
	 * Create various variables used in gameplay
	 ************/
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
	while(device->run())
	{
		if (device->isWindowActive())
		{
			quit = receiver.processInput(cameraPosition, cameraRotation, debugCamera);

			player.update(receiver.getKeys(), frametime);

			if (debugCamera)
			{
				cameraLookAtNode->setPosition(cameraPosition);
				cameraLookAtNode->setRotation(cameraRotation);
			}
			else
			{
				cameraLookAtNode->setRotation(cameraRotation);
				cameraLookAtNode->setPosition(player.getPos());
				cameraPosition = player.getPos();
			}
			updateCamera(camera, cameraLookAtNode, vector3df(0.0f,0.0f,-50.0f));
			camera->updateAbsolutePosition();
			driver->beginScene(true, true, SColor(255,100,101,140));
			smgr->drawAll();
			env->drawAll();

			int fps = driver->getFPS();
			frametime = (float)(timekeeper->getTime() - lasttime)/1000.0; 
			lasttime = timekeeper->getTime();

			//draw some text
			if (font)
			{
				core::stringw str = L"Alone [";
				str += driver->getName();
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
				font->draw(str,
					core::rect<s32>(30,10,300,90),
					video::SColor(255,255,255,255));
			}

			driver->endScene();

			if (lastFPS != fps)
			{
				core::stringw str = L"Ludum Dare 48 [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
			if (quit)
				break; //Break the loop
		}
	}

	device->closeDevice();
	SDL_Quit();

	return 0;
}

