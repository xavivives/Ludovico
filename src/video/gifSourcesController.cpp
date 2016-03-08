#include "gifSourcesController.h"

void gifSourcesController::setup()
{
    ofRegisterURLNotification(this); //when ofLoadURLAsync() is called,  urlResponse() is returned
    isGifLoading = false;
    urls.resize(0);

    setupSources();
    currentSource = HARD_CODED;
}

void gifSourcesController::setupSources()
{
    sourceHardCoded.setup();
    ofAddListener(eventX::ON_URLS_HARD_CODED, this, &gifSourcesController::handleHardCodedUrls);

    sourceGiphy.setup();
    ofAddListener(eventX::ON_URLS_GIPHY, this, &gifSourcesController::handleGiphyUrls);
    ofAddListener(eventX::SET_GIPHY_SUBJECT, this, &gifSourcesController::handleSetGiphySubject);
}

void gifSourcesController::update()
{
    if(!isGifLoading && urls.size()>0)
        if(!maxGifLoaded())
            loadNextGif();

    if (currentSource == GIPHY)
    {
        if(giphyNeedsUrls())
            sourceGiphy.getMoreUrls();
    }
    else if (currentSource == HARD_CODED)
    {
        if(urls.size()<20 && !sourceHardCoded.isLoadingUrls())
            sourceHardCoded.getMoreUrls();
    }
}

void gifSourcesController::urlResponse(ofHttpResponse & response) {
    ofxJSONElement  json;
    if(!response.status)
        return;

    if (response.status==200)
    {
        if(response.request.name == "gif")
        {
            saveGif(response);
        }
    } else
    {
        if(response.status == -1)
        {
            ofRemoveURLRequest(response.request.getID());
            //ofRemoveAllURLRequests(); //we have to do it that way since it automatically tries to load again and create multiple requests
            isGifLoading = false;
        }
    }
    if(response.request.name == "gif")
        isGifLoading = false;
}

void gifSourcesController::saveGif(ofHttpResponse response)
{
    int found = response.request.url.find_last_of("/")+1;
    string fileName = ofToString((int) ofRandom(9999))+"_"+response.request.url.substr(found);

    string fileDir =utils::getTempFolderPath()+"/"+fileName;

    bool fileWritten = ofBufferToFile(fileDir, response.data);
    if(fileWritten)
        utils::addGifDownloaded();

    //set file metadata
    utils::setXAttribute(fileDir, utils::getPlistData("CFBundleIdentifier")+".originalUrl", response.request.url);

    ofFile file (fileDir);
    float elapsed = float(ofGetElapsedTimeMillis()-startLoadingTimeStamp) / 1000; //seconds downloading;
    float size = float(file.getSize())/1000000*8; //size in megabits (Mb);
    float speed =size / elapsed; //in mbps
    utils::setDownloadSpeed(speed);

    if(size/8>5) //in MB
        ofLogWarning ("\n Big Gif: "+ ofToString(size/8) + " " + fileDir);
}

void gifSourcesController::loadNextGif()
{
    if(urls.size()<=0)
        return;

    int random = ofRandom(urls.size());

    string url = urls[random];
    urls.erase (urls.begin()+random);

    if(url == "")
        return;

    startLoadingTimeStamp = ofGetElapsedTimeMillis();
    ofLoadURLAsync(url, "gif");
    isGifLoading = true;
}

bool gifSourcesController::maxGifLoaded()
{
    ofDirectory dir(utils::getTempFolderPath());
    dir.allowExt("gif");
    dir.listDir();
    if(dir.numFiles()>20)
        return true;
    else
        return false;
}

void gifSourcesController::handleHardCodedUrls(eventX &e)
{
    urls.insert(urls.end(), e.strings.begin(), e.strings.end());
}

void gifSourcesController::handleSetGiphySubject(eventX &e)
{
    if(e.strings[0]=="")
    {
        currentSource = HARD_CODED;
        return;
    }
    currentSource = GIPHY;

    static eventX clearTempEvent;
    ofNotifyEvent(eventX::CLEAR_TEMP_FOLDER, clearTempEvent);

    static eventX clearBufferEvent;
    //ofNotifyEvent(eventX::CLEAR_BUFFER, clearBufferEvent);

    sourceGiphy.setSubject(e.strings[0]);
    urls.clear();
}

bool gifSourcesController::giphyNeedsUrls()
{
    if(urls.size()>20)
        return false;

    if(!sourceGiphy.isActive())
        return false;

    if(sourceGiphy.isLoadingUrls())
        return false;

    else if(sourceGiphy.getTotalCount() == -1)//we haven't check how many gifs the subject has.
        return true;

    if(sourceGiphy.getTotalCount() < sourceGiphy.getCurrentPage()*sourceGiphy.getItemsPerPage())
        return false;

    return true;
}

void gifSourcesController::handleGiphyUrls(eventX &e)
{
    if(e.ints[0] == gifSourceGiphy::GIPHY_SUCCESS)
    {
        urls.insert(urls.end(), e.strings.begin(), e.strings.end());
    }
    else
    {
        if(e.json["total_count"].asInt() <=0)
        {
            ofLogWarning("gifSourcesController::handleGiphyUrls,  No gifs for current subject");
        }
        else if(e.json["count"].asInt() <= 0)
        {
            ofLogWarning("gifSourcesController::handleGiphyUrls, No more gifs for current page");
        }
    }
}

