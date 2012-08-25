#include <level.h>

CLevel::CLevel(IVideoDriver* driver, ISceneManager* smgr) : mSmgr(smgr)
{
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	scene::IAnimatedMesh* terrainMesh = mSmgr->getMesh("../media/subdivided_plane.3ds");
	// scale the mesh by factor 100
	core::matrix4 m;
	m.setScale ( core::vector3df(5000,1,5000) );
	mSmgr->getMeshManipulator()->transformMesh( terrainMesh, m );

	scene::IMesh* tangentMesh;

	if (terrainMesh)
	{
		mSmgr->getMeshManipulator()->makePlanarTextureMapping(terrainMesh, 0.005f);
		mSmgr->getMeshManipulator()->recalculateNormals(terrainMesh);
		video::ITexture* normalMap = driver->getTexture("../media/normalmap.png");
		//if (normalMap)
		//	driver->makeNormalMapTexture(normalMap, 9.0f);

		tangentMesh = mSmgr->getMeshManipulator()->createMeshWithTangents(
			terrainMesh->getMesh(0));

		mGround = mSmgr->addMeshSceneNode(tangentMesh);
		mGround->setMaterialTexture(0, driver->getTexture("../media/tiles.png"));
		mGround->setMaterialTexture(1, normalMap);
		mGround->getMaterial(0).getTextureMatrix(0).setTextureScale(20.f, 20.f);
		mGround->getMaterial(0).getTextureMatrix(1).setTextureScale(20.f, 20.f);

		mGround->setMaterialFlag(video::EMF_FOG_ENABLE, false);
		mGround->setMaterialFlag(video::EMF_LIGHTING, false);
		mGround->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		mGround->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
		// adjust height for parallax effect
		mGround->getMaterial(0).MaterialTypeParam = 0.01f;
		mGround->setPosition(vector3df(-25.f, -5.f, 248.f));
		mGround->setRotation(vector3df(270.0f, 0.0f, 0.f));


		mWallsBox = smgr->addCubeSceneNode(-500.0f, 0, -1,
			vector3df(0.f, 0.f, 0.f),
			vector3df(0.f, 0.f, 0.f),
			vector3df(2.f, 2.0f, 1.0f)
			);
		mWallsBox->setMaterialTexture(0, driver->getTexture("../media/terrain.png"));
		mWallsBox->setMaterialFlag(video::EMF_LIGHTING, false);
		mWallsBox->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		mWallsBox->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		mGround->setMaterialType(video::EMT_SOLID);
		// drop mesh since it was created with a create.. call
		tangentMesh->drop();
	}

	// create triangle selector for the terrainMesh
	scene::ITriangleSelector* selector = mSmgr->createTriangleSelector(tangentMesh, mGround);
	mGround->setTriangleSelector(selector);
	selector->drop();
	selector = mSmgr->createTriangleSelector(mWallsBox->getMesh(), mWallsBox);
	mWallsBox->setTriangleSelector(selector);
	selector->drop();
}

CLevel::~CLevel()
{
}

void CLevel::addCollisionDetection(ISceneNode* node)
{
	// create collision response animator and attach it to the node
	scene::ISceneNodeAnimator* anim = mSmgr->createCollisionResponseAnimator(
		mGround->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
	anim = mSmgr->createCollisionResponseAnimator(
		mWallsBox->getTriangleSelector(), node, core::vector3df(1,1,1),
		core::vector3df(0,0,10),
		core::vector3df(0,0,0));
	node->addAnimator(anim);
	anim->drop();
}
