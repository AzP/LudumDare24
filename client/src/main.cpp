#include <irrlicht.h>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cassert>

#include "player.h"
#include "level.h"
#include "eventreceiver.h"
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
	CDevice deviceSetup(receiver);
	

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

	IrrlichtDevice* device = DeviceSetup.getDevice();
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

