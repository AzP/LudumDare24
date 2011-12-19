/**********
 * Event reciever. Handles all the mouse and keyboard inputs.
 **********/
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

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
}; 



