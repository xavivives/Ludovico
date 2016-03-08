
#include "videoController.h"

void videoController::setup()
{
    fx.setup();
    fileHandler.setup();
    sourcesController.setup();

    ofAddListener(eventX::VIDEO_CONTROLLER_UI_INPUT, this, &videoController::handleVideoControllerUiInput);

    //setSource(-1, "/Gifs/creepy");
}

void videoController::update()
{
    fileHandler.update();
    sourcesController.update();
}

void videoController::draw()
{
    fx.transform(fileHandler.getImage());
}

void videoController::setSource(int cell, string source)
{
    fileHandler.setSource(cell, source);
}

void videoController::handleVideoControllerUiInput(eventX &e)
{
    string actionId = e.strings[0];
    Json::Value data = e.json;

    if(actionId == "getLocalFolders")
        uiConnector::performAction("localFolders", "setLocalFolders", fileHandler.getFolders("/Gifs"));
    if(actionId == "setFolder")
        fileHandler.setSource(-1, data.asString());
    if(actionId == "setSubject")
    {
        static eventX event;
        event.strings[0] = data.asString();
        ofNotifyEvent(eventX::SET_GIPHY_SUBJECT, event); //TODO. Call directly. delete event type
        fileHandler.setSource(-1, "temp");
    }

}
