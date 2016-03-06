#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxLibwebsockets.h"

class socketController : public ofBaseApp{
public:
    void setup();
    void update();
    string getLocalhostUrl();

    ofxLibwebsockets::Server server;

    bool bSetup;

    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );

private:
    ofMutex mutexIn;
    ofMutex mutexOut;
    string rootPath;
    void handleSendUiAction(eventX &e);
    void handleSendUiActionNow(eventX &e);
    void handleSendUiDataStream(eventX &e);
    Json::Value scope; //to mimic Anguarjs $scope
    void merge(Json::Value& target, Json::Value obj);
    vector<Json::Value> toSendDataStream;
    vector<Json::Value> toSendMessages;
    vector<Json::Value> receivedMessages;//we write this first to ensure onMessage async event doesn't show up during merge
    void searchAndReplace(std::string& subject, const std::string& search, const std::string& replace);

};
