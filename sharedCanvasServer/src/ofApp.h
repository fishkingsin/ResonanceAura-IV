#pragma once

#include "ofMain.h"

#include "ofxLibwebsockets.h"
#include "ofxXmlSettings.h"
#include "Drawing.h"

#include "ofxGui.h"
class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxLibwebsockets::Server server;
    bool bConnected;
    
    // drawing stuff
    map<int, Drawing *> drawings;
    int             canvasID;
    
    vector<Drawing *> toDelete;
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
    ofxPanel panel;
    ofParameter<int> delay,lineWidth;
    void onParaChanged(int &i);
    void onLineWidthParaChanged(int &i);
    vector<ofFile>jsonFiles;
    
    void threadedFunction();
    void replayDrawing();
    void saveDrawing(Drawing *drawing);
    
    class ReplayThread : public ofThread{
    public:
        ofxLibwebsockets::Connection * connection;
        string setupstring;
        Json::Value root;
        
        void  startReplay(ofxLibwebsockets::Connection * _connection , Json::Value _root , string _setupstring){
            setupstring = _setupstring;
            connection = _connection;
            root = _root;
            
            startThread();
            
        }
        ~ReplayThread(){
            stopThread();
        }
        //--------------------------------------------------------------
        void threadedFunction(){
            while(isThreadRunning()){
                if(lock()){
                    
                    connection->send(setupstring);
                    Json::Value points = root["points"];
                    int n = points.size();
                    //testing
                    Json::FastWriter fastWriter;
                    for (int j = 0 ; j < n ; j++){
                        ostringstream message;
                        message << "{\"id\":" << ofToString(-1) << ",\"point\":" << fastWriter.write(points[j]["point"]) << "," << "\"color\":"<< fastWriter.write(root["setup"]["color"]) << "}";
                        ofLogNotice("message") << message.str();
                        connection->send( message.str() );
                        
                        ofSleepMillis(50);
                    }
                    stopThread();
                    unlock();
                    delete this;
                }
            }
        }
    };
    vector<ReplayThread*>replayThreads ;
    
};

