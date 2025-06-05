// src/ofApp.h
#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp
{
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void exit() override;

private:
	ofxKinect kinect;

	// raw 16‑bit depth and scratch buffer for median filter
	ofShortPixels rawDepth, tmpDepth;

	// CPU threshold mask
	ofPixels maskPixels; // 8‑bit (0/255)
	ofTexture maskTex;

	// CPU false‑color depth
	ofPixels colorPixels; // RGB
	ofTexture colorTex;

	// adjustable thresholds (millimetres)
	float minDepth = 500.0f;
	float maxDepth = 1500.0f;

	// tilt control
	int tiltAngle = 0;

	// Syphon servers
	ofxSyphonServer rgbSrv;
	ofxSyphonServer maskSrv;
	ofxSyphonServer colorSrv;
};
