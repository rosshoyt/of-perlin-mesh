#pragma once

#include "ofMain.h"
#include "MIDIPortState.h"

class ofApp : public ofBaseApp{

	public:
        ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // 3D graphics
        ofMesh mainMesh;
        ofEasyCam mainCam;
        int width, height;
        int widthNoteGrid, heightNoteGrid;
        bool b_messyMesh, b_perlinMesh, b_drawWireFrame;
        float perlinRange, perlinHeight;
    
        // map that stores each x/y coord pair's associated MIDI pitch value
        std::map<std::pair<int,int>,int> pointNoteMap;
    
    
        int getNoteFromPoint(const ofVec3f &point);
    
        // TODO refactor, store some calculated vars globally
        int getSegmentNumber(const int coord, const int dimLength, const int nSegments);
    
    
        // MIDI input
        MIDIPortState midiPortState;
};
