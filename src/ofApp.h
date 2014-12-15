#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 8000

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
	ofxCv::RunningBackground background;
    
    void setGUI();
    ofxUISuperCanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
	
    ofVideoGrabber cam;
    ofImage thresh;
    ofImage cropped;
    
    ofImage mask;
    
    float thresholdValue;
    float blurValue;
    float media;
    
    float x, y, h, w;
    bool keys[256] = {false};
    
    
    vector<cv::Point> points, points2;
    int graph_x = 0;
    float detection;
    bool state;
    float smoothValue;
};
