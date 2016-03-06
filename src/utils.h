#pragma once

#include "ofMain.h"
#include "eventX.h"
#include <sys/xattr.h>
#include <errno.h>

class utils: public ofBaseApp
{

public:
    void setup();
    static void setDownloadSpeed(float speed);
    static float getLastDownloadSpeed();
    static float getAvgDownloadSpeed();
    static string getPlistData(string property);
    static string getTempFolderPath(); //were we can save stuff for the session. Currently to store downloaded gif. Its a subfoldero userFolder
    static string getUserFolderPath(); //were to write stuff. config files...
    static string getDesktopImagePath();
    static string getLocalTime();
    static string getLocalTimeZone();
    static bool appIsSandboxed();
    static string getLocale();
    static int getTotalGifsDecoded();
    static int getTotalGifsDownloaded();
    static void addGifDecoded();
    static void addGifDownloaded();
    static bool isFullscreen();
    static bool wasFullscreen();
    static void setXAttribute(string filePath, string attributeName, string value);
    static string getXAttribute(string filePath, string attributeName);
    static void removeXAttribute(string filePath, string attributeName);

private:
    static float lastDownloadSpeed;
    static float avgDownloadSpeed;
    static int numDownloadSpeedSamples;
    static map<string, string> plistInfo;
    static string tempFolder;
    static string userFolder;
    static int totalGifsDecoded;
    static int totalGifsDownloaded;
    static bool hadFullscreen;
};
