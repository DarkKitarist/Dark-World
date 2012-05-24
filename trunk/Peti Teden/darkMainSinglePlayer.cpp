#include "StdAfx.h"
#include "darkMainSinglePlayer.h"
#include "HelperLogics.h"
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


darkMainSinglePlayer::darkMainSinglePlayer() 
{
    m_MoveSpeed		= 0.1f;
    m_RotateSpeed	= 0.3f;
 
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_bSettingsMode     = false;
	objectiveCompleted	= false;
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

	wallNode1 = wallNodes->createChildSceneNode ("wallNode1", Ogre::Vector3 (3000.0f, 160.0f, 3215.0f));

//Glavni Node za Sceno 1
	MainNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode", Ogre::Vector3 (0, 0, 0));

	darkThirdPersonCamera *exCamera = new darkThirdPersonCamera ("Extended Camera", m_pSceneMgr, m_pCamera);
	m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 8000, true);

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.4, 0.4, 0.4));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);

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

    getInput();
    moveCamera();
	if(mCollisionTools->collidesWithEntity(oldPos, mChar->worldNode->getPosition(),2.5f,-1.0f,ENTITY_MASK))
	{
		coll = true;
		mChar->worldNode->setPosition(oldPos);
	}
	//if(ogre->worldNode->getPosition().y > 230)
	//{
	//	coll = true;
	//	mChar->worldNode->setPosition(oldPos);
	////}
	cout<<"Z:  "<<ogre->worldNode->getPosition().z<<" X:  "<<ogre->worldNode->getPosition().x;
}

void darkMainSinglePlayer::buildGUI()
{
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