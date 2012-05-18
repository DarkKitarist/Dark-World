#include "StdAfx.h"
#include "darkCharrSelectMenu.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Ogre;
 
//inicializacije
darkCharrSelectMenu::darkCharrSelectMenu()
{
    m_MoveSpeed			= 0.1f;
    m_RotateSpeed		= 0.3f;
	charSelectionFlag	= 0;
	CharName			= "";
	CharType			= "";
	CharAttack			= "";
	infoText			= "";
	modelType			= "";
 
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_bSettingsMode     = false;
	m_nameSelected		= false;
	m_CharSelected		= false;
	m_bQuestionActive	= false;
	m_useOldProfile		= false;
//za izbiro 3 možnih modelov za igranje
	m_OgreSelected		= false;
	m_NinjaSelected		= false;
	m_RobotSelected		= false;

	profileNumber		= 0;

    m_pDetailsPanel		= 0;
}

//ta funkcija prebere naše izbire iz dokumenta in nas vpraša, èe želimo te izbire uporabiti
//prav tako se te izbire prenesejo na igralno površino
void darkCharrSelectMenu::populateProfile()
{
	int numLines = 0, wakawaka = 0;
	ifstream myfile("profile.dark");
	if(myfile.is_open())
	{
		while(!myfile.eof())
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
	wakawaka = profileRead[4].find_first_of(':') + 1;
	CharAttack = profileRead[4].substr(wakawaka, profileRead[4].length());

	//ce je karakter Ogre se nastavijo te nastavitve
	if(CharType == "Ogre")
	{
		m_pCurrentObject = m_pOgreHeadNode;
		m_pCurrentEntity = m_pOgreHeadEntity;
		CharType = "Ogre";
		modelType = "Sinbad.mesh";
		CharAttack = "Idle";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 23\n");
		infoText.append("Defense: 10\n");
		infoText.append("Speed: 5");
		m_CharSelected = true;
		m_nameSelected = true;
	}

	//ce je karakter Ninja se nastavijo te nastavitve
	else if(CharType == "Ninja")
	{
		m_pCurrentObject1 = m_pNinjaNode;
		m_pCurrentEntity1 = m_pNinjaEntity;
		CharType = "Ninja";
		modelType = "Ninja.mesh";
		CharAttack = "Idle";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 5\n");
		infoText.append("Defense: 8\n");
		infoText.append("Speed: 25");
		m_CharSelected = true;
		m_nameSelected = true;
	}

	//ce je karakter Robot se nastavijo te nastavitve
	else if(CharType == "Robot")
	{
		m_pCurrentObject2 = m_pRobotNode;
		m_pCurrentEntity2 = m_pRobotEntity;
		CharType = "Robot";
		modelType = "robot.mesh";
		CharAttack = "Idle";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 15\n");
		infoText.append("Defense: 15\n");
		infoText.append("Speed: 8");
		m_CharSelected = true;
		m_nameSelected = true;
	}
}
 
//tukaj se nalozijo zvokovi in razne druge stvari, ki so potrebne na zaèetku
void darkCharrSelectMenu::enter()
{
	engine2 = irrklang::createIrrKlangDevice();
	//ambient zvok
	engine2->play2D("media/darkMedia/Sounds/fire-1.mp3", true);
	int numLines = 0;

	//beremo iz dokumenta, ki vsebuje profil
	ifstream myfile("profile.dark");
	if(myfile.is_open())
	{
		while(!myfile.eof())
		{
			getline(myfile, profileRead[numLines]);			
			numLines++;
		}
	}
	myfile.close();
	//ce je dokument z profilom prazen, pomeni, da se profil ni ustvarjen
	if(numLines > 0)
    {
		OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Create new profile?", "An old profile has been detected\n\nPress YES to create new profile.\nPress NO to use old one.\n\n!!Old Profile will be deleted if NEW is created!!");
        m_bQuestionActive = true;
    }

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SinglePlayer...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
	
	//tukaj se nalozi kamera
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(0, 0, 5));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(5);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_pCurrentObject = 0;
	m_pCurrentObject1 = 0;
	m_pCurrentObject2 = 0;

    buildGUI();
    createScene();
}

//tukaj izgradimo GUI za izbiro karakterja
void darkCharrSelectMenu::buildGUI()
{
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLb2", "Singleplayer Menu", 200);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();

	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "GameLb3", "Character Creation:", 350);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_TOPLEFT, "Seperator0", 350);
    chatModes.push_back("Ogre");
    chatModes.push_back("Ninja");
    chatModes.push_back("Robot");
	m_selectRace = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPLEFT, "ChatModeSelMenu",   "      Race:  ", 200, 3, chatModes);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_TOPLEFT, "Seperator1", 350);
    basicAttack.push_back("Idle");
    basicAttack.push_back("Attack1");
    basicAttack.push_back("Attack2");
	m_selectAtack = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPLEFT, "ChatModeSelMenu1", "   Attack: ", 200, 3, basicAttack);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_TOPLEFT, "Seperator2", 350);
    Ogre::StringVector items;
    items.push_back("Character Name: ");
    items.push_back("Character Type: ");
	items.push_back("Character Attack: ");
	m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "Character Details", 350, items);
    m_pDetailsPanel->show();
 
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_TOPLEFT, "Seperator3", 350);
    infoText = "Character Information:\n\n";
	infoText.append("Atack: N/A\n");
    infoText.append("Defense: N/A\n");
    infoText.append("Speed: N/A");
	m_textBox = OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "InfoPanel", infoText, 350, 150);
 
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_TOPLEFT, "Seperator4", 350);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "GameLbl", "Character Creation Screen\n\nClick and type Character Name:\n", 350, 75);	
	m_InputButton = OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_TOPLEFT, "CharNameButton", "", 350);
	CharType = "None Selected";

	m_StartGameButton = OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "EnterSingleGame", "START",350);
	m_StartGameButton->hide();
}

//genericna funkcija
void darkCharrSelectMenu::enter(Config cfg)
{
	enter();
}

//prototip za PAUSE menu
bool darkCharrSelectMenu::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");
    return true;
}

//prototip za Resume, ki se bo uporabil ko bo delal PAUSE menu
void darkCharrSelectMenu::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
    buildGUI();
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

//funkcija za izhod
void darkCharrSelectMenu::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");
	engine2->stopAllSounds();
    m_pSceneMgr->destroyCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

//funkcija, ki ustvari vse 3D modele od katerih je samo Ogre viden na zacetku
void darkCharrSelectMenu::createScene()
{
	CharAttack = "Idle";
    m_pSceneMgr->setAmbientLight(ColourValue(0.4f, 0.4f, 0.4f));

    Ogre::Light* l = m_pSceneMgr->createLight("MainLight");
	l->setPosition(10,10,20);
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(1, -1, 0);
	//glavni node sveta
	Ogre::SceneNode *MainNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("MainWorldNode", Ogre::Vector3 (0, -60, -250));
	Ogre::ParticleSystem* partSystem = m_pSceneMgr->createParticleSystem("Smoke", "Examples/Smoke");
	//efekt ognja v ozadju
	MainNode->attachObject(partSystem);

	//tukaj se nalozi Ogre model
    m_pOgreHeadEntity = m_pSceneMgr->createEntity("Ogre", "Sinbad.mesh");
    m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SinbadNode");
    m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
    m_pOgreHeadNode->setPosition(Vector3(2, 0, -9));
	m_pOgreHeadNode->setVisible(true);
 
	//tukaj se vzpostavijo animacije
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


	//tukaj se nalozi Ninja model
	m_pNinjaEntity = m_pSceneMgr->createEntity("Ninja", "Ninja.mesh");
    m_pNinjaNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
    m_pNinjaNode->attachObject(m_pNinjaEntity);
	m_pNinjaNode->setScale(0.04f,0.04f,0.04f);
	m_pNinjaNode->setDirection(Ogre::Vector3(10,0,150),Ogre::Node::TS_LOCAL,Vector3::NEGATIVE_UNIT_Z);
    m_pNinjaNode->setPosition(Vector3(2, -4, -7));
	m_pNinjaNode->setVisible(false);
 
	//tukaj se vzpostavijo animacije
	m_animStateNinja = m_pNinjaEntity->getAnimationState("Idle3");
	m_animStateNinja->setEnabled(true);
	m_animStateNinja->setLoop(true);

	Ogre::AnimationStateSet* set = m_pNinjaEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator iter = set->getAnimationStateIterator();

	std::cout<<"Ninja Anims: " << std::endl;
	while(iter.hasMoreElements())
	{
		std::cout<<iter.getNext()->getAnimationName()<<std::endl;
	}

	//tukaj se nalozi Robot 
	m_pRobotEntity = m_pSceneMgr->createEntity("Robot", "robot.mesh");
    m_pRobotNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode");
    m_pRobotNode->attachObject(m_pRobotEntity);
	m_pRobotNode->setScale(0.12f,0.12f,0.12f);
	m_pRobotNode->setDirection(Ogre::Vector3(150,0,15),Ogre::Node::TS_LOCAL,Vector3::NEGATIVE_UNIT_Z);
    m_pRobotNode->setPosition(Vector3(2, -5, -9));
	m_pRobotNode->setVisible(false);

	//tukaj se vzpostavijo animacije za Robot-a
	m_animStateRobot = m_pRobotEntity->getAnimationState("Idle");
	m_animStateRobot->setEnabled(true);
	m_animStateRobot->setLoop(true);

	std::cout<<"Robot Anims: " << std::endl;
	Ogre::AnimationStateSet* set1 = m_pRobotEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator iter1 = set1->getAnimationStateIterator();

	while(iter1.hasMoreElements())
	{
		std::cout<<iter1.getNext()->getAnimationName()<<std::endl;
	}

	//tukaj se nastavijo sence
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 8000, true);
}

//moral sem ustvariti funkcijo,ki lovi iz tipkovnice znake in jih izpise na ekran
bool darkCharrSelectMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_bSettingsMode == true)
    {
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_BACK))
		{		
			CharName = "";
		}
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
		{
			CharName = CharName + "A";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
		{
			CharName = CharName + "S";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
		{
			CharName = CharName + "D";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
		{
			CharName = CharName + "F";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G))
		{
			CharName = CharName + "G";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_H))
		{
			CharName = CharName + "H";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_J))
		{
			CharName = CharName + "J";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_K))
		{
			CharName = CharName + "K";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_L))
		{
			CharName = CharName + "L";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Y))
		{
			CharName = CharName + "Y";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_X))
		{
			CharName = CharName + "X";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
		{
			CharName = CharName + "C";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_V))
		{
			CharName = CharName + "V";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_B))
		{
			CharName = CharName + "B";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_N))
		{
			CharName = CharName + "N";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_M))
		{
			CharName = CharName + "M";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
		{
			CharName = CharName + "Q";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
		{
			CharName = CharName + "W";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E))
		{
			CharName = CharName + "E";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_R))
		{
			CharName = CharName + "R";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_T))
		{
			CharName = CharName + "T";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Z))
		{
			CharName = CharName + "Z";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_U))
		{
			CharName = CharName + "U";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
		{
			CharName = CharName + "I";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O))
		{
			CharName = CharName + "O";
		}
		else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_P))
		{
			CharName = CharName + "P";
		}

		if(CharName.length() > 0)
		{
			m_nameSelected = true;
		}
		else if(CharName.length() == 0)
		{
			m_nameSelected = false;
		}
    }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		if(!m_bQuestionActive)
		{
			//pushAppState(findByName("PauseState"));
			shutdown();
		}
		return true;
    }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
    {
        if(m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
            m_pDetailsPanel->show();
        }
        else
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
            m_pDetailsPanel->hide();
        }
    }
  
    if(m_bSettingsMode && OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RETURN) ||
        OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPADENTER))
    {
    }
 
    if(!m_bSettingsMode || (m_bSettingsMode && !OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O)))
        OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}

//dedovana funkcija za lovenje spusta tipke
bool darkCharrSelectMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//funkcija, ki lovi premike miske
bool darkCharrSelectMenu::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
 
    if(m_bRMouseDown)
    {

    }
 
    return true;
}

//funkcija, ki lovi, ce je bil miskin gumb stisnjen
bool darkCharrSelectMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
 
    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = true;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = true;
    }
 
    return true;
}

//funkcija, ki lovi, ce je bil miskin gumb spuscen 
bool darkCharrSelectMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
 
    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = false;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = false;
    }
 
    return true;
}

//funkcija za stisk levega gumba na miski, ki sem jo uporabljal samo za teste
void darkCharrSelectMenu::onLeftPressed(const OIS::MouseEvent &evt)
{

}

//funkcija za premik kamere, ki sem jo uporabljal samo za test
void darkCharrSelectMenu::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

//funkcija za pridobivanje vnosov
void darkCharrSelectMenu::getInput()
{
    if(m_bSettingsMode == false)
    {
    }
}

//funkcija, ki skrbi, da se vsako sekundo izrise 60 frejmov oz. kolko mamo nastavljeno
void darkCharrSelectMenu::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
 
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
	//casovi za animacije
	m_animStateOgre->addTime(timeSinceLastFrame/1000);
	m_animStateOgre1->addTime(timeSinceLastFrame/1000);
	m_animStateNinja->addTime(timeSinceLastFrame/1000);
	m_animStateRobot->addTime(timeSinceLastFrame/1000);

    if(!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
    {
        if(m_pDetailsPanel->isVisible())
        {
            m_pDetailsPanel->setParamValue(0, CharName);
            m_pDetailsPanel->setParamValue(1, CharType);
			m_pDetailsPanel->setParamValue(2, CharAttack);
			m_InputButton->setCaption(CharName);
			m_textBox->setCaption(infoText);
        }
    }

	if(m_nameSelected && m_CharSelected)
	{
		m_StartGameButton->show();
	}
	else
	{
		m_StartGameButton->hide();
	}
 
    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
 
    m_TranslateVector = Vector3::ZERO;
 
    getInput();
    moveCamera();
}

//funkcija, ki se izvede ko uporabnik konca z izborom
//v tej funkciji se prav tako ustvari dokument s profilom
void darkCharrSelectMenu::buttonHit(OgreBites::Button *button)
{
	int numLines = 0;
    if(button->getName() == "CharNameButton")
    {
		m_bSettingsMode = !m_bSettingsMode;
	}
	if(button->getName() == "EnterSingleGame")
	{
		Ogre::String Test;	
		Test.append("Profile Number:" + Ogre::StringConverter::toString(profileNumber));
		Test.append("\n");
		Test.append("Profile Name:" + CharName);
		Test.append("\n");
		Test.append("Character Name:" + CharName);
		Test.append("\n");
		Test.append("Character Type:" + CharType);
		Test.append("\n");
		Test.append("Char Model:" + modelType);
		Test.append("\n");
		Test.append("Char Attack:" + CharAttack);
		ofstream myfile1("profile.dark");
		if(myfile1.is_open())
		{
			myfile1 << Test;
		}
		myfile1.close();
		changeAppState(findByName("DarkSinglePlayer"));
	}
}

//funkcija za dialog box za profil
void darkCharrSelectMenu::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
	{
        m_useOldProfile = false;
		m_bQuestionActive = false;

	}
    else
	{
		m_useOldProfile = true;
		m_bQuestionActive = false;
		populateProfile();
        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
	}
}

//funkcija z switch stavki, ki so vezani na dropdown sezname, kar izberemo se preslika na switch stavek
void darkCharrSelectMenu::itemSelected(OgreBites::SelectMenu* menu)
{
    switch(m_selectRace->getSelectionIndex())
    {
    case 0:
		m_pOgreHeadNode->setVisible(true);
		m_pNinjaNode->setVisible(false);
		m_pRobotNode->setVisible(false);
		CharType = "Ogre";
		modelType = "Sinbad.mesh";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 23\n");
		infoText.append("Defense: 10\n");
		infoText.append("Speed: 5");
		m_OgreSelected = true;
		m_RobotSelected = false;
		m_NinjaSelected = false;
		m_CharSelected = true;
		break;
    case 1:
		m_pOgreHeadNode->setVisible(false);
		m_pNinjaNode->setVisible(true);
		m_pRobotNode->setVisible(false);
		CharType = "Ninja";
		modelType = "Ninja.mesh";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 5\n");
		infoText.append("Defense: 8\n");
		infoText.append("Speed: 25");
		m_OgreSelected = false;
		m_NinjaSelected = true;
		m_RobotSelected = false;
		m_CharSelected = true;
		break;
    case 2:
		m_pOgreHeadNode->setVisible(false);
		m_pNinjaNode->setVisible(false);
		m_pRobotNode->setVisible(true);
		CharType = "Robot";
		modelType = "robot.mesh";
		infoText = "";
		infoText = "Character Information:\n\n";
		infoText.append("Atack: 15\n");
		infoText.append("Defense: 15\n");
		infoText.append("Speed: 8");
		m_OgreSelected = false;
		m_NinjaSelected = false;
		m_RobotSelected = true;
		m_CharSelected = true;
		break;
    }

	switch(m_selectAtack->getSelectionIndex())
    {
    case 0:
		if(m_OgreSelected)
		{
			m_pOgreHeadEntity->detachAllObjectsFromBone();		
			m_pOgreHeadEntity->attachObjectToBone("Sheath.L", mSword1);
			m_pOgreHeadEntity->attachObjectToBone("Sheath.R", mSword2);
			m_animStateOgre->setEnabled(false);
			m_animStateOgre->setLoop(false);
			m_animStateOgre = m_pOgreHeadEntity->getAnimationState("IdleTop");
			m_animStateOgre1 = m_pOgreHeadEntity->getAnimationState("IdleBase");
			m_animStateOgre->setEnabled(true);
			m_animStateOgre->setLoop(true);
			m_animStateOgre->setWeight(50);
			m_animStateOgre1->setEnabled(true);
			m_animStateOgre1->setLoop(true);
			CharAttack = "Idle";
		}
		if(m_NinjaSelected)
		{
			m_animStateNinja->setEnabled(false);
			m_animStateNinja->setLoop(false);
			m_animStateNinja = m_pNinjaEntity->getAnimationState("Idle3");
			m_animStateNinja->setEnabled(true);
			m_animStateNinja->setLoop(true);
			CharAttack = "Idle";
		}
		if(m_RobotSelected)
		{
			m_animStateRobot->setEnabled(false);
			m_animStateRobot->setLoop(false);
			m_animStateRobot = m_pRobotEntity->getAnimationState("Idle");
			m_animStateRobot->setEnabled(true);
			m_animStateRobot->setLoop(true);
			CharAttack = "Idle";
		}
		break;
    case 1:
		if(m_OgreSelected)
		{
			m_pOgreHeadEntity->detachAllObjectsFromBone();	
			m_pOgreHeadEntity->attachObjectToBone("Sheath.L", mSword1);
			m_pOgreHeadEntity->attachObjectToBone("Sheath.R", mSword2);
			m_animStateOgre->setEnabled(false);
			m_animStateOgre->setLoop(false);
			m_animStateOgre1->setEnabled(false);
			m_animStateOgre1->setWeight(0);
			m_animStateOgre1->setLoop(false);
			m_animStateOgre = m_pOgreHeadEntity->getAnimationState("Dance");
			m_animStateOgre->setEnabled(true);
			m_animStateOgre->setLoop(true);
			CharAttack = "Dance";
		}
		if(m_NinjaSelected)
		{
			m_animStateNinja->setEnabled(false);
			m_animStateNinja->setLoop(false);
			m_animStateNinja = m_pNinjaEntity->getAnimationState("Attack1");
			m_animStateNinja->setEnabled(true);
			m_animStateNinja->setLoop(true);
			CharAttack = "Attack1";
		}
		if(m_RobotSelected)
		{
			m_animStateRobot->setEnabled(false);
			m_animStateRobot->setLoop(false);
			m_animStateRobot = m_pRobotEntity->getAnimationState("Shoot");
			m_animStateRobot->setEnabled(true);
			m_animStateRobot->setLoop(true);
			CharAttack = "Shoot";
		}
		break;
    case 2:
		if(m_OgreSelected)
		{
			m_pOgreHeadEntity->detachAllObjectsFromBone();
			m_pOgreHeadEntity->attachObjectToBone("Handle.L", mSword1);
			m_pOgreHeadEntity->attachObjectToBone("Handle.R", mSword2);
			m_animStateOgre->setEnabled(false);
			m_animStateOgre->setLoop(false);
			m_animStateOgre1->setEnabled(false);
			m_animStateOgre1->setWeight(0);
			m_animStateOgre1->setLoop(false);
			m_animStateOgre = m_pOgreHeadEntity->getAnimationState("SliceHorizontal");
			m_animStateOgre->setEnabled(true);
			m_animStateOgre->setLoop(true);
			m_animStateOgre->setWeight(99);
			CharAttack = "Slice";
		}
		if(m_NinjaSelected)
		{
			m_animStateNinja->setEnabled(false);
			m_animStateNinja->setLoop(false);
			m_animStateNinja = m_pNinjaEntity->getAnimationState("Kick");
			m_animStateNinja->setEnabled(true);
			m_animStateNinja->setLoop(true);
			CharAttack = "Kick";
		}
		if(m_RobotSelected)
		{
			m_animStateRobot->setEnabled(false);
			m_animStateRobot->setLoop(false);
			m_animStateRobot = m_pRobotEntity->getAnimationState("Walk");
			m_animStateRobot->setEnabled(true);
			m_animStateRobot->setLoop(true);
			CharAttack = "Walk";
		}
		break;
    }
}