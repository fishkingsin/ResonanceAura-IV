#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "MyApa102.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

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
    
        void GeneratePattern();
        void GeneratePos();
		
        // effect setting
        int totalNumShape = 3;
        vector <Shape> shapes_L;
        vector <Shape> shapes_R;
    
        bool polyModeOdd = true;
    
        ofColor bgColor;
//        ofxVectorGraphics master;
        ofVbo vbo;
    
        ofFbo LEDTexture;
    
    MyApa102 myApa102;
};
