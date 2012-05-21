#ifndef FILTER_HPP
#define FILTER_HPP

#include <stdint.h>

/**
 * @brief This class defines a simple audio filter.
 * 
 * You cannot do more than apply this filter.
 * 
 * This is a pure virtual class, derive your own filters from this class.
 * 
 * @author Lena Brueder
 * @date 2012-05-21
 */
class AudioFilter
{
private:
    
public:
    /**
     * @brief Apply this filter to the given buffer with the given size.
     */
    virtual void apply(int16_t* buffer, int bufferSize)=0;
};

/**
 * @brief This class defines an IIR filter implementation.
 * 
 * If you want some implementation you can use, take the factory methods to get
 * one.
 * 
 * 
 * @author Lena Brueder
 * @date 2012-05-21
 */
class IIRFilter : public AudioFilter
{
private:
    IIRFilter();
protected:
    //coefficients for input values
    float a[64];
    //coefficients for feedback values
    float b[64];
    //input coefficient count
    int A;
    //feedback coefficient count
    int B;
public:
    /**
     * @brief Applies the given IIR filter to the input buffer in-place.
     * 
     * @remarks The function internally uses a history buffer of 64 samples, so
     *      you will not be able to use more than 64 coefficients.
     * @remarks The given buffer will be overwritten to gain some speed. If
     *      you need the data in that buffer for other purposes, go make a copy!
     */
    void apply(int16_t* buffer, int bufferSize);
    /**
     * @brief Creates a lowpass filter with given cutoff frequency at the given
     *  sample rate.
     * 
     * This function only works for some standard sample rates (22050, 32000,
     * 44100, 48000 and 96000Hz) and cutoff frequency 11025Hz, and creates a Chebyshev
     * filter of order 10.
     * 
     * @return an IIR filter that does lowpass filtering.
     */
    static IIRFilter* createLowpassFilter(uint32_t cutoffFreq, uint32_t sampleFreq);
    /**
     * @brief Creates a IIR filter that exactly does nothing.
     * 
     * This function is used mainly for testing.
     * 
     * @return An IIR filter that does not change the input signal.
     */
    static IIRFilter* createNOOPFilter();
};


#endif //FILTER_HPP
