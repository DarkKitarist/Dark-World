#ifndef DARK_SINGLE_PLAYER_H
#define DARK_SINGLE_PLAYER_H

#include "appState.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "darkFrameListener.h"
#include "CollisionTools.h"
#include "Config.h"

class darkMainSinglePlayer : public AppState
{
public:
	darkMainSinglePlayer();
 
	DECLARE_APPSTATE_CLASS(darkMainSinglePlayer)
	void playerInfo();
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
 

	void update(double timeSinceLastFrame);
 
private:
	Ogre::SceneNode *MainNode;
	irrklang::ISoundEngine* engine;
	irrklang::ISoundEngine* engine2;

	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;

	darkCharacterSpec *ogre;

	bool				m_bQuit;
	bool	mCollision;
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree		m_RotScale;
 
	darkFrameListener*		_listener;
	darkCharacter*			mChar;
	darkThirdPersonCamera*  exCamera2;
	unsigned int			mMode;
	void tempSave();
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;
};

#endif