/*
 *  Particle.cpp
 *  openFrameworks
 *
 *  Created by Stephen Braitsch on 11/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle3D.h"

Particle3D::Particle3D()
{
	position.x = 0;//ofRandom(-500, 500);
	position.y = 0;//ofRandom(-500, 500);
    position.z = 0;//ofRandom(-500, 500);
	velocity.x = ofRandom(-6,6);
	velocity.y = ofRandom(-6,6);
    velocity.z = ofRandom(-6,6);
	color = ofRandom(0, 255);
    size = 5;
	
    color = ofColor(ofRandom(96, 255), ofRandom(150,255), 255);
	age = 0;
	dead = false;
	maxAge = ofRandom(0, 50)+250;
    destSet = false;
    //cout << position << endl;
}

Particle3D::Particle3D(int x, int y)
{
	position.x = x;//ofRandom(-500, 500);
	position.y = y;//ofRandom(-500, 500);
    position.z = 0;//ofRandom(-500, 500);
	velocity.x = ofRandom(-2,2);
	velocity.y = ofRandom(-2,2);
    velocity.z = ofRandom(-2,2);
	color = ofRandom(0, 255);
    size = 5;
	
    color = ofColor(ofRandom(96, 255), ofRandom(150,255), 255);
	age = 0;
	dead = false;
	maxAge = ofRandom(0, 50)+250;
    destSet = false;
    //cout << position << endl;
}


int Particle3D::getSize() {
    return size;
}

void Particle3D::update()
{
	//age++;
    if(!destSet) {
        (position.x>0) ? velocity.x : velocity.x*=-1;
        (position.y>0) ? velocity.y : velocity.y*=-1;
        (position.z>0) ? velocity.z : velocity.z*=-1;
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
    } else if(!(position.distance(attraction) < 2)){
        ofVec3f dir = attraction - position;
        dir.normalize();
        float d = position.distance(attraction);
        (d < 50) ? dir *= 1 : dir *= ofRandom(15, 7);
        velocity = dir;
        
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
    }
    
    
	if (age >= maxAge) dead = true;
}

void Particle3D::setDestination(ofVec3f _d) {
    attraction = _d;
    destSet = true;
}

void Particle3D::setColor(ofFloatColor _c) {
    color = _c;
}
