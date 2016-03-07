#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "constants.h"
#include "ofxJson.h"
#include "parameterTransformer.h"
#include "vizTypes.h"

class fxFactory: public ofBaseApp
{

public:
    void setup();

    Json::Value getFxsForUi();
    ShaderObj getNewShaderObjByFxId(string fxId);
    ShaderObj getNewShaderObjByJson(Json::Value fx);
    VizObj getNewViz(Json::Value style);
    VizVarObj getNewVizVarObj(string varUId, string expr);
    bool isVizVarNameValid (string varName);
    bool fxExists(string fxId);

private:

    expressionKit* getNewExpressionKit(string uId, string exprId, string name, string defaultExpr, string groupUid);
    int idCount;
    string composeShader(string shaderUId, vector<string>sFxs, vector<string>uniformUIds, string shaderType);
    string getNewShaderUId();
    string getUniformUId(string varId, string shaderUId);
    void handleAddFx(eventX &e);

    string fragmentHeader;
    string fragmentMain;
    string fragmentFooter;
    string vertexHeader;
    string vertexMain;
    string vertexFooter;

    Json::Value jFx;
    Json::Value getFxModel(string fxId);
    Json::Value getExprModel(Json::Value fxModel, string exprId);
    string getUniformsCode(vector<string> uniformUIds);
    string getCodeForFx(string shaderUId, string fxId, int index);
    void createExpressions(string fxId, int index);
    bool shaderObjExist(string shaderUId);

};
