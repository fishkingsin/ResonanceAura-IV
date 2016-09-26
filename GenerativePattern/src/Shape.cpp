//
//  shape.cpp
//  Sony
//
//  Created by Kwankai on 15/3/16.
//
//

#include "Shape.h"

Shape::Shape(){
}

void Shape::init(){
    size = _size = 10;
    _x = x = ofRandom(ofGetWidth()/2);
    _y = y = ofGetHeight()*0.5;//ofRandom(ofGetHeight()/2);
    
    weight =_weight = 0;
    edge = _edge = 4;
    
}

void Shape::update(){
    x += (_x - x) * 0.005;
    y += (_y - y) * 0.01;

    
    size += (_size-size)*0.05;
    weight += (_weight-weight)*0.05;
    
    edge += (_edge-edge)*0.1;
}

void Shape::draw(){

    ofSetColor(255,255);
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    
    if (!master){
        ofBeginShape();
        _z=0;
    }else{
        ofPushMatrix();
        ofTranslate(0,0,z);
    }
  

    float step		= TWO_PI / int(edge);
    float angle		= 0.0;
    float radius	= size;
    
    for(int i = 0; i < edge; i++){
        float rx = x + cos(angle) * radius;
        float ry = y + sin(angle) * radius;
        
        ofVertex(rx, ry);
        angle += step;
    }
    
    ofNextContour(true);
    
    for(int i = 0; i <edge; i++){
        float rx = x + cos(angle) * (radius-weight);
        float ry = y + sin(angle) * (radius-weight);
        
        ofVertex(rx, ry);
        angle += step;
    }
    
    
    if (!master){
        ofEndShape();
    }else{
        ofPopMatrix();
    }
}
