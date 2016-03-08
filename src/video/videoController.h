#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "viz/fxController.h"
#include "video/gifFileHandler.h"
#include "video/gifSourcesController.h"
#include "uiConnector.h"

class videoController: public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();

private:
    void setSource(int cell, string source);
    void handleVideoControllerUiInput(eventX &e);

    gifFileHandler fileHandler;
    gifSourcesController sourcesController;
    fxController fx;

};


