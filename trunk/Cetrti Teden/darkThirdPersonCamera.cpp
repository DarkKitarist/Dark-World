#include "StdAfx.h"
#include "darkThirdPersonCamera.h"
#include <iostream>

using namespace std;

darkThirdPersonCamera::darkThirdPersonCamera(String name, SceneManager *sceneMgr, Camera *camera)
{
	// Basic member references setup
	mName = name;
	mSceneMgr = sceneMgr;
 
	// Create the camera's node structure
	mCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode (mName);
	mTargetNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode (mName + "_target");
	mCameraNode->setAutoTracking (true, mTargetNode); // The camera will always look at the camera target
	mCameraNode->setFixedYawAxis (true); // Needed because of auto tracking
 
	// Create our camera if it wasn't passed as a parameter
	if (camera == 0) {
		mCamera = mSceneMgr->createCamera (mName);
		mOwnCamera = true;
	}
	else {
		mCamera = camera;
			// just to make sure that mCamera is set to 'origin' (same position as the mCameraNode)
			mCamera->setPosition(0.0,0.0,0.0);
		mOwnCamera = false;
	}
	// ... and attach the Ogre camera to the camera node
	mCameraNode->attachObject (mCamera);
 
	// Default tightness
	mTightness = 0.5f;
}

void darkThirdPersonCamera::setTightness (Real tightness) 
{
    mTightness = tightness;
}
 
Real darkThirdPersonCamera::getTightness () 
{
    return mTightness;
}
 
Vector3 darkThirdPersonCamera::getCameraPosition () 
{
    return mCameraNode->getPosition ();
}
 
void darkThirdPersonCamera::instantUpdate (Vector3 cameraPosition, Vector3 targetPosition) 
{
    mCameraNode->setPosition (cameraPosition);
    mTargetNode->setPosition (targetPosition);
}
 
void darkThirdPersonCamera::update (Real elapsedTime, Vector3 cameraPosition, Vector3 targetPosition) 
{
    // Handle movement
    Vector3 displacement;
 
    displacement = (cameraPosition - mCameraNode->getPosition ()) * mTightness;
    mCameraNode->translate (displacement);
	//cout<<mCameraNode->getPosition()<<endl;
 
    displacement = (targetPosition - mTargetNode->getPosition ()) * mTightness;
    mTargetNode->translate (displacement);
	//cout<<mTargetNode->getPosition()<<endl;
}


darkThirdPersonCamera::~darkThirdPersonCamera(void)
{
	mCameraNode->detachAllObjects ();
	if (mOwnCamera)
		delete mCamera;
	mSceneMgr->destroySceneNode (mName);
	mSceneMgr->destroySceneNode (mName + "_target");
}
