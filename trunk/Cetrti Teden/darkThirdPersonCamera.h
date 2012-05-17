#pragma once
#include "Ogre.h"
#include "OIS.h"
#include "darkCharacter.h"
#include "Ogre.h"
#include <string.h>

using namespace Ogre;

class darkThirdPersonCamera
{
protected:
	Ogre::SceneNode *mTargetNode; // The camera target
	Ogre::SceneNode *mCameraNode; // The camera itself
	Ogre::Camera *mCamera; // Ogre camera
	Ogre::SceneManager *mSceneMgr;
	String mName;
	bool mOwnCamera; // To know if the ogre camera binded has been created outside or inside of this class
	Ogre::Real mTightness; // Determines the movement of the camera - 1 means tight movement, while 0 means no movement
public:
	darkThirdPersonCamera(String, Ogre::SceneManager*, Ogre::Camera *camera = 0);
	void setTightness (Ogre::Real);
	Ogre::Real getTightness (void);
	Ogre::Vector3 getCameraPosition (void);
	void instantUpdate (Ogre::Vector3, Ogre::Vector3);
	void update (Real, Ogre::Vector3, Ogre::Vector3);
	~darkThirdPersonCamera(void);
};

