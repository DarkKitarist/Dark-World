#include "StdAfx.h"
#include "darkMainMenu.h"

using namespace Ogre;

darkMenu::darkMenu()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}

Ogre::Vector3 translate(0,0,0);

void darkMenu::enter()
{
	engine = irrklang::createIrrKlangDevice();

	engine->play2D("media/darkMedia/Sounds/03-cage_the_elephant-aint_no_rest_for_the_wicked.mp3", true);
	engine->setSoundVolume(0.1f);
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager("TerrainSceneManager");
    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Vector3(960, 404, 2892));
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
	createGUI();
    createScene();
}

//Izgradnja GUI-ja
void darkMenu::createGUI()
{
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_CENTER, "MenuLbl2", "Welcome to darkWorld", 400);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn4", "Continue", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Singleplayer", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn3", "Load Game", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn2", "Options", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Game", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MenuLbl1", "Main Menu", 250);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_CENTER);

	Ogre::StringVector items;
    items.push_back("Press \"Singleplayer\" to play alone");
	items.push_back("Press \"Continue\" to play from last save");
	items.push_back("Press \"Load Game\" to load a previous Game");
	items.push_back("Press \"Options\" to set Video Preferences");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "MenuLb3", "Help", 400);
	m_pDetailsPanelMenu = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_BOTTOMLEFT, "Help", 400, items);

	Ogre::StringVector items1;
	items1.push_back("World Pos X");
	items1.push_back("World Pos Y");
	items1.push_back("World Pos Z");
	m_pDetailsPanel1 = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 300, items1);
    m_pDetailsPanel1->show();	
}

void darkMenu::createScene()
{

	MainNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode", Ogre::Vector3 (0, 0, 0));
	//Svet 
	m_pSceneMgr->setWorldGeometry("MenuWorld.cfg");
	m_pSceneMgr->setAmbientLight(ColourValue(0.1f, 0.1f, 0.1f));
    Ogre::Light* l = m_pSceneMgr->createLight("MainLight");
    l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(-1,-1,0);
	//animiran vozl
	TestAnimationNode = MainNode->createChildSceneNode ("", Ogre::Vector3 (2393.0f, 245.0f, 3464.0f));
	Ogre::Entity* knot = m_pSceneMgr->createEntity ("knot", "knot.mesh");
	TestAnimationNode->attachObject(knot);

	treeNodes = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("treeNodes", Ogre::Vector3 (0, 0, 0));
	
	//Drevesa
	TreeNode1 = treeNodes->createChildSceneNode ("SceneTree1", Ogre::Vector3 (1410.0f, 290.0f, 3215.0f));
	Ogre::Entity* TreeEntity1 = m_pSceneMgr->createEntity ("Tree1", "tree.07.mesh");
	TreeNode1->scale(3.0f,3.0f,3.0f);
	TreeNode1->attachObject(TreeEntity1);
	//Drevesa
	TreeNode2 = treeNodes->createChildSceneNode ("SceneTree2", Ogre::Vector3 (1749.0f, 490.0f, 4187.0f));
	Ogre::Entity* TreeEntity2 = m_pSceneMgr->createEntity ("Tree2", "tree.05.mesh");
	TreeNode2->scale(5.0f,5.0f,5.0f);
	TreeNode2->attachObject(TreeEntity2);
	//Drevesa	
	TreeNode3 = treeNodes->createChildSceneNode ("SceneTree3", Ogre::Vector3 (1220.0f, 420.0f, 2398.0f));
	Ogre::Entity* TreeEntity3 = m_pSceneMgr->createEntity ("Tree3", "tree.09.mesh");
	TreeNode3->scale(1.0f,1.0f,1.0f);
	TreeNode3->attachObject(TreeEntity3);
	//Sistem delcev
	ParticleNode1 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode1", Ogre::Vector3 (2027, 252, 2999));
	Ogre::ParticleSystem* partSystem1 = m_pSceneMgr->createParticleSystem("Smoke2", "Examples/PurpleFountain");
	ParticleNode1->attachObject(partSystem1);
	//skybox
	m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 8000, true);
	//sence
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.4, 0.4, 0.4));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);
}

void darkMenu::enter(Config cfg)
{
	enter();
}

void darkMenu::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

//Premikanje kamere in naprej in nazaj
bool darkMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	float _hitrostPremikanja = 0.05;
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Exit or ESC command issued", "Do you really want to leave the game?");
        m_bQuestionActive = true;
    } 

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
    {
		translate += Ogre::Vector3(0,0,-10*_hitrostPremikanja);
    } 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
    {
		translate += Ogre::Vector3(0,0,10*_hitrostPremikanja);
    } 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
    {
		translate += Ogre::Vector3(-10*_hitrostPremikanja,0,0);
    } 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
    {
		translate += Ogre::Vector3(10*_hitrostPremikanja,0,0);
    } 
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}
 
bool darkMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	translate = Vector3(0,0,0);
    return true;
}
 
bool darkMenu::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

bool darkMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}

bool darkMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void darkMenu::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	float rotX = (OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().X.rel*timeSinceLastFrame/1500)*-1;
	float rotY = (OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().Y.rel*timeSinceLastFrame/1500)*-1;
	//kameri, ki smo jo ustvarili sedaj dodelimo funkcije
	m_pCamera->yaw(Ogre::Radian(rotX));
	m_pCamera->pitch(Ogre::Radian(rotY));
	m_pCamera->moveRelative(translate*timeSinceLastFrame);
	TestAnimationNode->yaw(Ogre::Degree(0.01 * timeSinceLastFrame));
	//update x y z koordinat
	m_pDetailsPanel1->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getPosition().x));
	m_pDetailsPanel1->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getPosition().y));
	m_pDetailsPanel1->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getPosition().z));
}

void darkMenu::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
        shutdown();
    else
	{
        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
	}
    m_bQuestionActive = false;
}

void darkMenu::buttonHit(OgreBites::Button *button)
{
	//prototipi za GUI
    if(button->getName() == "ExitBtn")
	{
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
        OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Sure?", "Really leave?");
        m_bQuestionActive = true;
	}
    else if(button->getName() == "EnterBtn")
	{
		engine->stopAllSounds();
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
        //changeAppState(findByName("SinglePlayerMenuState"));
	}
	else if(button->getName() == "EnterBtn2")
	{
		engine->stopAllSounds();
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
		changeAppState(findByName("DarkOptionsMenu"));
	}
	else if(button->getName() == "EnterBtn3")
	{
		engine->stopAllSounds();
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
		//changeAppState(findByName("LoadSaveMenu"));
	}
	else if(button->getName() == "EnterBtn4")
	{
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
	}
}