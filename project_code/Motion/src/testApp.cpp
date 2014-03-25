#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    //ofSetVerticalSync(false);
    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(3); // make the points bigger
    m.setMode(OF_PRIMITIVE_POINTS);
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(1);
    openNIDevice.setRegister(true);
    //openNIDevice.setMirror(true);
    /* Hand Generator */
    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();
    openNIDevice.setMaxNumHands(4);
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true); 
        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true, 
        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false, 
        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(10); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    pe.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    pe.addParticle(true);
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        float xpos = ofMap(hand.getPosition().x, 0, 640, 0, ofGetWindowWidth());
        float ypos = ofMap(hand.getPosition().y, 0, 480, 0, ofGetWindowHeight());
        pe.addEmitter(ofPoint(xpos, ypos));
        pe.updatePosition(ofPoint(xpos, ypos));
        // set depthThresholds based on handPosition
        //ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        //ofPoint leftBottomNearWorld = handWorldPosition - 100; // ofPoint has operator overloading so it'll subtract/add 50 to x, y, z
        //ofPoint rightTopFarWorld = handWorldPosition + 100;
        //ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        //depthThreshold.setROI(roi);
    }
    pe.update();
    pe.addParticle(false);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();
    //if(numUsers > 0)
    // iterate through users
    /*
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        user.drawMask();
        ofPushMatrix();
        ofTranslate(320, 240, -1000);
        user.drawPointCloud();
        ofPopMatrix();
    }
    */
    for (int i = 0; i < m.getNumVertices(); i++) {
        ofPushMatrix();
        ofTranslate(m.getVertex(i) - ofVec3f(-320, -240, 1000));
        ofSetColor(255, 0, 0);
        ofCircle(0, 0, 5);
        ofPopMatrix();
    }
    ofPushMatrix();
    ofTranslate(320, 240, -1000);
    
        //m.draw();
    ofPopMatrix();
    ofDisableBlendMode();
    ofPopMatrix();
    /*
    if (m.getNumVertices() > 0) {
        cout << "Vertices: " << m.getNumVertices() << endl;
    } */
    ofPushMatrix();
    //ofTranslate(320, 240, -1000);
    pe.draw();
    ofPopMatrix();
    // draw some info regarding frame counts etc
	ofSetColor(0, 255, 0);
	string msg = "PARTICLES: " + ofToString(pe.getSize()) +  " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
	verdana.drawString(msg, 20, 480 - 20);
}

//--------------------------------------------------------------
void testApp::handEvent(ofxOpenNIHandEvent & event){
    // show hand event messages in the console
    ofLogNotice() << getHandStatusAsString(event.handStatus) << "for hand" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case ' ':
            int numUsers = openNIDevice.getNumTrackedUsers();
            for (int nID = 0; nID < numUsers; nID++){
                ofxOpenNIUser & user = openNIDevice.getTrackedUser(nID);
                user.drawMask();
                m.clear();
                m.append(user.getPointCloud()) ;
                pe.setMesh(m);
                pe.clear();
                cout << "Mesh Vertices: " << m.getNumVertices() << endl;
            }
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}