#pragma once
#include "darkCharacter.h"
#include "Ogre.h"
#include "irrKlang.h"

struct WorldData;

class darkCharacterSpec : public darkCharacter
{
protected:
	String mName;
	
	Ogre::AnimationState* _aniStateOgre;
	Ogre::AnimationState* _aniStateOgreRelaxedhands;
	Ogre::AnimationState* _aniStateTopOgre;
	Ogre::AnimationState* _aniStateOgreIdleTop;
	Ogre::AnimationState* _aniStateOgreIdleBase;
	Ogre::AnimationState* _aniStateOgreLeftFight;
	Ogre::AnimationState* _aniStateOgreSwords;

	Ogre::AnimationState* _aniStateRobotIdle;
	Ogre::AnimationState* _aniStateRobotWalk;
	Ogre::AnimationState* _aniStateRobotShoot;

	Ogre::AnimationState* _aniStateNinjaIdle;
	Ogre::AnimationState* _aniStateNinjaWalk;
	Ogre::AnimationState* _aniStateNinjaAtack;
	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;
	Ogre::RibbonTrail* mSwordTrail;

	Ogre::Viewport* mViewport;
	


public:
	Ogre::RaySceneQuery *rayQuery;
	Ogre::Camera* _testCamera;
	irrklang::ISoundEngine* engine;
	darkCharacterSpec(String name, SceneManager *sceneMgr, String cName, String cType, String mType, Ogre::SceneNode *MainGameNode,Ogre::Camera *testCameraDark);
	void update (Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput);
	void specUpdate(Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput, WorldData& wd);
	void setVisible (bool visible);
	void clampToTerrain();
	void setLeft(bool mLeft);
	bool getCharIsMoving();
	Ogre::String getName();
	Ogre::String getType();
	bool walked;
	bool mLeftPressed;
	bool drawSwords;
	bool sheeldSwords;
	bool charIsMoving;
	bool swordsDrawn;
	bool isRunning;
	bool swordsSheelded;
	Ogre::Timer _timer;
	bool mSwordsDrawn;
	Ogre::Real mTimer; 
	float maxRotate;
	String charName;
	String charType;
	String modelType;
	~darkCharacterSpec(void);
};

