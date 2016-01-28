#pragma once

#include <ofxAppUtils.h>

class KinectScene : public ofxFadeScene {

	public:

	KinectScene();
	void setup();
	void update();
	void updateEnter();
	void updateExit();
	void draw();
	void exit();


};
