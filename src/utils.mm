
#include "utils.h"

float utils::lastDownloadSpeed = -1;
float utils::avgDownloadSpeed = -1;
int utils::numDownloadSpeedSamples = 0;
map <string, string> utils::plistInfo;
string utils::tempFolder;
string utils::userFolder;
int utils::totalGifsDecoded;
int utils::totalGifsDownloaded;
bool utils::hadFullscreen = false;


void utils::setup()
{
    //Info.plist
    ofBuffer buffer = ofBufferFromFile("../Info.plist");
    string xmlString = buffer.getText();

    ofXml xml;
    bool succes = false;
    succes = xml.loadFromBuffer(xmlString);

    if(!succes)
        ofLogError("Utils: Was not possible to parse the Info.plist");

    succes = xml.setTo("dict");

    int num = xml.getNumChildren()/2;

    for(int i = 0; i<num ;i++)
    {
        xml.setTo("key["+ofToString(i)+"]");
        string key = xml.getValue("");
        xml.setToSibling();
        string value = xml.getValue("");
        xml.setToParent();
        plistInfo[key] = value;
        //cout << "\n "<< key << ": "<< value;
    }

    //Temp Folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *applicationSupportDirectory = [paths firstObject];
    userFolder = ([applicationSupportDirectory UTF8String]);

    if(!appIsSandboxed())
    {
        userFolder.append("/").append(utils::getPlistData("CFBundleExecutable"));
        ofDirectory dir(userFolder);
        if(!dir.exists())
            dir.createDirectory(userFolder);
    }

    tempFolder = userFolder;
    tempFolder = tempFolder.append("/temp");
    ofDirectory dir2(tempFolder);
    if(!dir2.exists())
       dir2.createDirectory(tempFolder);
}

float utils::getLastDownloadSpeed()
{
    return lastDownloadSpeed;
}

float utils::getAvgDownloadSpeed()
{
    return avgDownloadSpeed;
}

void utils::setDownloadSpeed(float speed)
{
    lastDownloadSpeed = speed;
    avgDownloadSpeed = ((avgDownloadSpeed * numDownloadSpeedSamples) + speed)/(numDownloadSpeedSamples +1);
    numDownloadSpeedSamples ++;
}

string utils::getPlistData(string property)
{
    if ( plistInfo.find(property) == plistInfo.end() )
    {
        cout << "\nUtils: Property '"<< property<<"' does not exist";
        return "";
    } else {
        return plistInfo.find(property)->second;
    }
}

string utils::getUserFolderPath()
{
    return userFolder;
}


string utils::getTempFolderPath()
{
    return tempFolder;
}

string utils::getDesktopImagePath()
{
    /*NSURL *imageURL = [[NSWorkspace sharedWorkspace] desktopImageURLForScreen:[NSScreen mainScreen]];
    return ([[imageURL absoluteString] UTF8String]);
     */
    return "";
}
string utils::getLocalTimeZone()
{
    NSString * timeZoneName= [[NSTimeZone systemTimeZone]name];
    return ([timeZoneName UTF8String]);
}

string utils::getLocalTime()
{
    NSDate *today = [NSDate date];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setTimeStyle:NSDateFormatterShortStyle];
    [dateFormatter setDateFormat:@"HH:mm:ss"];

    NSString *currentTime = [dateFormatter stringFromDate:today];
    [dateFormatter release];
    return ([currentTime UTF8String]);
}


bool utils::appIsSandboxed()
{
    NSDictionary* environ = [[NSProcessInfo processInfo] environment];
    return (nil != [environ objectForKey:@"APP_SANDBOX_CONTAINER_ID"]);
}

string utils::getLocale()
{
    //returning "" :(
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:0];
     return ([language UTF8String]);
     return ([[[NSLocale currentLocale] localeIdentifier] UTF8String]);

}

int utils::getTotalGifsDecoded()
{
    return totalGifsDecoded;
}

int utils::getTotalGifsDownloaded()
{
    return totalGifsDownloaded;
}

void utils::addGifDecoded()
{
    utils::totalGifsDecoded++;
}

void utils::addGifDownloaded()
{
    totalGifsDownloaded++;
}

bool utils::wasFullscreen()
{
    return hadFullscreen;
}

bool utils::isFullscreen()
{

    if((ofGetWindowWidth()!=0 || ofGetWindowHeight()!=0) &&(ofGetWindowWidth() == ofGetScreenWidth()) && (ofGetWindowHeight()==ofGetScreenHeight()))
    {
        hadFullscreen = true;
        return true;
    }

    return false;
}

void utils::setXAttribute(string filePath, string attributeName, string value)
{
    NSString *nsstring = [NSString stringWithCString:value.c_str()encoding:[NSString defaultCStringEncoding]];
    NSData* encodedString = [nsstring dataUsingEncoding:NSUTF8StringEncoding];
    int rc = setxattr(filePath.c_str(), attributeName.c_str(), [encodedString bytes], [encodedString length], 0, 0);
    if (rc==-1)
        ofLogWarning("ofxXAttr: Error writing xAttr: " + string(strerror(errno)) +" - File path: "+ filePath + " - Attribute: " + attributeName+ " - Value: " + value);
}

string utils::getXAttribute(string filePath, string attributeName)
{
    int bufferLength = getxattr(filePath.c_str(), attributeName.c_str(), NULL, 0, 0, 0);
    if (bufferLength == -1)
    {
        string error = strerror(errno);
        ofLogWarning("ofxXAttr: Error reading xAttr: " + string(strerror(errno)) +" - File path: "+ filePath + " - Attribute " + attributeName);
        return "";
    }

    char *buffer =(char*) malloc(bufferLength);

    int s = getxattr(filePath.c_str(), attributeName.c_str(),buffer, bufferLength, 0, 0);

    if(s > 0){

        char *value = (char*)malloc(s+1);
        strncat(value, buffer, s);
        free(buffer);
        string str(value);
        return str;
    }
    else
        ofLogWarning("ofxXAttr: Error reading X Attribute: " + string(strerror(errno)) +" - File path: "+ filePath + " - Attribute " + attributeName);

    return "";
}

void utils::removeXAttribute(string filePath, string attributeName)
{
    int rc= removexattr(filePath.c_str(), attributeName.c_str(), 0);

    if(rc==-1)
        ofLogWarning("ofxXAttr: Error deleting xAttr: " + string(strerror(errno)) +" - File path: "+ filePath + " - Attribute " + attributeName);
}



