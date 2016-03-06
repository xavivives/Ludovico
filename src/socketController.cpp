#include "socketController.h"

void socketController::setup() {
    //ofAddListener(eventX::SOCKET_JSON_BROADCAST, this, &socketController::handleSocketJsonBroadcast);
    ofAddListener(eventX::SEND_UI_ACTION, this, &socketController::handleSendUiAction);
    ofAddListener(eventX::SEND_UI_ACTION_NOW, this, &socketController::handleSendUiActionNow);
    ofAddListener(eventX::SEND_UI_DATA_STREAM, this, &socketController::handleSendUiDataStream);
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
    options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!

    rootPath = ofToDataPath("ui", true); //If release, we use the copied version in the app bundle
#ifdef OF_DEBUG
    rootPath = ofToDataPath("../../../../ui/public", true);//Otherwise we use the version of the root folder so we can edit html on runtime.
   // rootPath = ofToDataPath("../../../../ui3/public", true);//Otherwise we use the version of the root folder so we can edit html on runtime.
#endif
    ofLog(OF_LOG_NOTICE, "the number is %d", 10);

    options.documentRoot = rootPath;
    bSetup = server.setup( options );

    server.addListener(this);
    scope["actions"]=Json::Value(Json::arrayValue);
    scope["dataStream   "]=Json::Value(Json::arrayValue);
    receivedMessages.resize(0);
    toSendMessages.resize(0);
    toSendDataStream.resize(0);
}

string socketController::getLocalhostUrl()
{
    string base = "http://localhost:";
    return base.append(ofToString(server.getPort()));
}

void socketController::update()
{
    //RECEIVE
    mutexIn.lock();
        for(int i = 0; i < receivedMessages.size(); i++)
        {
            static eventX event;
            event.json = receivedMessages[i];
            ofNotifyEvent(eventX::UI_INPUT, event);
        }
        receivedMessages.clear();
    mutexIn.unlock();

    //SEND
    mutexOut.lock();
        vector <Json::Value> tempMessages= toSendMessages;
        toSendMessages.clear();
        vector <Json::Value> tempDataStream= toSendDataStream;
        toSendDataStream.clear();
    mutexOut.unlock();

    if(tempMessages.size() <=0 && tempDataStream.size() <=0 )
        return;

    for(int i = 0; i < tempMessages.size(); i++)
        scope["actions"].append(tempMessages[i]);
    string actions = scope.toStyledString();
    server.send(actions);
    scope["actions"] =  Json::Value(Json::arrayValue);

    for(int i = 0; i < tempDataStream.size(); i++)
        scope["dataStream"].append(tempDataStream[i]);
    string dataStream = scope.toStyledString();

    searchAndReplace(dataStream, " nan", " null"); //if we use another library or we don't use toStyledString() there won't be an space in front of nan.
    server.send(dataStream);
    scope["dataStream"] =  Json::Value(Json::arrayValue);

    tempMessages.clear();
}

void socketController::onMessage( ofxLibwebsockets::Event& args )
{

    mutexIn.lock();
        if ( !args.json.isNull() )
            receivedMessages.push_back(args.json);
    mutexIn.unlock();
}

void socketController::handleSendUiActionNow(eventX &e)
{
    mutexOut.lock();
    Json::Value json;
    json["actions"].append(e.json);
    string actions = json.toStyledString();
    server.send(actions);
    mutexOut.unlock();
}

void socketController::handleSendUiAction(eventX &e)
{
    mutexOut.lock();
    toSendMessages.push_back(e.json);
    mutexOut.unlock();
}

void socketController::handleSendUiDataStream(eventX &e)
{
    mutexOut.lock();
        toSendDataStream.push_back(e.json);
    mutexOut.unlock();
}

void socketController::merge(Json::Value& target, Json::Value obj)
{
    if (!target.isObject())
    {
        ofLogError("socketController::merge - target is not an object");
        return;
    }
    if(!obj.isObject())
    {
        ofLogError("socketController::merge - obj is not an object");
        return;
    }

    for (const auto& key : obj.getMemberNames())
    {
        if (target[key].isNull())
            target[key] = obj[key];

        else if (target[key].isObject())
            merge(target[key], obj[key]);

        else
            target[key] = obj[key];
    }
}

void socketController::onConnect( ofxLibwebsockets::Event& args )
{
    ofLogWarning("socketController::onConnect ");

}

void socketController::onOpen( ofxLibwebsockets::Event& args )
{
    ofLogWarning("socketController::onOpen " + args.conn.getClientIP() + ", " + args.conn.getClientName());
    cout << "socketController::onConnect " << ofGetElapsedTimeMillis();
}

void socketController::onClose( ofxLibwebsockets::Event& args )
{
    ofLogWarning("socketController::onClose");
}

void socketController::onIdle( ofxLibwebsockets::Event& args )
{
}

void socketController::onBroadcast( ofxLibwebsockets::Event& args )
{
}

void socketController::searchAndReplace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}


/*Json::Value socketController::oscToJson(eventX &e)
{
    vector<string> address = ofSplitString(e.oscAddress,"/");
    Json::Value json;
    Json::Value value;

    if(e.floats.size() > 0)
    {
        if(e.floats.size()>1)
            for(int i = 0; i < e.floats.size(); i++)
                value.append(e.floats[i]);
        else if (e.floats.size() == 1)
            value = e.floats[0];
    }
    else if(e.strings.size() > 0)
    {
        if(e.strings.size()>1)
            for(int i = 0; i < e.strings.size(); i++)
                value.append(e.strings[i]);
        else if (e.strings.size() == 1)
            value = e.strings[0];
    }

    if(address.size()==3)//sliders such as /colorTransform/red
        json["data"][address[1]][address[2]]=value;
    if(address.size()==4)//STRINGS suchs as /selector/toggleFolders/abstract. So last string is the value
        json["data"][address[1]][address[2]]= address[3];

    return json;
}
*/
