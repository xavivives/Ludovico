#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxGifDecoder.h"
#include "ofxVideoToSequence.h"
#include "gifBuffer.h"

class gifFileHandler: public ofBaseApp
{

public:
    void setup();
    void update();
    gifBuffer gifBuffer;
    ofxGifDecoder gifDecoder;
    ofxVideoToSequence videoDecoder;
    ofImage* getImage();
    void setSource(int cell, string newSource);
    Json::Value getFolders(string rootDirectory);

private:

    void clearTempFolder();
    int count = 0;
    int elapsedTime;
    void load(string path);

    void handleClearTempFolder(eventX &e);
    void onGifDecoded(ofxGifDecodedEvent &e);

    int random(int min, int max);
    bool playingRandomOnline;
    void loadFromTempFolder();
    void loadFromSourceFolder();
    void loadVideoFromTempFile();
    void deleteGifFromTemp(string path);
    string source;
    int lastCell;
    bool isTurboLoading;

};
