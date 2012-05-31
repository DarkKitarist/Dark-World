#ifndef DARK_SINGLE_PLAYER_H
#define DARK_SINGLE_PLAYER_H

#include "appState.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "darkFrameListener.h"
#include "CollisionTools.h"
#include "Config.h"

enum QueryFlagsSinglePlayerLevel1
{
    ENTITY_MASK  = 1<<8,
	WATER_MASK = 2<<8
};

enum CurrentGeomOpt{
	INSTANCE_OPT,
	STATIC_OPT,
	ENTITY_OPT
};

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
 
	void createGrassMesh();

	void update(double timeSinceLastFrame);
 
private:
	OgreBites::ProgressBar* HealthBar;
	Ogre::Real TrenutnoZdravje;

	OgreBites::ProgressBar* AttackPoints;
	Ogre::Real TrenutniAtackPoints;
	Ogre::SceneNode *MainNode;
	irrklang::ISoundEngine* engine;
	irrklang::ISoundEngine* engine2;
	Ogre::CompositorManager *compMgr;


	Ogre::Plane			mWaterPlane;
	Ogre::Entity*		mWater;
	Ogre::SceneNode*	mWaterNode;
	Ogre::RenderTarget* mRefractionTarget;
	Ogre::RenderTarget* mReflectionTarget;

	Ogre::SceneNode* testNode;
	Ogre::SceneNode* smallTreeNodes;

	Ogre::AnimationState*	m_OgreIdleTop;
	Ogre::AnimationState*	m_OgreIdleBottom;
	Ogre::AnimationState*	m_OgreRunningTop;
	Ogre::AnimationState*	m_OgreRunningBase;
	darkCharacterSpec *ogre;

	OgreBites::ParamsPanel*		m_pDetailsPanel2;
    OgreBites::ParamsPanel*		m_pDetailsPanel;
	bool				m_bQuit;
	bool	mCollision;
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree		m_RotScale;
 
	Ogre::RaySceneQuery*	m_pRSQ;
	Ogre::SceneNode*		m_pCurrentObject;
	Ogre::Entity*			m_pCurrentEntity;

	Ogre::RaySceneQuery*	m_pRSQ1;
	Ogre::SceneNode*		m_pCurrentObject1;
	Ogre::Entity*			m_pCurrentEntity1;

	Ogre::RaySceneQuery*	m_pRSQ2;
	Ogre::SceneNode*		m_pCurrentObject2;
	Ogre::Entity*			m_pCurrentEntity2;
	darkFrameListener*		_listener;
	darkCharacter*			mChar;
	darkThirdPersonCamera*  exCamera2;
	unsigned int			mMode;
	void tempSave();
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;

	int					profileNumber;
	Ogre::String		profileRead[40];
	Ogre::String		modelType;
	Ogre::String		profileName;
	Ogre::String		CharName;
	Ogre::String		CharType;
	Ogre::String		infoText;

	Ogre::SceneNode *wallNodes;
	Ogre::SceneNode *wallNode1;
	Ogre::SceneNode *wallNode2;
	Ogre::SceneNode *wallNode3;
	Ogre::SceneNode *wallNode4;
	Ogre::SceneNode *wallNode5;
	Ogre::SceneNode *chestNode1;
	Ogre::SceneNode *chestNode2;
	Ogre::SceneNode *chestNode3;

	Ogre::SceneNode *house;


	bool objectiveCompleted;
	bool objectiveCompleted1;
	bool objectiveCompleted2;
	bool objectiveCompleted3;
	OgreBites::TextBox *aditionaInfo;
	
	MOC::CollisionTools* mCollisionTools;
	StaticGeometry* mField;

	OgreBites::Label *pickup;

	const Real GRASS_WIDTH;
	const Real GRASS_HEIGHT;
};

#endif