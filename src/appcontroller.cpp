#include "appController.h"

void appController::setup()
{
    ofSetEscapeQuitsApp(false);

//#ifdef OF_RELEASE
    // Get the absolute location of the executable file in the bundle.
    CFBundleRef appBundle     = CFBundleGetMainBundle();
    CFURLRef	executableURL = CFBundleCopyExecutableURL(appBundle);
    char execFile[4096];
    if (CFURLGetFileSystemRepresentation(executableURL, TRUE, (UInt8 *)execFile, 4096))
    {
        // Strip out the filename to just get the path
        string strExecFile = execFile;
        int found = strExecFile.find_last_of("/");
        string strPath = strExecFile.substr(0, found);

        // Change the working directory to that of the executable
        if(-1 == chdir(strPath.c_str())) {
            ofLog(OF_LOG_ERROR, "Unable to change working directory to executable's directory.");
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Unable to identify executable's directory.");
    }
    CFRelease(executableURL);

    //ofSetDataPathRoot("lluis2.app/Contents/Resources/");
    ofSetDataPathRoot("../Resources/");
//#endif

    ofSetVerticalSync(true);
    ofSetFrameRate(30);

    ofAddListener(eventX::TOGGLE_FULLSCREEN, this, &appController::handleToggleFullScreen);
    ofAddListener(eventX::SET_FULLSCREEN, this, &appController::handleSetFullScreen);
    ofAddListener(eventX::SET_DISPLAY_WINDOW_POSITION, this, &appController::handleSetDisplayWindowPosition);
    ofAddListener(eventX::SET_DISPLAY_WINDOW_SIZE, this, &appController::handleSetDisplayWindowSize);
    ofAddListener(eventX::MOVE_WINDOW, this, &appController::handleMoveWindow);

    //window.hideTitleBar();

    int x =  (getScreenWidth() - ofGetWindowWidth()) * 0.5;
    int y = (getScreenHeight() - ofGetWindowHeight()) * 0.5;
    ofSetWindowPosition(x, y);
}

void appController::setFullScreen(bool fullScreen)
{
    if(fullScreen)
    {
        prevFullscreenWindowWidth = ofGetWindowWidth();
        prevFullscreenWindowHeight = ofGetWindowHeight();
        prevFullscreenWindowX = ofGetWindowPositionX();
        prevFullscreenWindowY = ofGetWindowPositionY();
    }

    ofSetFullscreen(fullScreen);

    if(!fullScreen)
    {
        ofSetWindowShape(prevFullscreenWindowWidth, prevFullscreenWindowHeight);
        ofSetWindowPosition(prevFullscreenWindowX,prevFullscreenWindowY);
    }
    isFullScreen = fullScreen;
    return;
}



void appController::update()
{
    if(prevWindowHeight != ofGetWindowWidth() || prevWindowHeight != ofGetWindowHeight())
    {
        static eventX event;
        event.ints.push_back(ofGetWindowWidth());
        event.ints.push_back(ofGetWindowHeight());
        ofNotifyEvent(eventX::ON_DISPLAY_WINDOW_RESIZE, event);
    }

    prevWindowWidth = ofGetWindowWidth();
    prevWindowHeight = ofGetWindowHeight();
}

void appController::draw()
{
    return;
    string msg = ofToString(ofGetFrameRate());
    ofDrawBitmapString(msg,  120, 20);
    int height = 20;
    if(isFullScreen)
        return;
    for (int i = 0; i < height; i++)
    {
        int alpha = ofMap(i,0,height,200,30);
        ofSetColor(30, 30, 30, alpha);
        ofDrawRectangle(0,i,ofGetWindowWidth(),1);
    }
    ofSetColor(255, 255, 255);
}

void appController::handleSetFullScreen(eventX &e)
{
    setFullScreen(e.bools[0]);
}

void appController::handleToggleFullScreen(eventX &e)
{
  setFullScreen(!isFullScreen);
}

void appController::handleSetDisplayWindowPosition(eventX &e)
{
    int x = e.ints[0];
    int y = e.ints[1];
    ofSetWindowPosition(0, 0);
}

void appController::handleMoveWindow(eventX &e)
{
    int x = e.floats[0] * getScreenWidth();
    int y = e.floats[1] * getScreenHeight();
    ofSetWindowPosition(x, y);
}

void appController::handleSetDisplayWindowSize(eventX &e)
{
    int width = e.ints[0];
    int height = e.ints[1];
    ofSetWindowShape(width, height);
}

int appController::getScreenWidth()
{
    CGRect screenFrame = CGDisplayBounds(kCGDirectMainDisplay);
    return screenFrame.size.width;
}

int appController::getScreenHeight()
{
    CGRect screenFrame = CGDisplayBounds(kCGDirectMainDisplay);
    return screenFrame.size.height;
}

void appController::mouseMoved(int x, int y)
{
    Json::Value json;
    json["scope"]["app"]["mouseX"] =  ofGetMouseX();
    json["scope"]["app"]["mouseY"] =  ofGetMouseY();

    static eventX jevent;
    jevent.json=json;
    ofNotifyEvent(eventX::SOCKET_JSON_BROADCAST, jevent);

    static eventX event;
    event.ints.push_back(x);
    event.ints.push_back(y);
    ofNotifyEvent(eventX::ON_MOUSE_MOVE, event);
}
