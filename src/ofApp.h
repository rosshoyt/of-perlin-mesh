#pragma once

#include "ofMain.h"
#include "MIDIPortState.h"
#include "Animated3DMesh.h"

class ofApp : public ofBaseApp{

	public:
        ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

        // MIDI input
        MIDIPortState midiPortState;

		// Display class -
		Animated3DMesh animated3DMesh;
};
