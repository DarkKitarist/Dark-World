#include "StdAfx.h"
#include "darkOptions.h"
#include <OgreRenderSystem.h>

#ifdef OGRE_STATIC_LIB
#if USE_RTSHADER_SYSTEM
    #include "ShaderSystem.h"
#endif
#endif

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

using namespace Ogre;
 
DarkOptionsMenu::DarkOptionsMenu()
{
    m_bQuit         = false;
	restarting		= false;
	m_bQuestionActive = false;
	mRendererMenu	= 0;
	menu			= 0;
    m_FrameEvent    = Ogre::FrameEvent();
}

void DarkOptionsMenu::enter()
{
	//Zaženemo vse pomembne stvari za zagon Options Menija
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering OptionsState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
	OgreFramework::getSingletonPtr()->m_pTrayMgr->showBackdrop("SdkTrays/Bands");
    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Vector3(0, 25, -50));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	m_bQuit = false;
	//Zgradimo GUI
	buildGUI();
    createScene();
}

void DarkOptionsMenu::buildGUI()
{
	//Zgradimo osnovne GUI elemente
	OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_RIGHT, "ApplyBtn", "Apply", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_RIGHT, "BackBtn", "Back", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "OptionsLbl1", "Options Menu", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_CENTER, "ConfigLabel", "Configuration");
	mRendererMenu = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_CENTER, "RendererMenu", "Render System", 450, 240, 10);

	//Ustvarimo meni za grafiène možnosti
	Ogre::StringVector rsNames;
	Ogre::RenderSystemList rsList = OgreFramework::getSingletonPtr()->m_pRoot->getAvailableRenderers();
	for (unsigned int i = 0; i < rsList.size(); i++)
	{
		rsNames.push_back(rsList[i]->getName());
	}
	mRendererMenu->setItems(rsNames);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createSeparator(OgreBites::TL_CENTER, "ConfigSeparator");
	mRendererMenu->selectItem(OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->getName());
	menu = mRendererMenu;
	while (OgreFramework::getSingletonPtr()->m_pTrayMgr->getNumWidgets(mRendererMenu->getTrayLocation()) > 3)
	{
		OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyWidget(mRendererMenu->getTrayLocation(), 3);
	}

	Ogre::ConfigOptionMap& options = OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystemByName(menu->getSelectedItem())->getConfigOptions();
	unsigned int i = 0;
	//ker beremo iz datoteke se zgradijo gradniki, ki predstavljajo možnosti za doloèen renderer
	for (Ogre::ConfigOptionMap::iterator it = options.begin(); it != options.end(); it++)
	{
		i++;
		OgreBites::SelectMenu* optionMenu = OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu
			(OgreBites::TL_CENTER, "ConfigOption" + Ogre::StringConverter::toString(i), it->first, 450, 240, 10);
		optionMenu->setItems(it->second.possibleValues);
		try
		{
			optionMenu->selectItem(it->second.currentValue);
		}
		catch (Ogre::Exception e)
		{
			optionMenu->addItem(it->second.currentValue);
			optionMenu->selectItem(it->second.currentValue);
		}
	}
}

void DarkOptionsMenu::createScene()
{
}

void DarkOptionsMenu::enter(Config cfg)
{
	enter();
}

//Unicimo vse elemente preden gremo iz Menija
void DarkOptionsMenu::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving OptionsState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideBackdrop();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

//Event za stisk tipke na tipkovnici
bool DarkOptionsMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) && !m_bQuestionActive)
    {
        m_bQuit = true;
        return true;
    }
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//Event za spust tipke na tipkovnici
bool DarkOptionsMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}

//Event za premik miske
bool DarkOptionsMenu::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}

//Event za klik miske
bool DarkOptionsMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}

//Event za spust miske
bool DarkOptionsMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void DarkOptionsMenu::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
	//Spemenimo GameState v MainMenuGameState
    if(m_bQuit == true)
    {
		changeAppState(findByName("mainMenuState"));
        return;
    }
}


void DarkOptionsMenu::reconfigure(const Ogre::String& renderer, Ogre::NameValuePairList& options)
{
	mNextRenderer = renderer;
	Ogre::RenderSystem* rs = OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystemByName(renderer);

	for (Ogre::NameValuePairList::iterator it = options.begin(); it != options.end(); it++)
	{
		rs->setConfigOption(it->first, it->second);
	}
	OgreFramework::getSingletonPtr()->m_pRoot->saveConfig();
}

void DarkOptionsMenu::buttonHit(OgreBites::Button *button)
{
	//Ko izvedemo apply se zapišejo v datoteko nastavitve
    if(button->getName() == "ApplyBtn")			
	{
		bool reset = false;
		Ogre::ConfigOptionMap& options =
			 OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystemByName(mRendererMenu->getSelectedItem())->getConfigOptions();

		Ogre::NameValuePairList newOptions;
		if (mRendererMenu->getSelectedItem() != OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->getName()) reset = true;

		for (unsigned int i = 3; i <  OgreFramework::getSingletonPtr()->m_pTrayMgr->getNumWidgets(mRendererMenu->getTrayLocation()); i++)
		{
			OgreBites::SelectMenu* menu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget(mRendererMenu->getTrayLocation(), i);
			if (menu->getSelectedItem() != options[menu->getCaption()].currentValue) reset = true;
			newOptions[menu->getCaption()] = menu->getSelectedItem();
		}
		if (reset) reconfigure(mRendererMenu->getSelectedItem(), newOptions);
		//Izpiše se message box, ki nas opozori, ce zelimo nastavitve sprejeti ali ne
		OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Do you want to apply Settings?", "Game will shutdown and you will have to restart it after the shutdown!\n\n\n !!WARNING - DirectX 9 does not WORK!!");
        m_bQuestionActive = true;
	}
	//Preidemo v mainMenuState
	else if(button->getName() == "BackBtn")
	{
		m_bQuit = true;
		changeAppState(findByName("mainMenuState"));
	}
}

void DarkOptionsMenu::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
        shutdown();
    else
        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
    m_bQuestionActive = false;
}