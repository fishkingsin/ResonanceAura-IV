
//
//  Beat.cpp
//  SONY
//
//  Created by Kwankai on 17/3/16.
//
//

#include "Beat.h"

Beat::Beat(){
}


void Beat::set(char _direction, int _num, int _edge, int _radius){
    
    direction= _direction;

    num=_num;
    edge = _edge;
    
    for (int i=0; i<num; i++){
        Shape temp;
    
      //  temp. radius = _radius;
        temp. weight = 0;
        //temp.edge= edge;
        
        shapes.push_back(temp);
    }
    
    init();
}

void Beat::init(){
    
    int originX, originY, originZ;

    switch(direction){
            
        case 'L':
            
            for(int i=0; i<num; i++){
              //  shapes[i].delay= i*10+5;
                shapes[i].x = -ofGetWidth()/2;
                shapes[i].y = ofGetHeight()/2;
                shapes[i].z = -1000-i*100;
    
                shapes[i]._x = ofGetWidth()/4;
                shapes[i]._y = ofGetHeight()/2;
                shapes[i]._z = -500;
            }

        break;
            
        case 'R':
        
            for(int i=0; i<num; i++){
           //     shapes[i].delay= i*10+5;
                shapes[i].x = ofGetWidth()/2*3;
                shapes[i].y = ofGetHeight()/2;
                shapes[i].z = -1000+i*100;
                
                shapes[i]._x = ofGetWidth()/4*3;
                shapes[i]._y = ofGetHeight()/2;
                shapes[i]._z = -500;
                
            }
        break;
            
        case 'F':
            
            for(int i=0; i<num; i++){
           //     shapes[i].delay= i*10+5;
                shapes[i].x = ofGetWidth()/2;
                shapes[i].y = ofGetHeight()/2;
                shapes[i].z = 0+i*100;
                
                shapes[i]._x = ofGetWidth()/2;
                shapes[i]._y = ofGetHeight()/2;
                shapes[i]._z = -500;
                
            }
            
        break;
            
        case 'B':
            
            for(int i=0; i<num; i++){
              //  shapes[i].delay= i*10+5;
                shapes[i].x = ofGetWidth()/2;
                shapes[i].y = ofGetHeight()/2;
                shapes[i].z = -500+i*100;
                
                shapes[i]._x = ofGetWidth()/2;
                shapes[i]._y = ofGetHeight()/2;
                shapes[i]._z = 0;
                
            }
        break;
    }

    
    cout<<"init";
}

void Beat::hit(char _direction){
    
    life= 200;
    
    if(_direction =='L' ||_direction =='R')
    {
        for(int i=0; i<num; i++){
            shapes[i]._x = ofGetWidth()/2;
            shapes[i]._z=0;
          //  shapes[i].speed=0.01;
        }
    }else if (_direction == 'B' || _direction == 'F'){
        
        for(int i=0; i<num; i++){
          //  shapes[i]._rotation=5;
        }
    }
    
}

void Beat::update(){
    for (int i=0; i<shapes.size(); i++){
        shapes[i].update();
        
        if(life<100){
      //      shapes[i]._alpha = 0;
        }else{
    //        shapes[i]._alpha=255;
        }

    }
    life--;
    
    //cout<< shapes[0].alpha <<"\n" ;
    
}

void Beat::draw(){
//    ofNextContour(true);

    for (int i=0; i<shapes.size(); i++){
        shapes[i].draw();
    }
    
//    output.endShape(false);
}

