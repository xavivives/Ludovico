#pragma once

#include "ofMain.h"
#include "ofxGifType.h"
#include "eventX.h"
#include "video/gifPlayer.h"

class gifBuffer: public ofBaseApp
{
public:
    void setup();
    void update();
    gifPlayer player;
    void load( shared_ptr<ofxGifType> newGif);
    ofImage* getImage();
    bool isFull();

    int maxBuffer;

private:
    shared_ptr<ofxGifType> toDelete; //if its time to delete a gif but its playing we will put it here so the buffer can keep working. When the gif is not played anymore we will delte it
    vector< shared_ptr<ofxGifType>> buffer;
    void clearBuffer();
    void playNext();

    void loadRandom();
    void handleClearBuffer(eventX &e);
    int random();
    void removeGifFromMemoryAndBuffer(int index = 0);
};
