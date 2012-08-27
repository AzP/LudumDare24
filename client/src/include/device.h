#ifndef DEVICE_H
#define DEVICE_H

#include <irrlicht.h>
#include "eventreceiver.h"

using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;
using namespace irr::io;

static const int windowX = 1280;
static const int windowY = 720;

class CDevice
{

public:
	CDevice(MyEventReceiver& receiver);
	IrrlichtDevice* getDevice() const { return m_device; }
	IVideoDriver* getDriver() const { return m_driver; } 
	ISceneManager* getSceneManager() const { return m_sceneMgr; }
	ISceneNode* getCameraLookAtNode() const { return m_cameraLookAtNode; }
	ICameraSceneNode* getCameraNode() const { return m_cameraNode; }
	IGUIFont* getFont() const { return m_font; }
	IGUIEnvironment* getEnv() const { return m_env; }
	ITimer* getTimer() const { return m_timer; }
	void updateFrameTime();
	float getFrameTime() const { return m_frametime; }
	float getFPS();

private:
	void setupIrrlicht(MyEventReceiver&);
	IrrlichtDevice* m_device;
	IVideoDriver* m_driver;
	ISceneManager* m_sceneMgr;
	ISceneNode *m_cameraLookAtNode;
	ICameraSceneNode *m_cameraNode;
	IGUIEnvironment *m_env;
	IGUIFont *m_font;
	ITimer *m_timer;

	float m_frametime;
	float m_lasttime;
};
#endif
