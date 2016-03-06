#pragma once

#include "ofMain.h"
#include "eventX.h"

class uiConnector: public ofBaseApp
{

public:

    void setup();
    static void performAction(string moduleId, string actionId, Json::Value data, bool now = false);
    static void updateDataStream(string streamId , string dataId, Json::Value data);

private:

    void handleInput(eventX &e);
    void sendFXControllerEvent(string actionId, Json::Value data = Json::nullValue);
    void sendVideoControllerEvent(string actionId, Json::Value data = Json::nullValue);
    void connect(string unitId, string actionId, Json::Value data);
};
