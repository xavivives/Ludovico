#pragma once
#include "ofMain.h"
#include "json.h"

class eventX : public ofEventArgs {

public:
    vector<float> floats;
    vector<int> ints;
    vector<string> strings;
    vector<bool> bools;
    string oscAddress;
    Json::Value json;

    eventX() {
        floats.resize(1);
        ints.resize(1);
        strings.resize(1);
        bools.resize(1);
    }

    static ofEvent <eventX> OSC_INPUT;

    static ofEvent <eventX> SET_RED;
    static ofEvent <eventX> SET_GREEN;
    static ofEvent <eventX> SET_BLUE;

    static ofEvent <eventX> SET_HUE;
    static ofEvent <eventX> SET_SATURATION;
    static ofEvent <eventX> SET_BRIGHTNESS;
    static ofEvent <eventX> SET_LIGHTNESS;
    static ofEvent <eventX> SET_CONTRAST;
    static ofEvent <eventX> SET_COLOR_TRANSFORM;
    static ofEvent <eventX> SET_INVERT;
    static ofEvent <eventX> SET_BLUR;
    static ofEvent <eventX> SET_STEREO_FX;

    static ofEvent <eventX> SET_STYLE;
    static ofEvent <eventX> CREATE_STYLE;
    static ofEvent <eventX> SAVE_STYLES;

    static ofEvent <eventX> ADD_FX;
    static ofEvent <eventX> ADD_VAR;
    static ofEvent <eventX> REMOVE_FX;
    static ofEvent <eventX> MOVE_FX;

    static ofEvent <eventX> SET_EXPRESSION;

    static ofEvent <eventX> SET_SPEED;

    static ofEvent <eventX> LAST_ENERGY_PEAK;
    static ofEvent <eventX> BEAT;
    static ofEvent <eventX> ENERGY;
    static ofEvent <eventX> LO_ENERGY;
    static ofEvent <eventX> MI_ENERGY;
    static ofEvent <eventX> HI_ENERGY;
    static ofEvent <eventX> IS_TEMPO;
    static ofEvent <eventX> SET_TEMPO;


    static ofEvent <eventX> SET_VOLUME_MULTIPLIER;
    static ofEvent <eventX> SET_AUTO_VOLUME;
    static ofEvent <eventX> FREQUENCY_SPECTRUM;

    static ofEvent <eventX> CLEAR_TEMP_FOLDER;

    static ofEvent <eventX> SET_FOLDERS;
    static ofEvent <eventX> SET_ROOT_FOLDER;
    static ofEvent <eventX> ADD_FOLDERS;
    static ofEvent <eventX> REMOVE_FOLDERS;
    static ofEvent <eventX> TOGGLE_FOLDERS;
    static ofEvent <eventX> SET_ANY_CONTENT;
    static ofEvent <eventX> SET_FOLDER_STATE;
    static ofEvent <eventX> SET_ALL_FOLDERS_STATE;
    static ofEvent <eventX> CURRENT_VIDEO_INFO_UPDATE;


    static ofEvent <eventX> STAY;
    static ofEvent <eventX> PLAY_NEXT;
    static ofEvent <eventX> PLAY_PREVIOUS;
    static ofEvent <eventX> CLEAR_BUFFER;
    static ofEvent <eventX> SET_CONTINUOS_NEXT;

    static ofEvent <eventX> SET_FULLSCREEN;
    static ofEvent <eventX> TOGGLE_FULLSCREEN;
    static ofEvent <eventX> SET_DISPLAY_WINDOW_POSITION;
    static ofEvent <eventX> SET_DISPLAY_WINDOW_SIZE;
    static ofEvent <eventX> MOVE_WINDOW;
    static ofEvent <eventX> ON_MOUSE_MOVE;
    static ofEvent <eventX> ON_DISPLAY_WINDOW_RESIZE;


    static ofEvent <eventX> SET_BIG_BEAT_VALUE;
    static ofEvent <eventX> SET_BEAT_VALUE;

    static ofEvent <eventX> FX_CONTROLLER_UI_INPUT;
    static ofEvent <eventX> VIDEO_CONTROLLER_UI_INPUT;
    static ofEvent <eventX> SOCKET_CONNECTED;
    static ofEvent <eventX> SOCKET_JSON_BROADCAST;
    static ofEvent <eventX> SEND_UI_ACTION_NOW;
    static ofEvent <eventX> SEND_UI_ACTION;
    static ofEvent <eventX> SEND_UI_DATA_STREAM;

    static ofEvent <eventX> UI_INPUT;

    static ofEvent <eventX> EXITING;

    static ofEvent <eventX> ON_URLS_HARD_CODED;
    static ofEvent <eventX> ON_URLS_GIPHY;
    static ofEvent <eventX> SET_GIPHY_SUBJECT;
};
