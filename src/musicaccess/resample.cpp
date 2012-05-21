#include "resample.hpp" 

#include "filter.hpp"
//for NULL
#include <cstring>

void Resampler22kHzMono::resample(uint32_t fromSampleRate, int16_t** samplePtr, int& sampleCount, unsigned int channelCount)
{
    //first convert to mono - we do not need stereo or more channels.
    unsigned int frameCount = sampleCount/channelCount;
    int16_t* monoSamples = new int16_t[frameCount];
    int16_t* samples = *samplePtr;
    for (unsigned int i = 0; i < frameCount; i++)
    {
        int32_t tmpVal = 0;
        int offset = channelCount * i;
        for (unsigned int j = 0; j < channelCount; j++)
        {
            tmpVal += samples[offset + j];
        }
        monoSamples[i] = tmpVal / channelCount;
    }
    //should have mono samples now. we can discard the old samples.
    delete[] samples;
    sampleCount = frameCount;
    *samplePtr = monoSamples;
    samples = monoSamples;
    
    int16_t* newSamples = NULL;
    
    //first, apply low-pass filtering - we need this prior to resampling.
    IIRFilter* filter = IIRFilter::createLowpassFilter(11025, fromSampleRate);
    filter->apply(monoSamples, sampleCount);
    
    if (fromSampleRate == 44100)
    {
        newSamples = new int16_t[sampleCount/2];
        sampleCount /= 2;
        for (int i=0; i<sampleCount; i++)
        {
            newSamples[i] = monoSamples[i*2];
        }
        
    }
    else
    {
        
    }
}
