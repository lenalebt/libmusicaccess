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
        //[B, A] = cheby2(10, 30, 22050/96000)
        case 96000:
        {
            filter->A=11;
            filter->a[0] =   1.0f;
            filter->a[1] =  -4.64188555620583f;
            filter->a[2] =  10.6539891664024f;
            filter->a[3] = -15.2717610663595f;
            filter->a[4] =  15.0120442690458f;
            filter->a[5] = -10.4593268347467f;
            filter->a[6] =   5.21974455735443f;
            filter->a[7] =  -1.82905518490062f;
            filter->a[8] =   0.434878097302506f;
            filter->a[9] =  -0.0630451044980516f;
            filter->a[10] =  0.00556817048521594f;
            
            filter->B=11;
            filter->b[0] =  0.0399172378769204f;
            filter->b[1] = -0.117696301620039f;
            filter->b[2] =  0.222810892925048f;
            filter->b[3] = -0.259595700346709f;
            filter->b[4] =  0.26319854449237f;
            filter->b[5] = -0.236118832775493f;
            filter->b[6] =  0.263198544492369f;
            filter->b[7] = -0.259595700346708f;
            filter->b[8] =  0.222810892925047f;
            filter->b[9] = -0.117696301620039f;
            filter->b[10] = 0.0399172378769203f;
            break;
        }
        //[B, A] = cheby2(10, 30, 22050/48000)
        case 48000:
        {
            filter->A=11;
            filter->a[0] =   1.0;
            filter->a[1] =   0.606366766925653;
            filter->a[2] =   2.21798030219759;
            filter->a[3] =   1.58453603858659;
            filter->a[4] =   2.06633354619972;
            filter->a[5] =   1.37932444592849;
            filter->a[6] =   0.978495051912331;
            filter->a[7] =   0.494082093903734;
            filter->a[8] =   0.206467314667207;
            filter->a[9] =   0.0603083651579653;
            filter->a[10] =  0.0103922664672677;
            
            filter->B=11;
            filter->b[0] =  0.101916546384486;
            filter->b[1] =  0.327304904601052;
            filter->b[2] =  0.774916746846691;
            filter->b[3] =  1.31608855667037;
            filter->b[4] =  1.79285656585924;
            filter->b[5] =  1.97811955122287;
            filter->b[6] =  1.79285656585923;
            filter->b[7] =  1.31608855667037;
            filter->b[8] =  0.77491674684669;
            filter->b[9] =  0.327304904601052;
            filter->b[10] = 0.101916546384486;
            break;
        }
        //[B, A] = cheby2(10, 30, 22050/44100)
        case 44100:
        {
            filter->A=11;
            filter->a[0] =  1.0;
            filter->a[1] =  1.50500881207773;
            filter->a[2] =  3.11938845853393;
            filter->a[3] =  3.4537421088511;
            filter->a[4] =  3.6312250035913;
            filter->a[5] =  2.82111784388383;
            filter->a[6] =  1.84680283924253;
            filter->a[7] =  0.938359373015455;
            filter->a[8] =  0.365567895786165;
            filter->a[9] =  0.0984972917934475;
            filter->a[10] = 0.0145345633709583;
            
            filter->B=11;
            filter->b[0] =  0.120554052756238;
            filter->b[1] =  0.499351161134224;
            filter->b[2] =  1.28386660703677;
            filter->b[3] =  2.34440530783764;
            filter->b[4] =  3.30331726478958;
            filter->b[5] =  3.69125540303757;
            filter->b[6] =  3.30331726478958;
            filter->b[7] =  2.34440530783764;
            filter->b[8] =  1.28386660703678;
            filter->b[9] =  0.499351161134226;
            filter->b[10] = 0.120554052756238;
            break;
        }
        //[B, A] = cheby2(10, 30, 22050/32000)
        case 32000:
        {
            filter->A=11;
            filter->a[0] =  1.0;                  
            filter->a[1] =  5.4354699524962;
            filter->a[2] = 14.8548806539766;
            filter->a[3] = 26.1817402752583;
            filter->a[4] = 32.5631491684131;
            filter->a[5] = 29.6421377378847;
            filter->a[6] = 19.9082881808206;
            filter->a[7] =  9.71338143459609;
            filter->a[8] =  3.2901038507573;
            filter->a[9] =  0.69844431306359;
            filter->a[10] = 0.0706387394299939;
            
            filter->B=11;
            filter->b[0] =  0.265779493545935;
            filter->b[1] =  2.03627271819535;
            filter->b[2] =  7.50509433573483;
            filter->b[3] = 17.4139783379517;
            filter->b[4] = 28.0688103442085;
            filter->b[5] = 32.7783638474241;
            filter->b[6] = 28.0688103442085;
            filter->b[7] = 17.4139783379517;
            filter->b[8] =  7.50509433573484;
            filter->b[9] =  2.03627271819535;
            filter->b[10] = 0.265779493545936;
            break;
        }
        case 22050:
        {
            filter->A=0;
            filter->b[0] = 1.0f;
            filter->B=1;
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
    filter->A = 0;
    filter->b[0] = 1.0f;
    filter->B = 1;
    return filter;
}

void IIRFilter::apply(int16_t* buffer, int bufferSize)
{
    //applies an IIR filter in-place.
    
    int16_t history[64];
    for (int i = 0; i < 64; i++)
    {
        history[i] = 0;
    }
    
    int historyPos=0;
    for (int i = 0; i < bufferSize; i++, historyPos++)
    {
        if (historyPos >= 64)
            historyPos -= 64;
        
        //save history, we need it because of the recursive structure (input values will be overwritten)
        history[historyPos] = buffer[i];
        
        float tmpVal=0.0f;
        for (int k = 0; k < B; k++)
        {
            //this is b[k] * x[i-k], written in terms of the history of our 1024 most recent input samples
            tmpVal += b[k] * history[(historyPos - k + 64) % 64];
        }
        for (int l = 1; l < A; l++)
        {
            tmpVal -= a[l] * ((i-l<0) ? buffer[0] : buffer[i - l]);
        }
        buffer[i] = (int16_t)tmpVal;
    }
}
