#include "resample.hpp" 

#include "filter.hpp"
//for NULL
#include <cstring>

#include <cmath>
#include <iostream>

#include <sndfile.h>

#define SINC_IMPLEMENTATION 0

namespace musicaccess
{

    #if SINC_IMPLEMENTATION==0
        //Implementation 0 is without any optimizations
        double sinc(double x)
        {
            if (x==0.0)
                return 1.0;
            
            x *= M_PI;
            return std::sin(x)/x;
        }
    #elif SINC_IMPLEMENTATION==1
        //Implementation 1 is with a lookup table for sin
        #define SIN_LUT_RESOLUTION 100
        float lut_sin[SIN_LUT_RESOLUTION+1] = {-100.0f};
        void lutsin_create()
        {
            float x;
            for (int i=0; i<SIN_LUT_RESOLUTION+1; i++)
            {
                x = M_PI_2 * i / SIN_LUT_RESOLUTION;
                lut_sin[i] = sin(x);
            }
        }
        /* 
         * This implementation uses a look-up table and linear interpolation.
         */
        float lutsin(float x)
        {
            if (lut_sin[0] == -100.0f)
                lutsin_create();
            
            float pos;
            int intpos;
            float val1, val2;
            
            if (x > M_PI)
            {
                x = x - int(x/(2*M_PI)+0.5)*(2*M_PI);
            }
            else if (x < -M_PI)
            {
                x = x + int(-x/(2*M_PI)+0.5)*(2*M_PI);
            }
            
            if (x > 0)
            {
                if (x<M_PI_2)
                {
                    pos = (x*SIN_LUT_RESOLUTION/M_PI_2);
                    intpos = pos;
                    val1 = lut_sin[intpos];
                    val2 = lut_sin[intpos+1];
                }
                else
                {
                    pos = ((M_PI - x)*SIN_LUT_RESOLUTION/M_PI_2);
                    intpos = pos;
                    val1 = lut_sin[intpos];
                    val2 = lut_sin[intpos+1];
                }
            }
            else
            {
                if (x>-M_PI_2)
                {
                    pos = (-x*SIN_LUT_RESOLUTION/M_PI_2);
                    intpos = pos;
                    val1 = -lut_sin[intpos];
                    val2 = -lut_sin[intpos+1];
                }
                else
                {
                    pos = ((M_PI + x)*SIN_LUT_RESOLUTION/M_PI_2);
                    intpos = pos;
                    val1 = -lut_sin[intpos];
                    val2 = -lut_sin[intpos+1];
                }
            }
            
            return val1 + (val2-val1) * (pos-intpos);
        }
        
        float sinc(float x)
        {
            x *= M_PI;
            return lutsin(x)/x;
        }
    #endif  //USE_LUT_SINC

    /**
     * @brief Calculates the greatest common divisor of two integers.
     * 
     * It is an iterative implementation.
     * 
     * @param a an integer value.
     * @param b an integer value.
     * @return the gcd of a and b.
     */
    int64_t gcd(int64_t a, int64_t b)
    {
      int64_t c = a % b;
      while(c != 0)
      {
        a = b;
        b = c;
        c = a % b;
      }
      return b;
    }

    template<typename T>
    inline
    T getArrayElement(T* array, int element, int count)
    {
        if (element < 0)
            return 0.0;
        else if (element >= count)
            return 0.0;
        else
            return array[element];
    }


    bool Resampler22kHzMono::resample(int16_t** samplePtr, int& sampleCount, double factor)
    {
        //use this to speed up for loops, as multiplication is faster than division
        double divisor = 1.0 / factor;
        
        int16_t* newSamples = NULL;
        int newSampleCount = int(sampleCount * factor) + 1;
        newSamples = new int16_t[newSampleCount];
        if (!newSamples)
            return false;
        
        double newValue;
        for (int i=0; i<newSampleCount; i++)
        {
            newValue = 0.0;
            double sourceMidPoint;
            int intSourceMidPoint;
            
            double testval=0.0;
            
            for (int j=-10; j<10+1; j++)
            {
                sourceMidPoint = double(i)/factor + j;
                intSourceMidPoint = std::floor(sourceMidPoint + 0.5);
                //+0.5 to automatically round the right way
                newValue += double(getArrayElement<int16_t>(*samplePtr, intSourceMidPoint, sampleCount)) *
                            //shifted sinc
                            sinc(sourceMidPoint - intSourceMidPoint + j);
                //testval += sinc(sourceMidPoint - intSourceMidPoint + j);
                //std::cout << sourceMidPoint << " " << sourceMidPoint - intSourceMidPoint + j << " " << sinc(sourceMidPoint - intSourceMidPoint + j) << " | ";
            }
            //std::cout << testval << std::endl;
            newSamples[i] = newValue;
        }
        
        delete[] *samplePtr;
        *samplePtr = newSamples;
        sampleCount = newSampleCount;
        
        return true;
        
    }

    //introduces zeros
    template<typename T>
    inline
    T getArrayElement(T* array, double time, int count, int32_t rate)
    {
        if (time < 0)
            return 0.0;
        else if (time >= double(count)/rate)
            return 0.0;
        else
            return array[int(time*rate)];
        
    }

    bool Resampler22kHzMono::resample(int16_t** samplePtr, int& sampleCount, int32_t fromRate, int32_t toRate)
    {
        double fs = fromRate;
        double fs_ = toRate;
        
        double T = 1.0/fs;
        double T_ = 1.0/fs_;
        
        int16_t* newSamples = NULL;
        int newSampleCount = int(sampleCount * fs_ / fs) + 1;
        newSamples = new int16_t[newSampleCount];
        if (!newSamples)
            return false;
        
        for (int m=0; m<newSampleCount; m++)
        {
            double t = m * T_;
            
            double newVal=0.0;
            double sum=0.0;
            for(int n=-10 + std::floor(t*fs+0.5); n <= 10 + std::floor(t*fs+0.5); n++ )
            {
                sum += double(getArrayElement<int16_t>(*samplePtr, n*T, sampleCount, fromRate)) *
                    sinc(fs*(t - n*T));
                newVal += sinc(fs*(t - n*T));
            }
            //std::cout << t << " " << double(getArrayElement<int16_t>(*samplePtr, t*fs*T, sampleCount, fromRate)) << " " << newVal << std::endl;
            newSamples[m] = sum;
        }
        
        delete[] *samplePtr;
        *samplePtr = newSamples;
        sampleCount = newSampleCount;
        
        return true;
    }

    /**
     * @todo apply hann window to input samples
     */
    bool Resampler22kHzMono::resample2(int16_t** samplePtr, int& sampleCount, int32_t fromRate, int32_t toRate)
    {
        double fs = fromRate;
        double fs_ = toRate;
        
        double T = 1.0/fs;
        double T_ = 1.0/fs_;
        
        int16_t* newSamples = NULL;
        int newSampleCount = int(sampleCount * fs_ / fs) + 1;
        newSamples = new int16_t[newSampleCount];
        if (!newSamples)
            return false;
        
        for (int m=0; m<newSampleCount; m++)
        {
            double t = m * T_;
            
            //double newVal=0.0;
            double sum=0.0;
            for(int n=-10 + std::floor(t*fs+0.5); n <= 10 + std::floor(t*fs+0.5); n++ )
            {
                sum += double(getArrayElement<int16_t>(*samplePtr, n*T, sampleCount, fromRate)) *
                    sinc(fs*(t - n*T));     //sinc-funktion hat die falsche breite!
                //newVal += sinc(fs*(t - n*T));
            }
            //std::cout << t << " " << double(getArrayElement<int16_t>(*samplePtr, t*fs*T, sampleCount, fromRate)) << " " << newVal << std::endl;
            newSamples[m] = sum;
        }
        
        delete[] *samplePtr;
        *samplePtr = newSamples;
        sampleCount = newSampleCount;
        
        return true;
    }

    bool Resampler22kHzMono::downsample(int16_t** samplePtr, int& sampleCount, unsigned int factor)
    {
        //use sampleCount / factor samples
        sampleCount /= factor;
        
        int16_t* newSamples = NULL;
        newSamples = new int16_t[sampleCount];
        if (!newSamples)    //failed to get memory
            return false;
        for (int i=0; i<sampleCount; i++)
        {
            //use every nth sample
            newSamples[i] = (*samplePtr)[i*factor];
        }
        delete[] *samplePtr;
        *samplePtr = newSamples;
        
        return true;
    }

    bool Resampler22kHzMono::resample(uint32_t fromSampleRate, int16_t** samplePtr, int& sampleCount, unsigned int channelCount)
    {
        //first convert to mono - we do not need stereo or more channels.
        unsigned int frameCount = sampleCount/channelCount;
        int16_t* monoSamples = new int16_t[frameCount];
        if (!monoSamples)   //failed to get memory
            return false;
        
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
        samples = NULL;
        sampleCount = frameCount;
        *samplePtr = monoSamples;
        
        //first, apply low-pass filtering - we need this prior to resampling.
        IIRFilter* filter = IIRFilter::createLowpassFilter(11025, fromSampleRate);
        filter->apply(*samplePtr, sampleCount);
        delete filter;
        filter=NULL;
        
        //write our filtered data to disk
        SF_INFO sfinfo;
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = fromSampleRate;
        sfinfo.channels = 1;
        
        SNDFILE* sndfileHandle = sf_open("./test-resampling-midway.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, *samplePtr, sampleCount);
        sf_close(sndfileHandle);
        
        //if we have an integer factor between sample rates, it is okay to
        //skip upsampling. this is /way/ faster.
        if ((fromSampleRate % 22050) == 0)
        {
            downsample(samplePtr, sampleCount, fromSampleRate / 22050);
        }
        else
        {
            //resample(samplePtr, sampleCount, 22050.0 / double(fromSampleRate));
            /*resample(samplePtr, sampleCount, fromSampleRate, 44100);
            
            SF_INFO sfinfo;
            sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
            sfinfo.samplerate = 44100;
            sfinfo.channels = 1;
            
            SNDFILE* sndfileHandle = sf_open("./test-resampling-midway2.wav", SFM_WRITE, &sfinfo);
            sf_writef_short(sndfileHandle, *samplePtr, sampleCount);
            sf_close(sndfileHandle);
            
            filter = IIRFilter::createLowpassFilter(11025, 44100);
            filter->apply(*samplePtr, sampleCount);
            downsample(samplePtr, sampleCount, 2);*/
            //resample(samplePtr, sampleCount, fromSampleRate, 22050);
            resample2(samplePtr, sampleCount, fromSampleRate, 22050);
            //do upsampling, then downsampling.
        }
        
        return true;
    }
}
