

#pragma once

#include "ofMain.h"

#include "appController.h"
#include "utils.h"
#include "ofxMacSystemProfiler.h"

/*
#include "audioController.h"
#include "oscController.h"
#include "connectorController.h"
#include "settingsController.h"
#include "socketController.h"

#include "analyticsController.h"

#include "parameterTransformer.h"
#include "uiConnector.h"
#include "ofAppWebUi.h"
#include "purchaseController.h"
#include "ofxMacWebView.h"
#include "ofxMacWindowStyles.h"
#include "videoController.h"
*/

class ofApp : public ofBaseApp {

public:
    ofApp() {}

    void setup();
    void update();
    void draw();

    void onLoaded();
    void mouseMoved(int x, int y );

    /*void keyPressed(int key);
    void keyReleased(int key);

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void updateGlobal();
    void audioIn(float * input, int bufferSize, int nChannels);
    void exit();
    void urlResponse(ofHttpResponse & response);
    */

     appController appController;
     utils utilsController;

    /*analyticsController analytics;
    //connectorController connector;
    audioController audio;
    //oscController osc;
    parameterTransformer parameters;
    ofSoundStream soundStream;
    videoController video;
    settingsController settings;
    socketController socket;

    uiConnector uiController;
    ofxMacWebView webUi;
    ofxMacWindowStyles window;
    purchaseController purchases;

    int elapsedTime;
*/
private:
     /*
    void checkResize();
    bool isKeyPressed(int key);
    void createUiWindow();
    void setupUi();
    void createBlankUi();
    int prevWidth;
    int prevHeight;
    vector<int> pressedKeys;
    ofImage logoImage;
    */
};
