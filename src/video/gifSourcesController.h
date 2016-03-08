#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxJSON.h"
#include "utils.h"
#include "video/gifSourceHardCoded.h"
#include "video/gifSourceGiphy.h"

class gifSourcesController: public ofBaseApp{

public:
    void setup();
    void update();
    void urlResponse(ofHttpResponse &response);

private:
    string currentSource;
    string GIPHY ="giphy";
    string HARD_CODED ="hardCoded";

    void setupSources();
    gifSourceHardCoded sourceHardCoded;
    gifSourceGiphy sourceGiphy;
    bool giphyNeedsUrls();
    bool hardCodedNeedsUrls();

    void handleHardCodedUrls(eventX &e);
    void handleGiphyUrls(eventX &e);
    void handleSetGiphySubject(eventX &e);

    vector<string> urls;
    void loadNextGif();
    void saveGif(ofHttpResponse response);

    ofURLFileLoader loader;

    bool maxGifLoaded();
    float startLoadingTimeStamp;
    bool isGifLoading;

};
