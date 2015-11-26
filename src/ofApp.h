#pragma once

#include <ctime>
#include <math.h>
#include "ofMain.h"

#include "ofxSlitScan.h"

#define NB_STRIKES_TO_WARNING 9
#define NB_STRIKES_TO_GAME_OVER 18
#define TOUCH_COOLDOWN 0.4

enum StrikeState
{
    INITIAL,
    STANDARD,
    WARNING,
    DANGER,
    CRITICAL,
    FATAL,
    GAME_OVER,
    WIN
};

enum Notes
{
    NONE,
    A,
    D,
    E,
    Fs,
    B,
    Cs
};

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
    
    int mouseX;
    ofVideoGrabber vidGrabber;
    ofShader shaderAverage;
    ofShader shaderColorSep;
    ofFbo fboAveraged;
    
    
    float intensity;
    float touchStartTime;
    
    
    //Strike and state management
    int strikeCount;
    StrikeState state;
    
    //Sounds
    int secCountToNextNote;
    bool halfnote;
    Notes nextNote;
    
    ofSoundPlayer A_sound;
    ofSoundPlayer B_sound;
    ofSoundPlayer Cs_sound;
    ofSoundPlayer D_sound;
    ofSoundPlayer E_sound;
    ofSoundPlayer Fs_sound;
    ofSoundPlayer strike00_sound;
    ofSoundPlayer strike01_sound;
    ofSoundPlayer strike02_sound;
    ofSoundPlayer heartBeatSlowLow_sound;
    ofSoundPlayer heartBeatSlow_sound;
    ofSoundPlayer heartBeatFast_sound;
    ofSoundPlayer gameOver_sound;
    ofSoundPlayer win_sound;
    
    void loadSounds();
    void manageMusic();
    
    //GameOver
    ofImage gameOver_image;
    ofImage initial_image;
    ofVideoPlayer win_vid;
};
