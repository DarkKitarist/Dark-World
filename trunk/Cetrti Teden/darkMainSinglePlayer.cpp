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
	mChar				= 0;
	mMode				= 0;
	exCamera2			= 0;
}

void darkMainSinglePlayer::playerInfo()
{
}

void darkMainSinglePlayer::enter()
{
	engine = irrklang::createIrrKlangDevice();
	engine2 = irrklang::createIrrKlangDevice();
	engine->play2D("media/darkMedia/Sounds/park_1.wav", true);

	//Sporocilo za Log
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SinglePlayer...");
	//Teren Manager
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager("TerrainSceneManager");

	//izbrisemo kurzor in razno
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();

	//Dodamo kamero
    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Ogre::Vector3(960, 404, 2892));
    m_pCamera->setNearClipDistance(5);
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth())/Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	m_pSceneMgr->setWorldGeometry("MenuWorld.cfg");

    buildGUI();
    createScene();
}

void darkMainSinglePlayer::enter(Config cfg)
{
	enter();
}

bool darkMainSinglePlayer::pause()
{
    return true;
}

void darkMainSinglePlayer::resume()
{
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
	Ogre::ColourValue fadeColour(0.9f, 0.9f, 0.9f);
//Ustvarimo sceno
	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Ogre", "Sinbad.mesh");
    m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SinbadNode");
    m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
    m_pOgreHeadNode->setPosition(Ogre::Vector3(950, 400, 2800));
	m_pOgreHeadNode->setVisible(true);
//Dodamo sence in skybox
	m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 8000, true);
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.4, 0.4, 0.4));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);
}

bool darkMainSinglePlayer::keyPressed(const OIS::KeyEvent &keyEventRef)
{ 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		//za prehod v pavzno stanje
        //pushAppState(findByName("PauseState"));
        return true;
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G))
    {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
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
    return true;
}
 
bool darkMainSinglePlayer::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
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
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame/1000;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
    getInput();
    moveCamera();
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
