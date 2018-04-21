#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    rPreview.set(0,0,ofGetWidth(),ofGetHeight());
    
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    kinect.setCameraTiltAngle(0);
    
    pc.setup(&kinect, 5, 5);
    
    touch.getPanel()->makePadGrid(20, 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if(kinect.isFrameNew()){
        pc.update();
        touch.testPoints(pc.getVertices());
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin(rPreview);
    ofEnableDepthTest();
    touch.drawDebug();
    pc.draw();
    ofDisableDepthTest();
    cam.end();
    
    kinect.drawDepth(0, 0, 320, 240);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
