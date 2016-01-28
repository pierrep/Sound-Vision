#pragma once

#include "ofMain.h"
#define MAX_PARTICLES 2000

// a basic noise-based moving particle
class NoiseParticle 
{
  float id;
  float x, y, xp, yp;
  float speed;
  float dir;
  ofColor col; 
  
public:
  NoiseParticle(float _id) {
	col = ofColor(255,50,255);
    id = _id;
	x = y = 256;
    speed = ofRandom(2, 6);
  }
  
  void update(float globalX, float globalY, float kinectWidth, float kinectHeight, ofPolyline& poly) {
    // let it flow, end with a new x and y position
    id += 0.01f;
    dir = (ofNoise(id, x/globalY, y/globalY)-0.5)*globalX;
	float angle = ofDegToRad(dir);
	x += cos(angle)*speed;
    y += sin(angle)*speed;

    // constrain to boundaries
    if (x<-10) x=xp=kinectWidth+10; if (x>kinectWidth+10) x=xp=-10;
    if (y<-10) y=yp=kinectHeight+10; if (y>kinectHeight+10) y=yp=-10;

    // if there is a polygon (more than 0 points)
    if ((poly.size() > 10) && (fabs(poly.getArea()) > 10000))
	{
      // if this particle is outside the polygon
      if (!poly.inside(x, y)) {
        // while it is outside the polygon
		  int count = 0;
        while(!poly.inside(x, y)) {
          // randomize x and y
          x = ofRandom(kinectWidth);
          y = ofRandom(kinectHeight);
		  count++;
		  if(count == 3) break;
        }

        // set previous x and y, to this x and y
        xp=x;
        yp=y;
      }
    }
    
    //ofLine(xp, yp, x, y); // line from previous to current position
    
  }

void updateVertexArrays( float w, float h, int i, float* posBuffer, float* colBuffer) {
	int vi = i * 4;
	posBuffer[vi++] = xp;
	posBuffer[vi++] = yp;
	posBuffer[vi++] = x;
	posBuffer[vi++] = y;

	// set previous to current position
    xp=x;
    yp=y;
	
	int ci = i * 6;
	if( 0 ) {
		// if drawing fluid, draw lines as black & white
		float alpha = 0.8f;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
	} else {
		// otherwise, use color
//		float vxNorm = vel.x * invWindowSize.x;
//		float vyNorm = vel.y * invWindowSize.y;
//		float v2 = vxNorm * vxNorm + vyNorm * vyNorm;
//#define VMAX 0.013f
//		if(v2>VMAX*VMAX) v2 = VMAX*VMAX;
//		float satInc = mass > 0.5 ? mass * mass * mass : 0;
//		satInc *= satInc * satInc * satInc;
//		ofColor color;
//		color.setHsb(0, v2 * 255.0f / ( VMAX * VMAX ) + satInc, ofLerp(0.5, 1, mass) * alpha * 255.0f);
		ofColor color;
		color.set(255,0,255,255);
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
	}
}

};
