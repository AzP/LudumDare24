#include <iostream>
#include <cassert>

#include "device.h"

float CDevice::getFPS()
{
	return m_driver->getFPS();
}

void CDevice::updateFrameTime()
{ 
	m_frametime = (m_timer->getTime() - m_lasttime)/1000.0;
	m_lasttime = m_timer->getTime();
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
	m_cameraNode = m_sceneMgr->addCameraSceneNodeFPS(NULL, 100.0f, 0.1f, -1, NULL, 0, false);
	//(ISceneNode *parent=0, f32 rotateSpeed=100.0f, f32 moveSpeed=0.5f, s32 id=-1, SKeyMap *keyMapArray=0,
	//s32 keyMapSize=0, bool noVerticalMovement=false, f32 jumpSpeed=0.f, bool invertMouse=false, bool makeActive=true)

	m_cameraNode->setFOV(PI/2.5f);
	m_cameraNode->setPosition(core::vector3df(0.0f,0.0f,0.0f));
	m_cameraNode->setFarValue(42000.0f);

	//Setup crosshair
	scene::ISceneNode* bill = m_sceneMgr->addBillboardSceneNode(
		m_cameraNode, 
		dimension2d<f32>(0.1, 0.1), vector3df(0,0,1));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
	//bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	bill->setMaterialTexture(0, m_driver->getTexture("../media/crosshair.png"));

	/************
	 * Set up fog
	 ************/
	m_driver->setFog(video::SColor(10,125,125,138), video::EFT_FOG_LINEAR, 
		3000.f, 10000.f, .01f, true, true);

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


