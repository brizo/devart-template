#include "testApp.h"

int matrixNum = 10;

//--------------------------------------------------------------
void testApp::setup() {
    ofSetWindowTitle("DevArt");
	ofBackground(255, 255, 255);
    
    //Syphon Setup
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320;
    gui = new ofxUICanvas();
    
    gui->addLabel("Syphon Clients", OFX_UI_FONT_LARGE);
    vector<string> names;
    names.push_back("No Clients");
    gui->autoSizeToFitWidgets();
    ddl = gui->addDropDownList("Syphon Clients", names);
    ddl->setLabelText("No Clients");
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(true);
    ddl->setShowCurrentSelected(true);
    //gui->setTheme(OFX_UI_THEME_BARBIE);
    gui->autoSizeToFitWidgets();
    gui->setDrawWidgetPadding(true);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    //setup our directory
	dir.setup();
    //setup our client
    client.setup();
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &testApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &testApp::serverRetired);
    
    dirIdx = -1;
    selectID = -1;
    serverName = "";
    appName = "";
	
	cameraRotation.set(0);
	zoom = -500;
	zoomTarget = 100;
	for (int i = 0; i < matrixNum; i++) {
        matrix[i] = new ledMatrix(0, 0, i * -40);
    }

    matrixSelect = 0;
    for (int i = 0; i < matrixNum; i++) {
        mState[i] = false;
    }
    for (int i = 0; i < matrixNum; i++) {
        matrix[i]->matrixOff();
    }
    
    syphonFBO.allocate(400, 400);
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
    // the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
    

}

//--------------------------------------------------------------
void testApp::update() {
    vector<ofxUILabelToggle *> toggles = ddl->getToggles();
    if(toggles.size() > 1) {
        ddl->removeToggle("No Clients");
    }
    if(dir.size() == 0 && toggles.size() == 0) {
        ddl->addToggle("No Clients");
        ddl->setLabelText("No Clients");
    }
    
    int t = ofGetFrameNum();
    /*
        for (int i = 0; i < matrixNum; i++) {
            matrix[i]->matrixOff();
        }
        matrix[matrixSelect]->matrixOn();
     */
    for (int i = 0; i < matrixNum; i++) {
        if(dir.serverExists(serverName, appName)) {
            syphonFBO.begin();
            ofClear(0,0,0);    // refreshes fbo, removes artifacts
            ofSetColor(255, 255, 255);
            client.draw(0, 0, 400, 400);
            syphonFBO.end();
            matrix[i]->loadImage(syphonFBO);
        } else {
            matrix[i]->matrixOn();
        }
    }
    if (t % 15 == 0) {
        //matrixSelect++;
        if(matrixSelect >= matrixNum) {
            matrixSelect = 0;
        }
    }

	// move the camera around
	float mx = (float)mouseX/(float)ofGetWidth();
	float my = (float)mouseY/(float)ofGetHeight();
	ofVec3f des(mx * 360.0, my * 360.0, 0);
	cameraRotation += (des-cameraRotation) * 0.03;
	zoom += (zoomTarget - zoom) * 0.03;
   
    for (int i = 0; i < matrixNum; i++) {
        matrix[i]->update();
    }
}

//--------------------------------------------------------------
void testApp::draw() {
	//ofBackgroundGradient(ofColor(0), ofColor(230, 240, 255));
	ofBackground(0, 0, 0);
   
    string info = ofToString(ofGetFrameRate(), 2)+"\n";
	info += "Particle Count: "+ofToString(NUM_BILLBOARDS);
	ofDrawBitmapStringHighlight(info, 30, 30);
	ofEnableDepthTest();
	//ofSetColor(0);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2 - 200, ofGetHeight()/2 - 200, zoom);
    ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.y, 0, 1, 0);
	//ofRotate(cameraRotation.y, 0, 0, 1);

    ofPushMatrix();
    ofTranslate(-14, 10, -410);
    if(dir.serverExists(serverName, appName)) {
        syphonFBO.draw(0, 0, 400, 400);
    }
    ofPopMatrix();
    ofEnableLighting();
    pointLight.enable();
    material.begin();

    for (int i = 0; i < matrixNum; i++) {
        matrix[i]->draw();
    }
    
    material.end();
    ofDisableLighting();
    
	ofPopMatrix();
    
    server.publishScreen();
}

//these are our directory's callbacks
void testApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        string thisserverName = dir.serverName;
        string thisappName = dir.appName;
        
        if(thisserverName == ""){
            thisserverName = "null";
        }
        if(thisappName == ""){
            thisappName = "null";
        }
        ddl->addToggle(thisappName + ":" + thisserverName);
        dirIdx++;
    }
    
}

void testApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    //    dirIdx = 0;
}

void testApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        string thisserverName = dir.serverName;
        string thisappName = dir.appName;
        
        if(serverName == thisserverName) {
            serverName = "";
        }
        if(appName == thisappName) {
            appName = "";
        }
        
        if(thisserverName == ""){
            thisserverName = "null";
        }
        if(thisappName == ""){
            thisappName = "null";
        }
        ddl->removeToggle(thisappName + ":" + thisserverName);
        dirIdx--;
    }
    //
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    int widgetID = e.widget->getID();
    if(widgetID == ddl->getID())
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        vector<ofxSyphonServerDescription> serverList = dir.getServerList();
        if(selected.size() > 0) {
            for(int i = 0; i < serverList.size(); i++) {
                string thisserverName = serverList[i].serverName;
                string thisappName = serverList[i].appName;
                
                if(thisserverName == ""){
                    thisserverName = "null";
                }
                if(thisappName == ""){
                    thisappName = "null";
                }
                string name = thisappName + ":" + thisserverName;
                cout << name << endl;
                if(name == selected[0]->getName()) {
                    client.set(serverList[i]);
                    serverName = thisserverName;
                    appName = thisappName;
                    if(serverName == "null") {
                        serverName = "";
                    }
                    if(appName == "null") {
                        appName = "";
                    }
                }
            }
            ofSetWindowTitle(serverName + ":" + appName);
            cout << "Setting Server: " << appName  + ":" + serverName << endl;
        }
    }
}


void testApp::exit()
{
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	if(key == OF_KEY_UP) zoomTarget +=10;
	if(key == OF_KEY_DOWN) zoomTarget -=10;
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
