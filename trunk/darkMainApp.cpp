
#include "StdAfx.h"
#include "darkMainApp.h"
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
	if(!OgreFramework::getSingletonPtr()->initOgre("darkOgreGame", 0, 0))
		return;
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Igra Zagnana!");
 
	m_pAppStateManager = new AppStateManager();
 
	IntroState::create(m_pAppStateManager, "IntroState");
	initState::create(m_pAppStateManager, "initState");
	
	m_pAppStateManager->start(m_pAppStateManager->findByName("initState"));
}