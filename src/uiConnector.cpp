#include "uiConnector.h"

void uiConnector::setup()
{
    ofAddListener(eventX::UI_INPUT, this, &uiConnector::handleInput);
}

void uiConnector::performAction(string moduleId , string actionId, Json::Value data, bool now)
{
    cout << "\n-->"<<moduleId<<"::"<<actionId;
    Json::Value action;
    action["id"] = moduleId;
    action["actionId"] = actionId;
    action["data"] = data;

    static eventX event;
    event.json=action;
    if(now)
        ofNotifyEvent(eventX::SEND_UI_ACTION_NOW, event);
    else
        ofNotifyEvent(eventX::SEND_UI_ACTION, event);

    //ofLogWarning("uiConnector:: performAction: "+ json.toStyledString());

    return;
}

void uiConnector::updateDataStream(string streamId , string dataId, Json::Value data)
{
    //cout << "\n-->Data::"<<streamId<<"::"<<dataId;
    Json::Value stream;
    stream["streamId"] = streamId;
    stream["dataId"] = dataId;
    stream["data"] = data;

    static eventX event;
    event.json=stream;
    ofNotifyEvent(eventX::SEND_UI_DATA_STREAM, event);

   // ofLogWarning("uiConnector:: performAction: "+ stream.toStyledString());

    return;
}

void uiConnector::handleInput(eventX &e)
{

    //ofLogWarning("uiConnector:: handleInput: "+ e.json.toStyledString());

    string unitId = e.json.get("unitId","").asString();
    string actionId = e.json.get("actionId","").asString();
    Json::Value data = e.json["data"];

    cout << "\n<--"<<unitId<<"::"<<actionId;

    connect(unitId, actionId, data);
}

void uiConnector::sendFXControllerEvent(string actionId, Json::Value data)
{
    static eventX event;
    event.strings[0] = actionId;
    event.json = data;
    ofNotifyEvent(eventX::FX_CONTROLLER_UI_INPUT, event);
}

void uiConnector::sendVideoControllerEvent(string actionId, Json::Value data)
{
    static eventX event;
    event.strings[0] = actionId;
    event.json = data;
    ofNotifyEvent(eventX::VIDEO_CONTROLLER_UI_INPUT, event);
}

void uiConnector::connect(string unitId, string actionId, Json::Value data)
{
    if(actionId == "log")
    {
        cout << "\nUI: " << data.asString();
    }

    if(actionId =="setTempo")
    {
        static eventX event;
        ofNotifyEvent(eventX::SET_TEMPO, event);
    }

    if(unitId == "LCSocket")
    {
        if(actionId =="connected")// a client connected
        {
            if(data.asFloat() == 1)
            {
                static eventX event;
                ofNotifyEvent(eventX::SOCKET_CONNECTED, event);
            }
        }
    }

    else if (unitId == "localFolders")
    {
        sendVideoControllerEvent(actionId, data);
    }

    else if(unitId == "availableFxs")
    {
        if(actionId =="addFx")
        {
            sendFXControllerEvent(actionId, data);
        }
        if(actionId =="getAvailableFxs")
        {
            sendFXControllerEvent(actionId);
        }
    }

    else if(unitId == "fxOptions")
    {
        if(actionId =="removeFx")
        {
            sendFXControllerEvent(actionId, data);
        }
        if(actionId =="moveFxToPosition")
        {
            sendFXControllerEvent(actionId, data);
        }
        if(actionId =="moveFxAmount")
        {
            sendFXControllerEvent(actionId, data);
        }
    }

    else if (unitId =="currentFxs")
    {
        sendFXControllerEvent(actionId);
    }

    else if (unitId =="currentVizVars")
    {
        sendFXControllerEvent(actionId);
    }

    else if (unitId == "expressionEditor")
    {
        if(actionId == "setExpression")
        {
            sendFXControllerEvent(actionId, data);
        }
        if(actionId == "getExprInitInfo")
        {
           sendFXControllerEvent(actionId, data);
        }
    }
    else if (unitId == "availableStyles")
    {
        sendFXControllerEvent(actionId, data);
    }
    else if (unitId == "currentViz")
    {
        sendFXControllerEvent(actionId, data);
    }
    else if (unitId == "controlItems")
    {
        sendFXControllerEvent(actionId, data);
    }
    else
    {
        cout << "\nNo match for unitId '"<<unitId<<"'";
    }

    /*

    if(unitId == "fxController")
    {

        if(actionId =="removeFx")
        {
            static eventX event;
            event.strings[0] =identifier;
            ofNotifyEvent(eventX::REMOVE_FX, event);
        }
        if(actionId =="setStyle")
        {
            static eventX event;
            event.strings[0] = identifier;
            ofNotifyEvent(eventX::SET_STYLE, event);
        }

        if(actionId =="saveStyles")
        {
            static eventX event;
            ofNotifyEvent(eventX::SAVE_STYLES, event);
        }


    }
    else if(unitId == "sound")
    {
        if(actionId =="volumeMultiplier")
        {
            static eventX event;
            event.floats[0] =data.asFloat();
            ofNotifyEvent(eventX::SET_VOLUME_MULTIPLIER, event);
        }
        else if(actionId == "autoVolume")
        {
            static eventX event;
            event.floats[0] = data.asFloat();
            ofNotifyEvent(eventX::SET_AUTO_VOLUME, event);
        }
    }

    else if(unitId == "source")
    {
        if(actionId =="setGiphySubject")
        {
            static eventX event;
            event.strings[0] = identifier;
            ofNotifyEvent(eventX::SET_GIPHY_SUBJECT, event);
        }
    }
     */
}

