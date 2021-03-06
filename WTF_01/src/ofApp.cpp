#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::loadSounds()
{
    A_sound.loadSound("./GameSounds/A.wav");
    A_sound.setMultiPlay(true);
    B_sound.loadSound("./GameSounds/B.wav");
    B_sound.setMultiPlay(true);
    Cs_sound.loadSound("./GameSounds/C#.wav");
    Cs_sound.setMultiPlay(true);
    D_sound.loadSound("./GameSounds/D.wav");
    D_sound.setMultiPlay(true);
    E_sound.loadSound("./GameSounds/E.wav");
    E_sound.setMultiPlay(true);
    Fs_sound.loadSound("./GameSounds/F#.wav");
    Fs_sound.setMultiPlay(true);
    strike00_sound.loadSound("./GameSounds/Strike00.wav");
    strike00_sound.setMultiPlay(true);
    strike01_sound.loadSound("./GameSounds/Strike01.wav");
    strike01_sound.setMultiPlay(true);
    strike02_sound.loadSound("./GameSounds/Strike02.wav");
    strike02_sound.setMultiPlay(true);
    heartBeatSlowLow_sound.loadSound("./GameSounds/HeartBeatSlowLow.wav");
    heartBeatSlowLow_sound.setMultiPlay(true);
    heartBeatSlowLow_sound.setLoop(true);
    heartBeatSlow_sound.loadSound("./GameSounds/HeartBeatSlow.wav");
    heartBeatSlow_sound.setLoop(true);
    heartBeatFast_sound.loadSound("./GameSounds/HeartBeatFast.wav");
    heartBeatFast_sound.setLoop(true);
    gameOver_sound.loadSound("./GameSounds/GameOver.wav");
    win_sound.loadSound("./GameSounds/Win.wav");
}

void ofApp::manageMusic()
{
    if(secCountToNextNote>0)
        secCountToNextNote--;
    else
    {
        ofSoundPlayer* note;
        switch(nextNote)
        {
            case A:
                note = &A_sound;
                break;
            case D:
                note = &D_sound;
                break;
            case E:
                note = &E_sound;
                break;
            case Fs:
                note = &Fs_sound;
                break;
            case B:
                note = &B_sound;
                break;
            case Cs:
                note = &Cs_sound;
                break;
            case NONE:
                note = NULL;
                break;
            default:
                note = NULL;
                cout << "Missing Note" << endl;
                break;
        }
        if((note!=NULL)&&(state!=GAME_OVER)&&(state!=WIN)
           &&(state!=CRITICAL)&&(state!=FATAL))
        {
            if(!halfnote)
                note->setPositionMS(500);
            note->play();
        }
        int timingForNextNote = rand()%8;
        if(timingForNextNote%2)
        {
            if(halfnote)
            {
                halfnote = false;
                timingForNextNote++;
            }
            else
            {
                halfnote = true;
            }
        }
        secCountToNextNote = timingForNextNote/2;
        Notes formerNote = nextNote;
        switch(state)
        {
            case CRITICAL:
            case FATAL:
            case GAME_OVER:
            case WIN:
                nextNote = NONE;
                break;
            case INITIAL:
            case STANDARD:
            case WARNING:
            case DANGER:
                if(formerNote==D)
                    nextNote = (rand()%3)?D:A;
                else if(formerNote==E)
                    nextNote = (rand()%3)?E:A;
                else if(formerNote==B)
                    nextNote = (rand()%3)?B:Fs;
                else if(formerNote==Cs)
                    nextNote = (rand()%3)?Cs:Fs;
                else
                {
                    if(state == INITIAL)
                    {
                        if(formerNote == A)
                        {
                            int rnd = rand()%5;
                            nextNote = (rnd==0)?A:((rnd<3)?D:E);
                        }
                        else if(formerNote==NONE)
                            nextNote = (Notes)(rand()%3+1);
                    }
                    else if(state == STANDARD)
                    {
                        if(formerNote == A)
                        {
                            int rnd = rand()%7;
                            nextNote = (rnd==0)?A:((rnd<3)?D:((rnd<5)?E:Fs));
                        }
                        else if(formerNote == Fs)
                            nextNote = (rand()%3)?Fs:A;
                    }
                    else if(state == WARNING)
                    {
                        if(formerNote==A)
                            nextNote = Fs;
                        else if(formerNote==Fs)
                        {
                            int rnd = rand()%5;
                            nextNote = (rnd==0)?Fs:((rnd<3)?B:Cs);
                            
                        }
                    }
                }
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
   // ofSetFullscreen(true);
    
    loadSounds();
    gameOver_image.loadImage("BlueScreenOfLoose.png");
    
    strikeCount = 0;
    state = INITIAL;
    
    secCountToNextNote = 0;
    halfnote = false;
    nextNote = NONE;
    
    ofImage distortionMap;
    distortionMap.loadImage("grey.png");

    //set up a slit scan with a maximum capacity of frames
    // in the distortion buffer
    slitScan.setup(1024, 768, 30);

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
    
    
    
    grabber.setDeviceID(grabber.listDevices().size() - 1);
    
    grabber.initGrabber(1024, 768);
    
    
    
    fboAveraged.allocate(1024, 768);
    
    
    // shaderAverage.load("average");
    shaderColorSep.load("color");
    
    
    shaderColorSep.setUniform2f("_size", 1024, 768);

}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    if(time(NULL)!=initial_time)
    {
        manageMusic();
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
    
    
    if (state == GAME_OVER)
    {
        intensity = 0;
    }
    else if (state == WIN)
    {
        intensity = 0;
    }
    else
    {
        intensity = strikeCount * 0.01;
    }
    float duration = 0.5f;
    intensity += (duration - CLAMP((ofGetElapsedTimef() - touchStartTime), 0.0, duration)) * 0.5f;
    
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

    
    
    fboAveraged.begin();
    
    
    
    
    shaderColorSep.begin();
    
    
    shaderColorSep.setUniform2f( "_offsetDivisor1", 0.2 + -ofGetElapsedTimef() * 0.2, 0.3+ ofGetElapsedTimef() * 0.15);
    shaderColorSep.setUniform2f("_offsetDivisor2", 0.23+ ofGetElapsedTimef() * 0.012, 0.45+ ofGetElapsedTimef() * 0.07);
    
    shaderColorSep.setUniform2f("_rOffset", sin(ofGetElapsedTimef() * 0.21), ofGetElapsedTimef() * 0.18);
    shaderColorSep.setUniform2f("_gOffset", ofGetElapsedTimef() * 0.04, -ofGetElapsedTimef() * 0.08);
    shaderColorSep.setUniform2f("_bOffset", -ofGetElapsedTimef() * 0.003, cos(ofGetElapsedTimef() * 0.012));
    
    shaderColorSep.setUniform1f("_hueOffset", ofGetElapsedTimef() * 0.2);
    shaderColorSep.setUniform1f("_time", ofGetElapsedTimef());
    shaderColorSep.setUniform1f("_intensity", intensity);
    
    
    if (state == GAME_OVER)
    {
        gameOver_image.draw(0, 0, 1024, 768);
    }
    else
    {
        slitScan.getOutputImage().draw(0, 0);
    }
   
    
    
    shaderColorSep.end();
    fboAveraged.end();
    fboAveraged.draw(0,0);

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    switch(key)
    {
        case ' ':
            
            if (ofGetElapsedTimef() - touchStartTime > TOUCH_COOLDOWN)
            {
                
            
            strikeCount++;
            
            touchStartTime = ofGetElapsedTimef();
            switch(strikeCount)
        {
            case 1:
                state = STANDARD;
                strike00_sound.play();
                return;
            case NB_STRIKES_TO_WARNING:
                state = WARNING;
                break;
            case NB_STRIKES_TO_GAME_OVER-3:
                state = DANGER;
                heartBeatSlowLow_sound.play();
                break;
            case NB_STRIKES_TO_GAME_OVER-2:
                state = CRITICAL;
                ofSoundStopAll();
                heartBeatSlow_sound.play();
                break;
            case NB_STRIKES_TO_GAME_OVER-1:
                state = FATAL;
                ofSoundStopAll();
                heartBeatFast_sound.play();
                break;
            case NB_STRIKES_TO_GAME_OVER:
                state = GAME_OVER;
                ofSoundStopAll();
                gameOver_sound.play();
                break;
        }
            switch(state)
        {
            case STANDARD:
                strike01_sound.play();
                break;
            case WARNING:
                strike02_sound.play();
                break;
        }
            }
            break;
        case OF_KEY_UP:
            if (state != GAME_OVER)
            {
                state = WIN;
                ofSoundStopAll();
                win_sound.play();
            }
            break;
        }


    if (key == 'r')
    {
        shaderColorSep.load("color");
        strikeCount = 0;
        state = INITIAL;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
mouseX = x;
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
