#include "ofApp.h"
#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// setup for nice jaggy-less rendering
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	//ofSetWindowShape(640*2, 480*2);

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();


	/* Scenes */
	// setup the render size (working area)
	transformer.setRenderSize(640, 480);

	// turn on transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	transformer.setTransforms(true, true, false, true, true);
	
	// set the ofxApp transformer so it's automatically applied in draw()
	setTransformer(&transformer);
	
	#ifdef HAVE_OFX_GUI
		// setup transform panel with transformer pointer,
		// loads settings & quad warper xml files if found
		panel.setup(&transformer);
	#endif	

	// load scenes
	sceneManager.add(new ParticleScene()); // save pointer
	sceneManager.add(new LineScene());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene("Lines", true);
	lastScene = sceneManager.getCurrentSceneIndex();
	
	// overlap scenes when transitioning
	sceneManager.setOverlap(true);
	
	// attach scene manager to this ofxApp so it's called automatically,
	setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void ofApp::update(){
	//this->kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	// the current scene is automatically drawn before this function
	// show the render area edges with a white rect
	if(isDebug()) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}

	//this->kinect.getDepthSource()->draw(0,0,640,480); // note that the depth texture is RAW so may appear dark
	//this->kinect.getColorSource()->draw(640,0,640,480);
	//this->kinect.getInfraredSource()->draw(0,480,640,480);
	//this->kinect.getBodyIndexSource()->draw(640,480,640,480);

	// 512 x 424

	// drop out of the auto transform space back to OF screen space
	transformer.pop();
	
	#ifdef HAVE_OFX_GUI
		// draw the transform panel when in debug mode
		if(isDebug()) {
			panel.draw();
		}
	#endif
	
	// draw current scene info using the ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(12, ofGetHeight()-8)
		<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName() << endl;
	
	transformer.push();


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	
		case 'd':
			bDebug = !bDebug;
			break;

		case 'f':
			ofToggleFullscreen();
			break;
	
		case OF_KEY_LEFT:
			sceneManager.prevScene();
			break;
			
		case OF_KEY_RIGHT:
			sceneManager.nextScene();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
