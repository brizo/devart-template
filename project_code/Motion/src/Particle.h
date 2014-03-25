/*
 *  Particle.h
 *  openFrameworks
 *
 *  Created by Stephen Braitsch on 11/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARTICLE
#define PARTICLE

#import "ofMain.h"

class Particle {
	
private:
	
	int size;
	int age;		// current age
	int maxAge;		// max age
    bool destSet;
	
public:
	Particle();
    Particle(int x, int y);
	
	void update();
	void draw();
    int getSize();
    void setDestination(ofVec3f _d);
    void setColor(ofFloatColor _c);

	bool dead;
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofVec3f attraction;
    ofFloatColor color;
};


#endif