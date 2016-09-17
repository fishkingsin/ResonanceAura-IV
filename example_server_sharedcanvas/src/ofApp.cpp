#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    //bConnected = server.setup( 9093 );
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
    bConnected = server.setup( options );
    
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    ofBackground(0);
    ofSetFrameRate(60);
    canvasID = 0;
    
    // OF drawing
    
    Drawing * d = new Drawing();
    d->_id = canvasID++;
    d->color.set(255,255,255);
    
    drawings.insert( make_pair( d->_id, d ));
    panel.setup();
    delay.set("delay", 0,0,100);
    panel.add(delay);
    delay.addListener(this, &ofApp::onParaChanged);
}
void ofApp::onParaChanged(int &i){
    server.send("{\"delay\":\"" + ofToString( i ) + "\"}" );
}
//--------------------------------------------------------------
void ofApp::update(){
    if ( toDelete.size() > 0 ){
        for ( auto & i : toDelete ){
            drawings.erase(i->_id);
            
            server.send("{\"erase\":\"" + ofToString( i->_id ) + "\"}" );
        }
        toDelete.clear();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( bConnected ){
        ofDrawBitmapString("WebSocket server setup at "+ofToString( server.getPort() ) + ( server.usingSSL() ? " with SSL" : " without SSL"), 20, 20);
        
        ofSetColor(150);
        ofDrawBitmapString("Click anywhere to open up client example", 20, 40);
    } else {
        ofDrawBitmapString("WebSocket setup failed :(", 20,20);
    }
    
    map<int, Drawing*>::iterator it = drawings.begin();
    
    ofNoFill();
    
    for (it; it != drawings.end(); ++it){
        Drawing * d = it->second;
        ofSetColor( d->color );
        ofBeginShape();
        for ( int i=0; i<d->points.size(); i++){
            ofVertex( d->points[i].x,d->points[i].y);
        }
        ofEndShape(false);
    }
    ofFill();
    
    panel.draw();
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    cout<<args.conn.getClientIP()<< endl;
    
    Drawing * d = new Drawing();
    d->_id = canvasID++;
    d->color.set(ofRandom(255),ofRandom(255),ofRandom(255));;
    d->conn = &( args.conn );
    
    drawings.insert( make_pair( d->_id, d ));
    
    // send "setup"
    args.conn.send( d->getJSONString("setup") );
    
    // send drawing so far
    map<int, Drawing*>::iterator it = drawings.begin();
    for (it; it != drawings.end(); ++it){
        Drawing * drawing = it->second;
        if ( d != drawing ){
            for ( int i=0; i<drawing->points.size(); i++){
                string x = ofToString(drawing->points[i].x);
                string y = ofToString(drawing->points[i].y);
                server.send( "{\"id\":"+ ofToString(drawing->_id) + ",\"point\":{\"x\":\""+ x+"\",\"y\":\""+y+"\"}," + drawing->getColorJSON() +"}");
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    // remove from color map
    for ( auto & it : drawings){
        Drawing * d = it.second;
        if ( *d->conn == args.conn ){
            toDelete.push_back(it.second);
            d->conn == NULL;
        }
    }
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    try{
        // trace out string messages or JSON messages!
        if ( !args.json.isNull() ){
            ofPoint point = ofPoint( args.json["point"]["x"].asFloat(), args.json["point"]["y"].asFloat() );
            
            // for some reason these come across as strings via JSON.stringify!
            int r = ofToInt(args.json["color"]["r"].asString());
            int g = ofToInt(args.json["color"]["g"].asString());
            int b = ofToInt(args.json["color"]["b"].asString());
            ofColor color = ofColor( r, g, b );
            
            int _id = ofToInt(args.json["id"].asString());
            
            map<int, Drawing*>::const_iterator it = drawings.find(_id);
            Drawing * d = it->second;
            if(d!=NULL){
                d->addPoint(point);
            }
        } else {
        }
        // send all that drawing back to everybody except this one
        vector<ofxLibwebsockets::Connection *> connections = server.getConnections();
        for ( int i=0; i<connections.size(); i++){
            if ( (*connections[i]) != args.conn ){
                connections[i]->send( args.message );
            }
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
    switch(key){
        case OF_KEY_BACKSPACE:
        {
            for ( auto & it : drawings){
                Drawing * d = it.second;
                if(d->points.size()>0){
                    string jsonString = d->getPointsJSONString("setup");
                    ofLogNotice("jsonString") << jsonString;
                    Json::Value root;
                    Json::Reader reader;
                    bool parsingSuccessful = reader.parse( jsonString.c_str(), root );     //parse process
                    if ( !parsingSuccessful )
                    {
                        std::cout  << "Failed to parse"
                        << reader.getFormattedErrorMessages();
                        return 0;
                    }
                    ofBuffer buffer;
                    buffer.append(jsonString);
                    ostringstream filename;
                    filename << ofGetTimestampString() << ".json";
                    ofFile file;
                    file.open(filename.str(),ofFile::WriteOnly);
                    file<<jsonString;
                    std::cout << root["setup"]["id"].asInt() << std::endl;
//                    toDelete.push_back(it.second);
                    
                    
                }
                
            }
        }
            break;
        case 'l':{
            jsonFiles.clear();
            ofDirectory dir;
            dir.allowExt("json");
            int n = dir.listDir("");
            for (auto & f : dir.getFiles()){
            
                ofLogNotice ("json ") << f.getFileName();
                jsonFiles.push_back(f);
            }
            Json::Value root;
            Json::Reader reader;
            int index = ofRandom(n);
            ofFile openFile(jsonFiles[index].getAbsolutePath());
            ofLogNotice() << "openFile.readToBuffer() " << openFile.readToBuffer();
            bool parsingSuccessful = reader.parse( openFile.readToBuffer() , root );     //parse process
            if(parsingSuccessful){
                
                ofLogNotice("parsingSuccessful setup") << root["setup"].toStyledString();
                vector<ofxLibwebsockets::Connection *> connections = server.getConnections();
                ofLogNotice("parsingSuccessful points") << root["points"].toStyledString();
                server.send("{\"erase\":\"" + ofToString( -1 ) + "\"}" );
                for ( int i=0; i<connections.size(); i++){
                    
                    Json::FastWriter fastWriter;
                    std::string setupstring = fastWriter.write(root["setup"]);
                    int _id = root["setup"]["id"].asInt();
                     connections[i]->send(setupstring);

                    
                    map<int, Drawing*>::iterator it = drawings.find(0);
                    Drawing * d = it->second;
                    d->points.clear();
                    d->_id = -1;
                    d->color.set(root["setup"]["color"]["r"].asInt(),
                                 root["setup"]["color"]["g"].asInt(),
                                 root["setup"]["color"]["b"].asInt());
                    
                    
                    Json::Value points = root["points"];
                    int n = points.size();
                    //testing
                    for (int j = 0 ; j < n ; j++){
                        ostringstream message;
                        message << "{\"id\":" << ofToString(_id) << ",\"point\":" << fastWriter.write(points[j]["point"]) << "," << "\"color\":"<< fastWriter.write(root["setup"]["color"]) << "}";
                        ofLogNotice("message") << message.str();
                         connections[i]->send( message.str() );
                        d->addPoint(ofPoint(points[j]["point"]["x"].asInt() ,points[j]["point"]["y"].asInt()));
                        ofSleepMillis(50);
                    }
                    //testing
                    
                }
                auto it = std::find(jsonFiles.begin(), jsonFiles.end(), openFile);
                if(it != jsonFiles.end())
                    jsonFiles.erase(it);
                break;
                
            }else{
                
            }
            
        }
            break;
        case ' ':
            string url = "http";
            if ( server.usingSSL() ){
                url += "s";
            }
            url += "://localhost:" + ofToString( server.getPort() );
            ofLaunchBrowser(url);
            break;
            
    }
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
    
        map<int, Drawing*>::iterator it = drawings.find(0);
        Drawing * d = it->second;
        d->addPoint(p);
        server.send( "{\"id\":-1,\"point\":{\"x\":\""+ ofToString(x)+"\",\"y\":\""+ofToString(y)+"\"}," + d->getColorJSON() +"}");
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
        ofPoint p(x,y);
    
        map<int, Drawing*>::iterator it = drawings.find(0);
        Drawing * d = it->second;
        d->addPoint(p);
        server.send( "{\"id\":-1,\"point\":{\"x\":\""+ ofToString(x)+"\",\"y\":\""+ofToString(y)+"\"}," + d->getColorJSON() +"}");
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
