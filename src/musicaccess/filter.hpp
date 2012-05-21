#ifndef FILTER_HPP
#define FILTER_HPP

#include <stdint.h>

/**
 * @brief This class defines a simple audio filter.
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
    virtual void apply(int16_t* buffer, int bufferSize)=0;
};

/**
 * @brief This class defines an IIR filter implementation without setting the coefficients.
 * 
 * Derive from this class and set the coefficients properly. Then call apply(), and
 * the filter will be applied in-place.
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
    float a[20];
    //coefficients for feedback values
    float b[20];
    //input coefficient count
    int A;
    //feedback coefficient count
    int B;
public:
    void apply(int16_t* buffer, int bufferSize);
    static IIRFilter* createLowpassFilter(uint32_t cutoffFreq, uint32_t sampleFreq);
    static IIRFilter* createNOOPFilter();
};


#endif //FILTER_HPP
