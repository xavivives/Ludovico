
#include "gifPlayer.h"

void gifPlayer::setup()
{
    isPlaying = true;
    gif =  make_shared<ofxGifType>();
}

void gifPlayer::load( shared_ptr<ofxGifType> loadedGif)
{
    gif = loadedGif;
    updateCurrentVideoInfo();
}

ofImage * gifPlayer::getImage()
{
    return &image;
}

void gifPlayer::update()
{
    if(!isPlayable())
        return;

    if(isPlaying)
    {
        currentFrame = getCurrentFrame();

        if((currentFrame!=prevFrame) || (prevFilePath != gif->filePath))
            image.setFromPixels(gif->frames[currentFrame]);

        prevFrame = currentFrame;
    }
}

void gifPlayer::updateCurrentVideoInfo()
{
    Json::Value gifInfo;
    gifInfo["name"] = gif->fileName;
    gifInfo["duration"] = gif->duration;
    gifInfo["width"] = gif->width;
    gifInfo["height"] = gif->height;
    static eventX event;
    event.json = gifInfo;
    ofNotifyEvent(eventX::CURRENT_VIDEO_INFO_UPDATE, event);
}

int gifPlayer::getCurrentFrame()
{
    float ph =  parameterTransformer::getPlayHead();

    bool countBackwards = false;
    if (ph<0)//if its negative we start counting from the end
        countBackwards = true;

    ph = abs(ph);

    int frameDelay = gif->delays[0]; //we assume all frames have the same delay... :/
    int ms = ph;//miliseconds since start

    if(ph >0 && ph<1 ) //if its between 0-1 we will use the value as a percentage of the total gif time
        ms = gif->delays[0] * gif->length * ph;

    if(frameDelay==0)
        frameDelay=100;

    int frame = ms/frameDelay;

    frame = frame % gif->length;

    if(countBackwards)
        frame = gif->length-1 - frame;
    if(frame<0)
    {
        frame = 0;
    }

    return frame;
}

bool gifPlayer::isPlayable()
{
    if(gif->status != "ok")
        return false;
    else
        return true;
}

 shared_ptr<ofxGifType> gifPlayer::getPlayingGif()
{
    return gif;
}
