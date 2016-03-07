
#include "fxController.h"

void fxController::setup()
{
    ofAddListener(eventX::FX_CONTROLLER_UI_INPUT, this, &fxController::handleFxControllerUiInput);

    factory.setup();
    styler.setup();

    fboSize = 512;

    bool sucess = initInitialResizing();

    if(!sucess)
    {
        ofLogError("fxController::setup() - initInitialResizing() was unsucessfull" );
        return;
    }

    pingFbo.allocate(fboSize,fboSize, GL_RGB);
    pongFbo.allocate(fboSize,fboSize, GL_RGB);//it will have problemes rendering images bigger than 1024

    setStyle("0001");
}

void fxController::transform(ofImage *image)
{
    if(!image->isAllocated()) //image is not set
        return;

    currentViz.calculateVizVars();

    pingFbo.begin();
    image->draw(0,0);
    pingFbo.end();

    drawShader(&initialResizingShader, pongFbo, pingFbo, false);

    //pingPong(0, pongFbo, pingFbo);
    pingPong(0, pingFbo, pongFbo);

    uiConnector::updateDataStream("currentFxs", "setCurrentFxs", currentViz.getCurrentFxs());
    uiConnector::updateDataStream("currentVizVars", "setCurrentVizVars", currentViz.getCurrentVizVars());

}

void fxController::pingPong(int index,  ofFbo &fboTo, ofFbo &fboFrom)
{
    if(currentViz.shaderObjs.size() == 0) //if its the last shader we just draw it on the canvas
    {
        drawLast(fboFrom);
        return;
    }
    else if (index >= currentViz.shaderObjs.size()-1)
    {
        drawShader(&currentViz.shaderObjs[index], fboTo, fboFrom, true);
        return;
    }
    drawShader(&currentViz.shaderObjs[index], fboTo, fboFrom, false);
    pingPong(index +1 , fboFrom, fboTo);
}

void fxController::drawShader(ShaderObj *shaderObj, ofFbo &fboTo, ofFbo &fboFrom, bool lastPass)
{
    if(!lastPass)
        fboTo.begin();

    shaderObj->shader.begin();

    shaderObj->calculate();
    shaderObj->setUniforms();

    if(!lastPass)
        fboFrom.draw(0,0);
    else
        drawLast(fboFrom);

    shaderObj->shader.end();

    if(!lastPass)
        fboTo.end();
}

void fxController::drawLast(ofFbo fbo) //draws the fbo centered
{
    float size = ofGetWindowHeight();
    float xOffset =  -0.5*(ofGetWindowHeight()-ofGetWindowWidth());
    float yOffset = 0;
    if(ofGetWindowWidth()>ofGetWindowHeight())
    {
        size = ofGetWindowWidth();
        xOffset = 0;
        yOffset = -0.5*(ofGetWindowWidth()-ofGetWindowHeight());
    }

    fbo.draw(xOffset, yOffset, size, size);
}

void fxController::broadcastAvailableFxs()
{
    uiConnector::performAction("availableFxs", "setAvailableFxs", factory.getFxsForUi());
}

void fxController::broadcastExprInitInfo(string groupUId,string exprUId)
{
    uiConnector::performAction("expressionEditor", "setExprInitInfo", currentViz.getExprInitInfo(groupUId, exprUId));
}

void fxController::broadcastCurrentFxs()
{
    uiConnector::performAction("currentFxs", "setCurrentFxs", currentViz.getCurrentFxs());
}

void fxController::broadcastCurrentVizVars()
{
    uiConnector::performAction("currentVizVars", "setCurrentVizVars", currentViz.getCurrentVizVars());
}

void fxController::addFx(string fxId)
{
    if(factory.fxExists(fxId))
        currentViz.addFx(factory.getNewShaderObjByFxId(fxId));

    broadcastCurrentFxs();
}

void fxController::addVar(string varName, string expr)
{
    if(!factory.isVizVarNameValid(varName))
    {
        ofLogError("fxController::addVar-  '"+varName+"' is not a valid vizVar name");
        return;
    }

    VizVarObj var = factory.getNewVizVarObj(varName, expr);
    var.calculate();
    currentViz.addVizVar(var);
    broadcastCurrentVizVars();
}

bool fxController::initInitialResizing()
{
    bool sucess = false;
    string size =ofToString(fboSize);
    ostringstream ox;
    ox << "if(videoWidth>videoHeight,-0.5(("<<size<<"/videoHeight)*videoWidth-"<<size<<"),0)";// if(videoWidth>videoHeight,-0.5((1024/videoHeight)*videoWidth-1024),0)
    ostringstream oy;
    oy << "if(videoWidth>videoHeight,0,-0.5(("<<size<<"/videoWidth)*videoHeight-"<<size<<"))";//if(videoWidth>videoHeight,0,-0.5((1024/videoWidth)*videoHeight-1024))
    ostringstream os;
    os << "if(videoWidth>videoHeight,"<<size<<"/videoHeight,"<<size<<"/videoWidth)";//if(videoWidth>videoHeight,1024/videoHeight,1024/videoWidth)

    initialResizingShader = factory.getNewShaderObjByFxId("zoom");
    if(initialResizingShader.initialized==false)
    {
        ofLogError("fxController::initInitialResizing");
        return false;
    }

    initialResizingShader.expressions["offsetX_0"]->setExpression(ox.str());
    initialResizingShader.expressions["offsetY_0"]->setExpression(oy.str());
    initialResizingShader.expressions["scale_0"]->setExpression(os.str());
    return true;
}

void fxController::handleFxControllerUiInput(eventX &e)
{
    string actionId = e.strings[0];
    Json::Value data = e.json;

    if(actionId == "getAvailableFxs")
        broadcastAvailableFxs();

    else if(actionId == "getExprInitInfo")
        broadcastExprInitInfo(data["groupUId"].asString(), data["exprUId"].asString());

    else if(actionId=="addVar")
        addVar(data.asString(), "1");
    else if(actionId=="addFx")
        addFx(data.asString());
    else if(actionId=="removeFx")
        removeFx(data.asString());
    else if(actionId=="moveFxToPosition")
        moveFxToPosition(data["fxUId"].asString(),data["position"].asInt());
    else if(actionId=="moveFxAmount")
        moveFxAmount(data["fxUId"].asString(),data["amount"].asInt());

    else if(actionId =="setExpression")
    {
        string exprUId = data["exprUId"].asString();
        string groupUId = data["groupUId"].asString();
        string expression = data["expression"].asString();

        currentViz.setExpression(groupUId, exprUId, expression);
        uiConnector::performAction("expressionEditor", "setExprValidation", currentViz.getExprValidation(groupUId, exprUId));
        uiConnector::performAction("controlItems", "getControls", currentViz.getControls());
    }

    else if(actionId =="getCurrentFxs")
        broadcastCurrentFxs();
    else if(actionId =="getCurrentVizVars")
        broadcastCurrentVizVars();
    else if(actionId == "createStyle")
    {
        createStyle(data.asString());
        broadcastStyles();
    }
    else if(actionId == "getAvailableStyles")
        broadcastStyles();
    else if(actionId == "setStyle")
    {
        setStyle(data.asString());
        uiConnector::performAction("controlItems", "getControls", currentViz.getControls());
    }

    else if(actionId == "setControl")
        currentViz.setControl( data["vizVarUId"].asString(), data["controlType"].asString(), data["value"].asFloat());
    else if(actionId == "initControls")
         uiConnector::performAction("controlItems", "getControls", currentViz.getControls());
}

void fxController::removeFx(string fxUId)
{
    currentViz.removeFx(fxUId);
    broadcastCurrentFxs();
}

void fxController::moveFxToPosition(string fxUId, int position)
{
    currentViz.moveFxToPosition(fxUId, position);
    broadcastCurrentFxs();
}

void fxController::moveFxAmount(string fxUId, int amount)
{
    currentViz.moveFxAmount(fxUId, amount);
    broadcastCurrentFxs();
}

void fxController::setStyle(string styleId)
{
    if(!styler.styleExist(styleId))
        return;

    Json::Value style = styler.getStyle(styleId);
    currentViz = factory.getNewViz(style);
}

void fxController::createStyle(string styleName)
{
    Json::Value style = styler.createStyle(currentViz.getViz(), styleName);
    styler.addStyle(style);
    styler.saveStylesToFile();
}

void fxController::broadcastStyles()
{
    uiConnector::performAction("availableStyles", "setAvailableStyles", styler.generateJsonStyles());
}
