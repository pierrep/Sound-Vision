#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxKinectForWindows2.h"
#include "ofxGui.h"
#include "ofxTransformPanel.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

class ParticleScene;

/// controls:
///     - L & R arrows to change scenes (note: wait for 2s transition)
///     - D arrow to change to "no scene" & U arrow to return to last scene
///     - 'd' key to toggle debug mode and enable control panel & warp editor
///     - 'm' key to mirror the render area on the x axis
///     - 'n' key to mirror the render area on the y axis
///     - 'a' key to enable aspect ratio scaling
///     - 'c' key to enable auto centering, only applied if apsect ratio scaling
///           is on and the quad warper is off
///     - 'q' key to toggle to quad warper
///     - 'f' to toggle fullscreen mode
///     - '-' & '=': remove/add particles when the particle scene is running
///     - 'o' to toggle scene overlap
///



class ofApp : public ofxApp { // <-- inherits ofxApp instead of ofBaseApp
	
	public:
	
		ofApp()  {}
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);		
		void windowResized(int w, int h);
		void setupSceneManager();
		void setupBox2D();
		void setupKinect();
		void setupRawDepthLookupTable();
		void generateDepthImage();


		// Kinect stuff
		ofxKFW2::Device kinect;
		ofxCvContourFinder 	contourFinder;
		int kinectWidth;
		int kinectHeight;
		vector<unsigned char> depthLookupTable;
		ofImage	depthImage;
		float	nearClipping, farClipping;

		// rendering transformer
		ofxTransformer transformer;
		ofxTransformPanel panel;

		// handles the scenes
		ofxSceneManager sceneManager;
		int lastScene;	

		/* ofxBox2D */
		ofxBox2d			box2d;

};
