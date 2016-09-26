//
//  MyApa102.hpp
//  ifva_everywhere_resonanceAura
//
//  Created by James Kong on 22/9/2016.
//
//

#ifndef MyApa102_hpp
#define MyApa102_hpp
#include "ofMain.h"
#ifdef TARGET_OSX
#else
#include "ofxLibApa102.h"
#endif
class MyApa102 : public ofThread{
public:
    MyApa102();
#ifdef TARGET_OSX
#else
    ofxAPA102 apa102;
#endif
    void setup(ofFbo *fbo);
    void threadedFunction();
    void update();
    void stop();
    vector<u_int8_t*> buf;
    
    ofFbo *fbo;
    ofPixels pixels;
    int length;
    unsigned char GAMMA [256];
    unsigned int microseconds;
    int width ;
    int height;
    float currentTime;
};

#endif /* MyApa102_h */
