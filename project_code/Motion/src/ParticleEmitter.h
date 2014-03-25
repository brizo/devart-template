#ifndef _EMITTER
#define _EMITTER


#include "ofMain.h"
#include "Particle.h"
#include "ofVbo.h"

//#define size 10000
#define NUM_BILLBOARDS 1000

class ParticleEmitter : public ofBaseApp{

private:
	int centerX;
	int centerY;
	float rotation;
    bool addState;
    ofPoint emitterPoint;
    vector<ofPoint> emitters;
	ofMesh m;
    bool meshSet;
    int vertPos;
	
public:
	void setup();
	void update();
	void draw();
    void addParticles(ofVec2f pos);
    void addParticle(bool);
    void updatePosition(ofPoint);
    //void setEmitters(vector<ofPoint> p);
    void addEmitter(ofPoint p);
    void clear();
    void setMesh(ofMesh _m);
    int getSize();
	
//	Particle* p[size];
    deque<Particle> p;
    // billboard particles
    ofShader billboardShader;
    ofImage texture;
    ofVboMesh billboards;
};

#endif
