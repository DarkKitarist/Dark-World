#include "StdAfx.h"
#include "darkMainMenu.h"

using namespace Ogre;

darkMenu::darkMenu()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
	conttin			= false;
	loading			= false;
}

Ogre::Vector3 translate(0,0,0);

void darkMenu::enter()
{
	engine = irrklang::createIrrKlangDevice();

	engine->play2D("media/darkMedia/Sounds/14-klaus_badelt-one_last_shot-fntx.mp3", true);
	engine->setSoundVolume(0.3f);
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager("TerrainSceneManager");
    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Vector3(2940, 247, 3047));
	m_pCamera->lookAt(Vector3(336, 423, 3491));
 
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
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
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
	TreeNode2 = treeNodes->createChildSceneNode ("SceneTree2", Ogre::Vector3 (1749.0f, 480.0f, 4187.0f));
	Ogre::Entity* TreeEntity2 = m_pSceneMgr->createEntity ("Tree2", "tree.05.mesh");
	TreeNode2->scale(5.0f,5.0f,5.0f);
	TreeNode2->attachObject(TreeEntity2);
	//Drevesa	
	TreeNode3 = treeNodes->createChildSceneNode ("SceneTree3", Ogre::Vector3 (1220.0f, 400.0f, 2398.0f));
	Ogre::Entity* TreeEntity3 = m_pSceneMgr->createEntity ("Tree3", "tree.09.mesh");
	TreeNode3->scale(1.0f,1.0f,1.0f);
	TreeNode3->attachObject(TreeEntity3);
	//Drevesa
	TreeNode4 = treeNodes->createChildSceneNode ("SceneTree4", Ogre::Vector3 (1535.0f, 150.0f, 2801.0f));
	Ogre::Entity* TreeEntity4 = m_pSceneMgr->createEntity ("Tree4", "tree.07.mesh");
	TreeNode4->scale(1.0f,1.0f,1.0f);
	TreeNode4->attachObject(TreeEntity4);

	TreeNode5 = treeNodes->createChildSceneNode ("SceneTree5", Ogre::Vector3 (473.0f, 700.0f, 4400.0f));
	Ogre::Entity* TreeEntity5 = m_pSceneMgr->createEntity ("Tree5", "tree.09.mesh");
	TreeNode5->scale(3.0f,3.0f,3.0f);
	TreeNode5->attachObject(TreeEntity5);

	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Ogre", "Sinbad.mesh");
    m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SinbadNode");
    m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
    m_pOgreHeadNode->setPosition(Vector3(1900, 220, 2743));
	m_pOgreHeadNode->setVisible(true);
	m_pOgreHeadNode->scale(16.0f,16.0f,16.0f);
	m_pOgreHeadNode->setDirection(-0.5,0,0);
	m_animStateOgre = m_pOgreHeadEntity->getAnimationState("IdleBase");
	m_animStateOgre1= m_pOgreHeadEntity->getAnimationState("IdleTop");
	m_animStateOgre->setEnabled(true);
	m_animStateOgre1->setEnabled(true);
	m_animStateOgre->setLoop(true);
	m_animStateOgre1->setWeight(50);
	m_animStateOgre1->setLoop(true);
	mSword1 = m_pSceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = m_pSceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	m_pOgreHeadEntity->attachObjectToBone("Sheath.L", mSword1);
	m_pOgreHeadEntity->attachObjectToBone("Sheath.R", mSword2);

	sexyTree = treeNodes->createChildSceneNode ("sexyNode1", Ogre::Vector3 (2600.0f, 150.0f, 2922.0f));
	Ogre::Entity* sexyEntity = m_pSceneMgr->createEntity ("sexyEntity1", "sexy.mesh");
	sexyTree->scale(1.0f,1.0f,1.0f);
	sexyTree->attachObject(sexyEntity);

	//Sistem delcev
	ParticleNode1 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode1", Ogre::Vector3 (2027, 200, 2999));
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
	m_animStateOgre->addTime(timeSinceLastFrame/1000);
	m_animStateOgre1->addTime(timeSinceLastFrame/1000);

	float rotX = (OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().X.rel*timeSinceLastFrame/1500)*-1;
	float rotY = (OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().Y.rel*timeSinceLastFrame/1500)*-1;
	TestAnimationNode->yaw(Ogre::Degree(0.01 * timeSinceLastFrame));
}

void darkMenu::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
		if (conttin == false && loading ==false)
			shutdown();
		else
			OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
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
		conttin = false;
		loading = false;
        m_bQuestionActive = true;
	}
    else if(button->getName() == "EnterBtn")
	{
		engine->stopAllSounds();
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
        changeAppState(findByName("DarkSelectMenu"));
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
		OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Under Construction", "You can press anything!");
		loading = true;
		//changeAppState(findByName("LoadSaveMenu"));
	}
	else if(button->getName() == "EnterBtn4")
	{
		engine->play2D("media/darkMedia/Sounds/ButtonKlik.wav");
		OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Under Construction", "You can press anything!");
		conttin = true;
	}
}