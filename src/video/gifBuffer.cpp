#include "gifBuffer.h"

void gifBuffer::setup()
{
    player.setup();
    maxBuffer = 10;
    toDelete = NULL;
    ofAddListener(eventX::CLEAR_BUFFER, this, &gifBuffer::handleClearBuffer);
}

void gifBuffer::update()
{
    if(buffer.size()>maxBuffer)
        removeGifFromMemoryAndBuffer(0);


    if(parameterTransformer::getChangeVideo())
        playNext();

    player.update();
}

bool gifBuffer::isFull()
{
    return (buffer.size()>=maxBuffer);
}

ofImage* gifBuffer::getImage()
{
    return player.getImage();
}

void gifBuffer::clearBuffer()
{
    ofLogNotice("gifBuffer::clearBuffer()");
    while(buffer.size()>0)
        removeGifFromMemoryAndBuffer();
}

void gifBuffer::playNext()
{
    if(buffer.size() <= 0)
        return;

    int r = random();
    player.load(buffer[r]);
}

void gifBuffer::load(shared_ptr<ofxGifType> newGif)
{
    buffer.push_back(newGif);
}

void gifBuffer::removeGifFromMemoryAndBuffer(int index)
{
    if( toDelete != NULL && player.getPlayingGif() != toDelete)
    {
        //delete toDelete;
        toDelete = NULL;
    }

    if(player.getPlayingGif() == buffer[index]) //we make sure is not the one being played right now
    {
        toDelete  = buffer[index];
        buffer.erase(buffer.begin()+index);
        ofLogWarning("gifBuffer::tryRemoveGifs "+ player.getPlayingGif()->fileName);
    }
    else
    {
        //delete buffer[index];
        buffer.erase(buffer.begin());
    }
}

int gifBuffer::random()
{
    if(buffer.size() == 1)
        return 0;
    return (rand() % (int)(buffer.size()-2 +1));
}

void gifBuffer::handleClearBuffer(eventX &e)
{
    clearBuffer();
}
