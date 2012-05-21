#include "tests.hpp"
#include "testframework.hpp"
#include <cstdlib>


#include "stringhelper.hpp"
#include "soundfile.hpp"
#include "filter.hpp"
#include "resample.hpp"

//for memcpy
#include <cstring>

#include "sndfile.h"

namespace tests
{
    int testEndsWith()
    {
        CHECK(endsWith("hello world", "world"));
        CHECK(!endsWith("hello world!", "world"));
        CHECK(endsWith("lalalala", "lala"));
        CHECK(endsWith("lalalala", "la"));
        CHECK(endsWith("lalalala", "lalalala"));
        CHECK(!endsWith("lala", "lalalala"));
        
        return EXIT_SUCCESS;
    }
    
    int testSoundFile()
    {
        SoundFile file;
        
        std::cerr << "testing mp3 files..." << std::endl;
        CHECK(!file.isFileOpen());
        CHECK(file.open("./testdata/test.mp3"));
        CHECK(file.isFileOpen());
        CHECK_EQ(file.getPosition(), 0u);
        CHECK_EQ(file.getChannelCount(), 2);
        CHECK_EQ(file.getSampleCount(), 1424384);
        CHECK_EQ(file.getSampleSize(), 2);
        CHECK_EQ(file.getSampleRate(), 44100);
        int16_t* buffer = NULL;
        buffer = new int16_t[1424384+2];
        //check for buffer allocation was possible
        CHECK(buffer != NULL);
        CHECK_EQ(file.readSamples(buffer, 500u), 500u);
        CHECK_EQ(file.getPosition(), 500u);
        CHECK_EQ(file.readSamples(buffer+500, 1424384+2), 1424384u-500u);
        CHECK_EQ(file.getPosition(), 1424384u);
        CHECK_EQ(buffer[0], -2);
        CHECK_EQ(buffer[501], -41);
        CHECK(file.close());
        CHECK(!file.isFileOpen());
        
        std::cerr << "testing wav files..." << std::endl;
        CHECK(file.open("./testdata/test.wav"));
        CHECK(file.isFileOpen());
        CHECK_EQ(file.getPosition(), 0u);
        CHECK_EQ(file.getChannelCount(), 2);
        CHECK_EQ(file.getSampleCount(), 1424384);
        CHECK_EQ(file.getSampleSize(), 2);
        CHECK_EQ(file.getSampleRate(), 44100);
        CHECK_EQ(file.readSamples(buffer, 500u), 500u);
        CHECK_EQ(file.getPosition(), 500u);
        CHECK_EQ(file.readSamples(buffer+500, 1424384+2), 1424384u-500u);
        CHECK_EQ(file.getPosition(), 1424384u);
        CHECK_EQ(buffer[0], -1);
        CHECK_EQ(buffer[501], -46);
        CHECK(file.close());
        CHECK(!file.isFileOpen());
        
        std::cerr << "testing ogg files..." << std::endl;
        CHECK(file.open("./testdata/test.ogg"));
        CHECK(file.isFileOpen());
        CHECK_EQ(file.getPosition(), 0u);
        CHECK_EQ(file.getChannelCount(), 2);
        CHECK_EQ(file.getSampleCount(), 1424384);
        CHECK_EQ(file.getSampleSize(), 2);
        CHECK_EQ(file.getSampleRate(), 44100);
        CHECK_EQ(file.readSamples(buffer, 500u), 500u);
        CHECK_EQ(file.getPosition(), 500u);
        CHECK_EQ(file.readSamples(buffer+500, 1424384+2), 1424384u-500u);
        CHECK_EQ(file.getPosition(), 1424384u);
        CHECK_EQ(buffer[0], 0);
        CHECK_EQ(buffer[501], -44);
        CHECK(file.close());
        CHECK(!file.isFileOpen());
        
        std::cerr << "testing flac files..." << std::endl;
        CHECK(file.open("./testdata/test.flac"));
        CHECK(file.isFileOpen());
        CHECK_EQ(file.getPosition(), 0u);
        CHECK_EQ(file.getChannelCount(), 2);
        CHECK_EQ(file.getSampleCount(), 1424384);
        CHECK_EQ(file.getSampleSize(), 2);
        CHECK_EQ(file.getSampleRate(), 44100);
        CHECK_EQ(file.readSamples(buffer, 500u), 500u);
        CHECK_EQ(file.getPosition(), 500u);
        CHECK_EQ(file.readSamples(buffer+500, 1424384+2), 1424384u-500u);
        CHECK_EQ(file.getPosition(), 1424384u);
        CHECK_EQ(buffer[0], 0);
        CHECK_EQ(buffer[501], -45);
        CHECK(file.close());
        CHECK(!file.isFileOpen());
        
        delete buffer;
        return EXIT_SUCCESS;
    }
    
    int testResampling()
    {
        return EXIT_FAILURE;
    }
    
    int testIIRFilter()
    {
        SoundFile file;
        file.open("./testdata/test.mp3");
        int16_t* buffer;
        int16_t* buffer2;
        
        CHECK_EQ(file.getSampleCount(), 1424384u);
        
        buffer = new int16_t[file.getSampleCount()];
        int monoSampleCount = file.getSampleCount()/2;
        buffer2 = new int16_t[monoSampleCount];
        file.readSamples(buffer, file.getSampleCount());
        
        std::cerr << "converting file to mono..." << std::endl;
        for(int i=0; i< file.getSampleCount(); i+=2)
        {
            buffer2[i/2] = (int32_t(buffer[i]) + int32_t(buffer[i+1])) / 2;
        }
        delete buffer;
        buffer = new int16_t[monoSampleCount];
        memcpy(buffer, buffer2, monoSampleCount*sizeof(int16_t));
        bool arraysAreEqual=true;
        CHECK(buffer != buffer2);
        
        std::cerr << "openend ./testdata/test.mp3 and copied the samples to another array." << std::endl;
        for (int i = 0; i < monoSampleCount; i++)
        {
            arraysAreEqual = arraysAreEqual && (buffer[i] == buffer2[i]);
        }
        CHECK(arraysAreEqual);
        CHECK(buffer != buffer2);
        
        std::cerr << "applying noop-filter and checking for arrays still being equal..." << std::endl;
        IIRFilter* noopFilter = NULL;
        noopFilter = IIRFilter::createNOOPFilter();
        CHECK(noopFilter != NULL);
        noopFilter->apply(buffer, monoSampleCount);
        
        for (int i = 0; i < monoSampleCount; i++)
        {
            arraysAreEqual = arraysAreEqual && (buffer[i] == buffer2[i]);
            if (!arraysAreEqual)
            {
                std::cerr << "i=" << i << std::endl;
                CHECK_EQ(buffer[i], buffer2[i]);
            }
        }
        CHECK(arraysAreEqual);
        CHECK(buffer != buffer2);
        
        delete noopFilter;
        noopFilter = NULL;
        
        
        std::cerr << "applying lowpass-filter..." << std::endl;
        IIRFilter* lowpassFilter = NULL;
        lowpassFilter = IIRFilter::createLowpassFilter(11025, file.getSampleRate());
        CHECK(lowpassFilter != NULL);
        lowpassFilter->apply(buffer, monoSampleCount);
        
        for (int i = 0; i < monoSampleCount; i++)
        {
            arraysAreEqual = arraysAreEqual && (buffer[i] == buffer2[i]);
        }
        CHECK(!arraysAreEqual);
        
        SF_INFO sfinfo;
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = 44100;
        sfinfo.channels = 1;
        
        SNDFILE* sndfileHandle = sf_open("./filtered.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, buffer, monoSampleCount);
        sf_close(sndfileHandle);
        
        
        return EXIT_SUCCESS;
    }
}
