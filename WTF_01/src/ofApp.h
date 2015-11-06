#pragma once

#include <ctime>
#include <math.h>
#include "ofMain.h"
#include "../addon/ofxSlitScan.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofxSlitScan slitScan;
        ofVideoGrabber grabber;

        time_t initial_time;
        int frameCount;

        float currentDelay;
        float minDelay;
        float maxDelay;

        float former_delay;
        float target_delay;

        int currentFramePerSeconds;
};
