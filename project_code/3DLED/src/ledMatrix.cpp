//
//  ledMatrix.cpp
//  3DLED
//
//  Created by Bryan Bui on 2/25/14.
//
//

#include "ledMatrix.h"

ledMatrix::ledMatrix(int xoff, int yoff, int zoff) {
    /*
    billboards.getVertices().resize(NUM_BILLBOARDS);
	billboards.getColors().resize(NUM_BILLBOARDS);
	billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
     */
    spacing = 40;
    offset = ofVec3f(xoff, yoff, zoff);
    fbo.allocate(400, 400);
        
    // ------------------------- billboard particles
    
	for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
                int i = (y * LEDx) + x;
            mColors[i].set(255, 255, 255, 255);
            sphere[i].setScale(0.25);
                sphere[i].setPosition(x * spacing, y * spacing + spacing, 0);
            /*
            billboards.getVertices()[i].set(x * spacing,
                                                y * spacing + spacing,
                                                0);
                
                billboards.getColors()[i].set(255, 255, 255);
                billboardSizeTarget[i] = 2;
                */
                
        }
    }
	
	/*
	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
	//billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader
	// this is used to change the
	// size of the particle
	if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard");
	}
	*/
	// we need to disable ARB textures in order to use normalized texcoords
	//ofDisableArbTex();
    //	texture.loadImage("/Users/brizo/Desktop/images/processing/IMG_5993.JPG");
	//texture.loadImage("dot.png");
    //texture.loadImage("circle.png");
    //ofEnableAlphaBlending();
    ofSetSphereResolution(12);
}

void ledMatrix::update() {
    float t = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
	
	
    for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// noise
		/*
        ofVec3f vec(ofSignedNoise(t, billboards.getVertex(i).y/div, billboards.getVertex(i).z/div),
                    ofSignedNoise(billboards.getVertex(i).x/div, t, billboards.getVertex(i).z/div),
                    ofSignedNoise(billboards.getVertex(i).x/div, billboards.getVertex(i).y/div, t));
		*/
        //		vec *= 10 * ofGetLastFrameTime();
        //		billboardVels[i] += vec;
        //		billboards.getVertices()[i] += billboardVels[i];
        //		billboardVels[i] *= 0.94f;
    	//billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i]));
	}
	

}

void ledMatrix::matrixOff() {
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            //billboards.getColors()[i].set(0, 0, 0);
            mColors[i].set(40, 40, 40, 255);
        }
    }
}

void ledMatrix::matrixOn() {
    for (int x=0; x < LEDx; x++) {
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            //billboards.getColors()[i].set(255, 255, 255);
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
            //billboards.getColors()[i].set(imagePix.getColor(x * wOff, y * hOff));
            mColors[i].set(imagePix.getColor(x * wOff, y * hOff));
            //
            
        }
    }
    image.draw(0, 0);
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
    //ofSetColor(100, 100, 100);
    
    ofEnableDepthTest();
    
    
    
    for (int x=0; x < LEDx; x++) {
        //ofLine(x * spacing, 0, x * spacing, LEDy * spacing);
        for(int y = 0; y < LEDy; y++) {
            int i = (y * LEDx) + x;
            //            ofMesh::sphere(2);
            ofFill();
            ofSetColor(mColors[i]);
            sphere[i].draw();
            
            }
    }
      
    
    ofSetColor(255);
    for (int i = 0; i < LEDx; i++) {
        ofLine(i * spacing, 0, i * spacing, LEDy * spacing);
    }
	/*
	// bind the shader so that wee can change the
	// size of the points via the vert shader
	
    billboardShader.begin();
	
	ofEnablePointSprites(); // not needed for GL3/4
	texture.getTextureReference().bind();
	billboards.draw();
	texture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	
	billboardShader.end();
    */
    ofPopMatrix();
    ofDisableAlphaBlending();
     
}

