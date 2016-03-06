#include "audioController.h"

void audioController::setup()
{
    ofAddListener(eventX::SET_TEMPO, this, &audioController::handleSetTempo);
    volumeBufferSize = 180;
    int bufferSize = 1024;
    //soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    audioBins.resize(fft->getBinSize());
    initTempo();
}

void audioController::update()
{
    checkTempo();
}

void audioController::audioIn(float * input, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++)
    {
        audioBins[i] = input[i*2]*0.5;//left
        // left[i] = input[i*2]*0.5;//left
        //right[i]	= input[i*2+1]*0.5;//right
    }

    analize(audioBins);

}

void audioController::analize(vector<float> &audioBins)
{
    audioBins = getFTT(audioBins);//microphone only have one channel.

    audioBins = getLogarithmicDistribution(audioBins);
   // substractBy(audioBins, 0.000001);

    applyLogarithmicVolume(audioBins);

    float normalizer = getNormalizedFactor(audioBins);
    float volume = getAvg(audioBins,0, 1) * normalizer;
    float loFreqEnergy = getAvg(audioBins, 0, 0.333) * normalizer;
    float miFreqEnergy = getAvg(audioBins, 0.333, 0.666) * normalizer;
    float hiFreqEnergy = getAvg(audioBins, 0.666, 1) * normalizer;

    static eventX energyEvent;
    energyEvent.floats[0] = volume;
    ofNotifyEvent(eventX::ENERGY, energyEvent);

    static eventX loEnergyEvent;
    loEnergyEvent.floats[0] = loFreqEnergy;
    ofNotifyEvent(eventX::LO_ENERGY, loEnergyEvent);

    static eventX miEnergyEvent;
    miEnergyEvent.floats[0] = miFreqEnergy;
    ofNotifyEvent(eventX::MI_ENERGY, miEnergyEvent);

    static eventX hiEnergyEvent;
    hiEnergyEvent.floats[0] = hiFreqEnergy;
    ofNotifyEvent(eventX::HI_ENERGY, hiEnergyEvent);

    multiplyBy(audioBins, normalizer);

    static eventX spectrumEvent;
    spectrumEvent.floats = audioBins;
    ofNotifyEvent(eventX::FREQUENCY_SPECTRUM, spectrumEvent);
}

vector<float> audioController::getFTT(vector<float> &bins)
{
    fft->setSignal(bins);
    vector<float> newBins;
    newBins.resize(fft->getBinSize());

    float* curFft = fft->getAmplitude();
    memcpy(&newBins[0], curFft, sizeof(float) * fft->getBinSize());
    return newBins;
}

vector<float> audioController::getLogarithmicDistribution(vector<float> &bins)
{
    vector<float> simple;
    simple.resize(log10(bins.size())*10);
   // cout << "\n"<<log10(bins.size())*10;
    int step = 0;
    int index = 0;
    int prevStep = -1;
    float avgStepValue = 0;
    int numStepValues = 0;
    int i = 0;
    while (i < bins.size() )
    {
        step = log10(i+1)*10; // the distribution is logaritmical

        if(step != prevStep)
        {
            numStepValues = 0;
            index++;
        }

        avgStepValue = ((avgStepValue * numStepValues) + bins[i])/(numStepValues+1);
        simple[index] = avgStepValue ; // we set the value of the avarage of all the values from the same step

        prevStep = step;
        i++;
        numStepValues++;

    }
    return simple;
}

void audioController::applyLogarithmicVolume(vector<float> &bins)
{
    for(int i = 0; i <bins.size(); i++)
        bins[i]= log10(1+bins[i]*5000);//the bigger the number is multiplied the smoother is the spectrum.
}

void audioController::multiplyBy(vector<float> &bins, float factor)
{
    for(int i = 0; i <bins.size(); i++)
        bins[i]= bins[i]*factor;
}

void audioController::substractBy(vector<float> &bins, float factor)
{
    for(int i = 0; i <bins.size(); i++)
        bins[i]= bins[i]-factor;
}

float audioController::getNormalizedFactor(vector <float> &bins)
{
    float currentVolume = getAvg(bins, 0, 1);
    //float currentVolume = getRMS(bins);
    volumeBuffer.push_back(currentVolume);
    while (volumeBuffer.size() > volumeBufferSize)
        volumeBuffer.erase(volumeBuffer.begin());
    float avgEnergy = 0;
    for(int i=0; i<volumeBuffer.size();i++)
        avgEnergy+= volumeBuffer[i];
    avgEnergy /=volumeBuffer.size();

    float autoVolumeMultiplier = 1/avgEnergy;
   //  float autoVolumeMultiplier = 1/currentVolume;
    return autoVolumeMultiplier;
}

float audioController::getAvg( vector<float> &values, float min, float max)
{
    int _min = min*values.size();
    int _max = max*values.size();
    float e = 0;

    for ( int i = _min; i<_max; i++)
        e += (values [i]);

    return e/(_max-_min);
}

float audioController::getMax(vector<float> &values, float min, float max)
{
    float maxValue = 0;
    for(int i = 0; i <values.size(); i++)
        if(values[i]>maxValue)
            maxValue = values[i];
    return maxValue;
}

float audioController::getRMS(vector<float> &values, float min, float max)
{
    int _min = min*values.size();
    int _max = max*values.size();
    float rms = 0;

    for ( int i = _min; i<_max; i++)
        rms+= values [i] * values[i];

    return rms/(_max-_min);
}


void audioController::initTempo()
{
    tempoKeys.resize(0);
    tempoKeys.push_back(ofGetElapsedTimeMillis());
    tempoKeys.push_back(ofGetElapsedTimeMillis()+500);//0.5 s = 120bpm
}

void audioController::handleSetTempo(eventX &e)
{
    setTempo();
}

void audioController::setTempo()
{
    long key = ofGetElapsedTimeMillis();

    if (tempoKeys.size()>=2)
    {
        tempoKeys.erase (tempoKeys.begin());
        tempoKeys.push_back(key);
    }
}

void audioController::checkTempo()
{
    long current =  ofGetElapsedTimeMillis();
    long delta=tempoKeys[1]-tempoKeys[0];

    static eventX event;
    event.floats[0] = 0;
    if((current%delta) <= ofGetLastFrameTime()*1000)
        event.floats[0] = 1;

    event.floats[1] = delta;
    ofNotifyEvent(eventX::IS_TEMPO, event);


}
