#include "ofApp.h"

ofApp::ofApp() : midiPortState(1, true, "unusedPort", "meshMIDIPort") {
    midiPortState.setupMIDIPort();
}
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    width = 50;
    height = 50;
    
    widthNoteGrid = 12;
    heightNoteGrid = 11;
    
    // set rendering styles to false;
    b_messyMesh = false;
    b_perlinMesh = false;
    b_drawWireFrame = true;
    
    // set initial values to use for perlin noise
    perlinRange = 1.0;
    perlinHeight = 5.0;
    
    ofBackground(255);
    mainCam.setPosition(0, 0, 80);
    
    // make points inide the mesth
    // add one vertex to the mesh across our width and height
    // we use these y and y vals to set the x and y coordinates of the mesh, adding a z value of zero to complete the 3D location of each vertex
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            mainMesh.addVertex(ofPoint(x-width/2, y-height/2,0));
            mainMesh.addColor(ofFloatColor(0,0,0));
        }
    }
    
    // here we loop through and join the vertices together as indices to make rows of triangles to make the wireframe grid
    for(int y = 0; y < height - 1; y++){
        for(int x = 0; x < width - 1; x++){
            
            int index1 = x+y*width;
            int index2 = (x+1)+y*width;
            int index3 = x+(y+1)*width;
            
            int index4 = (x+1)+y*width;
            int index5 = (x+1)+(y+1)*width;
            int index6 = x+(y+1)*width;
            
            printf("%d %d %d %d %d %d\n",index1, index2, index3, index4, index5, index6);
            
            mainMesh.addIndex(index1);
            mainMesh.addIndex(index2);
            mainMesh.addIndex(index3);
            
            mainMesh.addIndex(index4);
            mainMesh.addIndex(index5);
            mainMesh.addIndex(index6);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // change the z value for each vertex in our mesh
    int widthNormalized = width / 2 + width;
    int heightNoramalizer = height / 2 + width;
//
//    int width = width / 12;
//    int height = height / 12;
    
    
    if(b_messyMesh) {
        int numVertices = mainMesh.getNumVertices();
        auto notes = midiPortState.getChannelNotes(0);
        
        float noteXSizeOnGrid = width / widthNoteGrid;//std::static_cast<int>(boxWidth);
        float noteYSizeOnGrid = height / heightNoteGrid;
        //bool noteDown = notes.size() > 0;
        std::vector<int> notesDown;
//        for(auto it : notes.begin()){
//
//        }
        for(int i = 0; i < numVertices; i++){

            ofVec3f newPosition = mainMesh.getVertex(i);
            
            
            // amount we'll scale the Y value by if there is a note present
            //float scale = 5.0f;
            
            
            // set xPitchCoord // TODO Refactor to method
//            auto accum = 0.f;
            int xPitchCoord = 0;
//            while(xPitchCoord <= widthNoteGrid  && accum < width){
//
//            }
            auto accum = 0.f;    
            for(auto xPitchIndex = 0; (xPitchIndex < widthNoteGrid && accum < width); xPitchCoord++, accum += noteXSizeOnGrid){
                if(accum > newPosition.x){
                    //std::cout<<"found xCoord = "<< xPitchCoord << " Accum = " << accum << "\n";
                    //scale = 5.0f; // TODO control with ofxParam 'scaleAmount' (slider) in GUI
                    break;
                }
            }
            
            
            accum = 0.f;
            int yPitchCoord = 0;
            for(auto yPitchIndex = 0; (yPitchIndex < heightNoteGrid && accum < width); yPitchCoord++, accum += noteYSizeOnGrid){
                if(accum > newPosition.y){
                    //std::cout<<"found yCoord = "<< yPitchCoord << " Accum = " << accum << "\n";
                    //scale = 5.0f; // TODO control with ofxParam 'scaleAmount' (slider) in GUI
                    break;
                }
            }
            int pitch = (xPitchCoord + 1) * (yPitchCoord);
            std::cout<< "pitch = " << pitch << "\n";
            if(notes.count(pitch) > 0){
                std::cout<<"note was on, Scaling up Z val";
                
                newPosition.z = ofRandom(-1.0, 1.0) * 10.0f;
            }
            mainMesh.setVertex(i, newPosition);
        }
    }
    
    
    if(b_perlinMesh){
        // distort z val of each point in our mesh with perlin noise
        int i = 0;
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                ofVec3f newPosition = mainMesh.getVertex(i);
                newPosition.z = ofNoise(ofMap(x, 0, width, 0, perlinRange), ofMap(y, 0, height, 0, perlinRange)) * perlinHeight;
                mainMesh.setVertex(i, newPosition);
                i++;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    mainCam.begin();
    
    if(b_drawWireFrame){
        mainMesh.drawWireframe();
    } else{
        mainMesh.drawVertices();
    }
    mainCam.end();
    
    ofSetColor(100);
    string msg = "f: toggle full screen, spacebar: random z-value in meshvertices, w: draw wireframe or point cloud \np: use PerlinNoise for z-value in meshvertices\nUp-key Down-key: increase/devrease PerlinNoise input range\nRight-key Left-key: increase/decrease amplitude of Perlin Noise distortion\nclick and drag in window to move camera";
    ofDrawBitmapString(msg, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            b_messyMesh = !b_messyMesh;
            break;
        case 'w':
            b_drawWireFrame = !b_drawWireFrame;
            break;
        case 'p':
            b_perlinMesh = !b_perlinMesh;
            break;
            
        case OF_KEY_UP:
            perlinRange += 0.1;
            break;
        case OF_KEY_DOWN:
            if(perlinRange > 0.1)
                perlinRange -= 0.1;
            break;
            
        case OF_KEY_RIGHT:
            perlinHeight += 0.1;
            break;
        case OF_KEY_LEFT:
            if(perlinHeight > 0.1)
                perlinHeight -= 0.1;
            break;
    }
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
