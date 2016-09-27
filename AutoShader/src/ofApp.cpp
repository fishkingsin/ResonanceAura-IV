#include "ofApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    auto c = std::make_shared<Video::IPVideoGrabber>();
    
    ofxXmlSettings settings;
    settings.load("settings.xml");
    
    Poco::URI uri(settings.getValue("SETTINGS:URL", "http://127.0.0.1:7890/ipvideo"));
    c->setURI(uri);
    c->connect();
    
    grabbers.push_back(c);
    
    LEDTexture.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    myApa102.setup(&LEDTexture);
}
void ofApp::exit(){
    myApa102.stop();
}
//--------------------------------------------------------------
void ofApp::update(){
    for(std::size_t i = 0; i < grabbers.size(); i++)
    {
        grabbers[i]->update();
    }
     myApa102.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    LEDTexture.begin();
    ofClear(255,255,255, 0);
    
    
    ofSetColor(255,255,255,255);
    for(std::size_t i = 0; i < grabbers.size(); i++)
    {
        grabbers[i]->draw(0,0,ofGetWidth(),ofGetHeight());
    }
    LEDTexture.end();
    
    
    LEDTexture.draw(0,0);
    
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
