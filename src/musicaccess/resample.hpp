#ifndef RESAMPLE_HPP
#define RESAMPLE_HPP

#include <stdint.h>

namespace musicaccess
{

    float sinc(float x);


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
        /**
         * @brief Takes the given sound samples and changes the sample rate.
         * 
         * The new sample rate will be set through <code>factor</code>.
         * 
         * Usage:
         * @code
         * int sampleCount = 500;
         * int16_t* samples = new int16_t[sampleCount];
         * //resample from 32000Hz to 22050 Hz.
         * resample(&samples, sampleCount, 22050.0/32000.0);
         * //samples now has 345 samples. you can see the value in sampleCount.
         * @endcode
         * 
         * @param samplePtr input/output buffer. Use an array on the heap and pass its address to the function.
         * @param sampleCount the sample count of *samplePtr. Will output the new sample count, too.
         * @param factor the factor used to resample. use <code>toSampleRate/fromSampleRate</code> here.
         * @return if the operation was successful, or not. The operation may
         *      be unsuccessful on not having enough memory.
         */
        bool resample(int16_t** samplePtr, int& sampleCount, double factor);
        bool resample2(int16_t** samplePtr, int& sampleCount, int32_t fromRate, int32_t toRate);
        bool resample(int16_t** samplePtr, int& sampleCount, int32_t fromRate, int32_t toRate);
        /**
         * @brief Takes the given sound samples and uses every nth of it to build a new array.
         * 
         * Usage:
         * @code
         * int sampleCount = 500;
         * int16_t* samples = new int16_t[sampleCount];
         * downsample(&samples, sampleCount, 2);
         * //samples now has 250 samples. you can see the decreased value in sampleCount.
         * @endcode
         * 
         * @param samplePtr input/output buffer. Use an array on the heap and pass its address to the function.
         * @param sampleCount the sample count of *samplePtr. Will output the new sample count, too.
         * @param n every nth sample will be taken to build the new array
         * @return if the operation was successful, or not. The operation may
         *      be unsuccessful on not having enough memory.
         */
        bool downsample(int16_t** samplePtr, int& sampleCount, unsigned int n);
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
         * @return if the operation was successful, or not. The operation may
         *      be unsuccessful on not having enough memory.
         */
        bool resample(uint32_t fromSampleRate, int16_t** samplePtr, int& sampleCount, unsigned int channelCount);
    };
}

#endif  //RESAMPLE_HPP 
