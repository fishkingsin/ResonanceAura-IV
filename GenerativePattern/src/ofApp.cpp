#include "ofApp.h"

//--------------------------------------------------------------


int edgeList[6] = {4,100,6,8,12,100};
int sizeList[4] = {10,50,100,300};

int posXList[5] = {-180,0, 180, 360, 540};
int posYList[5] = {-72,0, 72, 144, 288};

int dirtion = 0;

void ofApp::setup(){
#ifdef TARGET_OSX
    ofSetLogLevel(OF_LOG_VERBOSE);
    
#endif
    bgColor.set(0,255);
    
    LEDTexture.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    
    for (int i=0; i<totalNumShape; i++){
        Shape temp;
        temp.init();
        shapes_L.push_back(temp);
        shapes_R.push_back(temp);
    }
    
    GeneratePattern();
    GeneratePos();
    myApa102.setup(&LEDTexture);
}
void ofApp::exit(){
    myApa102.stop();
}
//--------------------------------------------------------------
void ofApp::update(){
    for (int i=0; i<totalNumShape; i++){
        shapes_L[i].update();
        shapes_R[i].update();
    }
    
    if (ofGetFrameNum()% 120 == 0){
        
        GeneratePos();
    }
    
    if (ofGetFrameNum()% 720 == 0){
        dirtion = (int)ofRandom(2);
        GeneratePattern();
    }
    myApa102.update();
}



void ofApp::GeneratePattern(){
    
    int randomEdge = (int)ofRandom(0,6);
    int randomSize = (int)ofRandom(0,4);
    
    for (int i=0; i<totalNumShape; i++){
        shapes_L[i]._edge = edgeList[randomEdge];
        shapes_R[i]._edge = edgeList[randomEdge];
        
        
        shapes_L[i]._weight = shapes_R[i]._weight = sizeList[ (int)ofRandom(0,4)];
        shapes_R[i]._weight = shapes_R[i]._weight = sizeList[ (int)ofRandom(0,4)];
        
        shapes_L[i]._size = sizeList[randomSize];
        shapes_R[i]._size = sizeList[randomSize];
    }
}

void ofApp::GeneratePos(){
    
    
    
    for (int i=0; i<totalNumShape; i++){
        
        switch(dirtion){
            case 0:
                
                ofLogVerbose() << " horizontal";
                shapes_L[i]._x = posXList[(int)ofRandom(0,5)];
                shapes_R[i]._x = posXList[(int)ofRandom(0,5)];
                
                shapes_L[i]._y= ofGetHeight()*0.5;
                shapes_R[i]._y= ofGetHeight()*0.5;
                break;
            case 1:
                ofLogVerbose() << " vertical";
                shapes_L[i]._x = ofGetWidth()*0.5;
                shapes_R[i]._x = ofGetWidth()*0.5;
                
                shapes_L[i]._y= posYList[(int)ofRandom(0,5)];
                shapes_R[i]._y= posYList[(int)ofRandom(0,5)];
                break;
            default:
                ofLogVerbose() << " horizontal";
                shapes_L[i]._x = posXList[(int)ofRandom(0,5)];
                shapes_R[i]._x = posXList[(int)ofRandom(0,5)];
                
                shapes_L[i]._y= ofGetHeight()*0.5;
                shapes_R[i]._y= ofGetHeight()*0.5;
                break;
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(bgColor);
    
    LEDTexture.begin();
    ofClear(255,255,255, 0);
    
    
    ofSetColor(0,255);
    if(polyModeOdd){
        ofSetPolyMode(OF_POLY_WINDING_ODD);
        ofBeginShape();
    }
    
    
    
    for (int i=0; i<totalNumShape; i++){
        
        shapes_L[i].master = polyModeOdd;
        shapes_L[i].draw();
        ofNextContour(true);
        
        shapes_R[i].master = polyModeOdd;
        shapes_R[i].draw();
        ofNextContour(true);
    }
    
    
    if(polyModeOdd){
        ofEndShape();
    }
    
    LEDTexture.end();
    
    
    LEDTexture.draw(0,0);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if(key=='r'){
        GeneratePattern();
    }
    
    if(key==' '){
        polyModeOdd=!polyModeOdd;
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
