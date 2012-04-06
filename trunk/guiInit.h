#ifndef INIT_STATE_H
#define INIT_STATE_H

#include "appState.h"
#include "UtilsOgreDshow.h"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class initState : public AppState
{
public:
    initState();
 
	DECLARE_APPSTATE_CLASS(initState)
 
	void enter();
	void enter(Config cfg);
	void createScene();
	void exit();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
	void buttonHit(OgreBites::Button* button);
 
	void update(double timeSinceLastFrame);
 
private:
	OgreBites::Label *mTitleLabel;
	OgreBites::ParamsPanel *initParamTable;
	bool m_bQuit;
	bool flagOnce;
	bool time_ellapsed;
};

#endif