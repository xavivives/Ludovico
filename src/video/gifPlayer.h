#pragma once

#include "ofMain.h"
#include "ofxGifType.h"
#include "eventX.h"
#include "fxController.h"
//#include "analyticsController.h"
#include "parameterTransformer.h"

class gifPlayer: public ofBaseApp
{

public:
    void setup();
    void load(  shared_ptr<ofxGifType> loadedGif);
    void update();
    ofImage* getImage();
     shared_ptr<ofxGifType> getPlayingGif();

private:
    bool newGifLoaded;
    void play();
    int getCurrentFrame();
    bool isPlayable();
    void updateCurrentVideoInfo();

     shared_ptr<ofxGifType> gif;
    ofImage image;
    int elapsedTime;
    int currentFrame;
    int prevFrame;
    string prevFilePath;
    float currentSpeed;
    bool isPlaying;
};
