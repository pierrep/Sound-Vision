#pragma once

#include <ofxAppUtils.h>
#include "ofxOpenCv.h"
#include "ofxBox2D.h"
#include "TextureShape.h"


class ofApp;

class KinectScene : public ofxFadeScene {

	public:

	KinectScene();
	void setup();
	void update();
	void updateEnter();
	void updateExit();
	void draw();
	void exit();
	//bool shouldRemove(shared_ptr<ofxBox2dBaseShape>shape);

	ofxCvGrayscaleImage diffImg;
	ofxCvColorImage colourImg;

	ofImage img;

	vector <shared_ptr<ofxBox2dCircle> >	circles;
	vector <shared_ptr<ofxBox2dPolygon> >	polyShapes;

    // a vector of all the texture images
    vector <ofImage> textures;

    // a vector of all the texture shapes
    vector <shared_ptr<TextureShape> > shapes;

	ofApp* parentApp;
};
