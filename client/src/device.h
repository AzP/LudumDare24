#include <irrlicht.h>

#include "eventreceiver.h"

using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::gui;
using namespace irr::io;

static const int windowX = 800;
static const int windowY = 600;

class CDevice {

public:
	CDevice(MyEventReceiver& receiver);
	IrrlichtDevice* getDevice();

private:
	void setupIrrlicht(MyEventReceiver&);
	IrrlichtDevice* m_device;
};

CDevice::CDevice(MyEventReceiver& receiver)
{
	setupIrrlicht(receiver);
}

IrrlichtDevice* CDevice::getDevice()
{
	return m_device;
}

IVideoDriver* getDriver()
{

}

ISceneManager* getSceneManager()
{

}


void CDevice::setupIrrlicht(MyEventReceiver& receiver)
{
	// Create device
	const dimension2d<u32> windowDimensions(windowX, windowY);
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, windowDimensions, 
		32, false, true, true, &receiver); 
	device->setResizable(false); 

	// Check if device could be created. If not, terminate.
	if (device == 0) 
	{
		cerr << "Could not create device" << endl;
	}

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	gui::IGUIEnvironment* env = device->getGUIEnvironment();
	gui::IGUIFont* font = env->getFont("../media/fonthaettenschweiler.bmp");
	assert(font);

	/*************
	  Create the camera
	 *************/
	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setFOV(PI/2.0f);
	scene::ISceneNode *cameraLookAtNode = smgr->addEmptySceneNode(); //node the camera is set to look at
	camera->setPosition(core::vector3df(0.0f,0.0f,-15.0f));

	/************
	 * Set up fog
	 ************/
	driver->setFog(video::SColor(30,125,125,138), video::EFT_FOG_EXP, 50.f, 100.f, .01f, true, true);

	/***********************
	  Add the level and collision
	 ***********************/
	CLevel level(driver, smgr);

	//Creates the player
	CPlayer player = CPlayer(0, 0, device, level.getTriangleSelector());

	/*******************
	  Creates the other players
	 *******************/

	/************
	  Add the gui
	 ************/
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	//Create a timer to keep track of time
	ITimer *timekeeper = device->getTimer(); 
	int lasttime = timekeeper->getTime();

	/************
	  Add light and attatch to player
	 ************/
	scene::IVolumeLightSceneNode* playerVolumeLight =
		smgr->addVolumeLightSceneNode(player.getSceneNode(), -1);
	scene::ILightSceneNode* playerLight =
		smgr->addLightSceneNode(player.getSceneNode(), player.getRot() + vector3df(0,0,-10.0f),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 50.0f);
	// attach billboard to the light
	scene::ISceneNode* bill =
		smgr->addBillboardSceneNode(playerLight, core::dimension2d<f32>(30, 30));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlered.bmp"));

	//vector3d(horiz, height, vert);
	scene::ILightSceneNode* light2 =
		smgr->addLightSceneNode(level.getSceneNode(), core::vector3df(0,40,100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	bill = smgr->addBillboardSceneNode(light2, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlewhite.bmp"));

	scene::ILightSceneNode* light3 =
		smgr->addLightSceneNode(level.getSceneNode(), core::vector3df(0,40,-100),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 70.0f);
	bill = smgr->addBillboardSceneNode(light3, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlewhite.bmp"));
}

