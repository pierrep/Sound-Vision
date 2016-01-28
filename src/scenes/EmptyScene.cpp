#include "KinectScene1.h"

KinectScene::KinectScene() : ofxFadeScene("Kinect")
{
	setSingleSetup(false); // call setup each time the scene is loaded
	setFade(1000, 1000); // 1 second fade in/out

}

// scene setup
void KinectScene::setup() {

}

// called when scene is entering, this is just a demo and this
// implementation is not required for this class
void KinectScene::updateEnter() 
{
		
	// called on first enter update
	if(isEnteringFirst()) {
		ofLogNotice("KinectScene") << "update enter";
	}
		
	// fade scene calculates normalized alpha value for us
	ofxFadeScene::updateEnter();
			
	// finished entering?
	if(!isEntering()) {
		ofLogNotice("KinectScene") << "update enter done";
	}
}

// normal update
void KinectScene::update() {

}

// called when scene is exiting, this is just a demo and this
// implementation is not required for this class
void KinectScene::updateExit() {
		
	// called on first exit update
	if(isExitingFirst()) {
		ofLogNotice("KinectScene") << "update exit";
	}
			
	// fade scene calculates normalized alpha value for us
	ofxFadeScene::updateExit();
			
	// finished exiting?
	if(!isExiting()) {
		ofLogNotice("KinectScene") << "update exit done";
	}
}

// draw
void KinectScene::draw() {
	ofEnableAlphaBlending();

	ofDisableAlphaBlending();

}
		
// cleanup
void KinectScene::exit() {

}