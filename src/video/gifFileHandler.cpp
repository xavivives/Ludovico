#include "gifFileHandler.h"

void gifFileHandler::setup()
{
    gifBuffer.setup();
    gifDecoder.setup();
    ofAddListener(eventX::CLEAR_TEMP_FOLDER, this, &gifFileHandler::handleClearTempFolder);
    ofAddListener(ofxGifDecodedEvent::DECODED, this, &gifFileHandler::onGifDecoded);
    //ofAddListener(ofxVideoToSequenceEvent::DECODED, this, &gifFileHandler::onGifDecoded);


    // loadVideoFromTempFile();
     playingRandomOnline = true;
}

void gifFileHandler::update()
{
    if((!gifBuffer.isFull()) || (ofGetElapsedTimeMillis() - elapsedTime > 10000))
    {
        elapsedTime = ofGetElapsedTimeMillis();
        if(playingRandomOnline)
            loadFromTempFolder();
        else
            loadFromSourceFolder();
    }
    gifBuffer.update();
}

void gifFileHandler::setSource(int cell, string newSource)
{
    source = newSource; // "temp", localFolder path
    if(source =="temp")
        playingRandomOnline = true;
    else
        playingRandomOnline = false;

    //gifBuffer.clearCell(cell); instead of the code below
    static eventX event;
    ofNotifyEvent(eventX::CLEAR_BUFFER, event); //shouldn't be an event because gifFileHandler is the only responsable.
    isTurboLoading = true;
}

ofImage* gifFileHandler::getImage()
{
    return gifBuffer.getImage();
}

void gifFileHandler::deleteGifFromTemp(string path)
{
    ofLogNotice("gifFileHandler:: Deleting "+path);
    count ++;
    if (!ofFile::doesFileExist(path))
        return;
    ofFile file(path);
    //we should check that we are at the Temp folder...
    file.remove();
}

void gifFileHandler::clearTempFolder()
{
    ofLogNotice("gifFileHandler::clearTempFolder ");
    ofDirectory dir(utils::getTempFolderPath());
    dir.allowExt("gif");
    dir.listDir();

    if(dir.numFiles()<=0)
        return;

    for(int i = 0; i < dir.size(); i++)
    {
        if(ofFile::doesFileExist(dir.getPath(i)))
        {
            ofFile file(dir.getPath(i));
            file.remove();
        }
    }
}

void gifFileHandler::loadFromTempFolder()
{
    ofDirectory dir(utils::getTempFolderPath());
    dir.allowExt("gif");
    dir.listDir();

    if(dir.numFiles()<=0)
        return;

    string path = dir.getPath(0);

    if(!ofFile::doesFileExist(path))
    {
        loadFromTempFolder();
        return;
    }

    ofFile file (path);
    float size = float(file.getSize())/1000000; //size in MB
    if(size > 5)
    {
        //analyticsController::trackError("Gif Over 5Mb", utils::getXAttribute(path,  utils::getPlistData("CFBundleIdentifier")+".originalUrl"));
        deleteGifFromTemp(path);
        loadFromTempFolder();
        return;
    }
    load(path);
}

void gifFileHandler::loadFromSourceFolder()
{
    ofDirectory dir(source);
    dir.allowExt("gif");
    dir.listDir();

    if(dir.numFiles()<=0)
        return;

    string path = dir.getPath(random(0,dir.numFiles()-1));
    if(!ofFile::doesFileExist(path))
    {
        loadFromSourceFolder();
        return;
    }

    ofFile file (path);
    float size = float(file.getSize())/1000000; //size in MB
    if(size > 5)
    {
        //analyticsController::trackError("Gif Over 5Mb", utils::getXAttribute(path,  utils::getPlistData("CFBundleIdentifier")+".originalUrl"));
        loadFromSourceFolder();
        return;
    }
    load(path);
}

void gifFileHandler::loadVideoFromTempFile()
{
    ofDirectory dir(utils::getTempFolderPath());
    dir.allowExt("mp4");
    dir.listDir();

    if(dir.numFiles()<=0)
    {
        ofLogWarning("gifFileHandler::loadVideoFromTempFile() - No mp4 to load");
        return;
    }

    string path = dir.getPath(0);
    if(!ofFile::doesFileExist(path))
    {
        loadFromTempFolder();
        return;
    }

    ofFile file (path);
    videoDecoder.decode("/Users/Xavi/Library/Application Support/Ludovico X/temp/2121.mp4");
}

void gifFileHandler::load(string gifPath)
{
    gifDecoder.decode(gifPath);
}

void gifFileHandler::onGifDecoded(ofxGifDecodedEvent &e)
{
    utils::addGifDecoded();
    if(e.gif->status=="ok")
        gifBuffer.load(e.gif);
    //else
        //analyticsController::trackError("Error decoding", utils::getXAttribute(e.gif->filePath,  utils::getPlistData("CFBundleIdentifier")+".originalUrl"));

    if(playingRandomOnline)
        deleteGifFromTemp(e.path);
}

void gifFileHandler::handleClearTempFolder(eventX &e)
{
    clearTempFolder();
}

int gifFileHandler::random(int min, int max)
{
    return min + (rand() % (int)(max - min +1));
}

Json::Value gifFileHandler::getFolders(string rootDirectory)
{
    ofDirectory dir(rootDirectory);
    dir.listDir();

    Json::Value json;

    if(dir.numFiles()<=0)
    {
        ofLogWarning("No sub folders in '"+rootDirectory+"'");
        return json;
    }

    json = Json::Value(Json::arrayValue);

    for(int i=0; i<dir.numFiles(); i++)
    {
        string path = dir.getPath(i);

        ofDirectory folder(path);
        if(!folder.isDirectory())
            continue;

        folder.listDir();

        string name = dir.getName(i);
        int numFiles = folder.numFiles();

        Json::Value f;
        f["path"]=path;
        f["name"]=name;
        f["numFiles"]=numFiles;
        json.append(f);
        //cout << "\n"<< path<< " " << name;
    }

    return json;
}

