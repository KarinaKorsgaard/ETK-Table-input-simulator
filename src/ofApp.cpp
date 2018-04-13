#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup();
    gui.add(automatic.set("auto",true));
    gui.add(allon.set("allon",true));
    gui.add(freezeY.set("freezeY",true));
    gui.add(freezeX.set("freezeX",true));

    gui.add(frameRate.set("frameRate",0,0,60));
    gui.loadFromFile("settings.xml");
    
    for(int i = 0 ; i<12;i++){
        for(int j = 0 ; j<6;j++){
            
            button b = *new button;
            b.address = getAdress(0,i,j);
            b.secAdress = getAdress(1,i,j);
            b.table = i;
            b.buttonID = j;
            b.x = ofRandom(1);
            b.y = ofRandom(1);
            b.r = ofRandom(2*PI);
            b.on = false;
            buttons.push_back(b);
        }
    }
    automatic = false;
    sender.setup("localhost", PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetFrameRate(frameRate);
    double dt = ofGetLastFrameTime() / 3.0f;
    
    for(auto b : buttons){
        ofxOscMessage m;
        if(b.on){
            m.setAddress(b.address);
            m.addFloatArg(b.x*127.0f);
            m.addFloatArg(b.y*127.0f);
            m.addFloatArg(b.r);
            sender.sendMessage(m);
            m.clear();
        }
        m.setAddress(b.secAdress);
        if(b.on)m.addInt32Arg(1);
        else m.addInt32Arg(0);
        sender.sendMessage(m);
    }
    
    if(automatic){
        for(int i = 0; i<buttons.size();i++){
            button * b = &buttons[i];
            b->x += ofRandom(dt) * b->speed ; if(b->x > 1 || b->x<0)b->speed *=-1;
            b->y += ofRandom(dt) * b->speedy; if(b->y > 1 || b->y<0)b->speedy*=-1;
            if(freezeY && i<buttons.size()/2)b->y=0;
            if(freezeX && i<buttons.size()/2)b->x=0.5;
            
            b->x = CLAMP(b->x, 0, 1);
            b->y = CLAMP(b->y, 0, 1);
            
            b->r += dt; if(b->r > 2*PI)b->r = 0;
            if(ofRandom(1)>0.999)b->on = !b->on;
            if(allon)b->on = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    for(int i = 0; i<buttons.size();i++){
        button b = buttons[i];
        ofPushMatrix();
        ofTranslate(b.x * ofGetWidth(), b.y * ofGetHeight());
        ofRotateZ(ofRadToDeg(b.r));
        if(b.on)ofSetColor(ofColor::royalBlue);
        else ofSetColor(ofColor::orangeRed);
        ofDrawCircle(0, 0, 15);
        ofSetColor(ofColor::white);
        ofDrawLine(0, 0, 15, 0);
        ofDrawBitmapString(ofToString(b.table)+","+ofToString(b.buttonID), 10, 10);
        ofPopMatrix();
    }
    gui.draw();
    ofDrawBitmapString("right click and drag to change button rotation", 20, gui.getHeight()+40);
    ofDrawBitmapString("left click and drag to drag button", 20, gui.getHeight()+ 60);
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
    if(dragging!=-1 && button == 0){
        buttons[dragging].x = float(x)/float(ofGetWidth());
        buttons[dragging].y = float(y)/float(ofGetHeight());
        
    }
    if(dragging!=-1 && button == 0 ){
        buttons[dragging].r +=0.1; if(buttons[dragging].r>2*PI)buttons[dragging].r = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(!b_dragging){
        for(int i = 0; i<buttons.size();i++){
            if(ofDistSquared(x,y, buttons[i].x*ofGetWidth(), buttons[i].y*ofGetHeight())<30){
                dragging = i;
                buttons[dragging].on=!buttons[dragging].on;
                b_dragging = true;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    b_dragging= false;
    dragging = -1;
}



//--------------------------------------------------------------
string ofApp:: getAdress(int _firstSecond, int _tabel, int _button){
    ofFile file;
    file.open(ofToDataPath("oscAdresses.txt"));
    ofBuffer b = file.readToBuffer();
    string result = "";
    int indx = 0;
    
    
    for (auto line : b.getLines()) {
        string str = line;
#ifdef TARGET_WIN32
        str = encode(str, "UTF-8", "Windows-1252");
#endif
        if(indx == _tabel){
            result = str;
            if(_firstSecond == 1)
                result.append("/switch");
        }
        if(indx == 12+_button && _firstSecond == 0)
            result.append(str);
        if(indx == 12+6+_button && _firstSecond == 1)
            result.append(str);
        indx++;
    }
    ofLog() << result <<endl;
    return result;
}
