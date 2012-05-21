#include "filter.hpp"

#include <cstring>
#include <iostream>

IIRFilter::IIRFilter()
{
    
}

IIRFilter* IIRFilter::createLowpassFilter(uint32_t cutoffFreq, uint32_t sampleFreq)
{
    if (cutoffFreq != 11025)
    {
        std::cerr << "IIRFilter::createLowpassFilter(): only cutoff "
            << "frequency 11025Hz is supported for now, "
            << "will use that value instead of " << cutoffFreq << "."
            << std::endl;
        cutoffFreq = 11025;
    }
    IIRFilter* filter = new IIRFilter();
    switch (sampleFreq)
    {
        //values computed with matlabs signal processing toolbox via call
        //[B, A] = cheby2(10, 30, 11025/44100)
        case 44100:
        {
            filter->A=11;
            filter->a[0] = 1.0f;
            filter->a[1] = -4.17080464f;
            filter->a[2] =  8.91329399f;
            filter->a[3] = -11.9920025f;
            filter->a[4] =  11.1879050f;
            filter->a[5] = -7.42360145f;
            filter->a[6] =  3.55697835f;
            filter->a[7] = -1.19505797f;
            filter->a[8] =  0.27717187f;
            filter->a[9] = -0.03833772f;
            filter->a[10] = 0.00423613f;
            
            filter->B=11;
            filter->b[0] =  0.04332672f;
            filter->b[1] = -0.10186748f;
            filter->b[2] =  0.18758271f;
            filter->b[3] = -0.18697261f;
            filter->b[4] =  0.19241833f;
            filter->b[5] = -0.14919432f;
            filter->b[6] =  0.19241833f;
            filter->b[7] = -0.18697261f;
            filter->b[8] =  0.18758271f;
            filter->b[9] = -0.10186748f;
            filter->b[10] = 0.04332672f;
            break;
        }
        default:
        {
            std::cerr << "target sample frequency not available: "
                << sampleFreq << std::endl;
            break;
        }
    }
    
    return filter;
}
IIRFilter* IIRFilter::createNOOPFilter()
{
    IIRFilter* filter = new IIRFilter();
    filter->A=0;
    filter->b[0] = 1.0f;
    filter->B=1;
    return filter;
}

void IIRFilter::apply(int16_t* buffer, int bufferSize)
{
    //applies an IIR filter in-place.
    
    int16_t history[1024];
    for (int i = 0; i < 1024; i++)
    {
        history[i] = 0;
    }
    
    int historyPos=0;
    for (int i = 0; i < bufferSize; i++, historyPos++)
    {
        if (historyPos >= 1024)
            historyPos -= 1024;
        
        //save history, we need it because of the recursive structure (input values will be overwritten)
        history[historyPos] = buffer[i];
        
        float tmpVal=0.0f;
        for (int k = 0; k < B; k++)
        {
            //this is b[k] * x[i-k], written in terms of the history of our 1024 most recent input samples
            tmpVal += b[k] * history[(historyPos - k + 1024) % 1024];
        }
        for (int l = 1; l < A; l++)
        {
            tmpVal -= a[l] * ((i-l<0) ? 0 : buffer[i - l]);
        }
        //std::cerr << buffer[i] <<" <=> " << (int16_t)tmpVal << std::endl;
        buffer[i] = (int16_t)tmpVal;
    }
}
