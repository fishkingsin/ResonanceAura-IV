#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
     ofSetLogLevel(OF_LOG_VERBOSE);
    ofxXmlSettings settings;

    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
    options.bUseSSL = false;
    options.reconnect = true;
    options.reconnectInterval = 5000;
    

    if( settings.loadFile("settings.xml") ){
        ofLogVerbose() << "loaded settings.xml";
        int numPtTags = settings.getNumTags("SETTINGS:SERVER");

        if(numPtTags > 0){
            string server = settings.getValue("SETTINGS:SERVER","localhost");
            ofLogVerbose() << "loaded settings server "<<server;
            options.host = server;
            options.port = 9092;
        }


    }else{
        options.host = "localhost";
        options.port = 9092;
        ofLogVerbose() << "client.connect(localhost, 9093)";
    }

    client.connect(options);
    // connect to either the "server blob" or "server blob video" example!
    
    bConnected = false;
    
    client.addListener(this);

    ofBackground(0);
    ofSetFrameRate(60);
    for ( int i = 0 ; i < 256 ; i++){
        GAMMA[i] = int(pow(float(i) / 255.0, 2.7) * 255.0  * 0.1 + 0.5) ;
    }

    fbo.allocate(256,144,GL_RGB);
    largeFbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);
#ifdef TARGET_OSX
#else
    apa102.setup(fbo.getHeight());
#endif

    pixels.allocate(fbo.getWidth(),fbo.getHeight(),OF_IMAGE_COLOR);
    length = 4+(fbo.getHeight()*4)+4;
    buf = (u_int8_t*)malloc(length);
    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;

    buf[length-4] = 0xFF;
    buf[length-3] = 0xFF;
    buf[length-2] = 0xFF;
    buf[length-1] = 0xFF;
    
    startThread();
}
void ofApp::exit(){

    stopThread();
    for(int y = 0 ; y < fbo.getHeight() ; y++){

        int index = (y*4)+4;
        buf[index] = 0b11100000 | (0b00011111 & 0);
        buf[index+1] = 0;
        buf[index+2] = 0;
        buf[index+3] = 0;
    }
#ifdef TARGET_OSX
#else
    wiringPiSPIDataRW(0, buf, length);
#endif
}

void ofApp::threadedFunction(){
    while(isThreadRunning())
    {
            // Attempt to lock the mutex.  If blocking is turned on,
        if(lock())
        {

                // Unlock the mutex.  This is only
                // called if lock() returned true above.
            //scan each line x


            for(int x = 0 ; x < fbo.getWidth() ; x++){

                //scan each pixels of the line

                for(int y = 0 ; y <fbo.getHeight() ; y++){
                    // apa102.setFrameData(y,frames[x][y]);
                    ofColor c = pixels.getColor(x,y);
                    int index = (y*4)+4;
                    buf[index] = 0b11100000 | (0b00011111 & c.a);
                    buf[index+1] = GAMMA[c.b];
                    buf[index+2] = GAMMA[c.g];
                    buf[index+3] = GAMMA[c.r];
                }
#ifdef TARGET_OSX
#else
                wiringPiSPIDataRW(0, buf, length);
#endif
            }

            unlock();

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
//--------------------------------------------------------------
void ofApp::update(){

    //draw incoming drawing
    largeFbo.begin();
    map<int, Drawing*>::iterator it = drawings.begin();
    
    ofNoFill();
    
    for (it; it != drawings.end(); ++it){
        Drawing * d = it->second;
        ofSetColor( d->color );
        ofBeginShape();
        ofSetLineWidth(10);
        for ( int i=0; i<(int)d->points.size(); i++){
            ofVertex( d->points[i].x,d->points[i].y);
        }
        ofEndShape(false);
    }
    ofFill();
    largeFbo.end();
    //draw incoming drawing

    //draw small canvas drawing
    fbo.begin();
    largeFbo.draw(0,0,fbo.getWidth(),fbo.getHeight());
    fbo.end();
    //draw small canvas drawing
    fbo.readToPixels(pixels);

//     for(int x = 0 ; x < fbo.getWidth() ; x++){

//                 //scan each pixels of the line

//         for(int y = 0 ; y <fbo.getHeight() ; y++){
//                     // apa102.setFrameData(y,frames[x][y]);
//             ofColor c = pixels.getColor(x,y);
//             int index = (y*4)+4;
//             buf[index] = 0b11100000 | (0b00011111 & c.a);
//             buf[index+1] = GAMMA[c.b];
//             buf[index+2] = GAMMA[c.g];
//             buf[index+3] = GAMMA[c.r];
//         }
// #ifdef TARGET_OSX
// #else
//         wiringPiSPIDataRW(0, buf, length);
// #endif
        
    // }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( bConnected ){
        ofDrawBitmapString("WebSocket client connected" , 20, 20);
    } else {
        ofDrawBitmapString("WebSocket not connected", 20,20);
    }
    largeFbo.draw(0,0);
    fbo.draw(0,0);
//    map<int, Drawing*>::iterator it = drawings.begin();
//    
//    // ofNoFill();
//    
//    for (it; it != drawings.end(); ++it){
//        Drawing * d = it->second;
//        // ofSetColor( d->color );
//        // ofBeginShape();
//        for ( int i=0; i<(int)d->points.size(); i++){
//            // ofVertex( d->points[i].x,d->points[i].y);
//            ofDrawRectangle(d->points[i].x,d->points[i].y,10,10);
//        }
//        // ofEndShape(false);
//    }
    // ofFill();
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    ofSetLogLevel(OF_LOG_NOTICE);
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    bConnected = true;
    cout<<"new connection open"<<endl;
    cout<<args.conn.getClientIP()<< endl;
    
    // send drawing so far
    map<int, Drawing*>::iterator it = drawings.begin();
    for (it; it != drawings.end(); ++it){
        Drawing * drawing = it->second;
        for ( int i=0; i<(int)drawing->points.size(); i++){
            string x = ofToString(drawing->points[i].x);
            string y = ofToString(drawing->points[i].y);
            client.send( "{\"id\":"+ ofToString(drawing->_id) + ",\"point\":{\"x\":\""+ x+"\",\"y\":\""+y+"\"}," + drawing->getColorJSON() +"}");
        }
    }
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    bConnected = false;
    cout<<"on close"<<endl;
    // remove from color map
    
    map<int, Drawing*>::iterator it = drawings.begin();
    
    for (it; it != drawings.end(); ++it){
        Drawing * d = it->second;
        if ( *d->conn == args.conn ){
            d->conn == NULL;
            client.send("{\"erase\":\"" + ofToString( it->second->_id ) + "\"}" );
            drawings.erase( it );
        }
    }
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
  try{
    cout<<"got message "<<args.message<<endl;
    // trace out string messages or JSON messages!
    if ( !args.json.isNull() ){
        if (!args.json["setup"].isNull()){
            Drawing * d = new Drawing();
            d->_id = args.json["setup"]["id"].asInt();
            // for some reason these come across as strings via JSON.stringify!
            int r = ofToInt(args.json["setup"]["color"]["r"].asString());
            int g = ofToInt(args.json["setup"]["color"]["g"].asString());
            int b = ofToInt(args.json["setup"]["color"]["b"].asString());
            d->color.set(r, g, b);
            drawings.insert( make_pair( d->_id, d ));
            id = d->_id;
            color.set(r, g, b);
            cout << "setup with id:" << id << endl;
        }
        else if (args.json["id"].asInt() != id){
          cout << "received point" << endl;
          ofPoint point = ofPoint( args.json["point"]["x"].asFloat(), args.json["point"]["y"].asFloat() );

            // for some reason these come across as strings via JSON.stringify!
          int r = ofToInt(args.json["color"]["r"].asString());
          int g = ofToInt(args.json["color"]["g"].asString());
          int b = ofToInt(args.json["color"]["b"].asString());
          ofColor color = ofColor( r, g, b );

          int _id = args.json["id"].asInt();

          map<int, Drawing*>::const_iterator it = drawings.find(_id);
          Drawing * d;
          if (it!=drawings.end()){
            d = it->second;
        }
        else {
          d = new Drawing();
          d->_id = _id;
              // for some reason these come across as strings via JSON.stringify!
          int r = ofToInt(args.json["color"]["r"].asString());
          int g = ofToInt(args.json["color"]["g"].asString());
          int b = ofToInt(args.json["color"]["b"].asString());
          d->color.set(r, g, b);
          drawings.insert( make_pair( d->_id, d ));
          cout << "new drawing with id:" << _id << endl;
      }

      d->addPoint(point);
  }
} else {
}
}
catch(exception& e){
    ofLogError() << e.what();
}
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint p(x,y);
    
    map<int, Drawing*>::iterator it = drawings.find(id);
    if (it != drawings.end()){
      Drawing * d = it->second;
      d->addPoint(p);
      client.send( "{\"id\":"+ ofToString(id) + ",\"point\":{\"x\":\""+ ofToString(x)+"\",\"y\":\""+ofToString(y)+"\"}," + d->getColorJSON() +"}");
  }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    ofPoint p(x,y);
    map<int, Drawing*>::iterator it = drawings.find(id);
    if ( it == drawings.end() ) return;
    Drawing * d = it->second;
    d->addPoint(p);
    client.send( "{\"id\":"+ ofToString(id) + ",\"point\":{\"x\":\""+ ofToString(x)+"\",\"y\":\""+ofToString(y)+"\"}," + d->getColorJSON() +"}");
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
