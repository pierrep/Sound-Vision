#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxAppUtils.h"

#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
	#include "ofxGui.h"
	#include "ofxTransformPanel.h"
#endif

class ofApp : public ofxApp {

	public:
		ofApp()  {}

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxKFW2::Device kinect;

		/* scenes */
		// rendering transformer
		ofxTransformer transformer;

		// handles the scenes
		ofxSceneManager sceneManager;
		int lastScene;

	
		#ifdef HAVE_OFX_GUI
			ofxTransformPanel panel;
		#endif
			

};
