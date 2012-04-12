
#include "StdAfx.h"
#include "darkMainApp.h"
#include "darkMainMenu.h"
#include "IntroGameState.h"
#include "guiInit.h"


darkMainApp::darkMainApp()
{
	m_pAppStateManager = 0;
}

darkMainApp::~darkMainApp()
{
	delete m_pAppStateManager;
        delete OgreFramework::getSingletonPtr();
}

void darkMainApp::startGame()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("darkWorld", 0, 0))
		return;
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Igra Zagnana!");
 
	m_pAppStateManager = new AppStateManager();
	//tukaj dodamo GameState
	IntroState::create(m_pAppStateManager, "IntroState");
	initState::create(m_pAppStateManager, "initState");
	darkMenu::create(m_pAppStateManager, "mainMenuState");
	
	m_pAppStateManager->start(m_pAppStateManager->findByName("initState"));
}