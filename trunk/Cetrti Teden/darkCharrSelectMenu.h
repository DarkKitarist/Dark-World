#ifndef DARK_SELECT_MENU_H
#define DARK_SELECT_MENU_H

#include "appState.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

class darkCharrSelectMenu : public AppState
{
public:
	darkCharrSelectMenu();
 
	DECLARE_APPSTATE_CLASS(darkCharrSelectMenu)
 //funkcije
	void enter();
	void enter(Config cfg);
	void createScene();
	void exit();
	bool pause();
	void resume();
	void moveCamera();
	void getInput();
    void buildGUI();
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void onLeftPressed(const OIS::MouseEvent &evt);
    void itemSelected(OgreBites::SelectMenu* menu);
	void buttonHit(OgreBites::Button* button);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
	void populateProfile();

	void update(double timeSinceLastFrame);
 
private:
	//inits
	irrklang::ISoundEngine* engine2;
	Ogre::StringVector chatModes;
	Ogre::StringVector basicAttack;
	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;
	Ogre::MaterialPtr		m_pOgreHeadMat;
	Ogre::MaterialPtr		m_pOgreHeadMatHigh;
	Ogre::SceneNode*		m_pRobotNode;
	Ogre::Entity*			m_pRobotEntity;
	Ogre::MaterialPtr		m_pRobotMat;
	Ogre::MaterialPtr		m_pRobotHigh;
	Ogre::SceneNode*		m_pNinjaNode;
	Ogre::Entity*			m_pNinjaEntity;
	Ogre::MaterialPtr		m_pNinjaMat;
	Ogre::MaterialPtr		m_pNinjaMatHigh;
	Ogre::AnimationState*	m_animStateOgre;
	Ogre::AnimationState*	m_animStateOgre1;
	Ogre::AnimationState*	m_animStateOgre2;
	Ogre::AnimationState*	m_animStateNinja;
	Ogre::AnimationState*	m_animStateRobot;
	OgreBites::Button*		m_InputButton;
	OgreBites::Button*		m_StartGameButton;
	OgreBites::TextBox*		m_textBox;
	Ogre::Entity*			mSword1;
	Ogre::Entity*			mSword2;
	OgreBites::SelectMenu*	m_selectRace;
	OgreBites::SelectMenu*	m_selectAtack;
    OgreBites::ParamsPanel*		m_pDetailsPanel;
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real				m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	Ogre::Degree			m_RotScale;
	Ogre::SceneNode*		m_pCurrentObject;
	Ogre::Entity*			m_pCurrentEntity;
	Ogre::SceneNode*		m_pCurrentObject1;
	Ogre::Entity*			m_pCurrentEntity1;
	Ogre::SceneNode*		m_pCurrentObject2;
	Ogre::Entity*			m_pCurrentEntity2;

	float					m_MoveScale;
	bool					m_bQuit;
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;
	bool				m_nameSelected;
	bool				m_CharSelected;
	bool				m_bQuestionActive;
	bool				m_useOldProfile;
	bool				m_OgreSelected;
	bool				m_NinjaSelected;
	bool				m_RobotSelected;
	int					charSelectionFlag;
	int					profileNumber;
	Ogre::String		profileRead[40];
	Ogre::String		modelType;
	Ogre::String		profileName;
	Ogre::String		CharName;
	Ogre::String		CharType;
	Ogre::String		CharAttack;
	Ogre::String		infoText;

};

#endif
