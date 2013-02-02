#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>

using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::gui;

/**********
 * Event reciever. Handles all the mouse and keyboard inputs.
 **********/
class MyEventReceiver : public IEventReceiver
{ 
public: 

	MyEventReceiver()
		: wasPressedLastFrame(0), m_leftmousebutton(0), m_mouseX(0), m_mouseY(0), m_centerX(0), m_centerY(0), m_windowX(0), m_windowY(0)
	{
		for(u32 i = 0; i<irr::KEY_KEY_CODES_COUNT; ++i)
			mKeyIsDown[i] = false;
	}

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

	virtual bool isKeyDown(EKEY_CODE keyCode) const { return mKeyIsDown[keyCode]; }
	virtual const bool* getKeys() const { return mKeyIsDown; }
	virtual void setWindowDimension(int x, int y) { m_windowX = x; m_windowY = y; }
	virtual bool getLeftMouseState() const { return m_leftmousebutton; }
	virtual bool processInput(vector3df& cameraPosition, vector3df& cameraRotation, bool& debugCamera)
	{
		if (isKeyDown(irr::KEY_KEY_Q))
			return true; //Return true to quit
		if (isKeyDown(irr::KEY_KEY_D))
		{
			if(!wasPressedLastFrame)
				debugCamera = debugCamera ? false : true; //Toggle debug mode
			wasPressedLastFrame = true;
		}
		else
			wasPressedLastFrame = false;

		return false;
	}

private:
	bool mKeyIsDown[irr::KEY_KEY_CODES_COUNT];
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
