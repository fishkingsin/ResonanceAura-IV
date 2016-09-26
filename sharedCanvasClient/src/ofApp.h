#pragma once

#include "ofMain.h"

#include "ofxLibwebsockets.h"

#include "Drawing.h"
#include "ofxXmlSettings.h"
#ifdef TARGET_OSX
#else
#include "ofxLibApa102.h"
#endif
class ofApp : public ofBaseApp , ofThread{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        ofxLibwebsockets::Client client;
        bool bConnected;
    
        // drawing stuff
        map<int, Drawing *> drawings;
        int             id;
        ofColor color;
    
        // websocket methods
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

#ifdef TARGET_OSX
#else
 		ofxAPA102 apa102;
#endif
		void threadedFunction();
		vector<u_int8_t*> buf;

		ofFbo fbo;
		ofFbo largeFbo;
        ofPixels pixels;
        int length;
		unsigned char GAMMA [256];
        unsigned int microseconds;
        int width ;
        int height;
    int lineWidth;
};
