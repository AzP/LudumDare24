#include <level.h>
#include <cassert>

CLevel::CLevel(IVideoDriver* driver, ISceneManager* smgr) : m_smgr(smgr)
{
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	setupSkyBox(driver);

	//Tower setup
	video::ITexture* towerNormalMap = driver->getTexture("../media/towernormalmap.bmp");
	scene::IAnimatedMesh* towerMesh = m_smgr->getMesh("../media/tower.3ds");
	m_towerNode = m_smgr->addMeshSceneNode(towerMesh);
	m_towerNode->setMaterialTexture(0, driver->getTexture("../media/tower.png"));
	m_towerNode->setMaterialTexture(1, towerNormalMap);
	m_towerNode->getMaterial(0).getTextureMatrix(0).setTextureScale(1.f, 1.f);
	m_towerNode->getMaterial(0).getTextureMatrix(1).setTextureScale(20.f, 20.f);
	m_towerNode->setMaterialFlag(video::EMF_LIGHTING, true);
	m_towerNode->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	m_towerNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	m_towerNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
	m_towerNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
	m_towerNode->setPosition(vector3df(-0.6,-9.5,-4.2));
	m_towerNode->setScale ( core::vector3df(3,4,3) );


	// add terrain scene node
	m_terrain = smgr->addTerrainSceneNode( "../media/terrain-heightmap.png",
		m_towerNode, // parent node
		-1, // node id
		core::vector3df(-7000.f, -610.f, -7000.f),
		core::vector3df(0.f, 0.f, 0.f), // rotation
		core::vector3df(55.f, 7.3f, 55.f), // scale
		video::SColor ( 255, 255, 255, 255), // vertexColor
		5, // maxLOD
		scene::ETPS_17, // patchSize
		4 // smoothFactor
		);

	assert(m_terrain);
	m_terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	m_terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	m_terrain->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
	m_terrain->setMaterialFlag(EMF_ANTI_ALIASING, true);
	m_terrain->setMaterialTexture(0, driver->getTexture("../media/terrain-texture.png"));
	m_terrain->setMaterialTexture(1, driver->getTexture("../media/terrain-detailmap.png"));
	m_terrain->setMaterialType(video::EMT_DETAIL_MAP);
	m_terrain->scaleTexture(1.0f, 5000.0f);
	m_terrain->setDebugDataVisible(false);
	// create triangle selector for the terrainMesh
	scene::ITriangleSelector* selector = m_smgr->createTerrainTriangleSelector(m_terrain, 0);
	m_terrain->setTriangleSelector(selector);
	selector->drop();
	selector = m_smgr->createTriangleSelector(m_towerNode->getMesh(), m_towerNode);
	m_towerNode->setTriangleSelector(selector);
	selector->drop();
}

CLevel::~CLevel()
{
}

void CLevel::setupSkyBox(IVideoDriver* driver)
{
	//Skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	m_smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/skybox/box6.jpg"),
		driver->getTexture("../media/skybox/box5.jpg"),
		driver->getTexture("../media/skybox/box1.jpg"),
		driver->getTexture("../media/skybox/box2.jpg"),
		driver->getTexture("../media/skybox/box3.jpg"),
		driver->getTexture("../media/skybox/box4.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

}

void CLevel::addCollisionDetection(ISceneNode* node)
{
	// create collision response animator and attach it to the node
	scene::ISceneNodeAnimator* anim = m_smgr->createCollisionResponseAnimator(
		m_terrain->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
	anim = m_smgr->createCollisionResponseAnimator(
		m_towerNode->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
}
