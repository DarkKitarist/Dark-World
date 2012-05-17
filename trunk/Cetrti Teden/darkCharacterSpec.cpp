#include "StdAfx.h"
#include "darkCharacterSpec.h"
#include "darkCharacter.h"
#include "OIS.h"
#include "Ogre.h"
#include "CommonNetIdentifiers.h"

using namespace std;

darkCharacterSpec::darkCharacterSpec(String name, SceneManager *sceneMgr, String cName, String cType, String mType, Ogre::SceneNode *MainGameNode, Ogre::Camera* testCameraDark)
{
	engine = irrklang::createIrrKlangDevice();
	// Setup basic member references
	_testCamera = testCameraDark;
	mName = name;
	mSceneMgr = sceneMgr;
	charType = cType;
	charName = cName;
	modelType = mType;
	maxRotate = 0.0f;
	mLeftPressed = false;
	charIsMoving = false;
	isRunning = false;

	walked = false;
	mTimer = 0;
	_timer.reset();
	drawSwords = false;

	if(charType == "Robot")
	{
		worldNode = MainGameNode->createChildSceneNode("BaseNode_"+name, Vector3 (4400.0f, 0.0f, 4300.0f));
		// Setup basic node structure to handle 3rd person cameras
		mMainNode = worldNode->createChildSceneNode (mName+ "_mesh",Vector3 (0.0f, -0.55f, 0.0f));
		mSightNode = worldNode->createChildSceneNode (mName + "_sight", Vector3 (0.0f, 20.0f, 10.0f));
		mCameraNode = worldNode->createChildSceneNode (mName + "_camera", Vector3 (0.0f, 45.0f, -40.0f));
	}
	else if(charType == "Ogre")
	{
		worldNode = MainGameNode->createChildSceneNode("BaseNode_"+name, Vector3 (4400.0f, 0.0f, 4300.0f));
		// Setup basic node structure to handle 3rd person cameras
		mMainNode = worldNode->createChildSceneNode (mName+ "_mesh",Vector3 (0.0f, 15.0f, 0.0f));
		mSightNode = worldNode->createChildSceneNode (mName + "_sight", Vector3 (0.0f, 20.0f, 10.0f));
		mCameraNode = worldNode->createChildSceneNode (mName + "_camera", Vector3 (0.0f, 45.0f, -40.0f));
	}
	if(charType == "Ninja")
	{
		worldNode = MainGameNode->createChildSceneNode("BaseNode_"+name, Vector3 (4400.0f, 0.0f, 4300.0f));
		// Setup basic node structure to handle 3rd person cameras
		mMainNode = worldNode->createChildSceneNode (mName+ "_mesh",Vector3 (0.0f, -0.55f, 0.0f));
		mSightNode = worldNode->createChildSceneNode (mName + "_sight", Vector3 (0.0f, 20.0f, 10.0f));
		mCameraNode = worldNode->createChildSceneNode (mName + "_camera", Vector3 (0.0f, 45.0f, -40.0f));
	}
	// Give this character a shape :)


	mEntity = mSceneMgr->createEntity (mName, modelType);
	mMainNode->attachObject (mEntity);
	if(charType == "Robot")
	{
		mMainNode->scale(0.4f,0.4f,0.4f);
		mMainNode->yaw(Degree(-90),Ogre::Node::TS_WORLD);
	}
	else if(charType == "Ogre")
	{
		mMainNode->setScale(3.0f,3.0f,3.0f);
		mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
		mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
		mEntity->attachObjectToBone("Sheath.L", mSword1);
		mEntity->attachObjectToBone("Sheath.R", mSword2);

		NameValuePairList params;
		params["numberOfChains"] = "2";
		params["maxElements"] = "80";
		mSwordTrail = (RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
		mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
		mSwordTrail->setTrailLength(35);
		mSwordTrail->setVisible(false);
		mSwordTrail->addNode(mSword1->getParentNode());
		mSwordTrail->addNode(mSword2->getParentNode());
		sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);
		for (int i = 0; i < 2; i++)
		{
			mSwordTrail->setInitialColour(i, 1, 0.8f, 0);
			mSwordTrail->setColourChange(i, 0.75f, 1.25f, 1.25f, 1.25f);
			mSwordTrail->setWidthChange(i, 2.0f);
			mSwordTrail->setInitialWidth(i, 0.5);
		}
	}
	else if(charType == "Ninja")
	{
		mMainNode->scale(0.12f,0.12f,0.12f);
		mMainNode->yaw(Degree(-180),Ogre::Node::TS_WORLD);
	}
	
	if(modelType == "Sinbad.mesh")
	{
		_aniStateOgreIdleTop = mEntity->getAnimationState("IdleTop");
		_aniStateOgreIdleTop->setEnabled(true);
		_aniStateOgreIdleTop->setLoop(true);
		_aniStateOgreIdleTop->setWeight(50);

		_aniStateOgreRelaxedhands = mEntity->getAnimationState("HandsRelaxed");
		_aniStateOgreRelaxedhands->setEnabled(true);

		_aniStateOgreIdleBase = mEntity->getAnimationState("IdleBase");
		_aniStateOgreIdleBase->setEnabled(true);
		_aniStateOgreIdleBase->setLoop(true);
		//_aniStateOgreIdleBase->setWeight(50);

		_aniStateOgre  = mEntity->getAnimationState("RunBase");
		_aniStateOgre->setLoop(false);
		_aniStateOgre->setWeight(0);

		_aniStateTopOgre = mEntity->getAnimationState("RunTop");
		_aniStateTopOgre->setLoop(false);
		_aniStateTopOgre->setWeight(0);

		_aniStateOgreLeftFight = mEntity->getAnimationState("SliceHorizontal");
		_aniStateOgreLeftFight->setLoop(false);
		_aniStateOgreLeftFight->setWeight(0);


	}
	if(charType == "Robot")
	{
		_aniStateRobotIdle = mEntity->getAnimationState("Idle");
		_aniStateRobotIdle->setEnabled(true);
		_aniStateRobotIdle->setLoop(true);

		_aniStateRobotWalk = mEntity->getAnimationState("Walk");
		_aniStateRobotWalk->setLoop(false);

		_aniStateRobotShoot = mEntity->getAnimationState("Shoot");
		_aniStateRobotShoot->setLoop(false);

	}
	if(charType == "Ninja")
	{
		_aniStateNinjaIdle= mEntity->getAnimationState("Idle3");
		_aniStateNinjaIdle->setEnabled(true);
		_aniStateNinjaIdle->setLoop(true);

		_aniStateNinjaWalk = mEntity->getAnimationState("Walk");
		_aniStateNinjaWalk->setLoop(false);

		_aniStateNinjaAtack = mEntity->getAnimationState("Attack2");
		_aniStateNinjaAtack->setLoop(false);
	}

	rayQuery = mSceneMgr->createRayQuery(Ray(worldNode->getPosition(), Vector3::NEGATIVE_UNIT_Y));
	clampToTerrain();
}

void darkCharacterSpec::setLeft (bool mLeft)
{
	mLeftPressed = mLeft;
}


void darkCharacterSpec::update (Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput) 
{
	Ogre::Real topAnimSpeed = 1;
	walked = false;
	drawSwords = false;
	sheeldSwords = false;
	charIsMoving = false;

    // Handle movement
    if (input->isKeyDown (OIS::KC_W)) 
	{
		walked = true;
		if(input->isKeyDown (OIS::KC_LSHIFT))
		{
			worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, 510 * elapsedTime));
			isRunning = true;
		}
		else
		{
			worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, 110 * elapsedTime));
			isRunning = false;
		}
    }
    if (input->isKeyDown (OIS::KC_S)) 
	{
		walked = true;
        worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, -50 * elapsedTime));
    }
    if (input->isKeyDown (OIS::KC_A)) 
	{
		worldNode->yaw (Radian (2 * elapsedTime),Ogre::Node::TS_PARENT);
    }
    if (input->isKeyDown (OIS::KC_D)) 
	{
        worldNode->yaw (Radian (-2 * elapsedTime),Ogre::Node::TS_PARENT);
    }
    if (input->isKeyDown (OIS::KC_Q) && _timer.getMilliseconds()> 500) 
	{
		if(swordsDrawn)
		{
			sheeldSwords = true;
			drawSwords = false;
		}
		else
		{
			sheeldSwords = false;
			drawSwords = true;
		}
		_timer.reset();
    }

	if(drawSwords && charType == "Ogre")
	{
		mEntity->detachAllObjectsFromBone();
		mEntity->attachObjectToBone("Handle.L", mSword1);
		mEntity->attachObjectToBone("Handle.R", mSword2);
		_aniStateOgreRelaxedhands = mEntity->getAnimationState("HandsClosed");
		_aniStateOgreRelaxedhands->setEnabled(true);
		mSwordTrail->setVisible(true);
		swordsDrawn = true;
		swordsSheelded = false;
	}

	if(sheeldSwords && charType == "Ogre")
	{
		mEntity->detachAllObjectsFromBone();
		mEntity->attachObjectToBone("Sheath.L", mSword1);
		mEntity->attachObjectToBone("Sheath.R", mSword2);
		_aniStateOgreRelaxedhands = mEntity->getAnimationState("HandsRelaxed");
		_aniStateOgreRelaxedhands->setEnabled(true);
		mSwordTrail->setVisible(false);
		swordsSheelded = true;
		swordsDrawn = false;
	}

	if(mLeftPressed)
	{
		if(charType == "Ogre")
		{
			_aniStateOgreIdleTop->setEnabled(false);
			_aniStateOgreIdleTop->setLoop(false);
			_aniStateOgreIdleTop->setWeight(0);
			_aniStateOgreIdleBase->setEnabled(false);
			_aniStateOgreIdleBase->setLoop(false);
			_aniStateOgreLeftFight->setEnabled(true);
			_aniStateOgreLeftFight->setLoop(true);
			_aniStateOgreLeftFight->setWeight(90);
			
		}
		if(charType == "Robot")
		{
			_aniStateRobotIdle->setEnabled(false);
			_aniStateRobotIdle->setLoop(false);
			_aniStateRobotIdle->setWeight(0);
			_aniStateRobotShoot->setEnabled(true);
			_aniStateRobotShoot->setLoop(true);
			_aniStateRobotShoot->setWeight(90);
			_aniStateRobotWalk->setWeight(0);
		}

		if(charType == "Ninja")
		{
			_aniStateNinjaIdle->setEnabled(false);
			_aniStateNinjaIdle->setLoop(false);
			_aniStateNinjaAtack->setEnabled(true);
			_aniStateNinjaAtack->setLoop(true);
		}
	}
	else 
	{
		if(charType == "Ogre")
		{
			_aniStateOgreLeftFight->setTimePosition(0.0f);
			_aniStateOgreLeftFight->setEnabled(false);
			_aniStateOgreLeftFight->setWeight(0);
			_aniStateOgreIdleTop->setEnabled(true);
			_aniStateOgreIdleTop->setLoop(true);
			_aniStateOgreIdleTop->setWeight(50);
			_aniStateOgreIdleBase->setEnabled(true);
			_aniStateOgreIdleBase->setLoop(true);
		}
		if(charType == "Robot")
		{
			_aniStateRobotShoot->setTimePosition(0.0f);
			_aniStateRobotIdle->setEnabled(true);
			_aniStateRobotIdle->setLoop(true);
			_aniStateRobotIdle->setWeight(90);
			_aniStateRobotShoot->setEnabled(false);
			_aniStateRobotShoot->setLoop(true);
			_aniStateRobotShoot->setWeight(0);
			_aniStateRobotWalk->setWeight(0);
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaAtack->setTimePosition(0.0f);
			_aniStateNinjaIdle->setEnabled(true);
			_aniStateNinjaIdle->setLoop(true);
			_aniStateNinjaAtack->setEnabled(false);
			_aniStateNinjaAtack->setLoop(true);
		}
	}

	if(walked)
	{
		if(charType == "Ogre")
		{		
			//engine->play2D("media/darkMedia/Sounds/walking.mp3", false);
			_aniStateOgre->setEnabled(true);
			_aniStateOgre->setWeight(100);
			_aniStateTopOgre->setEnabled(true);
			_aniStateTopOgre->setWeight(90);
			_aniStateOgreIdleTop->setEnabled(false);
			_aniStateOgreIdleTop->setLoop(false);
			_aniStateOgreIdleTop->setWeight(0);
			_aniStateOgreIdleBase->setEnabled(false);
			_aniStateOgreIdleBase->setLoop(false);
			if(_aniStateOgre->hasEnded())
			{
				//engine->play2D("media/darkMedia/Sounds/walking.mp3", false);
				_aniStateOgre->setTimePosition(0.0f);
			}
			if(_aniStateTopOgre->hasEnded() && !mLeftPressed)
			{
				_aniStateTopOgre->setTimePosition(0.0f);
			}
		}
		if(charType == "Robot")
		{
			_aniStateRobotWalk->setEnabled(true);
			_aniStateRobotWalk->setWeight(60);
			_aniStateRobotIdle->setEnabled(false);
			_aniStateRobotIdle->setLoop(false);
			_aniStateRobotIdle->setWeight(0);
			if(_aniStateRobotWalk->hasEnded())
			{
				//engine->play2D("media/darkMedia/Sounds/walking.mp3", false);
				_aniStateRobotWalk->setTimePosition(0.0f);
			}
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaWalk->setEnabled(true);
			_aniStateNinjaIdle->setEnabled(false);
			_aniStateNinjaIdle->setLoop(false);
			if(_aniStateNinjaWalk->hasEnded())
			{
				//engine->play2D("media/darkMedia/Sounds/walking.mp3", false);
				_aniStateNinjaWalk->setTimePosition(0.0f);
			}
		}
	}
	else
	{
		if(charType == "Ogre")
		{
			_aniStateOgre->setTimePosition(0.0f);
			_aniStateOgre->setEnabled(false);
			_aniStateOgre->setWeight(0);
			
			_aniStateTopOgre->setTimePosition(0.0f);
			_aniStateTopOgre->setEnabled(false);
			_aniStateTopOgre->setWeight(0);

			_aniStateOgreIdleTop->setEnabled(true);
			_aniStateOgreIdleTop->setLoop(true);
			_aniStateOgreIdleTop->setWeight(50);

			_aniStateOgreIdleBase->setEnabled(true);
			_aniStateOgreIdleBase->setLoop(true);
		}

		if(charType == "Robot")
		{
			_aniStateRobotWalk->setTimePosition(0.0f);
			_aniStateRobotWalk->setEnabled(false);
			_aniStateRobotWalk->setWeight(0);
			_aniStateRobotIdle->setEnabled(true);
			_aniStateRobotIdle->setLoop(true);
			_aniStateRobotIdle->setWeight(90);
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaWalk->setTimePosition(0.0f);
			_aniStateNinjaWalk->setEnabled(false);
			_aniStateNinjaIdle->setEnabled(true);
			_aniStateNinjaIdle->setLoop(true);
		}
	}

	if(charType == "Ogre")
	{
		if(isRunning)
		{
			_aniStateOgre->addTime(elapsedTime*1.9f);
			_aniStateTopOgre->addTime(elapsedTime*1.9f);
		}
		else
		{
			_aniStateOgre->addTime(elapsedTime*1.4f);
			_aniStateTopOgre->addTime(elapsedTime*1.4f);
		}
		_aniStateOgreIdleTop->addTime(elapsedTime*2.0f);
		_aniStateOgreIdleBase->addTime(elapsedTime*2.0f);
		_aniStateOgreLeftFight->addTime(elapsedTime*0.45f);
	}

	else if(charType == "Robot")
	{
		_aniStateRobotIdle->addTime(elapsedTime*1.4f);
		if(isRunning)
			_aniStateRobotWalk->addTime(elapsedTime*1.9f);
		else
			_aniStateRobotWalk->addTime(elapsedTime*1.4f);
		_aniStateRobotShoot->addTime(elapsedTime*0.5f);
	}

	else if(charType == "Ninja")
	{
		_aniStateNinjaIdle->addTime(elapsedTime*1.1f);
		if(isRunning)
			_aniStateNinjaWalk->addTime(elapsedTime*1.9f);
		else
			_aniStateNinjaWalk->addTime(elapsedTime*1.4f);
		_aniStateNinjaAtack->addTime(elapsedTime*1.4f);
	}
	float rotX = (mInput->getMouseState().X.rel*elapsedTime/8)*-1;
	float rotY = ((mInput->getMouseState().Y.rel*elapsedTime/8)*1)/5;
	worldNode->yaw(Radian(rotX),Ogre::Node::TS_PARENT);
	std::cout<<rotX<<std::endl;


	if(rotX>0.2f || rotX<-0.2f)
	{
		charIsMoving = true;
	}

	if((_testCamera->getDerivedPosition().y >= (worldNode->getPosition().y + 10.0f)) && (_testCamera->getDerivedPosition().y <= (worldNode->getPosition().y + 50.0f)))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}
	else if((_testCamera->getDerivedPosition().y < (worldNode->getPosition().y + 10.0f)) && (rotY > 0))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}
	else if((_testCamera->getDerivedPosition().y > (worldNode->getPosition().y + 50.0f)) && (rotY < 0))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}
	clampToTerrain();
}

void darkCharacterSpec::specUpdate (Real elapsedTime, OIS::Keyboard *input, OIS::Mouse *mInput, WorldData& wd) 
{
	Ogre::Real topAnimSpeed = 1;
	walked = false;
	drawSwords = false;
	sheeldSwords = false;
	

	wd.hasMoved = true;
	wd.isWeaponOut = false;
	wd.forward = false;
	wd.backward = false;

    // Handle movement
    if (input->isKeyDown (OIS::KC_W)) 
	{
		walked = true;
		//worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, 100 * elapsedTime));

		if(input->isKeyDown (OIS::KC_LSHIFT))
		{
			worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, 510 * elapsedTime));
			isRunning = true;
		}
		else
		{
			worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, 110 * elapsedTime));
			isRunning = false;
		}
		wd.isWalking = true;		// Network - walking forward
		//wd.hasMoved = true;		// Network - walking forward
		wd.forward = true;
    }
	else
	{
		wd.isWalking = false;
		wd.forward = false;
	}

    if (input->isKeyDown (OIS::KC_S)) 
	{
		walked = true;
        worldNode->translate (worldNode->getOrientation () * Vector3 (0, 0, -50 * elapsedTime));
		wd.isWalking = true;		// Network - walking backward
		//wd.hasMoved = true;		// Network - walking backward
		wd.backward = true;
    }
	else
	{
		wd.backward = false;
	}
    if (input->isKeyDown (OIS::KC_A)) 
	{
		worldNode->yaw (Radian (2 * elapsedTime),Ogre::Node::TS_PARENT);
		//wd.hasMoved = true;		// Network - walking backward
    }
    if (input->isKeyDown (OIS::KC_D)) 
	{
        worldNode->yaw (Radian (-2 * elapsedTime),Ogre::Node::TS_PARENT);
		//wd.hasMoved = true;		// Network - walking backward
    }
    if (input->isKeyDown (OIS::KC_Q) && _timer.getMilliseconds()> 500) 
	{
		if(swordsDrawn)
		{
			sheeldSwords = true;
			drawSwords = false;
			//wd.isWeaponOut = false;		// Network - hasn't weapon out
		}
		else
		{
			sheeldSwords = false;
			drawSwords = true;
			//wd.isWeaponOut = true;		// Network - has weapon out
		}
		_timer.reset();
		wd.isWeaponOut = true;
    }

	if(drawSwords && charType == "Ogre")
	{
		mEntity->detachAllObjectsFromBone();
		mEntity->attachObjectToBone("Handle.L", mSword1);
		mEntity->attachObjectToBone("Handle.R", mSword2);
		_aniStateOgreRelaxedhands = mEntity->getAnimationState("HandsClosed");
		_aniStateOgreRelaxedhands->setEnabled(true);
		mSwordTrail->setVisible(true);
		swordsDrawn = true;
		swordsSheelded = false;
	}

	if(sheeldSwords && charType == "Ogre")
	{
		mEntity->detachAllObjectsFromBone();
		mEntity->attachObjectToBone("Sheath.L", mSword1);
		mEntity->attachObjectToBone("Sheath.R", mSword2);
		_aniStateOgreRelaxedhands = mEntity->getAnimationState("HandsRelaxed");
		_aniStateOgreRelaxedhands->setEnabled(true);
		mSwordTrail->setVisible(false);
		swordsSheelded = true;
		swordsDrawn = false;
	}

	if(mLeftPressed)
	{
		if(charType == "Ogre")
		{
			_aniStateOgreIdleTop->setEnabled(false);
			_aniStateOgreIdleTop->setLoop(false);
			_aniStateOgreIdleTop->setWeight(0);
			_aniStateOgreIdleBase->setEnabled(false);
			_aniStateOgreIdleBase->setLoop(false);
			//_aniStateOgreIdleBase->setWeight(0);
			_aniStateOgreLeftFight->setEnabled(true);
			_aniStateOgreLeftFight->setLoop(true);
			_aniStateOgreLeftFight->setWeight(90);
			
		}
		if(charType == "Robot")
		{
			//_aniStateRobotWalk->setEnabled(true);
			_aniStateRobotIdle->setEnabled(false);
			_aniStateRobotIdle->setLoop(false);
			_aniStateRobotIdle->setWeight(0);
			_aniStateRobotShoot->setEnabled(true);
			_aniStateRobotShoot->setLoop(true);
			_aniStateRobotShoot->setWeight(90);
			_aniStateRobotWalk->setWeight(0);
		}

		if(charType == "Ninja")
		{
			//_aniStateRobotWalk->setEnabled(true);
			_aniStateNinjaIdle->setEnabled(false);
			_aniStateNinjaIdle->setLoop(false);
			_aniStateNinjaAtack->setEnabled(true);
			_aniStateNinjaAtack->setLoop(true);
		}

		wd.isFighting = true;		// Network - fights
		wd.isIdle = false;			// Network - isn't idle
	}
	else 
	{
		if(charType == "Ogre")
		{
			_aniStateOgreLeftFight->setTimePosition(0.0f);
			_aniStateOgreLeftFight->setEnabled(false);
			_aniStateOgreLeftFight->setWeight(0);
			_aniStateOgreIdleTop->setEnabled(true);
			_aniStateOgreIdleTop->setLoop(true);
			_aniStateOgreIdleTop->setWeight(50);
			_aniStateOgreIdleBase->setEnabled(true);
			_aniStateOgreIdleBase->setLoop(true);
			//_aniStateOgreIdleBase->setWeight(50);
			//_aniStateTopOgre->setEnabled(true);
		}
		if(charType == "Robot")
		{
			_aniStateRobotShoot->setTimePosition(0.0f);
			//_aniStateRobotWalk->setEnabled(true);
			_aniStateRobotIdle->setEnabled(true);
			_aniStateRobotIdle->setLoop(true);
			_aniStateRobotIdle->setWeight(90);
			_aniStateRobotShoot->setEnabled(false);
			_aniStateRobotShoot->setLoop(true);
			_aniStateRobotShoot->setWeight(0);
			_aniStateRobotWalk->setWeight(0);
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaAtack->setTimePosition(0.0f);
			//_aniStateRobotWalk->setEnabled(true);
			_aniStateNinjaIdle->setEnabled(true);
			_aniStateNinjaIdle->setLoop(true);
			_aniStateNinjaAtack->setEnabled(false);
			_aniStateNinjaAtack->setLoop(true);
		}

		wd.isFighting = false;		// Network - doesn't fight
		wd.isIdle = false;			// Network - is idle
	}

	if(walked)
	{
		if(charType == "Ogre")
		{		
			_aniStateOgre->setEnabled(true);
			_aniStateOgre->setWeight(100);
			_aniStateTopOgre->setEnabled(true);
			_aniStateTopOgre->setWeight(90);
			_aniStateOgreIdleTop->setEnabled(false);
			_aniStateOgreIdleTop->setLoop(false);
			_aniStateOgreIdleTop->setWeight(0);
			_aniStateOgreIdleBase->setEnabled(false);
			_aniStateOgreIdleBase->setLoop(false);
			//_aniStateOgreIdleBase->setWeight(0);
			if(_aniStateOgre->hasEnded())
			{
				_aniStateOgre->setTimePosition(0.0f);
			}
			if(_aniStateTopOgre->hasEnded() && !mLeftPressed)
			{
				_aniStateTopOgre->setTimePosition(0.0f);
			}
		}
		if(charType == "Robot")
		{
			_aniStateRobotWalk->setEnabled(true);
			_aniStateRobotWalk->setWeight(60);
			_aniStateRobotIdle->setEnabled(false);
			_aniStateRobotIdle->setLoop(false);
			_aniStateRobotIdle->setWeight(0);
			if(_aniStateRobotWalk->hasEnded())
			{
				_aniStateRobotWalk->setTimePosition(0.0f);
			}
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaWalk->setEnabled(true);
			_aniStateNinjaIdle->setEnabled(false);
			_aniStateNinjaIdle->setLoop(false);
			if(_aniStateNinjaWalk->hasEnded())
			{
				_aniStateNinjaWalk->setTimePosition(0.0f);
			}
		}
	}
	else
	{
		if(charType == "Ogre")
		{
			_aniStateOgre->setTimePosition(0.0f);
			_aniStateOgre->setEnabled(false);
			_aniStateOgre->setWeight(0);
			
			_aniStateTopOgre->setTimePosition(0.0f);
			_aniStateTopOgre->setEnabled(false);
			_aniStateTopOgre->setWeight(0);

			_aniStateOgreIdleTop->setEnabled(true);
			_aniStateOgreIdleTop->setLoop(true);
			_aniStateOgreIdleTop->setWeight(50);

			_aniStateOgreIdleBase->setEnabled(true);
			_aniStateOgreIdleBase->setLoop(true);
			//_aniStateOgreIdleBase->setWeight(50);
		}

		if(charType == "Robot")
		{
			_aniStateRobotWalk->setTimePosition(0.0f);
			_aniStateRobotWalk->setEnabled(false);
			_aniStateRobotWalk->setWeight(0);
			_aniStateRobotIdle->setEnabled(true);
			_aniStateRobotIdle->setLoop(true);
			_aniStateRobotIdle->setWeight(90);
		}
		if(charType == "Ninja")
		{
			_aniStateNinjaWalk->setTimePosition(0.0f);
			_aniStateNinjaWalk->setEnabled(false);
			_aniStateNinjaIdle->setEnabled(true);
			_aniStateNinjaIdle->setLoop(true);
		}
	}

	if(charType == "Ogre")
	{
		_aniStateOgre->addTime(elapsedTime*1.4f);
		_aniStateTopOgre->addTime(elapsedTime*1.4f);
		_aniStateOgreIdleTop->addTime(elapsedTime*2.0f);
		_aniStateOgreIdleBase->addTime(elapsedTime*2.0f);
		_aniStateOgreLeftFight->addTime(elapsedTime*0.9f);
	}

	else if(charType == "Robot")
	{
		_aniStateRobotIdle->addTime(elapsedTime*1.4f);
		_aniStateRobotWalk->addTime(elapsedTime*1.4f);
		_aniStateRobotShoot->addTime(elapsedTime*0.9f);
	}

	else if(charType == "Ninja")
	{
		_aniStateNinjaIdle->addTime(elapsedTime*1.1f);
		_aniStateNinjaWalk->addTime(elapsedTime*1.4f);
		_aniStateNinjaAtack->addTime(elapsedTime*1.4f);
	}
	float rotX = (mInput->getMouseState().X.rel*elapsedTime/8)*-1;
	float rotY = ((mInput->getMouseState().Y.rel*elapsedTime/8)*1)/5;
	worldNode->yaw(Radian(rotX),Ogre::Node::TS_PARENT);

	if((_testCamera->getDerivedPosition().y >= (worldNode->getPosition().y + 10.0f)) && (_testCamera->getDerivedPosition().y <= (worldNode->getPosition().y + 50.0f)))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}
	else if((_testCamera->getDerivedPosition().y < (worldNode->getPosition().y + 10.0f)) && (rotY > 0))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}
	else if((_testCamera->getDerivedPosition().y > (worldNode->getPosition().y + 50.0f)) && (rotY < 0))
	{
		if(charType == "Robot")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->roll(Radian(rotY));
		}
		else if(charType == "Ogre")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(-rotY));
		}
		else if(charType == "Ninja")
		{
			worldNode->pitch(Radian(rotY));
			mMainNode->pitch(Radian(rotY));
		}
	}

	clampToTerrain();

	wd.or_w = worldNode->getOrientation().w;
	wd.or_x = worldNode->getOrientation().x;
	wd.or_y = worldNode->getOrientation().y;
	wd.or_z = worldNode->getOrientation().z;
	wd.pos_x = worldNode->getPosition().x;
	wd.pos_y = worldNode->getPosition().y;
	wd.pos_z = worldNode->getPosition().z;
	wd.mainOr_w = mMainNode->getOrientation().w;
	wd.mainOr_x = mMainNode->getOrientation().x;
	wd.mainOr_y = mMainNode->getOrientation().y;
	wd.mainOr_z = mMainNode->getOrientation().z;
}

Ogre::String darkCharacterSpec::getName()
{
	return charName;
}

Ogre::String darkCharacterSpec::getType()
{
	return charType;
}

bool darkCharacterSpec::getCharIsMoving()
{
	return charIsMoving;
}
 
// Change visibility - Useful for 1st person view ;)
void darkCharacterSpec::setVisible (bool visible) 
{
    mMainNode->setVisible (visible);
}

void darkCharacterSpec::clampToTerrain()
{
	rayQuery->setRay(Ray(worldNode->getPosition() + Vector3(0, 250, 0), Vector3::NEGATIVE_UNIT_Y));
	RaySceneQueryResult &qryResult = rayQuery->execute();
	RaySceneQueryResult::iterator i = qryResult.begin();
	if(i != qryResult.end() && i->worldFragment)
		worldNode->setPosition(worldNode->getPosition().x, i->worldFragment->singleIntersection.y + 1, worldNode->getPosition().z);
}


darkCharacterSpec::~darkCharacterSpec(void)
{
	mMainNode->detachAllObjects ();
	delete mEntity;
	mMainNode->removeAndDestroyAllChildren ();
	mSceneMgr->destroySceneNode (mName);
}
