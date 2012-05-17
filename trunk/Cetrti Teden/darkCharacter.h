#pragma once
#include "Ogre.h"
#include "OIS.h"

using namespace Ogre;

struct WorldData;

class darkCharacter
{
protected:
    Ogre::SceneManager *mSceneMgr;

public:
	Ogre::SceneNode *worldNode;
	Ogre::Entity *mEntity; // Character entity
	Ogre::SceneNode *mMainNode; // Main character node
	Ogre::SceneNode *mSightNode; // "Sight" node - The character is supposed to be looking here
    Ogre::SceneNode *mCameraNode; // Node for the chase camera  
	virtual void update(Real elapsedTime, OIS::Keyboard*, OIS::Mouse*) = 0;
	virtual void specUpdate(Real elapsedTime, OIS::Keyboard*, OIS::Mouse*, WorldData& wd) = 0;
	Ogre::SceneNode *getSightNode(void);
	Ogre::SceneNode *getCameraNode(void);
	Ogre::Vector3 getWorldPosition(void);
	darkCharacter(void);
	~darkCharacter(void);
};

