#include "gifSourceGiphy.h"

/*const int gifSourceGiphy::GIPHY_SUCCESS =1;
const int gifSourceGiphy::GIPHY_BAD_REQUEST =-1;
const int gifSourceGiphy::GIPHY_NO_DATA_FOUND = -2; //for currentPage = 0 there are no data
const int gifSourceGiphy::GIPHY_NO_MORE_DATA_FOUND =-3; //for currentPage >0 there are no tdata
const int gifSourceGiphy::GIPHY_SUCCESS_NO_DATA =-4;//the request when through but for some reason no data was returned
*/

void gifSourceGiphy::setup()
{
    HTTP_ID = "giphyUrl";
    itemsPerPage = 100;
    ofRegisterURLNotification(this);
    isUrlLoading = false;
    active = true;
    //setSubject("ludovico");
}

void gifSourceGiphy::urlResponse(ofHttpResponse & response)
{
    if(response.request.name != HTTP_ID)
        return;

    if(!response.status)
        return;

    if (response.status==200)
    {
        ofxJSONElement  json;
        json.parse(response.data.getText());
        processResponse(json);
    }
    else if(response.status == -1)
    {
        ofRemoveURLRequest(response.request.getID());
        static eventX event;
        event.ints[0]=GIPHY_BAD_REQUEST;
        event.strings = urls;
        ofNotifyEvent(eventX::ON_URLS_GIPHY , event);
    }

    isUrlLoading = false;
}

void gifSourceGiphy::getMoreUrls()
{
    isUrlLoading = true;
    cout <<"\nNew call:  "<<getGiphyApiUrl(subject, currentPage);
    ofLoadURLAsync(getGiphyApiUrl(subject, currentPage), HTTP_ID);
    currentPage++;
}

bool gifSourceGiphy::isLoadingUrls()
{
    return isUrlLoading;
}

void gifSourceGiphy::setSubject(string newSubject)
{
    ofLogNotice("gifSourceGiphy::setSubject - "+ newSubject);
    subject = newSubject;
    totalCount = -1;
    currentPage = 0;
    active = true;
    isUrlLoading = false;
}

void gifSourceGiphy::processResponse(Json::Value &json)
{
    static eventX event;

    if(json["meta"]["status"].asInt()==200)
    {
        vector<string> urls;
        for(int i = 0; i < json["data"].size(); ++i)
            urls.push_back(json["data"][i]["images"]["original"]["url"].asString());

        if(urls.size()>0)
            event.ints[0] = GIPHY_SUCCESS;

        event.strings = urls;
        event.json = json["pagination"];
        totalCount = json["pagination"]["total_count"].asInt();
    }
    else
    {
        ofLogError(" gifSourceGiphy::processResponse  "+json["meta"]["msg"].asString()); //probaly "Too many requests - try again later"
        event.ints[0] = GIPHY_SUCCESS_NO_DATA;
        active = false;
    }

    ofNotifyEvent(eventX::ON_URLS_GIPHY, event);
}

string gifSourceGiphy::getGiphyApiUrl(string subject, int page)//TODO: Handle errror;
{
    string key ="VGbAiGOXpim2c";
    string subj=ofJoinString(ofSplitString(subject, " "),"+");

    string url ="http://api.giphy.com/v1/gifs/search?";
    url.append("api_key=");
    url.append(key);
    url.append("&q=");
    url.append(subj);
    url.append("&limit=");
    url.append(ofToString(itemsPerPage));
    url.append("&offset=");
    url.append(ofToString(page*itemsPerPage));
    return url;
    //return "http://api.giphy.com/v1/gifs/search?q=cartoon+dance&api_key=dc6zaTOxFJmzC&limit=50&offset=0";
}

vector<string> gifSourceGiphy::getRandomUrls(int number)
{
    vector<string> randomUrls;
    randomUrls.resize(0);
    for(int i = 0; i<number; i++)
    {
        int random = ofRandom(urls.size());
        randomUrls.push_back(urls[random]);
    }
    return randomUrls;
}

int gifSourceGiphy::getTotalCount()
{
    return totalCount;
}

string gifSourceGiphy::getSubject()
{
    return subject;
}

int gifSourceGiphy::getCurrentPage()
{
    return currentPage;
}

int gifSourceGiphy::getItemsPerPage()
{
    return itemsPerPage;
}
int gifSourceGiphy::isActive()
{
    return active;
}
