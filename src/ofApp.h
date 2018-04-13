#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include "Poco/Unicode.h"
#include "Poco/UTF8String.h"
#include "Poco/String.h"

#define PORT 8000

struct button{
    float x;
    float y;
    float r;
    bool on;
    int table, buttonID;
    string address;
    string secAdress;
    float speed=1;
    float speedy=1;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
    
    ofxOscSender sender;
    ofxPanel gui;
    ofParameter<bool>automatic;
    ofParameter<bool>allon;
    ofParameter<bool>freezeY;
    ofParameter<bool>freezeX;
    ofParameter<int>frameRate;
    ofParameter<int>tables;
    ofParameter<int>buttonsPrTable;
    vector<button>buttons;
    int dragging = 0;
    bool b_dragging = false;
    
    string getAdress(int _firstSecond, int _tabel, int _button);
    
    string encode(string input, const string& inputEncoding, const string& outputEncoding) {
        
        Poco::TextEncoding::Ptr ie = Poco::TextEncoding::find(inputEncoding);
        Poco::TextEncoding::Ptr oe = Poco::TextEncoding::find(outputEncoding);
        Poco::TextConverter converter(*ie, *oe);
        string output;
        converter.convert(input, output);
        return output;
    }
};
