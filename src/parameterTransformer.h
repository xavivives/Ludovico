#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "exprtk.hpp"
#include "json.h"
#include "constants.h"
#include "uiConnector.h"

struct expressionKit: public ofBaseApp
{
    string uId; //unique Id for this specific expression, red2_0, changeVideo
    string groupUId; //unique Id for the grup of expressions its member of. In case of fx this is the shaderUId. In the case of vars is just "var"
    string exprId;// Id for the expression,red, changeVideo. In the case of vars is just its name
    string name; //Display name
    map <string, float> dynamicModifiers; //contains all dynamic variables such as sliders or buttons
    exprtk::parser<float> parser;
    exprtk::expression<float> expression;
    exprtk::symbol_table<float> values; //TODO rename to symbols
    string currentExpr;//We always compile this expr.
    string lastValidExpr;//we use it to compare it with the current Expr. So if its the same we don't have to compile the expression again.
    string defaultExpr;//the initial value and the one used when the expr fails (currentExpr takes its value)
    string writtenExpr;//what the user have written and sees in the interface
    float value;
    bool isValid;

    //vars specific for each expr
    float random;
    float slider;
    float button;
    float toggle;

    expressionKit()//constructor
    {
     /* uId = "";
        exprId = "";
        name = "";
        groupUId = "";
        currentExpr = "";
        lastValidExpr = "";
        defaultExpr = "";
        writtenExpr = "";
        isValid = true;
        value = 0.0;
      */
    }


    expressionKit(const expressionKit &kit)//copy constructor
    {
        uId = kit.uId;
        exprId = kit.exprId;
        name = kit.name;
        groupUId = kit.groupUId;
        currentExpr = kit.currentExpr;
        lastValidExpr = kit.lastValidExpr;
        defaultExpr = kit.defaultExpr;
        values = kit.values;
        isValid = kit.isValid;
        value = kit.value;
    }

    expressionKit& operator=(const expressionKit kit)//assigment
    {
        uId = kit.uId;
        exprId =kit.exprId;
        name = kit.name;
        groupUId = kit.groupUId;
        currentExpr = kit.currentExpr;
        lastValidExpr = kit.lastValidExpr;
        defaultExpr = kit.defaultExpr;
        values = kit.values;
        isValid = kit.isValid;
        value = kit.value;
        return *this;
    }

    ~expressionKit(){} //destructor

    void init(string _uId, string _exprId, string _name, string _defaultExpr, string _groupUId)
    {
        uId = _uId;
        exprId = _exprId;
        name = _name;
        defaultExpr = _defaultExpr;
        lastValidExpr = "";
        currentExpr = defaultExpr;
        writtenExpr = currentExpr;
        groupUId = _groupUId;

        button = 0;
        toggle = 1;
        slider = 0.5;
    }

    void calculate()
    {
        random = ofRandom(1);
        float result;

        if(currentExpr == lastValidExpr && lastValidExpr!="" )//nothing have changed from previous calculate() whether isValid or not
        {
            result = expression.value();
        }
        else //expression changed
        {
            bool compiled = parser.compile(currentExpr, expression);
            if(!compiled)
                 ofLogNotice("expressionKit:: Compiling error. " + parser.error());
            result = expression.value();

            if(!compiled || isnan(result))
            {
                ofLogNotice("expressionKit:: Exp failed: " + uId + " "+ currentExpr);
                parser.compile(defaultExpr, expression);
                result = expression.value();
                currentExpr = defaultExpr;
                lastValidExpr = defaultExpr;
            }
            else
            {
                ofLogNotice("expressionKit:: Exp vaild: " + uId + " "+ currentExpr);
                lastValidExpr = currentExpr;
            }
        }

        result= isnan(result)?1:result;//if the value starts as nan the previous check won't work

        if(currentExpr == writtenExpr)
            isValid = true;
        else
            isValid = false;


        value = result;
    }

    void setExpression(string newExpression)
    {
        currentExpr = newExpression;
        writtenExpr = newExpression;
    }

    void setControl(string controlType, float value)
    {
        if(controlType == "slider")
            slider = value;
        else if(controlType == "button")
            button = value;
        else if(controlType == "toggle")
            toggle = value;
    }

    void setVariable(string varId, float value)
    {
        if ( dynamicModifiers.find(varId) == dynamicModifiers.end() ) //doesn't exist
        {
            dynamicModifiers[varId] = value; //create a new item

            bool added =values.add_variable(varId, dynamicModifiers[varId]);//add it to the values to parse
            if(!added)
                 ofLogNotice("expressionKit:: Adding variable error for var: "+varId);

            bool compiled = parser.compile(defaultExpr, expression);//recompile parser
            if(!compiled)
                ofLogNotice("expressionKit:: Compiling error. "+parser.error());

            /*//trace existing vars
            std::deque<std::pair<std::string,float> > variable_list;
            values.get_variable_list(variable_list);
            for (std::size_t i = 0; i < variable_list.size(); ++i)
                cout << "\nvar :" << variable_list[i].first.c_str();
             */
        }
        else
        {
            dynamicModifiers[varId] = value; //just update the value
        }
    }

    void addVariable( string varId,  float &variable)
    {
         values.add_variable(varId, variable);
    }

    void addVector (string vectorId, vector<float> &vector)
    {
        values.add_vector(vectorId, vector);
    }

    void forceCompile()
    {
        parser.compile(currentExpr, expression);
    }

    string getUId()
    {
        return uId;
    }

    Json::Value getInfo(bool includeExpression)
    {
        Json::Value json;
        json["exprUId"]=getUId();
        json["name"]=name;
        json["groupUId"]=groupUId;
        if(includeExpression)
            json["expression"]=currentExpr;
        json["isValid"]=isValid;
        json["value"] = value;
        return json;
    }

    bool isVarUsed (string varName)
    {
        varName = ofToLower(varName);
        string expr = ofToLower(writtenExpr);

        //if(!isValid)
            //return false;

        if(varName.length()<=0)
            return false;

        if(expr.length()<=0)
            return false;

        int pos = expr.find(varName,0);

        if(pos==string::npos)
            return false;

        if(pos+varName.length()<expr.length())
            if(expr.substr(pos + varName.length(),1).find_first_not_of("_abcdefghijklmnopqrstuvwxyz0123456789", 0) == string::npos)
                return false;

        if(pos>0)
            if(expr.substr(pos -1 ,1).find_first_not_of("_abcdefghijklmnopqrstuvwxyz0123456789", 0) == string::npos)
                return false;

        return true;
    }
};

class parameterTransformer: public ofBaseApp
{

public:

    void setup();
    void update();
    static int getFxExpNum();

    static float getPlayHead();
    static float getChangeVideo();

    static void addVariableToExpKit(expressionKit *expKit, string varId, float &variable);
    static void addVectorToExpKit(expressionKit *expKit, string varId, vector<float> &vector);
    static void addAllVariablesToExpKit(expressionKit *expKit);

    static void setVariable(string expressionId, string varId,  float value);
    static void setExpression(string host, string expressionUId, string newExpression);
    static void addStaticExpKit(string parameterId, string name, string defaultExpr);

private:

    static Json::Value vectorToJson(vector <float> v);
    static void broadcastVariables();
    void moveVector(vector<float> &vec, float newValue);
    static map<string, expressionKit*> staticExpKits;

    static int numSamples;


    //MODIFIER VARS

    //vectors represent values over time. 0= current time, numSamples = the oldest time
    static vector<float> volume;
    static vector<float> lowFrequency;
    static vector<float> midFrequency;
    static vector<float> highFrequency;
    static vector<float> spectrum;
    static float isTempo;
    static vector< vector<float> > fs;
    static float tempoDeltaTime; //in ms
    static float videoTime; // = previous playHead
    static float videoWidth;
    static float videoHeight;
    static float videoDuration;
    static float videoProgress; //percentage 0-1
    static float elapsedTime; //since application started, in ms
    static float deltaTime; //since last frame, in ms
    static float mouseX;
    static float mouseY;
    static float windowWidth;
    static float windowHeight;

    static float currentVolume;
    static float currentLowFrequency;
    static float currentMidFrequency;
    static float currentHighFrequency;

    //static void setExpressions(Json::Value jsonStyle);
    static bool expressionIdExists(string expressionId);


    //handling other modifiers
    void handleVolume (eventX  &e);
    void handleLowFrequency (eventX  &e);
    void handleMidFrequency (eventX  &e);
    void handleHighFrequency (eventX  &e);
    void handleSpectrum (eventX  &e);
    void handleTempo (eventX  &e);
    void handleCurrentVideoInfoUpdate (eventX  &e);

    void handleSetExpressions(eventX &e);

    static float getVideoTime(); //it replicates the code on gifPlayer::getCurrentFrame :/

    static void broadcastStaticExprState(bool includeExpression);

    void handleSetExpression(eventX &e);
    void handleSocketConnected(eventX &e);
};

