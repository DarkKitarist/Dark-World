#ifndef DARK_OPTIONS_H
#define DARK_OPTIONS_H

#include "appState.h"
 
class DarkOptionsMenu : public AppState
{
public:
    DarkOptionsMenu();
	DECLARE_APPSTATE_CLASS(DarkOptionsMenu)
 
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
	void reconfigure(const Ogre::String& renderer, Ogre::NameValuePairList& options);
	void update(double timeSinceLastFrame);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
	void buildGUI();
 
	OgreBites::SelectMenu* mRendererMenu;
	OgreBites::SelectMenu* menu; 
	Ogre::String mNextRenderer;

private:
	bool m_bQuit;
	bool restarting;
	bool m_bQuestionActive;
};
#endif