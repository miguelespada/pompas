#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    cam.setDeviceID(1);
    cam.initGrabber(640, 480);
    thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    cropped.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    setGUI();
    gui->loadSettings("guiSettings.xml");
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    sender.setup(HOST, PORT);
    
	background.setLearningTime(100);
}

void ofApp::update() {
	cam.update();
    if(cam.isFrameNew()) {
     
    	convertColor(cam, thresh, CV_RGB2GRAY);
        background.setThresholdValue(thresholdValue);
		background.update(cam, thresh);
        
        thresh.update();
        
        cropped.allocate(w, h, OF_IMAGE_GRAYSCALE);
        cropped.cropFrom(thresh, x, y, w, h);
    
        blur(cropped, (int) blurValue);
        dilate(cropped);
        cropped.update();
        media = media * smoothValue + (1 - smoothValue) * mean(toCv(cropped))[0];
    }
}

void ofApp::draw() {
    ofPushStyle();

    thresh.draw(0, 0, 640, 480);
    
    if(media > detection){
        ofSetColor(0, 255, 0);
        if(!state){
            state = true;
            ofxOscMessage m;
            m.setAddress("/on");
            sender.sendMessage(m);
        }
    }
    else{
        ofSetColor(255, 0, 0);
        if(state){
            state = false;
            ofxOscMessage m;
            m.setAddress("/off");
            sender.sendMessage(m);
        }
    }
    ofNoFill();
    
    cropped.draw(x, y, w, h);

    ofRect(x, y, w, h);
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(640, 0);
    
    ofLine(0, ofGetHeight()  - detection, 640, ofGetHeight() - detection);
    graph_x = (graph_x + 1) % 640;
    ofLine(graph_x, ofGetHeight(), graph_x, ofGetHeight() - media);
    
    ofDrawBitmapString(ofToString(media), 20, 300);
    ofPopMatrix();
    ofPopStyle();
}

void ofApp::setGUI()
{
    gui = new ofxUISuperCanvas("Control");
    gui->addSpacer();
    gui->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
    gui->addSpacer();
    gui->addFPS();
    
    gui->addSpacer();
    gui->addSlider("Detection", 1, 50, &detection);
    gui->addSpacer();
    gui->addSlider("Smooth", 0, 1, &smoothValue);
    gui->addSpacer();
    gui->addSlider("THRESHOLD", 0.0, 255.0, &thresholdValue);
    gui->addSpacer();
    gui->addSlider("Blur", 0.0, 255.0, &blurValue);
    gui->addSpacer();
    gui->addLabel("Press & hold 'r' to Select ROI", OFX_UI_FONT_SMALL);
    gui->addSlider("ROI X", 0, 640, &x);
    gui->addSlider("ROI Y", 0, 480, &y);
    gui->addSlider("ROI W", 0, 640, &w);
    gui->addSlider("ROI H", 0, 480, &h);
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}


void ofApp::guiEvent(ofxUIEventArgs &e){
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    keys[key] = false;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(keys['r']){
        ofApp::w = (x - ofApp::x);
        ofApp::h = (y - ofApp::y);
        if(ofApp::w < 0) w = 0;
        if(ofApp::h < 0) h = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(keys['r']){
        ofApp::x = x;
        ofApp::y = y;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::exit(){
    gui->saveSettings("guiSettings.xml");
    
    delete gui;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    keys[key] = true;
    switch (key){
        
        case 'h':
            gui->toggleVisible();
            break;
        case 's':
            cam.videoSettings();
            break;
        case 'c':
            ofShowCursor();
            break;
        default:
            break;
    }
}