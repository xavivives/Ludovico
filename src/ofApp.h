

#pragma once

#include "ofMain.h"

#include "appController.h"
#include "utils.h"
#include "ofxMacSystemProfiler.h"
//#include "analyticsController.h"
#include "uiConnector.h"
#include "socketController.h"
#include "audioController.h"
#include "video/videoController.h"


//#include "oscController.h"
//#include "connectorController.h"
//#include "settingsController.h"
#include "parameterTransformer.h"
#include "uiConnector.h"
//#include "ofAppWebUi.h"
//#include "purchaseController.h"
//#include "ofxMacWebView.h"
//#include "ofxMacWindowStyles.h"



class ofApp : public ofBaseApp {

public:
    ofApp() {}

    void setup();
    void update();
    void draw();

    void onLoaded();
    void mouseMoved(int x, int y );
    void audioIn(float * input, int bufferSize, int nChannels);


    void keyPressed(int key);
    void keyReleased(int key);

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void updateGlobal();

    void exit();
    void urlResponse(ofHttpResponse & response);

    ofxMacSystemProfiler profiler;
    //analyticsController analytics;
    //connectorController connector;
    parameterTransformer parameters;
    ofSoundStream soundStream;
    appController appController;
    utils utilsController;
    uiConnector uiController;
    socketController socket;
    //analyticsController analytics;
    audioController audio;
    videoController video;

    //settingsController settings;

    //ofxMacWebView webUi;
    //ofxMacWindowStyles window;

    int elapsedTime;

private:

    void checkResize();
    bool isKeyPressed(int key);
    //void createUiWindow();
    void setupUi();
    //void createBlankUi();
    int prevWidth;
    int prevHeight;
    vector<int> pressedKeys;
    ofImage logoImage;

};
