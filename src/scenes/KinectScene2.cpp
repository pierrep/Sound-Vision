#include "KinectScene2.h"
#include "../ofApp.h"



KinectScene2::KinectScene2() : ofxFadeScene("Kinect2")
{
	setSingleSetup(true); // call setup each time the scene is loaded
	setFade(1000, 1000); // 1 second fade in/out		
	bUseDepth = true;
}

// scene setup
void KinectScene2::setup() {
	ofLogNotice("KinectScene2") << "start setup";
	parentApp = (ofApp*) (ofxGetAppPtr());
	diffImg.allocate(512,424);
	img.loadImage("nasturtium.png");

	for(int i = 0; i < MAX_PARTICLES; i++) 
	{
		NoiseParticle p((float)i/10000.0f);
		flow.push_back(p);
	}
	ofLogNotice("KinectScene2") << "finished setup";
}

// called when scene is entering, this is just a demo and this
// implementation is not required for this class
void KinectScene2::updateEnter() 
{
		
	// called on first enter update
	if(isEnteringFirst()) {
		ofLogNotice("KinectScene2") << "update enter";
	}
		
	// fade scene calculates normalized alpha value for us
	ofxFadeScene::updateEnter();
			
	// finished entering?
	if(!isEntering()) {
		ofLogNotice("KinectScene2") << "update enter done";
	}
}

// normal update
void KinectScene2::update() {

	if((parentApp->kinect.isOpen()) && (parentApp->kinect.getBodyIndexSource()->getHeight() > 0)) {
		if(bUseDepth) {
			diffImg.setFromPixels(parentApp->depthImage.getPixels(),parentApp->kinectWidth,parentApp->kinectHeight);
			diffImg.threshold(50);
			diffImg.blur();
		} else {
			diffImg.setFromPixels(parentApp->kinect.getBodyIndexSource()->getPixels(), 512, 424);
			diffImg.threshold(200);
		}

		parentApp->contourFinder.findContours(diffImg, 1000, (512*424)/2, 2, false);	// find holes
	}


}

// called when scene is exiting
void KinectScene2::updateExit() {
		
	// called on first exit update
	if(isExitingFirst()) {
		ofLogNotice("KinectScene2") << "update exit";
	}
			
	// fade scene calculates normalized alpha value for us
	ofxFadeScene::updateExit();
			
	// finished exiting?
	if(!isExiting()) {
		ofLogNotice("KinectScene2") << "update exit done";
	}
}

void KinectScene2::draw() 
{
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255*alpha);

	//img.draw(0,0,parentApp->getRenderWidth(), parentApp->getRenderHeight());

	ofPushStyle();
	if(parentApp->isDebug()) {
		diffImg.draw(0,0);
		parentApp->depthImage.draw(512,0);
	}

	for (int i = 0; i < parentApp->contourFinder.nBlobs; i++)  
	{  
		contour.clear();
		contour = parentApp->contourFinder.blobs[i].pts;   ;
		contour = contour.getSmoothed(3, 0);
		contour.close();

		//ofSetLineWidth(4);
		ofPolyline scaledContour = contour;
		for(int j=0;j<(int)scaledContour.size();j++){
			scaledContour[j].x*=parentApp->getRenderWidth()/512;
			scaledContour[j].y*=parentApp->getRenderHeight()/424;
		}
		scaledContour.draw();  

	}

	drawFlowfield();

	ofPopStyle();
	ofDisableAlphaBlending();

}

void KinectScene2::setupFlowField()
{
  // set all colors randomly now
  //setRandomColors(1);
}


void KinectScene2::drawFlowfield()
{
	float reScale = (float) parentApp->getRenderWidth() / parentApp->kinectWidth;

	// center and reScale from Kinect to custom dimensions
	ofTranslate(0, (parentApp->getRenderHeight() - parentApp->kinectHeight*reScale)/2);
	ofScale(reScale,reScale,reScale);

	// set global variables that influence the particle flow's movement
	globalX = ofNoise(ofGetFrameNum() * 0.01) * parentApp->getRenderWidth()/2 + parentApp->getRenderWidth()/4;
	globalY = ofNoise(ofGetFrameNum() * 0.005 + 5) * parentApp->getRenderHeight();
	
	// update and display all particles in the flow
	//if(contour.size() > 3) 
	{
		float w = parentApp->kinectWidth;
		float h = parentApp->kinectHeight;

		for (int i = 0; i < flow.size(); i++) 
		{
			flow.at(i).update(globalX,globalY,w,h,contour);
			flow.at(i).updateVertexArrays(w, h, i, posArray, colArray);
		}

		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ADD);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, posArray);
	
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colArray);
	
		glDrawArrays(GL_LINES, 0, MAX_PARTICLES * 2);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		ofPopStyle();


	}


	// set the colors randomly every 240th frame
	//setRandomColors(240);

}

// sets the colors every nth frame
void KinectScene2::setRandomColors(int nthFrame) 
{
	if (ofGetFrameNum() % nthFrame == 0) 
	{
		//// turn a palette into a series of strings
		//String[] paletteStrings = split(palettes[int(random(palettes.length))], ",");
		//// turn strings into colors
		//color[] colorPalette = new color[paletteStrings.length];
		//for (int i=0; i<paletteStrings.length; i++) {
		//  colorPalette[i] = int(paletteStrings[i]);
		//}
		//// set background color to first color from palette
		//bgColor = colorPalette[0];
		//// set all particle colors randomly to color from palette (excluding first aka background color)
		//for (int i=0; i<flow.length; i++) {
		//  flow[i].col = colorPalette[int(random(1, colorPalette.length))];
		//}
	}
}


// cleanup
void KinectScene2::exit() {

}
