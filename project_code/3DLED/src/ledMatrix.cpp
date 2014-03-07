//
//  ledMatrix.cpp
//  3DLED
//
//  Created by Bryan Bui on 2/25/14.
//
//

#include "ledMatrix.h"

ledMatrix::ledMatrix(int xoff, int yoff, int zoff) {
    spacing = 40;
    offset = ofVec3f(xoff, yoff, zoff);
    fbo.allocate(400, 400);
	for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            mColors[i].set(255, 255, 255, 255);
            sphere[i].setScale(0.25);
            sphere[i].setPosition(x * spacing, y * spacing + spacing, 0);
        }
    }
    ofSetSphereResolution(12);
}

void ledMatrix::update() {
    float t = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
}

void ledMatrix::matrixOff() {
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            mColors[i].set(40, 40, 40, 255);
        }
    }
}

void ledMatrix::matrixOn() {
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            mColors[i].set(255, 255, 255, 255);
        }
    }
}

void ledMatrix::loadImage(ofFbo image) {
    imagePix.clear();
    image.readToPixels(imagePix);
    
    imgW = image.getWidth();
    imgH = image.getHeight();
    
    int wOff = imgW / LEDx;
    int hOff = imgH / LEDy;
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            mColors[i].set(imagePix.getColor(x * wOff, y * hOff));
        }
    }
}

void ledMatrix::draw() {
    // Draw the floor
/*
	ofSetColor(40, 40, 40);
	glBegin(GL_QUADS);
	glVertex3f(-25, -15, 15);
	glVertex3f(-25, -15, -245);
	glVertex3f(245, -15, -245);
	glVertex3f(245, -15, 15);
	glEnd();
*/
    ofEnableAlphaBlending();
    ofPushMatrix();
	ofTranslate(offset);
    ofEnableDepthTest();
    
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            ofFill();
            ofSetColor(mColors[i]);
            sphere[i].draw();
        }
    }
    
    ofSetColor(255);
    for (int i = 0; i < LEDx; i++) {
        ofLine(i * spacing, 0, i * spacing, LEDy * spacing);
    }
	ofPopMatrix();
    ofDisableAlphaBlending();
}

