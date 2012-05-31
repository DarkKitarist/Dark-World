#include "StdAfx.h"
#include "darkMainSinglePlayer.h"
#include "HelperLogics.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;
using namespace Ogre;
using namespace OgreBites;

struct GrassVertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

darkMainSinglePlayer::darkMainSinglePlayer() : GRASS_WIDTH(40), GRASS_HEIGHT(40)
{
    m_MoveSpeed		= 0.1f;
    m_RotateSpeed	= 0.3f;
 
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_bSettingsMode     = false;
	objectiveCompleted	= false;
	objectiveCompleted1 = false;
	objectiveCompleted2 = false;
	objectiveCompleted3 = false;
	TrenutnoZdravje		= 1.0f;
	TrenutniAtackPoints	= 1.0f;
 
	mChar				= 0;
	mMode				= 0;
	exCamera2			= 0;

    m_pDetailsPanel	= 0;
}

time_t start2, end2;


void darkMainSinglePlayer::playerInfo()
{
	int numLines = 0, wakawaka = 0;
	ifstream myfile("profile.dark");
	if(myfile.is_open())
	{
		while(!myfile.eof()) // To get you all the lines.
		{
			getline(myfile, profileRead[numLines]);			
			numLines++;
		}
	}
	myfile.close();
	wakawaka = profileRead[0].find_first_of(':') + 1;
	profileNumber = Ogre::StringConverter::parseInt(profileRead[0].substr(wakawaka, profileRead[0].length()));
	wakawaka = profileRead[1].find_first_of(':') + 1;
	profileName = profileRead[1].substr(wakawaka, profileRead[1].length());
	CharName = profileName;
	wakawaka = profileRead[3].find_first_of(':') + 1;
	CharType = profileRead[3].substr(wakawaka, profileRead[3].length());
	wakawaka = profileRead[4].find_first_of(':') + 1;
	modelType = profileRead[4].substr(wakawaka, profileRead[4].length());
}

void darkMainSinglePlayer::createGrassMesh()
{
	MeshPtr mesh = MeshManager::getSingleton().createManual("grass", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	// create a submesh with the grass material
	SubMesh* sm = mesh->createSubMesh();
	sm->setMaterialName("Examples/GrassBlades");
	sm->useSharedVertices = false;
	sm->vertexData = OGRE_NEW VertexData();
	sm->vertexData->vertexStart = 0;
	sm->vertexData->vertexCount = 12;
	sm->indexData->indexCount = 18;

	// specify a vertex format declaration for our mesh: 3 floats for position, 3 floats for normal, 2 floats for UV
	VertexDeclaration* decl = sm->vertexData->vertexDeclaration;
    decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
    decl->addElement(0, sizeof(float) * 3, VET_FLOAT3, VES_NORMAL);
    decl->addElement(0, sizeof(float) * 6, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

	// create a vertex buffer
	HardwareVertexBufferSharedPtr vb = HardwareBufferManager::getSingleton().createVertexBuffer
		(decl->getVertexSize(0), sm->vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	GrassVertex* verts = (GrassVertex*)vb->lock(HardwareBuffer::HBL_DISCARD);  // start filling in vertex data

	for (unsigned int i = 0; i < 3; i++)  // each grass mesh consists of 3 planes
	{
		// planes intersect along the Y axis with 60 degrees between them
		Real x = Math::Cos(Degree(i * 60)) * GRASS_WIDTH / 2;
		Real z = Math::Sin(Degree(i * 60)) * GRASS_WIDTH / 2;

		for (unsigned int j = 0; j < 4; j++)  // each plane has 4 vertices
		{
			GrassVertex& vert = verts[i * 4 + j];

			vert.x = j < 2 ? -x : x;
			vert.y = j % 2 ? 0 : GRASS_HEIGHT;
			vert.z = j < 2 ? -z : z;

			// all normals point straight up
			vert.nx = 0;
			vert.ny = 1;
			vert.nz = 0;

			vert.u = j < 2 ? 0 : 1;
			vert.v = j % 2;
		}
	}

	vb->unlock();  // commit vertex changes

	sm->vertexData->vertexBufferBinding->setBinding(0, vb);  // bind vertex buffer to our submesh

	// create an index buffer
	sm->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer
		(HardwareIndexBuffer::IT_16BIT, sm->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// start filling in index data
	Ogre::uint16* indices = (Ogre::uint16*)sm->indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD);

	for (unsigned int i = 0; i < 3; i++)  // each grass mesh consists of 3 planes
	{
		unsigned int off = i * 4;  // each plane consists of 2 triangles

		*indices++ = 0 + off;
		*indices++ = 3 + off;
		*indices++ = 1 + off;

		*indices++ = 0 + off;
		*indices++ = 2 + off;
		*indices++ = 3 + off;
	}

	sm->indexData->indexBuffer->unlock();  // commit index changes
}

void darkMainSinglePlayer::enter()
{
	//ustvarimo Motion Blur
	engine = irrklang::createIrrKlangDevice();
	engine2 = irrklang::createIrrKlangDevice();
	engine->play2D("media/darkMedia/Sounds/park_1.wav", true);

	Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
		"Motion Blur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
	);
	{
		Ogre::CompositionTechnique *t = comp3->createTechnique();
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Initialisation pass for sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("sum");
			tp->setOnlyInitial(true);
		}
		/// Do the motion blur
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Combine");
			pass->setInput(0, "scene");
			pass->setInput(1, "sum");
			}
		}
		/// Copy back sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("sum");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Copyback");
			pass->setInput(0, "temp");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/MotionBlur");
			pass->setInput(0, "sum");
			}
		}
	}
	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();

	//dodamo kompositorje
	static bool firstTime = true;
	if (firstTime)
	{
		compMgr.registerCompositorLogic("HDR", new HDRLogic);
		firstTime = false;
	}
	//Sporocilo za Log
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SinglePlayer...");
	//Teren Manager
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager("TerrainSceneManager");

	//izbrisemo kurzor in razno
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();

	//Dodamo kamero
    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Ogre::Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(5);
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth())/Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	//Dolocimo kaksno obliko bo nas svet imel
	m_pSceneMgr->setWorldGeometry("darkPlayerWorld1.cfg");
	
	mCollisionTools = new MOC::CollisionTools(m_pSceneMgr);

	//Dodamo Kompositor Efekte
	Ogre::CompositorManager::getSingleton().addCompositor(m_pCamera->getViewport(), "Bloom", -1);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Bloom", true);
	Ogre::CompositorManager::getSingleton().addCompositor(m_pCamera->getViewport(), "B&W",-1);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "B&W", false);
	Ogre::CompositorManager::getSingleton().addCompositor(m_pCamera->getViewport(), "Motion Blur",-1);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Motion Blur", false);

	//Dobimo informacije o igralcu
	playerInfo();
    buildGUI();
    createScene();
}

void darkMainSinglePlayer::enter(Config cfg)
{
	enter();
	ogre->worldNode->setPosition(cfg["Pos_x"].toNumber<int>(), 
		cfg["Pos_y"].toNumber<int>(), cfg["Pos_z"].toNumber<int>());
}

bool darkMainSinglePlayer::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Bloom", false);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "B&W", false);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Motion Blur", false);
    return true;
}

void darkMainSinglePlayer::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
    buildGUI();
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Bloom", true);
    m_bQuit = false;
}

void darkMainSinglePlayer::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState..."); 
    m_pSceneMgr->destroyCamera(m_pCamera);
	engine->stopAllSounds();
	engine2->stopAllSounds();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}



void darkMainSinglePlayer::createScene()
{
//Luci za sceno 1
    m_pSceneMgr->setAmbientLight(ColourValue(0.1f, 0.1f, 0.1f));
    Ogre::Light* l = m_pSceneMgr->createLight("MainLight");
    l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(-1,-1,0);
	
	wallNodes = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("wallNodes", Ogre::Vector3 (0, 0, 0));

	//wallNode1 = wallNodes->createChildSceneNode ("wallNode1", Ogre::Vector3 (3000.0f, 160.0f, 3215.0f));

//Glavni Node za Sceno 1
	MainNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode", Ogre::Vector3 (0, 0, 0));

	darkThirdPersonCamera *exCamera = new darkThirdPersonCamera ("Extended Camera", m_pSceneMgr, m_pCamera);
	m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 8000, true);

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.4, 0.4, 0.4));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);

	createGrassMesh();
	Entity* grass = m_pSceneMgr->createEntity("Grass", "grass");

	mField = m_pSceneMgr->createStaticGeometry("Field");
	mField->setRegionDimensions(Ogre::Vector3(140, 140, 140));
	mField->setOrigin(Ogre::Vector3(7110, 1500, 710));
	for (int x = -280; x < 280; x += 20)
	{
		for (int z = -280; z < 280; z += 20)
		{
			Ogre::Vector3 pos(x + Math::RangeRandom(-7, 7), 0, z + Math::RangeRandom(-7, 7));
			Quaternion ori(Degree(Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 scale(1, Math::RangeRandom(0.85, 1.15), 1);

			mField->addEntity(grass, pos, ori, scale);
		}
	}

	mField->build();

	wallNode1 = wallNodes->createChildSceneNode ("Wall11", Ogre::Vector3 (7956.0f, 150.0f, 1020.0f));
	Ogre::Entity* wallEntity1 = m_pSceneMgr->createEntity ("Wall1", "wall1.mesh");
	wallNode1->scale(4.2f,0.5f,0.5f);
	wallNode1->setDirection(-0.5,0,0);
	wallNode1->attachObject(wallEntity1);

	wallNode2 = wallNodes->createChildSceneNode ("Wall12", Ogre::Vector3 (6900.0f, 150.0f, 100.0f));
	Ogre::Entity* wallEntity2 = m_pSceneMgr->createEntity ("Wall2", "wall1.mesh");
	wallNode2->scale(4.4f,0.5f,0.5f);
	wallNode2->setDirection(0,0,0);
	wallNode2->attachObject(wallEntity2);

	wallNode3 = wallNodes->createChildSceneNode ("Wall13", Ogre::Vector3 (1200.0f, 150.0f, 8000.0f));
	Ogre::Entity* wallEntity3 = m_pSceneMgr->createEntity ("Wall3", "wall1.mesh");
	wallNode3->scale(4.7f,0.5f,0.5f);
	wallNode3->setDirection(0,0,0);
	wallNode3->attachObject(wallEntity3);

	wallNode4 = wallNodes->createChildSceneNode ("Wall14", Ogre::Vector3 (15.0f, 150.0f, 6580.0f));
	Ogre::Entity* wallEntity4 = m_pSceneMgr->createEntity ("Wall4", "wall1.mesh");
	wallNode4->scale(5.3f,0.5f,0.5f);
	wallNode4->setDirection(-0.5,0,0);
	wallNode4->attachObject(wallEntity4);

	wallNode5 = wallNodes->createChildSceneNode ("Wall1Gate", Ogre::Vector3 (6600.0f, 150.0f, 2222.0f));
	Ogre::Entity* wallEntity5 = m_pSceneMgr->createEntity ("Wall1GGate", "wall1.mesh");
	wallNode5->scale(0.8f,0.5f,0.5f);
	wallNode5->attachObject(wallEntity5);

	chestNode1 = wallNodes->createChildSceneNode ("chest1", Ogre::Vector3 (1899.0f, 188.0f, 1810.0f));
	Ogre::Entity* chestEntity1 = m_pSceneMgr->createEntity ("chestGate1", "chestW.mesh");
	chestNode1->scale(0.05f,0.05f,0.05f);
	chestNode1->attachObject(chestEntity1);

	chestNode2 = wallNodes->createChildSceneNode ("chest2", Ogre::Vector3 (7029.0f, 188.0f, 7655.0f));
	Ogre::Entity* chestEntity2 = m_pSceneMgr->createEntity ("chestGate2", "chestW.mesh");
	chestNode2->scale(0.05f,0.05f,0.05f);
	chestNode2->attachObject(chestEntity2);

	chestNode3 = wallNodes->createChildSceneNode ("chest3", Ogre::Vector3 (7600.0f, 188.0f, 5020.0f));
	Ogre::Entity* chestEntity3 = m_pSceneMgr->createEntity ("chestGate3", "chestW.mesh");
	chestNode3->scale(0.05f,0.05f,0.05f);
	chestNode3->attachObject(chestEntity3);

	house = wallNodes->createChildSceneNode ("house1", Ogre::Vector3 (7324.0f, 167.0f, 756.0f));
	Ogre::Entity* houseEnt1 = m_pSceneMgr->createEntity ("houseGate1", "highlanderhouse.01.mesh");
	house->scale(6.0f,6.05f,6.05f);
	house->attachObject(houseEnt1);

//Glavni Karakter
	ogre = new darkCharacterSpec ("MainCharacter", m_pSceneMgr, CharName, CharType, modelType, MainNode, m_pCamera);
	mChar = ogre;
	exCamera2 = exCamera;
}

bool darkMainSinglePlayer::keyPressed(const OIS::KeyEvent &keyEventRef)
{ 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("darkPause"));
		//shutdown();
        return true;
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G))
    {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
    }

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E))
    {
		if(ogre->worldNode->getPosition().distance(chestNode1->getPosition()) < 23)
		{
			objectiveCompleted	= true;
		}
		if(ogre->worldNode->getPosition().distance(chestNode2->getPosition()) < 23)
		{
			objectiveCompleted1	= true;
		}
		if(ogre->worldNode->getPosition().distance(chestNode3->getPosition()) < 23)
		{
			objectiveCompleted2	= true;
		}
    }

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
    {
		engine2->play2D("media/darkMedia/Sounds/walking.mp3", true);
	}

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_B))
	{
		
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Bloom", true);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Bloom", false);
	}

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_N))
	{
		
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "B&W", true);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "B&W", false);
	}
    return true;
}

bool darkMainSinglePlayer::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	engine2->stopAllSounds();
    return true;
}

bool darkMainSinglePlayer::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) 
	{
		return true;
	}
    return true;
}

bool darkMainSinglePlayer::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
  
    if(id == OIS::MB_Left && TrenutniAtackPoints > 0.1f)
    {
		ogre->setLeft(true);
		if(CharType == "Ninja" || CharType == "Ogre")
		{
			engine2->play2D("media/darkMedia/Sounds/Sword.wav", true);
		}
		else
		{
			engine2->play2D("media/darkMedia/Sounds/ShotGun.wav", true);
		}
        m_bLMouseDown = true;
    }

    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = true;
    }
 
    return true;
}

bool darkMainSinglePlayer::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_bLMouseDown = false;
	engine2->stopAllSounds();
	ogre->setLeft(false);
	return true;
}

void darkMainSinglePlayer::onLeftPressed(const OIS::MouseEvent &evt)
{
}

void darkMainSinglePlayer::moveCamera()
{
}

void darkMainSinglePlayer::getInput()
{
}


void darkMainSinglePlayer::update(double timeSinceLastFrame)
{
	if(ogre->getCharIsMoving())
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Motion Blur", true);
	}
	else
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Motion Blur", false);
	}

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame/1000;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

	if(!m_bLMouseDown && TrenutniAtackPoints < 1.0)
	{
		TrenutniAtackPoints = TrenutniAtackPoints + 0.001;
	}

	if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	bool coll = false;
 
	Ogre::Vector3 oldPos = mChar->worldNode->getPosition();
	mCollisionTools->calculateY(mChar->worldNode,true,true,1.0f,ENTITY_MASK);

	if (mChar) {
	mChar->update (m_FrameEvent.timeSinceLastFrame, OgreFramework::getSingletonPtr()->m_pKeyboard, OgreFramework::getSingletonPtr()->m_pMouse);
		if (exCamera2) {
			switch (mMode) {
				case 0: // 3rd person chase
					exCamera2->update (m_FrameEvent.timeSinceLastFrame, 
									mChar->getCameraNode()->_getDerivedPosition(),
									mChar->getSightNode()->_getDerivedPosition());
					break;
				case 1: // 3rd person fixed
					exCamera2->update (m_FrameEvent.timeSinceLastFrame, 
										Ogre::Vector3(0, 200, 0), 
										mChar->getSightNode ()->_getDerivedPosition());
					break;
				case 2: // 1st person
					exCamera2->update (m_FrameEvent.timeSinceLastFrame, 
										mChar->getWorldPosition (), 
										mChar->getSightNode ()->_getDerivedPosition());
					break;
			}
		}
	}
 
	// 3rd Person - Chase Camera
	if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown (OIS::KC_F1)) {
		mMode = 0;
		if (mChar)
			static_cast<darkCharacterSpec *>(mChar)->setVisible (true);
		if (exCamera2) {
			if (mChar)
				exCamera2->instantUpdate (mChar->getCameraNode ()->_getDerivedPosition(), mChar->getSightNode ()->_getDerivedPosition());
			exCamera2->setTightness (0.5f);
		}
	}
	// 3rd Person - Fixed Camera
	if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown (OIS::KC_F2)) {
		mMode = 1;
		if (mChar)
			static_cast<darkCharacterSpec *>(mChar)->setVisible (true);
		if (exCamera2) {
			if (mChar)
				exCamera2->instantUpdate (Ogre::Vector3 (0, 200, 0), mChar->getSightNode ()->_getDerivedPosition());
			exCamera2->setTightness (0.5f);
		}
	}
	// 1st Person
	if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown (OIS::KC_F3))  {
		mMode = 2;
		if (mChar)
			static_cast<darkCharacterSpec *>(mChar)->setVisible (false);
		if (exCamera2) {
			if (mChar)
				exCamera2->instantUpdate (mChar->getWorldPosition (), mChar->getSightNode ()->_getDerivedPosition());
			exCamera2->setTightness (1.0f);
		}
	}

	if(objectiveCompleted && objectiveCompleted1 && objectiveCompleted2)
	{
		objectiveCompleted3 = true;
		wallNode5->detachAllObjects();
	}

	if(!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
    {
        if(m_pDetailsPanel->isVisible())
        {
			if(!objectiveCompleted)
				m_pDetailsPanel->setParamValue(1, "Not Completed");
			else
				m_pDetailsPanel->setParamValue(1, "Completed");
			if(!objectiveCompleted1)
				m_pDetailsPanel->setParamValue(2, "Not Completed");
			else
				m_pDetailsPanel->setParamValue(2, "Completed");
			if(!objectiveCompleted2)
				m_pDetailsPanel->setParamValue(3, "Not Completed");
			else
				m_pDetailsPanel->setParamValue(3, "Completed");
			if(!objectiveCompleted3)
				m_pDetailsPanel->setParamValue(4, "Not Completed");
			else
				m_pDetailsPanel->setParamValue(4, "Completed");
			m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(ogre->worldNode->getPosition().distance(chestNode1->getPosition())) + " m");
			m_pDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(ogre->worldNode->getPosition().distance(chestNode2->getPosition())) + " m");
			m_pDetailsPanel->setParamValue(8, Ogre::StringConverter::toString(ogre->worldNode->getPosition().distance(chestNode3->getPosition())) + " m");
			m_pDetailsPanel->setParamValue(9, Ogre::StringConverter::toString(ogre->worldNode->getPosition().distance(house->getPosition())) + " m");
        }
    }

    getInput();
    moveCamera();
	if(mCollisionTools->collidesWithEntity(oldPos, mChar->worldNode->getPosition(),2.5f,-1.0f,ENTITY_MASK))
	{
		coll = true;
		mChar->worldNode->setPosition(oldPos);
	}
	else if(ogre->worldNode->getPosition().distance(chestNode1->getPosition()) < 23)
	{
		pickup->show();
	}
	else if(ogre->worldNode->getPosition().distance(chestNode2->getPosition()) < 23)
	{
		pickup->show();
	}
	else if(ogre->worldNode->getPosition().distance(chestNode3->getPosition()) < 23)
	{
		pickup->show();
	}
	else
	{
		pickup->hide();
	}

	if(ogre->worldNode->getPosition().y > 200)
	{
		coll = true;
		mChar->worldNode->setPosition(oldPos);
	}
}

void darkMainSinglePlayer::buildGUI()
{
	Ogre::StringVector items2;
	items2.push_back("Find the 3 chests.");
	items2.push_back("Then the wall will dissapear");
	items2.push_back("Finaly go home!");

	m_pDetailsPanel2 = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_BOTTOMRIGHT, "DetailsPanel2", 300, items2);
    m_pDetailsPanel2->show();

	Ogre::StringVector items;
	items.push_back("Objectives:");
    items.push_back("1. First Chests Picked");
	items.push_back("2. Second Chests Picked");
	items.push_back("3. Third Chests Picked");
	items.push_back("4. Gate Opened");
	items.push_back("Distance");
	items.push_back("First Chest");
	items.push_back("Second Chest");
	items.push_back("Third Chest");
	items.push_back("Finish");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "GameLevel1Lb12", "Objectives", 300);
	m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_BOTTOMLEFT, "DetailsPanel", 300, items);
    m_pDetailsPanel->show();	

	pickup = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_CENTER, "Pickuplabel", "Pickup", 250);
	pickup->hide();
	
}

void darkMainSinglePlayer::buttonHit(OgreBites::Button *button)
{
}

void darkMainSinglePlayer::itemSelected(OgreBites::SelectMenu* menu)
{
}

void darkMainSinglePlayer::tempSave()
{
}