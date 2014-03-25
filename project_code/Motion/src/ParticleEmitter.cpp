#include "ParticleEmitter.h"


//--------------------------------------------------------------
void ParticleEmitter::setup(){
	ofSetFrameRate(100);
	ofBackground(0, 0, 0);
	ofHideCursor();
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
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("spark.png");
	ofEnableAlphaBlending();
    
    addState = false;
    emitterPoint.set(0, 0);
    meshSet = false;
    vertPos = 0;
}

//--------------------------------------------------------------
void ParticleEmitter::update(){
    if(p.size() > 0) cout << "Pos: " << p.at(0).position << " Dest: " << p.at(0).attraction << endl;
    if(addState) {
//        cout << emitters.size() << endl;
        for(int i = 0; i < emitters.size(); i++) {
            addParticles(emitters.at(i));
        }
    }
    if (p.size() > billboards.getNumVertices()) {
        //cout << p.size() << endl;
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
        billboards.setNormal(i, ofVec3f(12 + p.at(i).getSize() * 1,0,0));
        
    }
    
	rotation+=.05;
    /*
    float t = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// noise
		ofVec3f vec(ofSignedNoise(t, billboards.getVertex(i).y/div, billboards.getVertex(i).z/div),
                    ofSignedNoise(billboards.getVertex(i).x/div, t, billboards.getVertex(i).z/div),
                    ofSignedNoise(billboards.getVertex(i).x/div, billboards.getVertex(i).y/div, t));
		
		vec *= 10 * ofGetLastFrameTime();
     */
//		billboardVels[i] += vec;
//		billboards.getVertices()[i] += billboardVels[i];
//		billboardVels[i] *= 0.94f;
//    	billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i] * ofNoise(t+i),0,0));
	//}

}

//--------------------------------------------------------------
void ParticleEmitter::draw(){
    ofSetColor(255);
    ofDrawBitmapString("Fps: " + ofToString( ofGetFrameRate()), 15,15);
	//ofTranslate(centerX, centerY);
	//ofRotate(rotation);
//	for (int i=0; i<size; i++) p[i]->draw();
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
    Particle temp(pos.x, pos.y);
    if(meshSet) {
        ofSeedRandom();
        int _rand = ofRandom(m.getNumVertices() - 1);
        ofVec3f _dest = m.getVertex(vertPos) - ofVec3f(-320, -240, 1000);
        temp.setDestination(_dest);
        temp.setColor(m.getColor(vertPos));
        vertPos++;
        if(vertPos > m.getNumVertices()) {
            vertPos = 0;
            cout << "Filled up each Pixel\n";
        }
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
