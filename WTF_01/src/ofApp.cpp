#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);

    ofImage distortionMap;
    distortionMap.loadImage("grey.png");

    //set up a slit scan with a maximum capacity of frames
    // in the distortion buffer
    slitScan.setup(640, 480, 30);

    slitScan.setDelayMap(distortionMap);
    //blending means the edges between the scans are feathered
    slitScan.setBlending(true);

    srand(time(NULL));
    initial_time = time(NULL);
    frameCount = 0;

    currentDelay = former_delay = target_delay = 1;
    minDelay = 0.5f;
    maxDelay = 1.5f;

    currentFramePerSeconds = 15;

    //time delay is the deepest in history the delay can go
    //and width is the number of frames the distortion will encompass
    //note that the delay cannot be more than the total capacity
    slitScan.setTimeDelayAndWidth(minDelay*currentFramePerSeconds, (maxDelay-minDelay)*currentFramePerSeconds);

    //set up the grabber
    grabber.initGrabber(640, 480);
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    if(time(NULL)!=initial_time)
    {
        currentFramePerSeconds = (19 * currentFramePerSeconds + frameCount)/20;

        former_delay = target_delay;
        float newDelay = (rand()%1000)/1000.f+0.5f;
        float delta = currentDelay - newDelay;
        float currFrameRate = 1/(float) currentFramePerSeconds;
        if(delta > currFrameRate)
            delta = currFrameRate;
        else if(delta < -currFrameRate)
            delta = - currFrameRate;
        target_delay = currentDelay - delta;

        initial_time = time(NULL);
        frameCount = 0;
    }
    if(grabber.isFrameNew()){
        ++frameCount;
        slitScan.addImage(grabber);
    }
}

float sigmoid(float x)
{
    return 1/(1+exp(-10*(x-0.5)));
}

//--------------------------------------------------------------
void ofApp::draw(){
    float coef = frameCount/(float)currentFramePerSeconds;
    coef = sigmoid(coef);

    currentDelay = former_delay*(1-coef) + target_delay*coef;

    slitScan.setTimeDelayAndWidth(currentDelay*currentFramePerSeconds, 0);

    slitScan.getOutputImage().draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
