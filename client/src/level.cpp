#include <level.h>
#include <cassert>

CLevel::CLevel(IVideoDriver* driver, ISceneManager* smgr) : mSmgr(smgr)
{
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	//scene::IAnimatedMesh* terrainMesh = mSmgr->getMesh("../media/subdivided_plane.3ds");
	// scale the mesh by factor 100
	//core::matrix4 m;
	//	m.setScale ( core::vector3df(5000,1,5000) );
	//mSmgr->getMeshManipulator()->transformMesh( terrainMesh, m );

	scene::IMesh* tangentMesh;

	//mSmgr->getMeshManipulator()->makePlanarTextureMapping(terrainMesh, 0.005f);
	//mSmgr->getMeshManipulator()->recalculateNormals(terrainMesh);
	video::ITexture* normalMap = driver->getTexture("../media/normalmap.png");
	//if (normalMap)
	//	driver->makeNormalMapTexture(normalMap, 9.0f);

	/*
	tangentMesh = mSmgr->getMeshManipulator()->createMeshWithTangents(terrainMesh->getMesh(0));
	mGround = mSmgr->addMeshSceneNode(tangentMesh);
	mGround->setMaterialTexture(0, driver->getTexture("../media/tiles.png"));
	mGround->setMaterialTexture(1, normalMap);
	mGround->getMaterial(0).getTextureMatrix(0).setTextureScale(20.f, 20.f);
	mGround->getMaterial(0).getTextureMatrix(1).setTextureScale(20.f, 20.f);

	mGround->setMaterialFlag(video::EMF_LIGHTING, false);
	mGround->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	mGround->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	mGround->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
	// adjust height for parallax effect
	mGround->getMaterial(0).MaterialTypeParam = 0.01f;
	mGround->setPosition(vector3df(-25.f, -5.f, 248.f));
	mGround->setRotation(vector3df(0.0f, 0.0f, 0.f));
	mGround->setMaterialType(video::EMT_SOLID);
	*/
	scene::IAnimatedMesh* towerMesh = mSmgr->getMesh("../media/tower.3ds");
	m_towerNode = mSmgr->addMeshSceneNode(towerMesh);
	m_towerNode->setMaterialTexture(0, driver->getTexture("../media/tower.png"));
	m_towerNode->setMaterialTexture(1, normalMap);
	m_towerNode->getMaterial(0).getTextureMatrix(0).setTextureScale(1.f, 1.f);
	m_towerNode->getMaterial(0).getTextureMatrix(1).setTextureScale(20.f, 20.f);
	m_towerNode->setMaterialFlag(video::EMF_LIGHTING, true);
	m_towerNode->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	m_towerNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	m_towerNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
	m_towerNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
	m_towerNode->setPosition(vector3df(-0.6,-9.5,-4.2));
	m_towerNode->setScale ( core::vector3df(3,4,3) );

	// drop mesh since it was created with a create.. call
	//tangentMesh->drop();

	// create triangle selector for the terrainMesh
	//scene::ITriangleSelector* selector = mSmgr->createTriangleSelector(tangentMesh, mGround);
	//mGround->setTriangleSelector(selector);
	//selector->drop();
	//	selector = mSmgr->createTriangleSelector(m_towerNode->getMesh(), m_towerNode);
	//	m_towerNode->setTriangleSelector(selector);
	//	selector->drop();

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/skybox/box6.jpg"),
		driver->getTexture("../media/skybox/box5.jpg"),
		driver->getTexture("../media/skybox/box1.jpg"),
		driver->getTexture("../media/skybox/box2.jpg"),
		driver->getTexture("../media/skybox/box3.jpg"),
		driver->getTexture("../media/skybox/box4.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

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
	scene::ITriangleSelector* selector = mSmgr->createTerrainTriangleSelector(m_terrain, 0);
	m_terrain->setTriangleSelector(selector);
	selector->drop();
	selector = mSmgr->createTriangleSelector(m_towerNode->getMesh(), m_towerNode);
	m_towerNode->setTriangleSelector(selector);
	selector->drop();
}

CLevel::~CLevel()
{
}

void CLevel::addCollisionDetection(ISceneNode* node)
{
	// create collision response animator and attach it to the node
	scene::ISceneNodeAnimator* anim = mSmgr->createCollisionResponseAnimator(
		m_terrain->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
	anim = mSmgr->createCollisionResponseAnimator(
		m_towerNode->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
}
