#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    cam.initGrabber(640, 480);
    thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    setGUI();
    gui->loadSettings("guiSettings.xml");
    ofSetLogLevel(OF_LOG_VERBOSE);
    curFlow = &farneback;
    curFlow = &pyrLk;
    sender.setup(HOST, PORT);
}

void ofApp::update() {
	cam.update();
    if(cam.isFrameNew()) {
        blur(cam, (int) blurValue);
     
    	convertColor(cam, thresh, CV_RGB2GRAY);
        threshold(thresh, thresholdValue);
    
        
        curFlow = &pyrLk;
        
        pyrLk.setMaxFeatures( (int)maxFeatures );
        pyrLk.setQualityLevel(qualityLevel );
        pyrLk.setMinDistance( (int)minDistance );
        pyrLk.setWindowSize( (int)winSize );
        pyrLk.setMaxLevel( (int)maxLevel );
        
        thresh.update();
        curFlow->calcOpticalFlow(thresh);
    }
}

void ofApp::draw() {
	cam.draw(0, 0);
    
    thresh.draw(640, 0, 320, 240);
    curFlow->draw(0, 0);
    
    countNonZero(curFlow->));
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
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
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
    
    switch (key){
        case 'h':
            gui->toggleVisible();
            break;
        case 's':
            cam.videoSettings();
            break;
        default:
            break;
    }
}