#include "ofApp.h"

ofApp::ofApp() : midiPortState(16, false), animated3DMesh(&midiPortState) {
    midiPortState.setupMIDIPort();
}
//--------------------------------------------------------------
void ofApp::setup(){
    //animated3DMesh.setMIDIPortState(&midiPortState);
    ofBackground(255);
    animated3DMesh.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    animated3DMesh.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    animated3DMesh.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    animated3DMesh.keyPressed(key);
}



