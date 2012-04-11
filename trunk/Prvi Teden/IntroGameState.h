#ifndef INTRO_GAME_STATE_H
#define INTRO_GAME_STATE_H

#include "appState.h"
#include "UtilsOgreDshow.h"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class IntroState : public AppState
{
public:
    IntroState();
 
	DECLARE_APPSTATE_CLASS(IntroState)
 
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
	OgreBites::Label *mTitleLabel2;
	OgreBites::Label *mTitleLabel3;
	bool m_bQuit;
	bool flagOnce;
	bool time_ellapsed;
	OgreUtils::DirectShowMovieTexture* dshowMovieTextureSystem;
};

#endif