//
//  MyApa102.cpp
//  ifva_everywhere_resonanceAura
//
//  Created by James Kong on 22/9/2016.
//
//

#include "MyApa102.h"
#include "ofxXmlSettings.h"
MyApa102::MyApa102(){
    microseconds = 1;
}

void MyApa102::setup(ofFbo *ifbo){
    fbo = ifbo;
    width =  fbo->getWidth();
    height = fbo->getHeight();
    pixels.allocate(width, height,OF_IMAGE_COLOR);
    length = 4+(height*4)+4;
    buf.resize(width);
    
    ofxXmlSettings settings;
    settings.load("settings.xml");
    float brightness = settings.getValue("SETTINGS:BRIGHTNESS",1.0);
    float gamma = settings.getValue("SETTINGS:GAMMA",2.7);
    
    for ( int i = 0 ; i < 256 ; i++){
        GAMMA[i] = int(pow(float(i) / 255.0, gamma ) * 255.0  * brightness ) ;
    }
    for(int i = 0 ; i < buf.size() ; i++){
        buf[i] = (u_int8_t*)malloc(length);
        buf[i][0] = 0x00;
        buf[i][1] = 0x00;
        buf[i][2] = 0x00;
        buf[i][3] = 0x00;
        
        buf[i][length-4] = 0xFF;
        buf[i][length-3] = 0xFF;
        buf[i][length-2] = 0xFF;
        buf[i][length-1] = 0xFF;
    }
#ifdef TARGET_OSX
#else
    apa102.setup(height);
#endif
    ofThread::startThread();
}

void MyApa102::update(){
    microseconds = (int)(sinf(currentTime)*150)+150;
    currentTime+=0.005;
//    ofLogVerbose() << "microseconds " << microseconds;
    fbo->readToPixels(pixels);
    for(int x = 0 ; x < width ; x++){
        
        //scan each pixels of the line
        
        for(int y = 0 ; y <height ; y++){
            
            ofColor c = pixels.getColor(x,y);
            int index = (y*4)+4;
            buf[x][index] = 0b11100000 | (0b00011111 & c.a);
            buf[x][index+1] = GAMMA[c.b];
            buf[x][index+2] = GAMMA[c.g];
            buf[x][index+3] = GAMMA[c.r];
        }
    }
}

void MyApa102::stop(){
    ofThread::stopThread();
    for(int y = 0 ; y < height ; y++){
        
        int index = (y*4)+4;
        buf[0][index] = 0b11100000 | (0b00011111 & 0);
        buf[0][index+1] = 0;
        buf[0][index+2] = 0;
        buf[0][index+3] = 0;
    }
#ifdef TARGET_OSX
#else
    apa102.send(buf[0], length);
#endif
    
}
void MyApa102::threadedFunction(){
    while(ofThread::isThreadRunning())
    {
        // Attempt to lock the mutex.  If blocking is turned on,
        if(ofThread::lock())
        {
            
            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            //scan each line x
            
            
            for(int x = 0 ; x < width ; x++){
                
                
#ifdef TARGET_OSX
//                ofLogVerbose("threadedFunction()") << "apa102.send(buf["<<x<<"], length);";
#else
                usleep(microseconds);
                apa102.send(buf[x], length);
#endif
            }
            
            ofThread::unlock();
            
            // Sleep for 1 second.
            // sleep(1000);
            
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
    
}
