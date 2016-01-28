#pragma once

#include <ofxAppUtils.h>
#include "ofxOpenCv.h"
#include "ofxBox2D.h"
#include "NoiseParticles.h"

class ofApp;

class KinectScene2 : public ofxFadeScene {

	public:

	KinectScene2();
	void setup();
	void update();
	void updateEnter();
	void updateExit();
	void draw();
	void exit();

	void setupFlowField();
	void drawFlowfield();
	void setRandomColors(int nthFrame);

	ofxCvGrayscaleImage diffImg;
	ofPolyline			contour;
	bool bUseDepth;

	// an array called flow of 2250 Particle objects (see Particle class)
	vector<NoiseParticle> flow;
	float globalX, globalY;
	float posArray[MAX_PARTICLES * 2 * 2];
    float colArray[MAX_PARTICLES * 3 * 2];
    int curIndex;

	ofImage img;


	ofApp* parentApp;
};
