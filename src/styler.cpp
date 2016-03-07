
#include "styler.h"

void styler::setup()
{
    defaultStylesPath =  constants::DEFAULT_JSON_STYLES_FILE_NAME; //it is in the app bundle folder (can't be modified)
    customStylesPath =  utils::getUserFolderPath().append("/").append(constants::CUSTOM_JSON_STYLES_FILE_NAME); //it is in a lib folder

    loadStylesFromFile();
}

void styler::loadStylesFromFile()
{

    //cout << "\ncustom: "<< customStylesPath;
    //cout << "\ndefault: "<< defaultStylesPath;
    string filePath = customStylesPath;
    if(!ofFile(filePath).exists())
    {
        ofLogWarning("Custom style path not created yet");
        filePath = defaultStylesPath;
        if(!ofFile(filePath).exists())
        {
            ofSystemTextBoxDialog("The default style file doesn't exist. ", filePath);
            ofLogWarning("Default style path not found");
            return;
        }
    }
    ofxJSONElement jsonFile;
    bool parsingSuccessful = jsonFile.open(filePath);
    if(parsingSuccessful)
    {
        for(int i = 0; i < jsonFile[constants::STYLES_KEY].size(); ++i)
            styles.push_back(jsonFile[constants::STYLES_KEY][i]);
    }
    else
    {
        ofLogWarning("styler:: Unable to parse "+filePath);
    }

}
void styler::saveStylesToFile()
{
    //cout << "\nSaving styles to "<< customStylesPath;
    Json::Value jsonStyles;
    jsonStyles[constants::STYLES_KEY] = generateJsonStyles();
    string content =jsonStyles.toStyledString();
    ofBuffer buffer;
    buffer.set(content.c_str(), content.size());
    bool fileWritten = ofBufferToFile(customStylesPath, buffer);
    if(!fileWritten)
        ofLogWarning("Styler:: Couldn't save styles to " + customStylesPath);
}

Json::Value styler::generateJsonStyles()
{
    Json::Value json;
    for(int i = 0; i<styles.size(); i++)
        json.append(styles[i]);
    return json;
}

Json::Value styler::createStyle(Json::Value viz, string name)
{

    Json::Value style;
    style[constants::FXS_KEY] = viz["fxs"];
    style[constants::VIZ_VARS_KEY] = viz["vizVars"];
    style[constants::STYLE_UID_KEY] =getNewId();
    style[constants::NAME_KEY] = name;
    style[constants::TIMESTAMP_KEY] = ofGetUnixTime();
    return style;
}

string styler::getNewId()//TODO: server generated
{
    return ofToString((int)ofRandom(9999999));
}

void styler::logStyles()
{
    cout << "\nSTYlES:";
    for(int i = 0; i<styles.size(); i++)
        cout <<styles[i][constants::NAME_KEY];
    cout << "\n";
}

void styler::addStyle(Json::Value style)
{
    styles.push_back(style);
}

Json::Value styler::getStyle(string styleUId)
{
    for(int i = 0; i<styles.size(); i++)
    {
        if(styles[i][constants::STYLE_UID_KEY].asString() == styleUId)
            return styles[i];
    }
    ofLogError("styler::getStyle - No style found for "+styleUId);
    return Json::nullValue;
}

bool styler::styleExist(string styleUId)
{
    for(int i = 0; i<styles.size(); i++)
    {
        if(styles[i][constants::STYLE_UID_KEY].asString() == styleUId)
            return true;
    }
    ofLogError("styler::styleExist - No style found for "+styleUId);
    return false;
}
