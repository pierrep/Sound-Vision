
#include "ofApp.h"
#include "scenes/scenes.h"

#define SCENEX 640 * 2
#define SCENEY 480 * 2
#define	MIN_DEPTH_CLIP		50.0f
#define MAX_DEPTH_CLIP		10000.0f

//--------------------------------------------------------------
void ofApp::setup() {

	// setup for nice jaggy-less rendering
	//ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetWindowShape(640 * 2, 480 * 2);
	ofSetLogLevel(OF_LOG_WARNING);

	setupBox2D();
	setupSceneManager();
	setupKinect();
}

//--------------------------------------------------------------
void ofApp::update() {

	// the current scene is automatically updated before this function	

	kinect.update();
	generateDepthImage();

	box2d.update();	

	// update the transform panel when in debug mode
	if(isDebug()) {
		panel.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// the current scene is automatically drawn before this function

	// show the render area edges with a white rect
	if(isDebug()) {
		ofPushStyle();
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
		ofPopStyle();
	}
	
	// drop out of the auto transform space back to OF screen space
	transformer.pop();
	
	// draw the transform panel when in debug mode
	if(isDebug()) {
		panel.draw();
	}
	
	// draw current scene info 
	ofSetColor(200);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),12, ofGetHeight()-30);
	ofxBitmapString(12, ofGetHeight()-8) << "Current Scene: " << sceneManager.getCurrentSceneIndex() << " " << sceneManager.getCurrentSceneName() << endl;
	stringstream ss;
	ss << "nearClipping [- n / + m]: " << ofToString(nearClipping) << endl;
	ss << "farClipping  [+ g / - f]: " << ofToString(farClipping) << endl;
	ofDrawBitmapString(ss.str(), 536, ofGetHeight()-40);
	ofDrawBitmapString("Num Blobs: "+ofToString(contourFinder.nBlobs), 0, ofGetHeight() - 60);
	
	// go back to the auto transform space
	//
	// this is actually done automatically if the transformer is set but
	// included here for completeness
	transformer.push();

	// the warp editor is drawn automatically after this function
}

//--------------------------------------------------------------
void ofApp::generateDepthImage() 
{
	if((!kinect.isOpen()) || (kinect.getDepthSource() == nullptr)) {
		return;
	}

	ofPixels_<unsigned short> pixels = this->kinect.getDepthSource()->getPixelsRef();

	int rawindex = 0;
	int displayindex = 0;

	for (int x = 0; x < pixels.size(); x++) {
		unsigned char greyValue = depthLookupTable[pixels[rawindex++]];

		depthImage.getPixels()[displayindex++] = greyValue;
		//depthImage.getPixels()[displayindex++] = greyValue;
		//depthImage.getPixels()[displayindex++] = greyValue;
	}

	depthImage.update();

}
//--------------------------------------------------------------
void ofApp::setupKinect()
{
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();
	kinectWidth = 512;
	kinectHeight = 424;

	depthImage.allocate(kinectWidth, kinectHeight, OF_IMAGE_GRAYSCALE);

	nearClipping = 500.0;
	farClipping = 2200.0;
	setupRawDepthLookupTable();
}

//--------------------------------------------------------------
void ofApp::setupSceneManager() 
{
	// setup the render size (working area)
	transformer.setRenderSize(SCENEX, SCENEY);

	// turn on transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	transformer.setTransforms(true, true, false, true, true);

	// set the ofxApp transformer so it's automatically applied in draw()
	setTransformer(&transformer);

	// setup transform panel with transformer pointer,
	// loads settings & quad warper xml files if found
	panel.setup(&transformer);

	// load scenes
	sceneManager.add(new ParticleScene()); 
	sceneManager.add(new LineScene());
	sceneManager.add(new KinectScene());
	sceneManager.add(new KinectScene2());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene("Kinect2", true);
	lastScene = sceneManager.getCurrentSceneIndex();
	
	// overlap scenes when transitioning
	sceneManager.setOverlap(true);
	
	// attach scene manager to this ofxApp so it's called automatically,
	// you can also call the callbacks (update, draw, keyPressed, etc) manually
	// if you don't set it
	//
	// you can also turn off the auto sceneManager update and draw calls with:
	// setSceneManagerUpdate(false);
	// setSceneManagerDraw(false);
	//
	// the input callbacks in your scenes will be called if they are implemented
	//
	setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void ofApp::setupBox2D() 
{
	// Box2d
	box2d.init();
	box2d.setGravity(0, 1);
	//box2d.createGround();
	box2d.setFPS(30.0);

}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	switch (key) {
	
		case 'd':
			bDebug = !bDebug;
			break;
			
		case 'n':
			nearClipping -= 10.0f;
			if (nearClipping < MIN_DEPTH_CLIP) nearClipping = MIN_DEPTH_CLIP;
			setupRawDepthLookupTable();
			break;
		case 'm':
			nearClipping += 10.0f;
			if (nearClipping > (farClipping - 255)) nearClipping = farClipping - 255;
			setupRawDepthLookupTable();
			break;
		case 'g':
			farClipping += 50.0f;
			if (farClipping > MAX_DEPTH_CLIP) farClipping = MAX_DEPTH_CLIP;
			setupRawDepthLookupTable();
			break;
		case 'f':
			farClipping -= 50.0f;
			if (farClipping < (nearClipping + 255)) farClipping = nearClipping + 255;
			setupRawDepthLookupTable();
			break;
	
		case OF_KEY_LEFT:
			sceneManager.prevScene();
			break;
			
		case OF_KEY_RIGHT:
			sceneManager.nextScene();
			break;
			
		case OF_KEY_DOWN:
			if(sceneManager.getCurrentScene()) { // returns NULL if no scene selected
				lastScene = sceneManager.getCurrentSceneIndex();
			}
			sceneManager.noScene();
			break;
			
		case OF_KEY_UP:
			sceneManager.gotoScene(lastScene);
			break;
			
		case 'o':
			sceneManager.setOverlap(!sceneManager.getOverlap());
			break;
	}
}

//--------------------------------------------------------------
void ofApp::setupRawDepthLookupTable()
{
	// if you want near to be black and far to be white, switch these
	unsigned char nearColor = 255;
	unsigned char farColor = 0;

	// how far back we want to gradient before hitting the max distance.
	// everything past the max distance will be clampped to 'farcolor'
	unsigned int maxDepthLevels = 8400;
	depthLookupTable.resize(maxDepthLevels);

	depthLookupTable[0] = 0;
	for (unsigned int i = 1; i < maxDepthLevels; i++)
	{
		depthLookupTable[i] = ofMap(i, nearClipping, farClipping, nearColor, farColor, true);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

	// transformer.setNewScreenSize() is automatically called if the transformer is set
	
}

