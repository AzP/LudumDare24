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

		else if (event.EventType == EET_MOUSE_INPUT_EVENT) 
		{ 
			//mouse position
			m_mouseX = event.MouseInput.X; 
			m_mouseY = event.MouseInput.Y; 
			m_centerX = ((float)(m_mouseX-m_windowX/2))/((float)(m_windowX));
			m_centerY = ((float)(m_mouseY-m_windowY/2))/((float)(m_windowY));

			if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				m_leftmousebutton = true;
			}    
			if( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
				m_leftmousebutton = false;
			} 
		}

		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
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

	virtual bool isKeyDown(EKEY_CODE keyCode) const
	{
		return mKeyIsDown[keyCode];
	}

	virtual const bool* getKeys() const
	{
		return mKeyIsDown;
	}

	virtual void setWindowDimension(int x, int y)
	{
		m_windowX = x;
		m_windowY = y;
	}

	MyEventReceiver()
	{
		for(u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			mKeyIsDown[i] = false;
	}

	virtual bool processInput(vector3df& cameraPosition, vector3df& cameraRotation, bool& debugCamera)
	{
		if (debugCamera)
		{
			if (isKeyDown(KEY_KEY_F))
				cameraPosition.Z += 5; //Forward
			if (isKeyDown(KEY_KEY_D))
				cameraPosition.Z -= 5; //Backward
			if (isKeyDown(KEY_KEY_C))
				cameraPosition.Y += 1; //Up
			if (isKeyDown(KEY_KEY_T))
				cameraPosition.Y -= 1; //Down
			if (isKeyDown(KEY_KEY_N))
				cameraPosition.X += 1; //Right
			if (isKeyDown(KEY_KEY_H))
				cameraPosition.X -= 1; //Left
		}
		if (isKeyDown(KEY_UP))
			cameraRotation.X -= 1; //Up
		if (isKeyDown(KEY_DOWN))
			cameraRotation.X += 1; //Down
		if (isKeyDown(KEY_RIGHT))
			cameraRotation.Z -= 1; //Right
		if (isKeyDown(KEY_LEFT))
			cameraRotation.Z += 1; //Left
		if (isKeyDown(KEY_KEY_D))
		{
			if(!wasPressedLastFrame)
				debugCamera = debugCamera ? false : true; //Toggle debug mode
			wasPressedLastFrame = true;
		}
		else
			wasPressedLastFrame = false;

		if (cameraRotation.X > -40)
			cameraRotation.X = -40;
		if (cameraRotation.X < -140)
			cameraRotation.X = -140;


		if (isKeyDown(KEY_KEY_Q))
			return true; //Return true to quit

		return false;
	}

private:
	bool mKeyIsDown[KEY_KEY_CODES_COUNT];
	bool wasPressedLastFrame;
	bool m_leftmousebutton;
	s32 m_mouseX; 
	s32 m_mouseY; 
	float m_centerX;
	float m_centerY;
	int m_windowX;
	int	m_windowY;
}; 

#endif
