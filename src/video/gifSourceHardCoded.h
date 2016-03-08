#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "utils.h"

class gifSourceHardCoded: public ofBaseApp
{

public:
    void setup();
    void getMoreUrls();
    bool isLoadingUrls();

private:
    vector<string> getRandomUrls(int number);
    void addUrls();
    vector<string> urls;
    int lastGifsNumber;
};
