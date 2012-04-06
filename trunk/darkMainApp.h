//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef DARK_MAIN_APP_H
#define DARK_MAIN_APP_H

#include "darkFramework.h"
#include "AppStateManager.h"

class darkMainApp
{
public:
	darkMainApp();
	~darkMainApp();
 
	void startGame();
 
private:
	AppStateManager*	m_pAppStateManager;
};

#endif
