#include "fxFactory.h"
#define STRINGIFY(A) #A

void fxFactory::setup()
{
    idCount = 0;
    if(!ofFile(constants::SHADER_COMPOSER_FILE_NAME).exists())
    {
        ofLogError("fxFactory::setup() - Couldn't find "+ constants::SHADER_COMPOSER_FILE_NAME);
        ofSystemTextBoxDialog("Ludovico was unable the find the following file. Please, report this error to @xavivives on twitter.",ofFile(constants::SHADER_COMPOSER_FILE_NAME).getAbsolutePath());
        return;
     }


    ofxJSONElement jsonFile;
    bool parsingSuccessful = jsonFile.open(constants::SHADER_COMPOSER_FILE_NAME);
    if(!parsingSuccessful)
    {
        ofLogError("fxFactory::setup() - Couldn't parse json "+ constants::SHADER_COMPOSER_FILE_NAME);
        ofSystemTextBoxDialog("Ludovico was unable the parse the following file. Please, report this error to @xavivives on twitter.",ofFile(constants::SHADER_COMPOSER_FILE_NAME).getAbsolutePath());
        return;
    }


    jFx =  jsonFile["fx"];

    fragmentHeader = jsonFile["fragment"]["header"].asString();
    fragmentMain = jsonFile["fragment"]["main"].asString();
    fragmentFooter = jsonFile["fragment"]["footer"].asString();

    vertexHeader = jsonFile["vertex"]["header"].asString();
    vertexMain = jsonFile["vertex"]["main"].asString();
    vertexFooter = jsonFile["vertex"]["footer"].asString();

}

string fxFactory::composeShader(string shaderUId, vector<string>sFxs, vector<string>uniformUIds,string  shaderType )
{
    string s = "";
    if(shaderType=="fragment")
        s.append(fragmentHeader);
    else if(shaderType=="vertex")
        s.append(vertexHeader);

    if(shaderType=="fragment")
        s.append("uniform sampler2DRect texture0;");

    s.append(getUniformsCode(uniformUIds));

    if(shaderType=="fragment")
        s.append(fragmentMain);
    else if(shaderType=="vertex")
        s.append(vertexMain);

    for(int i = 0; i<sFxs.size(); i++)
        s.append(getCodeForFx(shaderUId,sFxs[i],i));

    if(shaderType=="fragment")
        s.append(fragmentFooter);
    else if(shaderType=="vertex")
        s.append(vertexFooter);

    return s;
}

ShaderObj fxFactory::getNewShaderObjByFxId(string fxId)
{
    ShaderObj shaderObj;
    shaderObj.shaderUId = getNewShaderUId();

    vector <string> uniformsUIds;
    Json::Value fx = getFxModel(fxId);

    if(fx == Json::nullValue)
    {
        shaderObj.initialized = false;
        return shaderObj;
    }


    shaderObj.fxName = fx["fxName"].asString();
    for(int i = 0; i < fx["vars"].size(); i++)
    {
        string uniformUid = getUniformUId(fx["vars"][i]["exprId"].asString(), shaderObj.shaderUId);
        uniformsUIds.push_back(uniformUid);
        shaderObj.expressions[uniformUid] = new expressionKit();
        shaderObj.expressions[uniformUid] ->init(uniformUid, fx["vars"][i]["exprId"].asString(), fx["vars"][i]["exprName"].asString(), fx["vars"][i]["defaultExpr"].asString(), shaderObj.shaderUId);
        parameterTransformer::addAllVariablesToExpKit( shaderObj.expressions[uniformUid] );

        //setExpressionKit(shaderObj.expressions[uniformUid],uniformUid, fx["vars"][i]["exprId"].asString(), fx["vars"][i]["exprName"].asString(), fx["vars"][i]["defaultExpr"].asString(), shaderObj.shaderUId);
    }

    shaderObj.fxId = fx["fxId"].asString();
    vector<string> sFxs;
    sFxs.resize(0);
    sFxs.push_back(fxId);

    if(fx["shaderType"].asString() == "fragment")
        shaderObj.setFragment(composeShader(shaderObj.shaderUId, sFxs, uniformsUIds, "fragment"), uniformsUIds);
    if(fx["shaderType"].asString() == "vertex")
        shaderObj.setVertex(composeShader(shaderObj.shaderUId, sFxs, uniformsUIds, "vertex"), uniformsUIds);

    shaderObj.initialized = true;
    return shaderObj;
}

ShaderObj fxFactory::getNewShaderObjByJson(Json::Value fx)
{
    ShaderObj shaderObj;
    shaderObj.shaderUId = getNewShaderUId();

    vector <string> uniformsUIds;
    Json::Value fxModel = getFxModel(fx["fxId"].asString());

    shaderObj.fxName = fxModel["fxName"].asString();
    for(int i = 0; i < fx["expressions"].size(); i++)
    {
        string exprId = fx["expressions"][i]["exprId"].asString();
        string uniformUid = getUniformUId(exprId, shaderObj.shaderUId);
        uniformsUIds.push_back(uniformUid);
        Json::Value exprModel = getExprModel(fxModel, exprId );
        string expressionName = exprModel["exprName"].asString();

        shaderObj.expressions[uniformUid] = new expressionKit();
        shaderObj.expressions[uniformUid] ->init(uniformUid, fx["expressions"][i]["exprId"].asString(), expressionName, fx["expressions"][i]["expr"].asString(), shaderObj.shaderUId);
        parameterTransformer::addAllVariablesToExpKit( shaderObj.expressions[uniformUid] );




        //shaderObj.expressions[uniformUid] = setExpressionKit(uniformUid,fx["expressions"][i]["exprId"].asString(), expressionName, fx["expressions"][i]["expr"].asString(), shaderObj.shaderUId);
    }

    shaderObj.fxId = fx["fxId"].asString();

    vector<string> sFxs;//what is this???
    sFxs.resize(0);
    sFxs.push_back(shaderObj.fxId);

    if(fxModel["shaderType"].asString() == "fragment")
        shaderObj.setFragment(composeShader(shaderObj.shaderUId, sFxs, uniformsUIds, "fragment"), uniformsUIds);
    if(fxModel["shaderType"].asString() == "vertex")
        shaderObj.setVertex(composeShader(shaderObj.shaderUId, sFxs, uniformsUIds, "vertex"), uniformsUIds);

    return shaderObj;
}

expressionKit* fxFactory::setExpressionKit(expressionKit* expKit, string uId, string exprId, string name, string defaultExpr, string groupUid)
{
    //expressionKit* expKit;
    //expKit = new expressionKit(); //It must be created on the heap so it keeps the references of parameterTransformer variables
    expKit->init(uId, exprId, name, defaultExpr, groupUid);
    parameterTransformer::addAllVariablesToExpKit(expKit);
   // return expKit;
}

string fxFactory::getUniformUId(string varId, string shaderUId)
{
    string str = varId;
    str.append("_").append(shaderUId);
    return str;
}

string fxFactory::getNewShaderUId()
{
    return ofToString(idCount++);
}

Json::Value fxFactory::getFxsForUi()
{
    Json::Value json;
    for(int i = 0; i < jFx.size(); i++)
    {
        if(jFx[i]["isVisible"]!=true)
            break;

        Json::Value j;
        j["fxName"] = jFx[i]["fxName"].asString();
        j["fxId"] = jFx[i]["fxId"].asString();
        j["fxDescription"] = jFx[i]["fxDescription"].asString();
        json.append(j);
    }
    return json;
}

string fxFactory::getUniformsCode(vector<string> uniformUIds)
{
    string str = "";

    for(int e = 0; e <uniformUIds.size(); e++)
    {
        string u = "\nuniform float ";
        u.append(uniformUIds[e]);
        u.append(";");
        str.append(u);
    }

    str.append("\n");
    return str;
}

string fxFactory::getCodeForFx(string shaderUId, string fxId, int index)
{
    Json::Value fx= getFxModel(fxId);
    string code = fx["code"].asString();

    for(int i = 0; i < fx["vars"].size(); i++)
    {
        string varId = fx["vars"][i]["exprId"].asString();
        ofStringReplace(code, varId, getUniformUId(varId, shaderUId));
    }

    string c = "\n    //";
    c.append(fx["fxName"].asString());
    c.append("\n");
    c.append(code);
    c.append("\n");

    return c;
}

bool fxFactory::fxExists(string fxId)
{
    for(int i = 0; i < jFx.size(); i++)
    {
        if(jFx[i]["fxId"]==fxId)
            return true;
    }
    return false;
}

Json::Value fxFactory::getFxModel(string fxId)
{
    for(int i = 0; i < jFx.size(); i++)
    {
        if(jFx[i]["fxId"]==fxId)
            return jFx[i];
    }
    ofLogError("fxFactory::getFxModel - fxId '" +fxId+ "' doesn't exist");
    return Json::nullValue;
}

Json::Value fxFactory::getExprModel(Json::Value fxModel, string exprId)
{
    for(int i = 0; i < fxModel["vars"].size(); i++)
    {
        if(fxModel["vars"][i]["exprId"]==exprId)
            return fxModel["vars"][i];
    }
    ofLogError("fxFactory::getExpressionModel - exprId '" +exprId+ "' doesn't exist for fx with fxId: '"+fxModel["fxId"].asString()+"'");
    return Json::nullValue;
}

VizObj fxFactory::getNewViz(Json::Value style)
{
    VizObj viz;

    if(style==Json::nullValue)
        ofSystemAlertDialog("Model not found");

    //vars
    for(int i = 0; i < style[constants::VIZ_VARS_KEY].size(); i++)
    {
        string varUId = style[constants::VIZ_VARS_KEY][i]["vizVarUId"].asString();
        string expr = style[constants::VIZ_VARS_KEY][i]["expr"].asString();
        string name = style[constants::VIZ_VARS_KEY][i]["name"].asString();
        viz.addVizVar(getNewVizVarObj(varUId, expr));
    }

    //fxs
    for(int i = 0; i < style[constants::FXS_KEY].size(); i++)
    {
        string fxId = style[constants::FXS_KEY][i][constants::FX_ID_KEY].asString();


        if(!fxExists(fxId))
        {
            ofLogError("fxFactory::getNewViz - fxId '" +fxId+ "' doesn't exist");
            break;
        }
        Json::Value model = getFxModel(fxId);
        viz.addFx(getNewShaderObjByJson(style[constants::FXS_KEY][i]));
    }

    return viz;
}

VizVarObj fxFactory::getNewVizVarObj(string varUId, string expr)
{
    VizVarObj var;
    var.varUId = varUId;
    //var.expression =  setExpressionKit(varUId, "var", varUId, expr, "");

    var.expression = new expressionKit();
    var.expression ->init(varUId, "var", varUId, expr, "");
    parameterTransformer::addAllVariablesToExpKit(var.expression);

    return var;
}

bool fxFactory::isVizVarNameValid (string varName) //php valid var. only letters, numbers and underscore. First character can't be a number
{
    if(varName.length()<=0)
        return false;

    if(varName.length()>20)
        return false;

    if(varName.substr(0,1).find_first_not_of("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0) != string::npos)
        return false;

    if(varName.find_first_not_of("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 0) != string::npos)
        return false;

    return true;
}
