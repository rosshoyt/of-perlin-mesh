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
    
    
    std::map<std::pair<int,int>,int> pointNoteMap;
    
    int getNoteFromPoint(const ofVec3f &point){
        std::cout << "Finding note for point (" << point << ")" << std::endl;
//        int xNorm = float(width) / float(widthNoteGrid);
//        int yNorm = float(height) / float(heightNoteGrid);
        int xSegment = getSegmentNumber(point.x, width, widthNoteGrid);
        int ySegment = getSegmentNumber(point.y, height, heightNoteGrid);
        int noteNumber = ySegment * heightNoteGrid + xSegment;
        std::cout<< "Pitch = " << noteNumber << ", xSegment #: " << xSegment << ", ySegment #: " << ySegment << std::endl;
        
        return noteNumber;
        
    }
    //ofVec3f normalizePoint
    int getSegmentNumber(const int coord, const int dimLength, const int nSegments){
        float segmentSize = float(dimLength) / float(nSegments);
        // first compensate for the offset which was introduced to center the mesh
        float coordOffset = dimLength / 2;
        int normalizedCoord = coord + coordOffset;
        
        //std::cout << "Getting segment of normalized coord " << normalizedCoord << " using segments of size " << segmentSize << std::endl;
        
        //std::cout << "Checking if normCoord is less than... ";
        
        int pitchGridIndex = 1;
        for(; pitchGridIndex <= nSegments; pitchGridIndex++){
            float gridSegmentMaxVal = pitchGridIndex * segmentSize;
            //std::cout << gridSegmentMaxVal << "... ";
            
            if(normalizedCoord < gridSegmentMaxVal){
                break;
            }
        }
        //std::cout << std::endl;
        
        return pitchGridIndex - 1;
        
    }
    
    
        // MIDI input
        MIDIPortState midiPortState;
};
