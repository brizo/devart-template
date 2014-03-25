#include "testApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

	ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    synapse.setup();
    ofxOpenNIUser user;
    user.setUseMaskTexture(true);
    user.setUsePointCloud(true);
    user.setPointCloudDrawSize(2); // this is the size of the glPoint that will be drawn for the point cloud
    user.setPointCloudResolution(2); // this is the step size between points for the cloud -> eg., this sets it to every second point
    synapse.openNIDevice.setBaseUserClass(user); // this becomes the base class on which tracked users are created
    // allows you to set all tracked user properties to the same type easily
    // and allows you to create your own user class that inherits from ofxOpenNIUser
    
}

void ofApp::exit()
{
    cout << "Exit from app!" << endl;

}

//--------------------------------------------------------------
void ofApp::update()
{
    synapse.update();

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255, 255, 255);
    //synapse.draw();
    // do some drawing of user clouds and masks
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    int numUsers = synapse.openNIDevice.getNumTrackedUsers();
    for (int nID = 0; nID < numUsers; nID++){
        ofxOpenNIUser & user = synapse.openNIDevice.getTrackedUser(nID);
        user.drawMask();
        ofPushMatrix();
        ofTranslate(320, 240, -1000);
        user.drawPointCloud();
        cout << "Draw point cloud\n";
        ofPopMatrix();
    }
    ofDisableBlendMode();
    ofPopMatrix();

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

