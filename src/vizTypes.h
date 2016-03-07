#pragma once

#include "ofMain.h"
#include "ofxJson.h"
#include "parameterTransformer.h"

class VizVarObj
{
public:
    string varUId;
    expressionKit* expression;

    void calculate()
    {
        expression->calculate();
    }
};

class ShaderObj
{
public:
    bool initialized;
    string shaderUId;
    string fxName;
    string fxId;
    bool isVisible;
    int position;
    vector<string> uniformsUids;
    ofShader shader;
    map<string, expressionKit*> expressions;

    void setFragment(string fragment, vector<string> _uinformsUids )
    {
        uniformsUids = _uinformsUids;
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
        shader.linkProgram();
    }

    void setVertex(string vertex, vector<string> _uinformsUids )
    {
        uniformsUids = _uinformsUids;
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
        shader.linkProgram();
    }

    void calculate()
    {
        for(int i = 0; i<uniformsUids.size(); i++)
            expressions[uniformsUids[i]]->calculate();
    }

    void setUniforms()
    {
        for(int i = 0; i<uniformsUids.size(); i++)
            shader.setUniform1f(uniformsUids[i], expressions[uniformsUids[i]]->value);
    }

    void addVizVar(VizVarObj *vizVarObj)
    {
        typedef std::map<string, expressionKit*>::iterator it_type;
        for(it_type iterator = expressions.begin(); iterator != expressions.end(); iterator++)
        {
            expressionKit *expKit = iterator->second;
            expKit->addVariable(vizVarObj->varUId, vizVarObj->expression->value);
        }

    }

    ~ShaderObj()
    {
        cout<<"\nTODO: ShaderObj needs to be destoryed!!!\n";
    }
};




class VizObj
{
public:
    string vizUId;
    string vizName;
    vector<ShaderObj> shaderObjs;
    vector<VizVarObj> vizVarObjs;

    void calculateVizVars()
    {
        for(int i = 0; i < vizVarObjs.size(); i++)
            vizVarObjs[i].calculate();
    }

    void addFx(ShaderObj shaderObj)
    {
        shaderObjs.push_back(shaderObj);
        for(int i = 0; i < vizVarObjs.size(); i++)
            shaderObjs[shaderObjs.size()-1].addVizVar(&vizVarObjs[i]);
    }

    void addVizVar(VizVarObj varObj)
    {
        vizVarObjs.push_back(varObj);

        for(int i = 0; i < shaderObjs.size(); i++)
            shaderObjs[i].addVizVar(&vizVarObjs[vizVarObjs.size()-1]);
    }

    void removeFx(string fxUId)
    {
        for(int i = 0; i < shaderObjs.size(); i++)
        {
            if(shaderObjs[i].shaderUId == fxUId)
            {
                shaderObjs.erase(shaderObjs.begin()+i);
            }
        }
    }

    void moveFxToPosition(string fxUId, int position)
    {
        cout << "\nmoveFxToPosition: "<< fxUId << " " << position;
    }

    void moveFxAmount(string fxUId, int amount)
    {

        int currentIndex = -1;
        for(int i = 0; i < shaderObjs.size(); i++)
        {
            if(shaderObjs[i].shaderUId == fxUId)
            {
                currentIndex = i;
                break;

            }
        }
        if(currentIndex == -1)
        {
            ofLogError("moveFxAmount: Fx with fxUid: '"+fxUId+"'not found");
            return;
        }


        ShaderObj element = shaderObjs[currentIndex]; //duplicate
        shaderObjs.erase(shaderObjs.begin()+currentIndex);//remove


        int offset = currentIndex + amount;
        int size = shaderObjs.size();


        if(offset >= size)
            shaderObjs.push_back(element); //insert last

        else if(offset <= 0)
            shaderObjs.insert(shaderObjs.begin(), element);//insert first

        else
             shaderObjs.insert(shaderObjs.begin()+currentIndex+amount, element);//insert
    }

    void setExpression(string groupUId, string expressionUId, string newExpression)
    {
        if(groupUId == "var")
        {
            for(int i = 0; i < vizVarObjs.size(); i++)
                if(vizVarObjs[i].varUId == expressionUId)
                {
                    vizVarObjs[i].expression->setExpression(newExpression);
                    vizVarObjs[i].expression->calculate();
                }

        }
        else if(shaderObjExist(groupUId))
        {
            ShaderObj *shaderObj = getShaderObjById(groupUId);
            shaderObj->expressions[expressionUId]->setExpression(newExpression);
            shaderObj->expressions[expressionUId]->calculate(); //we calculate it already to get validation
        }

        ofLogWarning("VizObj::setExpression -  expressionUId '"+expressionUId+ "' of groupUid '"+groupUId+"'  set to new expression '" + newExpression+"'");
    }

    void setControl(string varUId, string controlType, float value)
    {
        for(int i = 0; i < vizVarObjs.size(); i++)
        {
            if(vizVarObjs[i].varUId == varUId)
            {
                vizVarObjs[i].expression->setControl(controlType, value);
                return;
            }

        }
        ofLogWarning("VizObj::setControl -  varUId '"+varUId+ "'doesn't exist");
    }

    Json::Value getExprInitInfo(string groupUId, string exprUId)
    {
        Json::Value json;
        json["groupUId"] = groupUId;
        json["exprUId"] = exprUId;

        if(groupUId == "var")
        {
            for(int i = 0; i < vizVarObjs.size(); i++)
            {
                if(vizVarObjs[i].varUId == exprUId)
                {
                    json["defaultExpr"] = vizVarObjs[i].expression->defaultExpr;
                    json["currentExpr"] = vizVarObjs[i].expression->currentExpr;
                }
            }
        }
        else if(shaderObjExist(groupUId))
        {
            ShaderObj *shaderObj = getShaderObjById(groupUId);
            json["defaultExpr"] = shaderObj->expressions[exprUId]->defaultExpr;
            json["currentExpr"] = shaderObj->expressions[exprUId]->currentExpr;
        }

        return json;

        ofLogWarning("VizObj::getExprInitInfo: "+json.toStyledString());
    }

    ShaderObj* getShaderObjById(string shaderUId)
    {
        for(int i = 0; i < shaderObjs.size(); i++)
        {
            if(shaderObjs[i].shaderUId == shaderUId)
                return &shaderObjs[i];
        }
        ofLogError("VizObj::getShaderObjById - "+shaderUId+ " not found");
        return nullptr;
    }

    bool shaderObjExist(string shaderUId)
    {
        for(int i = 0; i < shaderObjs.size(); i++)
        {
            if(shaderObjs[i].shaderUId == shaderUId)
                return true;
        }

        ofLogError("VizObj::shaderObjExist - shaderUId'"+shaderUId+ "' was not found");
        return false;
    }

    Json::Value getInfo() //getInfo and geStateInfo should merge once the UI can handle them properly
    {
        Json::Value json;

        for(int i = 0; i< shaderObjs.size();i++)
        {
            Json::Value fx;
            fx["shaderUId"]= shaderObjs[i].shaderUId;
            fx["fxName"]= shaderObjs[i].fxName;

            typedef std::map<string, expressionKit*>::iterator it_type;
            for(it_type iterator = shaderObjs[i].expressions.begin(); iterator != shaderObjs[i].expressions.end(); iterator++)
            {
                expressionKit *expKit = iterator->second;
                fx["fxExpressions"].append(expKit->getUId());
                json[expKit->groupUId] = fx;
            }
        }
        return json;
    }

    Json::Value getStateInfo(bool includeExpression)
    {
        Json::Value expressions;

        for(int i = 0; i< shaderObjs.size();i++)
        {
            typedef std::map<string, expressionKit*>::iterator it_type;
            for(it_type iterator = shaderObjs[i].expressions.begin(); iterator != shaderObjs[i].expressions.end(); iterator++)
            {
                expressionKit *expKit = iterator->second;
                Json::Value exprState = expKit->getInfo(includeExpression);
                expressions[expKit->getUId()] = exprState;
                expressions[expKit->getUId()]["host"]="fxController";
            }
        }
        return expressions;
    }

    Json::Value getCurrentVizVars()
    {
        //VIZVARS
        Json::Value json;
        for(int i = 0; i < vizVarObjs.size(); i++)
        {
            Json::Value vizVar;
            vizVar["vizVarUId"]=vizVarObjs[i].varUId;
            vizVar["expr"]=vizVarObjs[i].expression->currentExpr;
            vizVar["value"]=vizVarObjs[i].expression->value;
            json.append(vizVar);
        }
        return json;
    }

    Json::Value getCurrentFxs()
    {
        bool getExpression = true;
        Json::Value fxs;
        for(int i = 0; i< shaderObjs.size();i++)
        {
            Json::Value fx;
            fx["fxName"] = ( shaderObjs[i].fxName);
            fx["fxId"] = ( shaderObjs[i].fxId);
            fx["fxUId"] = ( shaderObjs[i].shaderUId);
            Json::Value expressions;
            typedef std::map<string, expressionKit*>::iterator it_type;

            for(it_type iterator = shaderObjs[i].expressions.begin(); iterator != shaderObjs[i].expressions.end(); iterator++)
            {
                expressionKit *expKit = iterator->second;
                expressions.append(expKit->getInfo(getExpression));
            }
            fx["expressions"] = expressions;


            fxs.append(fx);
        }
        return fxs;
    }

    Json::Value getExprValidation(string groupUId, string exprUId)
    {
        if(groupUId == "var")
        {
            for(int i = 0; i < vizVarObjs.size(); i++)
                if(vizVarObjs[i].varUId == exprUId)
                {
                    return vizVarObjs[i].expression->getInfo(true);
                    ofLogError("VizObj::getExprValidation - Var with exprUId '"+exprUId+"' was not found");
                }
        }
        else if(shaderObjExist(groupUId))
        {
            ShaderObj *shaderObj = getShaderObjById(groupUId);
            return shaderObj->expressions[exprUId]->getInfo(true);
        }
        ofLogError("VizObj::getExprValidation - groupUId'"+groupUId+ "' or exprUId '"+exprUId+"' was not found");
        return Json::nullValue;
    }


    Json::Value getViz()
    {
        //VIZVARS
        Json::Value json;
        for(int i = 0; i < vizVarObjs.size(); i++)
        {
            Json::Value fx;
            fx["vizVarUId"]=vizVarObjs[i].varUId;
            fx["expr"]=vizVarObjs[i].expression->currentExpr;
            json["vizVars"].append(fx);

        }
        //FXS
        for(int i = 0; i < shaderObjs.size(); i++)
        {
            Json::Value fx;
            fx["fxId"]=shaderObjs[i].fxId;
            fx["fxName"]=shaderObjs[i].fxName;
            fx["fxUId"] = ( shaderObjs[i].shaderUId);
            fx["position"]=i;

            typedef std::map<string, expressionKit*>::iterator it_type;
            for(it_type iterator = shaderObjs[i].expressions.begin(); iterator != shaderObjs[i].expressions.end(); iterator++)
            {
                expressionKit *expKit = iterator->second;
                Json::Value expression;
                expression["exprId"] = expKit->exprId;
                expression["expr"] = expKit->currentExpr;

                fx["expressions"].append(expression);
            }
            json["fxs"].append(fx);
        }
        return json;
    }

    Json::Value getControls()
    {
        Json::Value json = Json::Value(Json::arrayValue);

        for(int i = 0; i < vizVarObjs.size(); i++)
        {
            if(vizVarObjs[i].expression->isVarUsed("slider"))
            {
                Json::Value control;
                control["vizVarUId"] = vizVarObjs[i].varUId;
                control["value"] = vizVarObjs[i].expression->value;
                control["controlType"] = "slider";
                json.append(control);
            }

            if(vizVarObjs[i].expression->isVarUsed("button"))
            {
                Json::Value control;
                control["vizVarUId"] = vizVarObjs[i].varUId;
                control["value"] = vizVarObjs[i].expression->value;
                control["controlType"] = "button";
                json.append(control);
            }

            if(vizVarObjs[i].expression->isVarUsed("toggle"))
            {
                Json::Value control;
                control["vizVarUId"] = vizVarObjs[i].varUId;
                control["value"] = vizVarObjs[i].expression->value;
                control["controlType"] = "toggle";
                json.append(control);
            }
        }
        return json;
    }
};

