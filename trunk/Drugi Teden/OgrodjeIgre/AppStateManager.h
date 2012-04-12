#ifndef APP_STATE_MANAGER_H
#define APP_STATE_MANAGER_H

#include "appState.h"
#include "Config.h"
 
class AppStateManager : public AppStateListener
{
public:
	typedef struct
	{
		Ogre::String name;
		AppState* state;
	} state_info;
 
	AppStateManager();
	~AppStateManager();
 
	void manageAppState(Ogre::String stateName, AppState* state);
 
	AppState* findByName(Ogre::String stateName);
 
	void start(AppState* state);
	void changeAppState(AppState* state);
	void changeAppState(AppState* state, Config cfg);
	bool pushAppState(AppState* state);
	void popAppState();
	void pauseAppState();
	void shutdown();
    void popAllAndPushAppState(AppState* state);
 
protected:
	void init(AppState *state);
 
	std::vector<AppState*>		m_ActiveStateStack;
	std::vector<state_info>		m_States;
	bool				m_bShutdown;
};

#endif