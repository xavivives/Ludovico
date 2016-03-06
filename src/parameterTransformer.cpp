
#include "parameterTransformer.h"

map<string, expressionKit*> parameterTransformer::staticExpKits;
//vector<expressionKit*> parameterTransformer::fxExpKits;

int parameterTransformer::numSamples = 100;

float parameterTransformer:: currentVolume=0;
float parameterTransformer:: currentLowFrequency=0;
float parameterTransformer:: currentMidFrequency=0;
float parameterTransformer:: currentHighFrequency=0;

//modifiers
vector<float> parameterTransformer::volume (numSamples,0);
vector<float> parameterTransformer::lowFrequency(numSamples,0);
vector<float> parameterTransformer::midFrequency(numSamples,0);
vector<float> parameterTransformer::highFrequency(numSamples,0);
vector<float> parameterTransformer::spectrum(numSamples,0);
vector<vector<float> > parameterTransformer::fs(30,vector<float>(0));//review this

float parameterTransformer::isTempo = 0;
float parameterTransformer::tempoDeltaTime=0;
float parameterTransformer::elapsedTime=0;
float parameterTransformer::deltaTime=0;
float parameterTransformer::mouseX = 0;
float parameterTransformer::mouseY = 0;
float parameterTransformer::windowWidth = 0;
float parameterTransformer::windowHeight = 0;
float parameterTransformer::videoWidth = 0;
float parameterTransformer::videoHeight = 0;
float parameterTransformer::videoDuration = 0;
float parameterTransformer::videoProgress=0;
float parameterTransformer::videoTime=0;

void parameterTransformer::setup()
{
    for(int i =0;i<fs.size();i++)
        fs[i].resize(numSamples);

    spectrum.resize(30);
    //volume.resize(numSamples);
    lowFrequency.resize(numSamples);
    midFrequency.resize(numSamples);
    highFrequency.resize(numSamples);

    ofAddListener(eventX::ENERGY, this, &parameterTransformer::handleVolume);
    ofAddListener(eventX::LO_ENERGY, this, &parameterTransformer::handleLowFrequency);
    ofAddListener(eventX::MI_ENERGY, this, &parameterTransformer::handleMidFrequency);
    ofAddListener(eventX::HI_ENERGY, this, &parameterTransformer::handleHighFrequency);
    ofAddListener(eventX::IS_TEMPO, this, &parameterTransformer::handleTempo);
    ofAddListener(eventX::FREQUENCY_SPECTRUM, this, &parameterTransformer::handleSpectrum);
    ofAddListener(eventX::CURRENT_VIDEO_INFO_UPDATE, this, &parameterTransformer::handleCurrentVideoInfoUpdate);
    ofAddListener(eventX::SET_EXPRESSION, this, &parameterTransformer::handleSetExpression);

    ofAddListener(eventX::SOCKET_CONNECTED, this, &parameterTransformer::handleSocketConnected);

    //ofAddListener(eventX::SET_EXPRESSIONS, this, &parameterTransformer::handleSetExpressions);

    //addStaticExpKit( "playhead", "Playhead", "elapsedTime%tempoDeltaTime");
     addStaticExpKit( "playhead", "Playhead", "currentValue+deltaTime*(avg(volume[1],volume[2],volume[3],volume[4],volume[5],volume[6],volume[7],volume[8],volume[9],volume[10])*2)");
    addStaticExpKit( "changeVideo", "Change Video", "if(volume[0]*0.95>max(volume[1],volume[2],volume[3],volume[4],volume[5],volume[6],volume[7],volume[8],volume[9],volume[10],volume[11],volume[12]),1,0)");
}

void parameterTransformer::update()
{
    moveVector(volume,currentVolume);
    moveVector(lowFrequency,currentLowFrequency);
    moveVector(midFrequency,currentMidFrequency);
    moveVector(highFrequency,currentHighFrequency);

    for(int i =0;i<fs.size();i++)
        moveVector(fs[i],spectrum[i]);

    deltaTime = ofGetElapsedTimeMillis() - elapsedTime;
    elapsedTime = ofGetElapsedTimeMillis();
    windowWidth = ofGetWindowWidth();
    windowHeight = ofGetWindowHeight();
    mouseX = ofClamp(ofGetMouseX(),0,windowWidth)/windowWidth;
    mouseY = ofClamp(ofGetMouseY(),0,windowHeight)/windowHeight;
    videoTime = getVideoTime();
    videoProgress = videoTime==0?0:videoTime/videoDuration;

    typedef std::map<string, expressionKit*>::iterator it_type;
    for(it_type iterator = staticExpKits.begin(); iterator != staticExpKits.end(); iterator++)
        iterator->second->calculate();

    //for(int i =0;i<fxExpKits.size();i++)
     //   fxExpKits[i]->calculate();

    broadcastVariables();
}

/*int parameterTransformer::getFxExpNum()
{
    return fxExpKits.size();
}
*/
void parameterTransformer::broadcastVariables()
{
    Json::Value json;

    json["spectrum"] = vectorToJson(spectrum);
    json["volume"] = volume[0];
    json["lowFrequency"] = lowFrequency[0];
    json["midFrequency"] = midFrequency[0];
    json["highFrequency"] = highFrequency[0];
    json["elapsedTime"] = elapsedTime;
    json["deltaTime"] = deltaTime;
    json["videoTime"] = videoTime;
    json["videoDuration"] = videoDuration;
    json["videoWidth"] = videoWidth;
    json["videoHeight"] = videoHeight;
    json["videoProgress"] = videoProgress;
    json["isTempo"] = isTempo;
    json["tempoDeltaTime"] = tempoDeltaTime;
    json["mouseX"] = mouseX;
    json["mouseY"] = mouseY;
    json["windowWidth"] = windowWidth;
    json["windowHeight"] = windowHeight;

    uiConnector::updateDataStream("parameters", "setParameters",json);

    /*static eventX event;
    event.json=json;
    ofNotifyEvent(eventX::SOCKET_JSON_BROADCAST, event);

    broadcastStaticExprState(false);*/
}

Json::Value parameterTransformer::vectorToJson(vector<float> v)
{
    Json::Value value;
    for(int i = 0; i < v.size(); i++)
        value.append(v[i]);

    return value;
}

void parameterTransformer::moveVector(vector<float> &vec, float newValue)
{
    for(int i =vec.size()-1; i>0; i--)
        vec[i] = vec[i-1];
    vec[0] = newValue;
}

void parameterTransformer::setVariable(string expressionId, string varId, float value)
{
    if(expressionIdExists(expressionId))
        staticExpKits.at(expressionId)->setVariable(varId, value);
}

/*void parameterTransformer::setExpressions( Json::Value jsonStyle)
{
    for(int i = 0; i < jsonStyle[constants::EXPRESSIONS_KEY].size(); ++i)
    {
        string expressionId = jsonStyle[constants::EXPRESSIONS_KEY][i][constants::EXPRESSION_ID_KEY].asString();
        string newExpression = jsonStyle[constants::EXPRESSIONS_KEY][i][constants::EXPRESSION_KEY].asString();

        setExpression(expressionId,newExpression);

        //let's update the UI
        Json::Value json;
        json["scope"]["properties"][expressionId][constants::EXPRESSION_KEY]= newExpression;
        static eventX event;
        event.json=json;
        ofNotifyEvent(eventX::SOCKET_JSON_BROADCAST, event);
        ofLogNotice("parameterTransformer:: Expression to JS "+expressionId+ " set to" + newExpression);
    }
}*/

bool parameterTransformer::expressionIdExists(string expressionId)
{
    if ( staticExpKits.find(expressionId) == staticExpKits.end() ) //check if expressionId exists
    {
       // ofLogWarning("\n parameterTransformer::expressionIdExists - Expression "+ expressionId+ " is not registered");
        return false;
    }
    else
        return true;
}

void parameterTransformer::addStaticExpKit(string parameterId, string name, string defaultExpr)
{
    expressionKit *expKit = new expressionKit();

    staticExpKits[parameterId] = expKit;
    expKit->init(parameterId, parameterId, name, defaultExpr, "none");
    addAllVariablesToExpKit(expKit);
}

void parameterTransformer::addAllVariablesToExpKit(expressionKit *expKit)
{
    expKit->values.add_constants();
    expKit->addVariable("currentValue", expKit->value);

    expKit->addVariable("random", expKit->random);
    expKit->addVariable("slider", expKit->slider);
    expKit->addVariable("button", expKit->button);
    expKit->addVariable("toggle", expKit->toggle);

    expKit->addVariable("elapsedTime", elapsedTime);
    expKit->addVariable("deltaTime", deltaTime);

    expKit->addVariable("mouseX", mouseX);
    expKit->addVariable("mouseY", mouseY);
    expKit->addVariable("windowWidth", windowWidth);
    expKit->addVariable("windowHeight", windowHeight);

    expKit->addVariable("videoWidth", videoWidth);
    expKit->addVariable("videoHeight", videoHeight);
    expKit->addVariable("videoTime", videoTime);
    expKit->addVariable("videoDuration", videoDuration);
    expKit->addVariable("videoProgress", videoProgress);

    expKit->addVariable("isTempo", isTempo);
    expKit->addVariable("tempoDeltaTime", tempoDeltaTime);

    expKit->addVector("volume", volume);
    expKit->addVector("lowFrequency", lowFrequency);
    expKit->addVector("midFrequency", midFrequency);
    expKit->addVector("highFrequency", highFrequency);

    for(int e =0;e<fs.size();e++)
         expKit->addVector("fs"+ofToString(e), fs[e]);

    expKit->expression.register_symbol_table( expKit->values);
}

float parameterTransformer::getPlayHead(){ return  staticExpKits.at("playhead")->value;}
float parameterTransformer::getChangeVideo(){return staticExpKits.at("changeVideo")->value;}

void parameterTransformer::handleVolume (eventX  &e){currentVolume = e.floats[0];}
void parameterTransformer::handleLowFrequency (eventX  &e){currentLowFrequency = e.floats[0];}
void parameterTransformer::handleMidFrequency (eventX  &e){currentMidFrequency = e.floats[0];}
void parameterTransformer::handleHighFrequency (eventX  &e){currentHighFrequency = e.floats[0];}
void parameterTransformer::handleSpectrum (eventX  &e){spectrum = e.floats;}
void parameterTransformer::handleTempo (eventX  &e){isTempo = e.floats[0]; tempoDeltaTime = e.floats[1];}//TODO: tempoDelta time should have its own event

void parameterTransformer::handleCurrentVideoInfoUpdate (eventX  &e)
{
    videoWidth= e.json["width"].asFloat();
    videoHeight = e.json["height"].asFloat();
    videoDuration = e.json["duration"].asFloat();
}
//void parameterTransformer::handleSetExpressions (eventX  &e){setExpressions(e.json);}

float parameterTransformer::getVideoTime()
{
    float ph = getPlayHead();

    bool countBackwards = false;
    if (ph<0)//if its negative we start counting from the end
        countBackwards = true;

    ph = abs(ph);
    float ms = fmod(ph,videoDuration);
    if(countBackwards)
        ms = videoDuration-ms;
    ms = isnan(ms)?0:ms;
    return ms;
}

void parameterTransformer::broadcastStaticExprState(bool includeExpression)
{
    Json::Value infos;

    typedef std::map<string, expressionKit*>::iterator it_type;
    for(it_type iterator = staticExpKits.begin(); iterator != staticExpKits.end(); iterator++)
    {
        expressionKit *expKit = iterator->second;
        Json::Value info = expKit->getInfo(includeExpression);
        infos[expKit->getUId()] = info;
        infos[expKit->getUId()]["host"]="videoController";
    }

    Json::Value json;
    json["scope"]["expressions"]=infos;

    static eventX event;
    event.json=json;
    ofNotifyEvent(eventX::SOCKET_JSON_BROADCAST, event);
}

void parameterTransformer::handleSetExpression(eventX &e)
{
    if(e.strings[0]=="videoController")
        setExpression(e.strings[1], e.strings[2], e.strings[3]);
}

void parameterTransformer::setExpression(string host, string expressionUId, string newExpression)
{
    staticExpKits[expressionUId]->setExpression(newExpression);
    ofLogWarning("parameterTransformer::setExpression -  Setting expression: "+expressionUId+ " of "+host+"  set to" + newExpression);
}

void parameterTransformer::handleSocketConnected(eventX &e)
{
    broadcastStaticExprState(true);
}
