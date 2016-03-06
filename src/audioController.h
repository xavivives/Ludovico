#pragma once

#include "ofMain.h"
#include "eventX.h"
#include "ofxFft.h"

class audioController : public ofBaseApp{
public:
    void setup();
    void update();
    void analize(vector<float> &audioBins);
    void audioIn(float * input, int bufferSize, int nChannels);

private:
    ofxFft* fft;
    vector<float> audioBins;
    ofSoundStream soundStream;

    vector<float> getFTT(vector<float> &bins);
    vector<float> getLogarithmicDistribution(vector<float> &bins);
    void applyLogarithmicVolume(vector<float> &bins);
    void multiplyBy(vector<float> &bins, float factor);
    void substractBy(vector<float> &bins, float factor);

    float getMax(vector<float> &values ,float min = 0, float max=1 );
    float getRMS(vector<float> &values, float min = 0, float max=1);
    float getAvg(vector<float> &values ,float min = 0, float max=1 );

    float getNormalizedFactor(vector <float> &bins);

    int volumeBufferSize;
    vector <float> volumeBuffer;
    ofMutex mutex;

    void initTempo();
    void checkTempo();
    void setTempo();
    void handleSetTempo(eventX &e);
    vector <long> tempoKeys;
};
