#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
#if defined(TARGET_OPENGLES)
    ofGLESWindowSettings settings;
    settings.width = 1280;
    settings.height = 720;
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
#else
    ofSetupOpenGL( 1024,768, OF_WINDOW);			// <-------- setup the GL context
#endif
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
