#include <irrlicht.h>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

#include "player.h"

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

bool keys[KEY_KEY_CODES_COUNT];
IrrlichtDevice* device;
static const int windowX = 800;
static const int windowY = 600;
bool quitGame = false;
bool leftmousebutton = false;


/*********
  Event reciever. Handles all the mouse and keyboard inputs.
 *********/
class MyEventReceiver : public IEventReceiver
{ 
public: 
	virtual bool OnEvent(const SEvent& event) 
	{ 
		if(event.EventType == EET_KEY_INPUT_EVENT) 
		{ 
			keys[event.KeyInput.Key] = event.KeyInput.PressedDown; 

			//In case of quit event
			if (event.KeyInput.Key == KEY_KEY_Q)
			{
			}
		} 
		else if (event.EventType == EET_MOUSE_INPUT_EVENT) 
		{ 
			if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
			}    
			if( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
			} 
		}

		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			if (device == 0)
			{
				return false;
			}    
			switch(event.GUIEvent.EventType)
			{

				case EGET_BUTTON_CLICKED:

					if (id == 101)
					{
						return true;
					}

					if (id == 102)
					{
						exit(0);
						return true;
					}
					break;

				default:
					break;
			}
		}

		return false; 
	}    
}; 

int main(int argc, char* argv[])
{
	MyEventReceiver receiver; 

	// Create device
	const dimension2d<u32> windowDimensions(windowX, windowY);
	device = createDevice(video::EDT_OPENGL, windowDimensions, 
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

	/*************
	  Create the camera
	 *************/
	scene::ICameraSceneNode *camera = device->getSceneManager()->addCameraSceneNode();
	camera->setFOV(PI/2.0f);
	scene::ISceneNode *cameraLookAtNode = smgr->addEmptySceneNode(); //node the camera is set to look at

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

	//Create a timer to keep track of time
	ITimer *timekeeper = device->getTimer(); 
	int lasttime = timekeeper->getRealTime();


	/************
	  Add lights
	 ************/
	smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f,0.9f,0.7f,1.0f),
		1000.0f);
	smgr->addLightSceneNode(0, core::vector3df(0,-400,0),
		video::SColorf(0.2f,0.4f,0.5f,1.0f),
		1000.0f);

	/************
	 * Create various variables used in gameplay
	 ************/
	float frametime;

#ifdef IRRKLANG
	irrklang::vec3df* sndPos = new irrklang::vec3df();
#endif

	cerr <<"starting main rendering loop " <<endl;
	int lastFPS = -1;
	leftmousebutton = false;
	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, 0);
			smgr->drawAll();
			driver->endScene();

			int fps = driver->getFPS();
			frametime = (float)(timekeeper->getRealTime() - lasttime)/1000.0; 
			lasttime = timekeeper->getRealTime();

			//if (quitGame)
			{
				break; //Then exit the loop
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
	}

	device->closeDevice();
	SDL_Quit();

	return 0;
}
