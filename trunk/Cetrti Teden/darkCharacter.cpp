#include "StdAfx.h"
#include "darkCharacter.h"
#include "Ogre.h"
#include "CommonNetIdentifiers.h"

// Updates the character (movement...)
void update (Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput);
void specUpdate (Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput, WorldData& wd);
// The three methods below returns the two camera-related nodes, 
// and the current position of the character (for the 1st person camera)
Ogre::SceneNode *darkCharacter::getSightNode() {
    return mSightNode;
}

Ogre::SceneNode *darkCharacter::getCameraNode () {
    return mCameraNode;
}

Ogre::Vector3 darkCharacter::getWorldPosition () {
    return mMainNode->_getDerivedPosition ();
}

darkCharacter::darkCharacter(void)
{
}


darkCharacter::~darkCharacter(void)
{
}
