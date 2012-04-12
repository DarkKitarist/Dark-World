/*

TO je koda za vstopni prostor, je podobna guiInit ampak se razlikuje v tem, da je delujoè GUI, da delujejo tipke in da se v ozadju predvaja film

*/
#include "StdAfx.h"
#include "IntroGameState.h"
#include <time.h>

using namespace Ogre;

IntroState::IntroState()
{
    m_bQuit         = false;
	flagOnce		= true;
	time_ellapsed   = false;
    m_FrameEvent    = Ogre::FrameEvent();
}
time_t start, end;

void IntroState::enter()
{
	start = clock();
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering IntroState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Ogre::Vector3(0, 5, 20));
    m_pCamera->lookAt(Ogre::Vector3(0, 5, 0));
    m_pCamera->setNearClipDistance(1);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	dshowMovieTextureSystem = new OgreUtils::DirectShowMovieTexture (1282, 720);

	//tukaj naložimo film in ga zaženemo
	Ogre::String movieName="media\\Nvidia Intro logo.mp4";
	dshowMovieTextureSystem->loadMovie(movieName);
	dshowMovieTextureSystem->playMovie();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	createGUI();
    createScene();
}

void IntroState::createGUI()
{
	mTitleLabel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "IntroLabel1", "", 150);
	mTitleLabel2 = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "IntroLabel2", "", 300);
	mTitleLabel3 = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "IntroLabel3", "", 300);
}

void IntroState::createScene()
{
	MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState("rockwall.tga");
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

	//tukaj prilepimo film na prej ustvarjeno teksturo
	mat=Ogre::MaterialManager::getSingleton().getByName(materialName);
	tex=mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	tex->setTextureName(dshowMovieTextureSystem->getMovieTexture()->getName());

	mTitleLabel->setCaption("Intro");
	mTitleLabel->show();

	mTitleLabel2->setCaption("Press ESC to Exit Game!");
	mTitleLabel2->show();

	mTitleLabel3->setCaption("Press Space to skip!");
	mTitleLabel3->show();


}
 
void IntroState::enter(Config cfg)
{
	enter();
}

void IntroState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving IntroState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
	dshowMovieTextureSystem->stopMovie();
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
	delete dshowMovieTextureSystem;
}

bool IntroState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	//Za preskok filma
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_SPACE))
    {
        m_bQuit = true;
		changeAppState(findByName("mainMenuState"));	
		return true;
    }

	//Za izhod iz igre
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		shutdown();
		return true;
    }
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}
 
bool IntroState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}
 
bool IntroState::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

bool IntroState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

bool IntroState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

void IntroState::update(double timeSinceLastFrame)
{
	end = clock();
	double dif = difftime(end, start);
	if(dif > 9200 && flagOnce == true)
	{
		m_bQuit = true;
		changeAppState(findByName("mainMenuState"));
		time_ellapsed = true;
		flagOnce = false;
	}
	else if (dif < 9200)
		//teksturo tukaj osvežujemo vsaki frejm
		dshowMovieTextureSystem->updateMovieTexture();
}

void IntroState::buttonHit(OgreBites::Button *button)
{

}