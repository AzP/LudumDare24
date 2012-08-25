#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>

using namespace std;
using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;
using namespace irr::io;


/**********
 * Event reciever. Handles all the mouse and keyboard inputs.
 **********/
class MyEventReceiver : public IEventReceiver
{ 
public: 
	virtual bool OnEvent(const SEvent& event) 
	{ 
		if(event.EventType == EET_KEY_INPUT_EVENT) 
			mKeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown; 

		return false;
	} 

	virtual bool isKeyDown(EKEY_CODE keyCode) const
	{
		return mKeyIsDown[keyCode];
	}

	virtual const bool* getKeys() const
	{
		return mKeyIsDown;
	}

	MyEventReceiver()
	{
		for(u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			mKeyIsDown[i] = false;
	}
	/*
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
	 */
virtual bool processInput(vector3df& cameraposition, vector3df& camerarotation, bool& debugCamera)
{
	if (isKeyDown(KEY_KEY_F))
		cameraposition.Z += 5; //Forward
	if (isKeyDown(KEY_KEY_D))
		cameraposition.Z -= 5; //Backward
	if (isKeyDown(KEY_KEY_C))
		cameraposition.Y += 1; //Up
	if (isKeyDown(KEY_KEY_T))
		cameraposition.Y -= 1; //Down
	if (isKeyDown(KEY_KEY_N))
		cameraposition.X += 1; //Right
	if (isKeyDown(KEY_KEY_H))
		cameraposition.X -= 1; //Left
	if (isKeyDown(KEY_KEY_P))
		camerarotation.X += 1; //Up
	if (isKeyDown(KEY_KEY_U))
		camerarotation.X -= 1; //Down
	if (isKeyDown(KEY_KEY_I))
		camerarotation.Z += 1; //Right
	if (isKeyDown(KEY_KEY_E))
		camerarotation.Z -= 1; //Left
	if (isKeyDown(KEY_KEY_G))
	{
		if(!wasPressedLastFrame)
			debugCamera = debugCamera ? false : true; //Toggle debug mode
		wasPressedLastFrame = true;
	}
	else
		wasPressedLastFrame = false;

	if (isKeyDown(KEY_KEY_Q))
		return true; //Return true to quit

	return false;
}

private:
bool mKeyIsDown[KEY_KEY_CODES_COUNT];
bool wasPressedLastFrame;
}; 

#endif
