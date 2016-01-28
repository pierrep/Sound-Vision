#include "KinectScene.h"
#include "../ofApp.h"

//bool KinectScene::shouldRemove(shared_ptr<ofxBox2dBaseShape>shape) 
//{
//    return !ofRectangle(0, -400, parentApp->getRenderWidth(), parentApp->getRenderHeight()+400).inside(shape->getPosition());
//}

static bool shouldRemove(shared_ptr<TextureShape>shape) {
    return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape->getPosition());
}

KinectScene::KinectScene() : ofxFadeScene("Kinect")
{
	setSingleSetup(true); // call setup once
	setFade(1000, 1000); // 1 second fade in/out		
	colourImg.allocate(512,424);
	diffImg.allocate(512,424);
	img.loadImage("clouds.png");
}

// scene setup
void KinectScene::setup() {
	parentApp = (ofApp*) (ofxGetAppPtr());
	    
	// find all the texture files and load them
    ofDirectory dir;
    ofDisableArbTex();
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        textures.push_back(ofImage(dir.getPath(i)));
    }
    printf("%i Textures Loaded\n", (int)textures.size());
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

	if((parentApp->kinect.isOpen()) && (parentApp->kinect.getBodyIndexSource()->getHeight() > 0)) {
		diffImg.setFromPixels(parentApp->kinect.getBodyIndexSource()->getPixels(), 512, 424);
		diffImg.threshold(200);
		//diffImg.blur();
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....

		parentApp->contourFinder.findContours(diffImg, 1000, (512*424)/2, 2, true);	// find holes
	}

	//// add some circles every so often
	//if((int)ofRandom(0, 10) == 0) {
 //       shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
 //       circle->setPhysics(0.8, 0.8, 0.1);
	//	circle->setup(parentApp->box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 20));
	//	circles.push_back(circle);
	//}

	// add some shapes every so often
	if((int)ofRandom(0, 10) == 0) {
		 
        shapes.push_back(shared_ptr<TextureShape>(new TextureShape));
		shapes.back().get()->setPhysics(0.8, 0.8, 0.1);
        shapes.back().get()->setTexture(&textures[(int)ofRandom(textures.size())]);
        shapes.back().get()->setup(parentApp->box2d, (ofGetWidth()/2)+ofRandom(-400, 400), -20, ofRandom(100, 150));
    }
	
    // remove shapes offscreen
	ofRemove(shapes, shouldRemove);

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

	ofSetColor(255, 255, 255, 255*alpha);
	//parentApp->kinect.getDepthSource()->draw(0,0,320,240); // note that the depth texture is RAW so may appear dark
	//parentApp->kinect.getColorSource()->draw(320,0,320,240);
	//parentApp->kinect.getInfraredSource()->draw(0,240,320,240);
	//parentApp->kinect.getBodyIndexSource()->draw(320,240,320,240);

	img.draw(0,0,parentApp->getRenderWidth(), parentApp->getRenderHeight());

	ofPushStyle();
	if(parentApp->isDebug()) {
		diffImg.draw(0,0);
	}

	polyShapes.clear();
	for (int i = 0; i < parentApp->contourFinder.nBlobs; i++)  
	{  
		//contourFinder.blobs[i].draw(0,0);		
		ofPolyline contour = parentApp->contourFinder.blobs[i].pts;   ;
		contour = contour.getSmoothed(3, 0);
		contour.close();

		for(int j=0;j<(int)contour.size();j++){
				contour[j].x*=parentApp->getRenderWidth()/512;
				contour[j].y*=parentApp->getRenderHeight()/424;
		}

		ofSetLineWidth(4);
		contour.draw();  


		shared_ptr<ofxBox2dPolygon> poly = shared_ptr<ofxBox2dPolygon>(new ofxBox2dPolygon);
		poly.get()->addVertices(contour.getVertices());
		poly.get()->setPhysics(1.0, 0.3, 0.3);
		poly.get()->triangulatePoly(25,-1);
		poly.get()->create(parentApp->box2d.getWorld());
		polyShapes.push_back(poly);
	}

	ofSetColor(250,5,220);
	ofNoFill();
	for (int i=0; i<polyShapes.size(); i++) {
		//polyShapes[i]->draw();
        
        //ofCircle(polyShapes[i]->getPosition(), 3);
	}

	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetColor(255,180,247);
		circles[i]->draw();
	}

		
	for (int i=0; i<shapes.size(); i++) {
        shapes[i].get()->draw();
    }



	ofPopStyle();
	ofDisableAlphaBlending();

}
		
// cleanup
void KinectScene::exit() {

}