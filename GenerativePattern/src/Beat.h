//
//  Beat.hpp
//  SONY
//
//  Created by Kwankai on 17/3/16.
//
//

#ifndef Beat_h
#define Beat_h


#include "ofMain.h"
#include "Shape.h"


class Beat {
    
public:
    
    Beat();
    void set(char _direction, int _num, int _edge, int _radius);
    void init();
    void update();
    void draw();

    
    // variables
    
    float x, _x;
    float y,_y;
    float z,_z;
    char direction;
    float speed;
    int num;
    int weight;
    int size;
    int _size;
    
    int life = 400;
    bool dead = false;
    int edge;
    ofColor color;
    
//    ofxVectorGraphics output;
    
    
    void hit(char _direction);
    
    vector <Shape> shapes;
private:
    
};
#endif
