#include "ParticleEmitter.h"


//--------------------------------------------------------------
void ParticleEmitter::setup(){
	//ofSetFrameRate(100);
	//ofBackground(0, 0, 0);
	//ofHideCursor();
	ofEnableSmoothing();
	centerX = ofGetScreenWidth() / 2;
	centerY = ofGetScreenHeight() / 2;
	rotation = 0;
//	for (int i=0; i<size; i++) p[i] = new Particle();
    //p.assign(, Particle());
    billboards.getVertices().resize(p.size());
	billboards.getColors().resize(p.size());
	billboards.getNormals().resize(p.size(),ofVec3f(0));
    
    for(int i = 0; i != p.size(); i++) {
        billboards.getVertices()[i].set(p.at(i).position);
		billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(100, 255), 255, 255));
    }
	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
	// load the bilboard shader
	// this is used to change the
	// size of the particle
	if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard");
	}
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("circle.png");
	ofEnableAlphaBlending();
    
    addState = false;
    emitterPoint.set(0, 0);
    meshSet = false;
    vertPos = 0;
}

//--------------------------------------------------------------
void ParticleEmitter::update(){
    if(addState) {
        for(int i = 0; i < emitters.size(); i++) {
            for(int n = 0; n < 5; n++) {
                addParticles(emitters.at(i));
            }
        }
    }
    if (p.size() > billboards.getNumVertices()) {
        billboards.getVertices().resize(p.size());
        billboards.getColors().resize(p.size());
        billboards.getNormals().resize(p.size(),ofVec3f(0));
    }
    for(int i = 0; i != p.size(); i++) {
        p.at(i).update();
        if(p.at(i).dead) {
            p.erase( p.begin() + i );
        }
        billboards.getVertices()[i].set(p.at(i).position);
		billboards.getColors()[i].set(p.at(i).color);
        billboards.setNormal(i, ofVec3f(p.at(i).getSize(),0,0));
    }
	rotation+=.05;
}

//--------------------------------------------------------------
void ParticleEmitter::draw(){
    ofSetColor(255);
    //ofDrawBitmapString("Fps: " + ofToString( ofGetFrameRate()), 15,15);
	//ofTranslate(centerX, centerY);
	//ofRotate(rotation);
//	for (int i=0; i<size; i++) p[i]->draw();
    ofEnableAlphaBlending();
    ofPushMatrix();
    // bind the shader so that wee can change the
	// size of the points via the vert shader
	billboardShader.begin();
	ofEnablePointSprites(); // not needed for GL3/4
	texture.getTextureReference().bind();
	billboards.draw();
	texture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	billboardShader.end();
    ofPopMatrix();
}

void ParticleEmitter::addParticles(ofVec2f pos) {
    Particle3D temp(pos.x, pos.y);
    if(meshSet) {
        //ofSeedRandom();
        int _rand = ofRandom(0, m.getNumVertices() - 1);
        ofVec3f _dest = m.getVertex(_rand) - ofVec3f(-320, -240, 1200);
        temp.setDestination(_dest);
        temp.setColor(m.getColor(_rand));
        
//        cout << "destination set: " <<_dest <<  endl;
    }
    p.push_back(temp);
}

void ParticleEmitter::addParticle(bool addP) {
    addState = addP;
    if (!addP) {
        emitters.clear();
    }
}

void ParticleEmitter::updatePosition(ofPoint p) {
    emitterPoint = p;
}

void ParticleEmitter::addEmitter(ofPoint p){
    emitters.push_back(p);
}

void ParticleEmitter::clear() {
    p.clear();
}

void ParticleEmitter::setMesh(ofMesh _m) {
    m = _m;
    meshSet = true;
}

int ParticleEmitter::getSize() {
    return p.size();
}
