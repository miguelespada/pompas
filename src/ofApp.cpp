#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    cam.setDeviceID(1);
    cam.initGrabber(640, 480);
    thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    cropped.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    mask.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    setGUI();
    gui->loadSettings("guiSettings.xml");
    ofSetLogLevel(OF_LOG_VERBOSE);
    curFlow = &farneback;
    curFlow = &pyrLk;
    sender.setup(HOST, PORT);
	background.setLearningTime(100);
	background.setThresholdValue(10);
}

void ofApp::update() {
	cam.update();
    if(cam.isFrameNew()) {
        blur(cam, (int) blurValue);
     
    	convertColor(cam, thresh, CV_RGB2GRAY);
    
        
		background.update(cam, thresh);
        
        if(points.size() > 0){
            ofxCv::fillPoly(points, mask);
            subtract(thresh, mask, thresh);
        }
        
        if(points2.size() > 0){
            ofxCv::fillPoly(points2, mask);
            subtract(thresh, mask, thresh);
        }
        cropped.allocate(w, h, OF_IMAGE_GRAYSCALE);
        cropped.cropFrom(thresh, x, y, w, h);
        threshold(cropped, thresholdValue);

//        curFlow = &pyrLk;
//        
//        pyrLk.setMaxFeatures( (int)maxFeatures );
//        pyrLk.setQualityLevel(qualityLevel );
//        pyrLk.setMinDistance( (int)minDistance );
//        pyrLk.setWindowSize( (int)winSize );
//        pyrLk.setMaxLevel( (int)maxLevel );
//        
          cropped.update();
          thresh.update();
//        curFlow->calcOpticalFlow(thresh);
    }
}

void ofApp::draw() {
    thresh.draw(0, 0, 640, 480);
    
    cropped.draw(640, 0);
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofRect(x, y, w, h);
    ofSetColor(255);
}

void ofApp::setGUI()
{
    gui = new ofxUISuperCanvas("Control");
    gui->addSpacer();
    gui->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
    gui->addSpacer();
    gui->addFPS();
    
    gui->addSpacer();
    gui->addSlider("THRESHOLD", 0.0, 255.0, &thresholdValue)->setTriggerType(OFX_UI_TRIGGER_ALL);
    
    gui->addSpacer();
    gui->addSlider("Blur", 0.0, 255.0, &blurValue)->setTriggerType(OFX_UI_TRIGGER_ALL);
    
    gui->addSpacer();
    gui->addSpacer();
    gui->addLabel("Press & hold 'r' to Select ROI", OFX_UI_FONT_SMALL);
    gui->addSlider("ROI X", 0, 640, &x);
    gui->addSlider("ROI Y", 0, 480, &y);
    gui->addSlider("ROI W", 0, 640, &w);
    gui->addSlider("ROI H", 0, 480, &h);
    
    gui->addSpacer();
    
//    
//     gui->addSlider("pyrScale", 0, 1, &pyrScale);
//     gui->addSlider("levels", 1, 8, &levels);
//     gui->addSlider("winsize", 4, 64, &winsize);
//     gui->addSlider("iterations", 1, 8, &iterations);
//     gui->addSlider("polyN", 5, 10, &polyN);
//     gui->addSlider("polySigma", 1.1, 2, &polySigma);
//    
//    
//    gui->addSpacer();

    
    gui->addSlider("winSize", 4, 64, &winSize);
    gui->addSlider("maxLevel", 0, 8, &maxLevel);
    
    gui->addSlider("maxFeatures", 1, 1000, &maxFeatures);
    gui->addSlider("qualityLevel", 0.001, .02, &qualityLevel);
    gui->addSlider("minDistance", 1, 16, &minDistance);

    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}


void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
    int kind = e.getKind();
    ofLogVerbose() << "got event from: " << name  << " " << kind;
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
    if(keys['1']){
        if(points.size() == 4){
            points.clear();
        }
        else {
            points.push_back(cv::Point(x, y));
        }
    }
    
    if(keys['2']){
        if(points2.size() == 4){
            points2.clear();
        }
        else{
            points2.push_back(cv::Point(x, y));
        }
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