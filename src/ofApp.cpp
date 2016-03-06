#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel(OF_LOG_WARNING);

    appController.setup();
    utilsController.setup();
    ofxMacSystemProfiler::init();
    /*

    analytics.setup();
    uiController.setup();
    socket.setup();


    //connector.setup();
    //osc.setup();
    audio.setup();
    video.setup();

    parameters.setup();

    soundStream.setup(this, 0, 2, 44100, 2048, 4);
    purchases.setup();

    pressedKeys.clear();
    window.disableFullScreen();
    window.paintTitleBar();

    elapsedTime = ofGetElapsedTimeMillis();
    logoImage.load("ui/icon_128x128.png");
    setupUi();
    */
}



void ofApp::update()
{
    /*analytics.update();
    audio.update();//needed so it doesn't crash
    parameters.update();
    video.update();
    */
    appController.update();
    /*
    socket.update();
    */
}

void ofApp::draw()
{
    /*
    ofBackground(255, 30, 100);
    logoImage.draw((ofGetWindowWidth() -logoImage.getWidth())*0.5,(ofGetWindowHeight() - logoImage.getHeight())*0.5);

    video.draw();
    */
    appController.draw();

}

/*void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    vector<float> audioBins;
    audioBins.resize(bufferSize);
    for (int i = 0; i < bufferSize; i++)
    {
        audioBins[i] = input[i*2]*0.5;//left
        // left[i] = input[i*2]*0.5;//left
        //right[i]	= input[i*2+1]*0.5;//right
    }

    audio.analize(audioBins);
}*/

/*void ofApp::keyPressed(int key)
{
    if(key==118)//v
    {
       ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else if(key==119)//w
    {
        ofSetLogLevel(OF_LOG_WARNING);
    }
    else if(key==110)//n
    {
        ofSetLogLevel(OF_LOG_NOTICE);
    }

    if(key==27)//ESC
    {
        static eventX event;
        event.bools[0] = false;
        ofNotifyEvent(eventX::SET_FULLSCREEN, event);
    }
}

void ofApp::keyReleased(int key)
{
    for(int i =0; i<pressedKeys.size(); i++)
    {
        if(pressedKeys[i] == key)
            pressedKeys.erase(pressedKeys.begin()+i);
    }
}

bool ofApp::isKeyPressed(int key)
{
    for(int i =0; i<pressedKeys.size(); i++)
    {
        if(pressedKeys[i] == key)
            return true;
    }
    return false;
}

*/
void ofApp::mouseMoved(int x, int y)
{
    appController.mouseMoved(x, y);
}

/*
void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
    //Double click
    if (ofGetElapsedTimeMillis() - elapsedTime < 500)
    {
        static eventX event;
        ofNotifyEvent(eventX::TOGGLE_FULLSCREEN, event);
        elapsedTime = 0;
    }

    elapsedTime = ofGetElapsedTimeMillis ();
}

void ofApp::windowResized(int w, int h)
{

}

void ofApp::gotMessage(ofMessage msg)
{

}

void ofApp::dragEvent(ofDragInfo info) {
    for (int i = 0; i < info.files.size(); i++)
        cout << info.files[i] << endl;

    static eventX event;
    event.strings = info.files;
    ofNotifyEvent(eventX::SET_ANY_CONTENT, event);

    //printf("dragged to position %3.1f, %3.1f\n", info.position.x, info.position.y);
}

void ofApp::exit()
{
    //static eventX event;
    //ofNotifyEvent(eventX::EXITING, event);
}

void ofApp::createUiWindow()
{
    ofGLFWWindowSettings settings;
    settings.width = 300;
    settings.height = 500;
    //ofSetWindowPosition(0, 0);
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    shared_ptr<ofAppWebUi> secondApp(new ofAppWebUi);
    ofRunApp(guiWindow, secondApp);
    ////secondApp->setPath(socket.getLocalhostUrl());
    secondApp->setPath(socket.getLocalhostUrl());
}

void ofApp::createBlankUi()
{
    webUi.setup();
    webUi.load(socket.getLocalhostUrl());
    webUi.showBackground(true);
    webUi.show(true);
}

void ofApp::setupUi()
{
    //createBlankUi();
    //createUiWindow();
}*/
