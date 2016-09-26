//
//  Drawing.h
//  SharedCanvas
//
//  Created by Brett Renfer on 5/29/12.
//

class Drawing {
public:
    int             _id;
    ofColor         color;
    vector<ofPoint> points;
    ofxLibwebsockets::Connection * conn;
    
    Drawing(){
        conn = new ofxLibwebsockets::Connection(); _id = -1;
    }
    
    void addPoint( ofPoint p ){
        if ( points.size() + 1 > 500 ){
            //points.erase( points.begin() );
        }
        points.push_back( p );
    }
    
    void erase() {
        points.clear();
    }
    
    void eraseLast() {
        while (points.back().x >= 0) {
            points.pop_back();
        }
        
    }
    
    string getColorJSON( bool object=false){
        int r = color.r;
        int g = color.g;
        int b = color.b;
        
        stringstream jsonstring;
        if ( object ){
            jsonstring<<"{\"color\":{\"r\":\""<<r<<"\",\"g\":\""<<g<<"\",\"b\":\""<<b<<"\"}}";
        } else {
            jsonstring<<"\"color\":{\"r\":\""<<r<<"\",\"g\":\""<<g<<"\",\"b\":\""<<b<<"\"}";
        }
        return jsonstring.str();
    }
    string getPointJSON( bool object=false){
        std::mutex mtx;
        int r = color.r;
        int g = color.g;
        int b = color.b;
        
        stringstream jsonstring;
        mtx.lock();
        jsonstring << "\"points\":[" ;
        int i = 0 ;
        for ( auto & p : points){
            ofLogNotice() << p;
            
            if(0 != i){
                
                jsonstring<<",{\"point\":{\"x\":"<<p.x<<",\"y\":"<<p.y << "}}";
                
            }else{
                jsonstring<<"{\"point\":{\"x\":"<<p.x<<",\"y\":"<<p.y << "}}";
            }
            i++;
        }
        jsonstring << "]" ;
        mtx.unlock();
        return jsonstring.str();
    }
    
    string getJSONString( string wrapper){
        
        stringstream jsonstring;
        if ( wrapper != "" ){
            jsonstring << "{\"" + wrapper << "\":";
        }
        jsonstring<<"{" +getColorJSON()+",\"id\":"<< _id<<"}";
        
        if ( wrapper != "" ){
            jsonstring << "}";
        }
        return jsonstring.str();
    }
    string getPointsJSONString( string wrapper){
        
        stringstream jsonstring;
        if ( wrapper != "" ){
            jsonstring << "{\"" + wrapper << "\":";
        }
        jsonstring<<"{" +getColorJSON()<<",\"id\":"<< _id<<"},"<<getPointJSON();
        
        if ( wrapper != "" ){
            jsonstring << "}";
        }
        return jsonstring.str();
    }
};
