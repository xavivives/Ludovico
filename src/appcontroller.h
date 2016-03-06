#pragma once

#include "ofMain.h"
#include "eventX.h"

class appController: public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();
    int getScreenWidth();
    int getScreenHeight();
    void mouseMoved(int x, int y);

private:
    int prevWindowWidth;//used to check changes on resizing
    int prevWindowHeight;
    int prevFullscreenWindowWidth;//used to go back to the same size before fullscreen
    int prevFullscreenWindowHeight;
    int prevFullscreenWindowX;
    int prevFullscreenWindowY;
    void handleSetFullScreen(eventX &e);
    void handleToggleFullScreen(eventX &e);
    void handleSetDisplayWindowPosition(eventX &e);
    void handleSetDisplayWindowSize(eventX &e);
    void handleMoveWindow(eventX &e);

    void setFullScreen(bool fullscreen);
    bool isFullScreen;

    int displayWindowWidth;
    int displayWindowHeight;
    string windowAppName;
};
