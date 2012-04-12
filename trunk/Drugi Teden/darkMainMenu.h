#ifndef DARK_MENU_H
#define DARK_MENU_H

#include "appState.h"
 
class darkMenu : public AppState
{
public:
    darkMenu();
	DECLARE_APPSTATE_CLASS(darkMenu)
 
	void enter();
	void enter(Config cfg);
	void createScene();
	void exit();
	void createGUI();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef); 
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
	void buttonHit(OgreBites::Button* button);
	void update(double timeSinceLastFrame);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

private:
	Ogre::OverlayContainer* mDialogShade;
	irrklang::ISoundEngine* engine;
	OgreBites::ParamsPanel*		m_pDetailsPanelMenu;
	OgreBites::ParamsPanel*		m_pDetailsPanel1;

	Ogre::SceneNode *TestAnimationNode;
	Ogre::SceneNode *MainNode;
	Ogre::SceneNode *treeNodes;
	Ogre::SceneNode *TreeNode1;
	Ogre::SceneNode *TreeNode2;
	Ogre::SceneNode *TreeNode3;
	Ogre::SceneNode *ParticleNode1;

	bool m_bQuit;
	bool m_bQuestionActive;
};
#endif