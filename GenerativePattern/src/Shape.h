//
//  shape.h
//  Sony
//
//  Created by Kwankai on 15/3/16.
//
//

#ifndef shape_h
#define shape_h

#include "ofMain.h"


class Shape {
    
public:
    
    void init();
    void update();
    void draw();
    
    // variables
    float x, _x;
    float y, _y;
    float z, _z;
    
    int _edge, edge;
    int weight, _weight;
    int _size, size;
    ofColor color;
//    ofxVectorGraphics output;
    
   // bool flip =false;
    bool master = false;
    Shape();
private:

};
#endif
