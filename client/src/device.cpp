#include <iostream>
#include <cassert>

#include "device.h"

float CDevice::getFPS()
{
	return m_driver->getFPS();
}

float CDevice::getFrameTime()
{ 
	int frametime = (m_timer->getTime() - m_lasttime)/1000.0;
	m_lasttime = m_timer->getTime();
	return frametime;
}


CDevice::CDevice(MyEventReceiver& receiver)
: m_lasttime(0)
{
	setupIrrlicht(receiver);
}

void CDevice::setupIrrlicht(MyEventReceiver& receiver)
{
	receiver.setWindowDimension(windowX, windowY);

	// Create device
	const dimension2d<u32> windowDimensions(windowX, windowY);
	m_device = createDevice(video::EDT_OPENGL, windowDimensions, 
		32, false, true, true, &receiver); 
	m_device->setResizable(false); 
	m_device->getCursorControl()->setVisible(false);

	// Check if device could be created. If not, terminate.
	if (m_device == 0) 
	{
		cerr << "Could not create device" << endl;
	}

	m_driver = m_device->getVideoDriver();
	m_sceneMgr = m_device->getSceneManager();

	m_env = m_device->getGUIEnvironment();
	m_font = m_env->getFont("../media/fonthaettenschweiler.bmp");
	assert(m_font);

	/*************
	  Create the camera
	 *************/
	m_cameraNode = m_sceneMgr->addCameraSceneNode();
	m_cameraNode->setFOV(PI/2.0f);
	m_cameraNode->setPosition(core::vector3df(0.0f,0.0f, -10.0f));
	m_cameraLookAtNode = m_sceneMgr->addEmptySceneNode(); //node the camera is set to look at

	scene::ISceneNode* bill = m_sceneMgr->addBillboardSceneNode(m_cameraLookAtNode, core::dimension2d<f32>(30, 30));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, m_driver->getTexture("../media/particlewhite.bmp"));
	/************
	 * Set up fog
	 ************/
	m_driver->setFog(video::SColor(30,125,125,138), video::EFT_FOG_EXP, 50.f, 100.f, .01f, true, true);

	/*******************
	  Creates the other players
	 *******************/

	/************
	  Add the gui
	 ************/
	m_driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	m_driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	//Create a timer to keep track of time
	m_timer = m_device->getTimer(); 
}


