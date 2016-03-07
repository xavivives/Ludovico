#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxJSONElement.h"
#include "constants.h"
#include "utils.h"

class styler: public ofBaseApp{

public:
    void setup();
    Json::Value createStyle(Json::Value viz, string name);
    void addStyle(Json::Value style);
    Json::Value  generateJsonStyles();
    Json::Value  getStyle(string styleUId);
    bool styleExist(string styleUId);
    void saveStylesToFile();

private:
    string  defaultStylesPath;
    string customStylesPath;
    Json::Value jsonStyles;
    void loadStylesFromFile();

    vector<Json::Value> styles;
    string getNewId();
    void logStyles();
};
