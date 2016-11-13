#include "ofApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef TARGET_OSX
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
    auto c = std::make_shared<Video::IPVideoGrabber>();
    
    ofxXmlSettings settings;
    settings.load("settings.xml");
    
    Poco::URI uri(settings.getValue("SETTINGS:URL", "http://127.0.0.1:7890/ipvideo"));
    c->setURI(uri);
    c->connect();
    
    grabber = c;
    
    LEDTexture.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    myApa102.setup(&LEDTexture);
#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
    
#endif
    shader.enableWatchFiles();
}
void ofApp::exit(){
    myApa102.stop();
}
//--------------------------------------------------------------
void ofApp::update(){
    shader.setUniform3f("iResolution",640,480,0);
    shader.setUniform1f("iGlobalTime", ofGetFrameNum());
    grabber->update();
    
    myApa102.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    LEDTexture.begin();
    shader.begin();
    ofClear(255,255,255, 0);
    
    
    ofSetColor(255,255,255,255);
    
    grabber->draw(0,0,ofGetWidth(),ofGetHeight());
    shader.end();
    LEDTexture.end();
    
    shader.begin();
    LEDTexture.draw(0,0);
    shader.end();
    
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
