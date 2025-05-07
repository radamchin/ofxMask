#include "ofApp.h"
#include "ofxMask.h"

ofxMask alpha_mask;
ofxMask lumi_mask;

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Setup a Alpha Mask
	alpha_mask.allocate(300,300, ofxMask::ALPHA, ofFbo::maxSamples()); // alt. ofxMask::LUMINANCE
    
    // Create the mask
	alpha_mask.beginMask();
        ofSetColor(255,255);
        ofDrawCircle(100,100,50);
    
        ofSetColor(255,192);
        ofDrawCircle(200,100,50);
    
        ofSetColor(255,128);
        ofDrawCircle(200,200,50);
    
        ofSetColor(255,96);
        ofDrawCircle(100,200,50);
	alpha_mask.endMask();
    
    // Draw something with the mask
	alpha_mask.begin();
        ofSetColor(255,255);
        ofTranslate(150,150);
        ofRotateDeg(45);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawRectangle(0,0,120,120);
	alpha_mask.end();
    
    
    // Setup a Luminance Mask
    lumi_mask.allocate(300,300, ofxMask::LUMINANCE, ofFbo::maxSamples());
    
    lumi_mask.beginMask();
        ofSetColor(255,255);
        ofDrawCircle(100,100,50);
    
        ofSetColor(255,192);
        ofDrawCircle(200,100,50);
    
        ofSetColor(255,128);
        ofDrawCircle(200,200,50);
    
        ofSetColor(255,96);
        ofDrawCircle(100,200,50);
    lumi_mask.endMask();
    
    lumi_mask.begin();
        ofSetColor(255,255);
        ofTranslate(150,150);
        ofRotateDeg(45);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawRectangle(0,0,120,120);
    lumi_mask.end();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient( ofColor(32,32,32), ofColor::darkGreen);
    
	ofPushMatrix();
    ofDrawBitmapString("Alpha Masker", 20, 40);
    alpha_mask.drawMasker();
    
	ofTranslate(300,0);
    ofDrawBitmapString("Alpha Maskee", 20, 40);
	alpha_mask.drawMaskee();
    
	ofTranslate(300,0);
    ofDrawBitmapString("Alpha Mask", 20, 40);
	alpha_mask.draw();
    
	ofPopMatrix();
    
    // Draw luminence mask
    ofPushMatrix();
    
    ofTranslate(0,300);
    ofDrawBitmapString("Luminance Masker", 20, 40);
    lumi_mask.drawMasker();
    
    ofTranslate(300,0);
    ofDrawBitmapString("Luminance Maskee", 20, 40);
    lumi_mask.drawMaskee();
    
    ofTranslate(300,0);
    ofDrawBitmapString("Luminance Mask", 20, 40);
    lumi_mask.draw();
    
    ofPopMatrix();
    
    
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
