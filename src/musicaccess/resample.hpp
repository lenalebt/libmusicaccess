#ifndef RESAMPLE_HPP
#define RESAMPLE_HPP

#include <stdint.h>

class Resampler22kHzMono
{
private:
    
public:
    void resample(uint32_t fromHz, int16_t** samplePtr, int& sampleCount, unsigned int channelCount);
};

#endif  //RESAMPLE_HPP 
