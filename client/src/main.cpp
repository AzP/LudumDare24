#include <irrlicht.h>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

#include "player.h"
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

int main(int argc, char* argv[])
{
	MyEventReceiver receiver; 

	// Create device
	const dimension2d<u32> windowDimensions(windowX, windowY);
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, windowDimensions, 
		32, false, false, true, &receiver); 
	device->setResizable(false); 

	// Check if device could be created. If not, terminate.
	if (device == 0) 
	{
		cerr << "Could not create device" << endl;
		return 1; 
	}

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	gui::IGUIFont* font2 =
		device->getGUIEnvironment()->getFont("../media/fonthaettenschweiler.bmp");


	/*************
	  Create the camera
	 *************/
	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setFOV(PI/2.0f);
	scene::ISceneNode *cameraLookAtNode = smgr->addEmptySceneNode(); //node the camera is set to look at
	camera->setPosition(core::vector3df(0.0f,0.0f,-15.0f));

	/***********************
	  Add the level and collision
	 ***********************/

	/*******************
	  Creates the other players
	 *******************/

	/************
	  Add the gui
	 ************/

	/**********
	  Creates the player
	 *********/
	CPlayer player = CPlayer(400, 300, device);

	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	//Create a timer to keep track of time
	ITimer *timekeeper = device->getTimer(); 
	int lasttime = timekeeper->getTime();

	/************
	  Add lights
	 ************/
	/*
	   smgr->addLightSceneNode(0, core::vector3df(0,0,0),
	   video::SColorf(1.0f,0.9f,0.7f,1.0f),
	   1000.0f);
	   smgr->addLightSceneNode(0, core::vector3df(0,-400,0),
	   video::SColorf(0.2f,0.4f,0.5f,1.0f),
	   1000.0f);
	 */
	/************
	 * Create various variables used in gameplay
	 ************/
	float frametime = 0;

#ifdef IRRKLANG
	irrklang::vec3df* sndPos = new irrklang::vec3df();
#endif

	cerr <<"starting main rendering loop " <<endl;
	int lastFPS = -1;
	while(device->run())
	{
		if (device->isWindowActive())
		{
			player.update(receiver.getKeys(), frametime);
			driver->beginScene(true, true, 0);
			smgr->drawAll();
			driver->draw2DImage(player.getSprite(), position2d<s32>(player.getPosX(),player.getPosY()));
			driver->endScene();

			int fps = driver->getFPS();
			frametime = (float)(timekeeper->getTime() - lasttime)/1000.0; 
			lasttime = timekeeper->getTime();

			if (receiver.isKeyDown(KEY_KEY_Q))
			{
				break; //Then exit the loop
			}
			//draw some text
			if (font)
			{
				core::stringw str = L"Alone [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				font->draw(str,
					core::rect<s32>(130,10,300,50),
					video::SColor(255,255,255,255));
			}
			if (font2)
			{
				core::stringw str = L"Alone [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				font->draw(str,
					core::rect<s32>(130,40,300,50),
					video::SColor(255,255,255,255));
			}
			if (lastFPS != fps)
			{
				core::stringw str = L"Ludum Dare 48 [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		camera->updateAbsolutePosition();
	}

	device->closeDevice();
	SDL_Quit();

	return 0;
}
