#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    
    ofxOscSender sender;
    
    void setGUI();
    ofxUISuperCanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
	
	ofVideoGrabber cam;
	ofImage thresh;
    
    
    float thresholdValue;
    float blurValue;
    
    ofxCv::FlowFarneback farneback;
    ofxCv::FlowPyrLK pyrLk;
    
    ofxCv::Flow* curFlow;
    
    float pyrScale;
    float levels;
    float winsize;
    float iterations;
    float polyN;
    float polySigma;
    
    float winSize;
    float maxLevel;
    float maxFeatures;
    float qualityLevel;
    float minDistance;

};
