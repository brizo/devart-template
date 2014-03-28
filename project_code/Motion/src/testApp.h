#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ParticleEmitter.h"

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxSyphon.h"

// comment this line out if you don't wanna use the GUI
// you will need ofxSimpleGuiToo, ofxMSAInteractiveObject & ofxXmlSettings
// if you don't use the GUI, you won't be able to see the fluid parameters
#define USE_GUI
#define ENABLE_SYPHON

#ifdef USE_GUI
#include "ofxSimpleGuiToo.h"
#endif

#define MAX_DEVICES 2


class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

private:
    void setupGUI();
    void setupKinect();
    void handEvent(ofxOpenNIHandEvent & event);
    void drawKinectMesh();
	ofxOpenNI openNIDevice;
    ofTrueTypeFont verdana;
    ParticleEmitter pe;
    ofMesh m;
    ofImage bg;
    bool drawMesh;
    
    /* Fluid Stuff */
    void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
    void drawFluidTexture();
    void setupFluid();
    ofTexture fluidTexture;
    unsigned char*	fluidPixels;

    float                   colorMult;
    float                   velocityMult;
	int                     fluidCellsX;
	bool                    resizeFluid;
	bool                    drawFluid;
	bool                    drawParticles;
	msa::fluid::Solver      fluidSolver;
	ParticleSystem          particleSystem;
	ofVec2f                 pMouse;
    
    ofxSyphonServer mServer;
    // used for viewing the point cloud
	ofEasyCam easyCam;
};

#endif
