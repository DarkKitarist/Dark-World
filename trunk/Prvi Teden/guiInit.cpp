/*

To je inicializacijska stopnja, ki sem jo moral narediti, drugaèe se GUI ne prikaže pravilno v Vstopnem prostoru.

*/

#include "StdAfx.h"
#include "guiInit.h"
#include <time.h>

using namespace Ogre;

initState::initState()
{
    m_bQuit         = false;
	flagOnce		= true;
	time_ellapsed   = false;
    m_FrameEvent    = Ogre::FrameEvent();
}
//ura za prekinitev faze
time_t start1, end1;

void initState::enter()
{
	start1 = clock();
	//Sporoèilo, ki se izpiše v konzoli
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering guiInit...");
 
	//osnovne stvari kot so kamera pa to 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Ogre::Vector3(0, 5, 20));
    m_pCamera->lookAt(Ogre::Vector3(0, 5, 0));
    m_pCamera->setNearClipDistance(1);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	//
	mTitleLabel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_NONE, "initLable", "", 150);
	mTitleLabel->hide();

	Ogre::StringVector items;
	initParamTable = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_NONE, "initHelp", 0, items);
	initParamTable->hide();

	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	//zaènemo renderirat sceno
    createScene();
}

void initState::createScene()
{
	MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("Loading.jpg");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
 
	//Ustvarimo 2D kvadrat, ki prekrije celonti ekrat
	Rectangle2D* rect = new Rectangle2D(true);
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial("Background");
 
	//najprej renderiramo ozadje potem pa vse ostalo
	rect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
 
	rect->setBoundingBox(AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
 
	// pripnemo teksturo na ozadje
	SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
	node->attachObject(rect);

	Ogre::MaterialPtr mat;
	Ogre::TextureUnitState* tex;
 
	Ogre::String materialName="Background";
	if (!Ogre::MaterialManager::getSingleton().resourceExists(materialName))
	{
		throw("Error, material doesn't exist!");
	}
}

void initState::enter(Config cfg)
{
	enter();
}

//oèistimo za sabo, ko preidemo
void initState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving initState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	Ogre::String materialName="MyMaterial";
	if (Ogre::MaterialManager::getSingleton().resourceExists(materialName))
	{
		Ogre::MaterialPtr mat;
		Ogre::TextureUnitState* tex;
		mat=Ogre::MaterialManager::getSingleton().getByName(materialName);
		tex=mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		tex->setTextureName(Ogre::String(""));
	}
}

//funkcije, ki se ukvarjajo s tipkovnico in miško
bool initState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    return true;
}
 
bool initState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool initState::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

bool initState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}
 
bool initState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//update funkcija, ki se kliše vsaki frame
void initState::update(double timeSinceLastFrame)
{
	end1 = clock();
	double dif = difftime(end1, start1);
	if(dif > 2000 && flagOnce == true)
	{
		m_bQuit = true;
		changeAppState(findByName("IntroState"));
		time_ellapsed = true;
		flagOnce = false;
	}
}

void initState::buttonHit(OgreBites::Button *button)
{

}