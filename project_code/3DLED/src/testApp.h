#pragma once
#include "ofMain.h"
#include "ledMatrix.h"
#include "ofxUI.h"
#include "ofxSyphon.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
		
		ofVec3f cameraRotation;
		float zoom, zoomTarget;
    
    ledMatrix *matrix[10];
    ofVideoPlayer 		fingerMovie;
    
    bool mState[10];
    int matrixSelect;
    
    //Syphon Stuff
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonServerDirectory dir;
    ofxSyphonClient client;
    ofxSyphonServer server;
    int dirIdx, selectID;
    string serverName, appName;
    
    ofxUICanvas *gui;
    ofxUIDropDownList *ddl;
    
    ofFbo syphonFBO;
    
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofMaterial material;
    ofLight pointLight;

};









