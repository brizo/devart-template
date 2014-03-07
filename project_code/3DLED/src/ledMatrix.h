//
//  ledMatrix.h
//  3DLED
//
//  Created by Bryan Bui on 2/25/14.
//
//

#ifndef ___DLED__ledMatrix__
#define ___DLED__ledMatrix__

#include <iostream>
#include "ofMain.h"
#include "ofVbo.h"

//#define NUM_BILLBOARDS 2000
#define LEDx    10
#define LEDy    10
#define LEDz    10

static int NUM_BILLBOARDS = LEDx * LEDy;

class ledMatrix {
public:
    ledMatrix(int, int, int);
    ~ledMatrix();
    
    void update();
    void draw();
    
    //Matrix Control Functions
    void matrixOff();
    void matrixOn();
    void loadImage(ofFbo image);
    
private:
    // billboard particles
    float billboardSizeTarget[100];
    ofSpherePrimitive sphere[100];
    ofColor mColors[100];
    
    
    ofShader billboardShader;
    ofImage texture;
    
    //ofVboMesh billboards;
    int spacing;
    ofVec3f offset;
    
    //Our Image size.  It doesn't matter what the size is, we'll scale it down and then
    //show only those pixels in the LEDs
    ofPixels imagePix;
    ofTexture tex;
    ofFbo fbo;
    int imgW, imgH;
};


#endif /* defined(___DLED__ledMatrix__) */
