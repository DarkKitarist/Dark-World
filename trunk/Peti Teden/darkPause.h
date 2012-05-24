#ifndef DARK_PAUSE_H
#define DARK_PAUSE_H

#include "appState.h"

class darkPause : public AppState
{
public:
    darkPause();
 
    DECLARE_APPSTATE_CLASS(darkPause)
 
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
    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
 
    void update(double timeSinceLastFrame);
 
private:
    bool m_bQuit;
    bool m_bQuestionActive;
};

#endif