#include "eventX.h"

// ACTIONS
ofEvent <eventX> eventX::OSC_INPUT;

ofEvent <eventX> eventX::SET_RED;
ofEvent <eventX> eventX::SET_GREEN;
ofEvent <eventX> eventX::SET_BLUE;
ofEvent <eventX> eventX::SET_HUE;
ofEvent <eventX> eventX::SET_BRIGHTNESS;
ofEvent <eventX> eventX::SET_LIGHTNESS;
ofEvent <eventX> eventX::SET_SATURATION;
ofEvent <eventX> eventX::SET_CONTRAST;
ofEvent <eventX> eventX::SET_SPEED;
ofEvent <eventX> eventX::SET_COLOR_TRANSFORM;
ofEvent <eventX> eventX::SET_INVERT;
ofEvent <eventX> eventX::SET_BLUR;
ofEvent <eventX> eventX::SET_STEREO_FX;

ofEvent <eventX> eventX::SET_STYLE;
ofEvent <eventX> eventX::CREATE_STYLE;
ofEvent <eventX> eventX::SAVE_STYLES;

ofEvent <eventX> eventX::ADD_FX;
ofEvent <eventX> eventX::ADD_VAR;
ofEvent <eventX> eventX::REMOVE_FX;
ofEvent <eventX> eventX::MOVE_FX;

ofEvent <eventX> eventX::SET_EXPRESSION;

ofEvent <eventX> eventX::SET_VOLUME_MULTIPLIER;
ofEvent <eventX> eventX::SET_AUTO_VOLUME;
ofEvent <eventX> eventX::FREQUENCY_SPECTRUM;

ofEvent <eventX> eventX::CLEAR_TEMP_FOLDER;

ofEvent <eventX> eventX::SET_FOLDERS;
ofEvent <eventX> eventX::SET_ROOT_FOLDER;
ofEvent <eventX> eventX::ADD_FOLDERS;
ofEvent <eventX> eventX::REMOVE_FOLDERS;
ofEvent <eventX> eventX::TOGGLE_FOLDERS;
ofEvent <eventX> eventX::SET_ANY_CONTENT;
ofEvent <eventX> eventX::SET_FOLDER_STATE;
ofEvent <eventX> eventX::SET_ALL_FOLDERS_STATE;
ofEvent <eventX> eventX::CURRENT_VIDEO_INFO_UPDATE;

ofEvent <eventX> eventX::STAY;
ofEvent <eventX> eventX::PLAY_NEXT;
ofEvent <eventX> eventX::PLAY_PREVIOUS;
ofEvent <eventX> eventX::SET_CONTINUOS_NEXT;
ofEvent <eventX> eventX::CLEAR_BUFFER;

ofEvent <eventX> eventX::SET_FULLSCREEN;
ofEvent <eventX> eventX::TOGGLE_FULLSCREEN;
ofEvent <eventX> eventX::SET_DISPLAY_WINDOW_POSITION;
ofEvent <eventX> eventX::SET_DISPLAY_WINDOW_SIZE;
ofEvent <eventX> eventX::MOVE_WINDOW;
ofEvent <eventX> eventX::ON_MOUSE_MOVE;
ofEvent <eventX> eventX::ON_DISPLAY_WINDOW_RESIZE;

ofEvent <eventX> eventX::SET_BIG_BEAT_VALUE;
ofEvent <eventX> eventX::SET_BEAT_VALUE;

// TRIGGERS
ofEvent <eventX> eventX::LAST_ENERGY_PEAK;
ofEvent <eventX> eventX::BEAT;
ofEvent <eventX> eventX::ENERGY;
ofEvent <eventX> eventX::LO_ENERGY;
ofEvent <eventX> eventX::MI_ENERGY;
ofEvent <eventX> eventX::HI_ENERGY;
ofEvent <eventX> eventX::IS_TEMPO;
ofEvent <eventX> eventX::SET_TEMPO;

ofEvent <eventX> eventX::FX_CONTROLLER_UI_INPUT;
ofEvent <eventX> eventX::VIDEO_CONTROLLER_UI_INPUT;
ofEvent <eventX> eventX::SOCKET_CONNECTED;
ofEvent <eventX> eventX::SOCKET_JSON_BROADCAST;
ofEvent <eventX> eventX::SEND_UI_ACTION_NOW;
ofEvent <eventX> eventX::SEND_UI_ACTION;
ofEvent <eventX> eventX::SEND_UI_DATA_STREAM;


ofEvent <eventX> eventX::UI_INPUT;

ofEvent <eventX> eventX::EXITING;

//
ofEvent <eventX> eventX::ON_URLS_HARD_CODED;
ofEvent <eventX> eventX::ON_URLS_GIPHY;
ofEvent <eventX> eventX::SET_GIPHY_SUBJECT;

