#include "StdAfx.h"
#include "darkOgreApp.h"
#include "darkFrameListener.h"
#include <string.h>

using namespace Ogre;

//void darkOgreApp::darkResources()
//{
//	/*ConfigFile cf;
//	cf.load("resources_d.cfg");
//	ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
//	Ogre::String sectionName, typeName, dataname;
//	while(sectionIter.hasMoreElements())
//	{
//		sectionName = sectionIter.peekNextKey();
//		ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
//		ConfigFile::SettingsMultiMap::iterator i;
//		for(i = settings->begin(); i != settings->end(); ++i)
//		{
//			typeName = i->first;
//			dataname = i->second;
//			ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
//		}		
//	}
//	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();*/
//}
//
//void darkOgreApp::createScene()
//{
//
//	////_sceneManager->setWorldGeometry("terrainTest.cfg");
//	//_sceneManager->setAmbientLight(ColourValue(0.2f, 0.2f, 0.2f));
//
// //   Ogre::Light* l = _sceneManager->createLight("MainLight");
//
// //   l->setType(Light::LT_DIRECTIONAL);
// //   l->setDirection(-1, -1, 0);
//
//
// //   SceneNode *razorNode;
// //   Entity *razorEntity;
// //   for (unsigned int i = 0; i < 30; ++i) {
// //       razorNode = _sceneManager->getRootSceneNode ()->createChildSceneNode (StringConverter::toString (i), Ogre::Vector3(Math::RangeRandom (-1000, 1000), 0, Math::RangeRandom (-1000, 1000)));
// //       razorEntity = _sceneManager->createEntity (StringConverter::toString (i), "razor.mesh");
// //       razorNode->attachObject (razorEntity);
// //   }
// //
// //   // Main character
//	////darkCharacterSpec *ogre = new darkCharacterSpec ("Main", _sceneManager, "Test", "Test");
//	////darkThirdPersonCamera *exCamera = new darkThirdPersonCamera ("Extended Camera", _sceneManager, _cam);
// //
// //   // Frame listener to manage both character and camera updating and different camera modes
// //   // Need to create it here as we want to change some parameters here, thus avoiding defining 
// //   // ogre and exCamera as member variables
//	////_listener = new darkFrameListener (_window, _cam, ogre);
// //  /// static_cast<darkFrameListener *>(_listener)->setCharacter (ogre);
// //   //static_cast<darkFrameListener *>(_listener)->setExtendedCamera (exCamera);
//
//	////String test = "Sinbad.mesh";
//	////Entity* sinbad = _sceneManager->createEntity(test);
//	////_sceneManager->getRootSceneNode()->attachObject(sinbad);
//	//_sceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 5000, false);
//	//_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
//}
//
//
//int darkOgreApp::startup()
//{
//	//_root = new Ogre::Root("plugins_d.cfg");
//	//if(!_root->showConfigDialog())
//	//{
//	//	return 0;
//	//}
//	//_window =  _root->initialise(true, "darkGame");
//	//_sceneManager = _root->createSceneManager("TerrainSceneManager");
//	//_cam = _sceneManager->createCamera("Camera");
//	//_cam->setPosition(Ogre::Vector3(0,0,0));
//	////cam->lookAt(Ogre::Vector3(0,0,0));
//	//_cam->setNearClipDistance(5);
//	////_cam->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
//	//Ogre::Viewport* viewPort = _window->addViewport(_cam);
//	//viewPort->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
//
//	//_cam->setAspectRatio(Ogre::Real(viewPort->getActualWidth()/Ogre::Real(viewPort->getActualHeight())));
//
//
//	//darkResources();
//	//createScene();
//	//_sceneManager->setWorldGeometry("terrainTest2.cfg");
//	////_listener = new darkFrameListener(window, cam);
//	//_root->addFrameListener(_listener);
//	//_root->startRendering();
//
//	return 0;
//}
//
//darkOgreApp::darkOgreApp(void)
//{
//	/*_sceneManager = NULL;
//	_root = NULL;*/
//}
//
//
//darkOgreApp::~darkOgreApp(void)
//{
//	//delete _root;
//}
