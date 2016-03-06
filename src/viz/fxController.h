#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "fxFactory.h"

//#include "parameterTransformer.h"
//
//#include "styler.h"
//#include "vizTypes.h"
//#include "uiConnector.h"

class fxController: public ofBaseApp
{

public:
    void setup();
    void transform(ofImage *image);

private:
    /*
    VizObj currentViz;
    styler styler;
    int fboSize;
    void drawShader(ShaderObj *shaderObj, ofFbo &fboTo, ofFbo &fboFrom, bool lastPass);
    void pingPong(int index,  ofFbo &fboTo, ofFbo &fboFrom);
    void drawLast(ofFbo fbo);
    ofShader  basicShader;
    ShaderObj initialResizingShader;
    ofFbo pingFbo;
    ofFbo pongFbo;
    fxFactory factory;
    ofShader getNewShader(string fragment, string vertex);


    void addFx(string fxId);
    void setStyle(string styleId);
    void createStyle(string styleName);

    void addVar(string varName, string expr);
    void removeFx(string fxUId);
    void moveFxToPosition(string fxUId, int position);
    void moveFxAmount(string fxUId, int position);

    void broadcastCurrentFxs();
    void broadcastCurrentVizVars();
    void broadcastAvailableFxs();
    void broadcastStyles();
    void broadcastExprInitInfo(string groupUId,string exprUId);

    void handleSocketConnected(eventX &e);
    void handleFxControllerUiInput(eventX &e);
    void handleSetStyle(eventX &e);

    bool initInitialResizing();

    Json::Value getExprInfo(expressionKit *expKit, bool includeExpression);
    Json::Value getAllExprInfo();
    */
};
