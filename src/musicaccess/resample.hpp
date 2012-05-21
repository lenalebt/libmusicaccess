#ifndef RESAMPLE_HPP
#define RESAMPLE_HPP

#include <stdint.h>

/**
 * @brief Implements a sound resampler from any given sample rate to a
 * 22.05kHz, mono, 16bit format.
 * 
 * @code
 * //assumption: you have some 44.1kHz stereo data in variable "data",
 * //500.000 samples.
 * Resampler22kHzMono resampler;
 * int samplecount = 500000;
 * resampler.resample(44100, &data, samplecount, 2);
 * //now you have samplecount == 125000, saved in data.
 * @endcode
 * 
 * 
 * @bug Currently only works for 44100kHz source formats.
 * 
 * @author Lena Brueder
 * @date 2012-05-21
 */
class Resampler22kHzMono
{
private:
    
public:
    /**
     * @brief Does resampling to 22.05kHz, 16bit, mono.
     * 
     * This function first converts from any channel count to mono
     * through calculating the arithmetic mean of the corresponding samples.
     * Then, a low pass filter is applied to the input signal, and
     * up-/downsampling takes place.
     * 
     * @param fromSampleRate the sample rate of the input signal.
     * @param samplePtr this is input and output at the same time. Since
     *      you will much likely not get the same sample count out of the function,
     *      you will get another memory address.
     * @param sampleCount reads in the current sample count and outputs the new sample count
     *      of the new, resampled data.
     * @param channelCount the channel count of the input data.
     */
    void resample(uint32_t fromSampleRate, int16_t** samplePtr, int& sampleCount, unsigned int channelCount);
};

#endif  //RESAMPLE_HPP 
