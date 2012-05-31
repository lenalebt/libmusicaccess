#include "filter.hpp"

#include <cstring>
#include <iostream>
#include <cmath>

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
    std::cout << "sample freq:" << sampleFreq << std::endl;
    
    switch (sampleFreq)
    {
        //values computed with matlabs signal processing toolbox via call
        //[B, A] = cheby2(10, 30, 20000/96000)
        case 96000:
        {
            filter->A=11;
            filter->a[0] =                1.0;
            filter->a[1] =  -5.13791278442468;
            filter->a[2] =   12.7034948111288;
            filter->a[3] =  -19.4271132806392;
            filter->a[4] =   20.1896522792837;
            filter->a[5] =  -14.7975371925123;
            filter->a[6] =   7.72246541159747;
            filter->a[7] =  -2.82342654761712;
            filter->a[8] =  0.694138704177127;
            filter->a[9] = -0.104095015620437;
            filter->a[10] =0.0081623838862578;
            
            filter->B=11;
            filter->b[0] = 0.0366376910854463;
            filter->b[1] = -0.131440632474027;
            filter->b[2] =  0.267100843060208;
            filter->b[3] = -0.350793728871396;
            filter->b[4] =  0.372103616285047;
            filter->b[5] = -0.359386808910857;
            filter->b[6] =  0.372103616285047;
            filter->b[7] = -0.350793728871396;
            filter->b[8] =  0.267100843060208;
            filter->b[9] = -0.131440632474028;
            filter->b[10] =0.0366376910854465;
            break;
        }
        //[B, A] = cheby2(10, 30, 20000/48000)
        case 48000:
        {
            filter->A=11;
            filter->a[0] =                 1.0;
            filter->a[1] =  -0.351339554044922;
            filter->a[2] =    1.99761208836343;
            filter->a[3] =  0.0267422461694866;
            filter->a[4] =    1.41159014852395;
            filter->a[5] =   0.415565484599429;
            filter->a[6] =   0.526530508693692;
            filter->a[7] =   0.218188291325935;
            filter->a[8] =   0.109375437830548;
            filter->a[9] =  0.0339422807172835;
            filter->a[10] = 0.0073284849693376;
            
            filter->B=11;
            filter->b[0] =  0.085483072364353;
            filter->b[1] =   0.18594371591255;
            filter->b[2] =  0.429909724177782;
            filter->b[3] =   0.65492061486749;
            filter->b[4] =  0.870721642988351;
            filter->b[5] =  0.941577876527107;
            filter->b[6] =   0.87072164298835;
            filter->b[7] =  0.654920614867488;
            filter->b[8] =   0.42990972417778;
            filter->b[9] =  0.185943715912549;
            filter->b[10] =0.0854830723643525;
            break;
        }
        //[B, A] = cheby2(10, 30, 20000/44100)
        //case 32000:
        case 44100:
        {
            filter->A=11;
            filter->a[0] =                  1.0;
            filter->a[1] =    0.475678383484675;
            filter->a[2] =     2.14273134937769;
            filter->a[3] =     1.35591975175209;
            filter->a[4] =     1.92036679380658;
            filter->a[5] =      1.2227197346226;
            filter->a[6] =    0.892209132026299;
            filter->a[7] =    0.447002694836208;
            filter->a[8] =    0.189494268512368;
            filter->a[9] =   0.0559809482180957;
            filter->a[10] = 0.00990323300836715;
            
            filter->B=11;
            filter->b[0] =  0.0994826893233996;
            filter->b[1] =   0.305682592456337;
            filter->b[2] =   0.717110110295409;
            filter->b[3] =    1.20338713490773;
            filter->b[4] =    1.63194304858149;
            filter->b[5] =    1.79679513851627;
            filter->b[6] =    1.63194304858149;
            filter->b[7] =    1.20338713490773;
            filter->b[8] =   0.717110110295407;
            filter->b[9] =   0.305682592456337;
            filter->b[10] = 0.0994826893233995;
            break;
        }
        //[B, A] = cheby2(10, 30, 20000/32000)
        case 32000:
        {
            filter->A=11;
            filter->a[0] =                   1;  
            filter->a[1] =    4.16245715800272;
            filter->a[2] =    9.66995033402719;
            filter->a[3] =    15.1521776287505;
            filter->a[4] =    17.3849207525383;
            filter->a[5] =    15.0365556651447;
            filter->a[6] =    9.85637443793209;
            filter->a[7] =    4.81388218394767;
            filter->a[8] =    1.67417166486821;
            filter->a[9] =    0.37481395344454;
            filter->a[10] = 0.0412050026538857;
            
            filter->B=11;
            filter->b[0] = 0.202990136426641;
            filter->b[1] =  1.34570123615771;
            filter->b[2] =  4.48754769751886;
            filter->b[3] =  9.73825408586497;
            filter->b[4] =  15.1017750665291;
            filter->b[5] =  17.4139723363153;
            filter->b[6] =  15.1017750665291;
            filter->b[7] =  9.73825408586499;
            filter->b[8] =  4.48754769751888;
            filter->b[9] =  1.34570123615772;
            filter->b[10] =0.202990136426642;
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
        
        double tmpVal=0.0;
        for (int k = 0; k < B; k++)
        {
            //this is b[k] * x[i-k], written in terms of the history of our 1024 most recent input samples
            tmpVal += b[k] * history[(historyPos - k + 64) % 64];
        }
        for (int l = 1; l < A; l++)
        {
            tmpVal -= a[l] * ((i-l<0) ? 0.0 : buffer[i - l]);
        }
        buffer[i] = int16_t(std::floor(tmpVal+0.5));
    }
}
