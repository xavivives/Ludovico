#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxJSON.h"
#include "utils.h"

class gifSourceGiphy: public ofBaseApp{

public:
    void setup();
    void getMoreUrls();
    bool isLoadingUrls();
    void urlResponse(ofHttpResponse & response);
    void setSubject(string newSubject);
    string getSubject();
    int getTotalCount();
    int getCurrentPage();
    int getItemsPerPage();
    int isActive();


    static const int GIPHY_SUCCESS =1;
    static const int GIPHY_BAD_REQUEST =-1;
    static const int GIPHY_NO_DATA_FOUND = -2; //for currentPage = 0 there are no data
    static const int GIPHY_NO_MORE_DATA_FOUND =-3; //for currentPage >0 there are no tdata
    static const int GIPHY_SUCCESS_NO_DATA =-4;//the request when through but for some reason no data was returned

private:
    string subject;
    int totalCount;
    int currentPage;
    int itemsPerPage;
    string HTTP_ID;
    vector<string> getRandomUrls(int number);
    void processResponse(Json::Value  &json);
    string getGiphyApiUrl(string subject, int offset);
    ofURLFileLoader loader;
    vector<string> urls;
    bool isUrlLoading;
    bool active;
};
