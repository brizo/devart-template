#include "testApp.h"
#include "MSACore.h"
#include "MSAFluidSolver.h"

//--------------------------------------------------------------
void testApp::setup() {
    mServer.setName("Motion");

    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(2); // make the points bigger
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    pe.setup();
    bg.loadImage("kaleidoscope-pattern.jpg");
    setupKinect();
    setupFluid();
    setupGUI();
    
    /* Fluid Stuff */
    fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(4.118).setVisc(0.0001).setColorDiffusion(0);
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
	ofEnableAlphaBlending();
}

void testApp::setupGUI() {
#ifdef USE_GUI
	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
    gui.addSlider("colorMult", colorMult, 0, 100);
    gui.addSlider("velocityMult", velocityMult, 0, 100);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
    gui.addToggle("fs.doRGB", fluidSolver.doRGB);
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
	gui.addToggle("drawFluid", drawFluid);
	gui.addToggle("drawParticles", drawParticles);
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    gui.show();
#endif
}

void testApp::setupKinect() {
    m.setMode(OF_PRIMITIVE_POINTS);
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(1);
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    /* Hand Generator */
    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();
    openNIDevice.setMaxNumHands(4);
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    openNIDevice.start();
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(4); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
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
        ofVec2f eventPos = ofVec2f(xpos, ypos);
        ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
        ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
        addToFluid(mouseNorm, mouseVel, true, true);
        pMouse = eventPos;
    }
    pe.update();
    pe.addParticle(false);
    if(resizeFluid)
    {
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		resizeFluid = false;
        setupFluid();
    }
	fluidSolver.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDisableDepthTest();
    ofSetColor(255, 255, 255);
    bg.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    drawFluidTexture();
    ofEnableDepthTest();
	if(drawParticles)
		particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid);
	
    ofPushMatrix();
    //easyCam.begin();
    ofRotateX(15);
    ofRotateY(15);
    pe.draw();
    //easyCam.end();
    ofPopMatrix();
    // draw some info regarding frame counts etc
	ofSetColor(0, 255, 0);
	string msg = "PARTICLES: " + ofToString(pe.getSize()) +  " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
	verdana.drawString(msg, 20, ofGetWindowHeight() - 20);
#ifdef USE_GUI
	gui.draw();
#endif
#ifdef ENABLE_SYPHON
    mServer.publishScreen();
#endif
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
        case ' ': {
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
        }
            break;
        case 'h':
            gui.toggleDraw();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, true, true);
	pMouse = eventPos;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, true, true);
	pMouse = eventPos;
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

void testApp::drawKinectMesh() {
    for (int i = 0; i < m.getNumVertices(); i++) {
        ofPushMatrix();
        ofTranslate(m.getVertex(i) - ofVec3f(-320, -240, 1000));
        ofSetColor(255, 0, 0);
        ofCircle(0, 0, 5);
        ofPopMatrix();
    }
    ofPushMatrix();
    ofTranslate(320, 240, -1000);
    m.draw();
}

void testApp::setupFluid() {
    int texWidth	= fluidSolver.getWidth()  - 2;
    int texHeight	= fluidSolver.getHeight() - 2;
    int texPixCount	= texWidth * texHeight * 4;
    if(fluidPixels) delete []fluidPixels;
    fluidPixels		= new unsigned char[ texPixCount ];
    fluidTexture.allocate( texWidth, texHeight, GL_RGBA );
}

void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

// add force and dye to fluid, and create particles
void testApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);
        const float colorMult = 100;
        int index = fluidSolver.getIndexForPos(pos);
		if(addColor) {
			ofColor drawColor;
			drawColor.setHsb((ofGetFrameNum() % 255), 255, 255);
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			if(drawParticles)
				particleSystem.addParticles(pos * ofVec2f(ofGetWindowSize()), 10);
		}
		if(addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
    }
}

void testApp::drawFluidTexture() {
    //ofEnableAlphaBlending();
    //glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glEnable(GL_BLEND);
    int fw = fluidSolver.getWidth();
    int fh = fluidSolver.getHeight();
    msa::Vec2f vel;
    msa::Color color;
    int index = 0;
    for(int j=1; j < fh-1; j++) {
        for(int i=1; i < fw-1; i++) {
            fluidSolver.getInfoAtCell(i, j, &vel, &color);
            int r = (unsigned char)min(color.r * 255, 255.0f);
            int g = (unsigned char)min(color.g * 255, 255.0f);
            int b = (unsigned char)min(color.b * 255, 255.0f);
            fluidPixels[index++] = r;
            fluidPixels[index++] = g;
            fluidPixels[index++] = b;
            fluidPixels[index++] = max(b, max(r, g));
        }
    }
    int texWidth	= (int)fluidTexture.getWidth();
    int texHeight	= (int)fluidTexture.getHeight();
    fluidTexture.loadData( fluidPixels, texWidth, texHeight, GL_RGBA );
    fluidTexture.draw( 0, 0, ofGetWindowWidth(), ofGetWindowHeight() );
}