#include "tests.hpp"
#include "testframework.hpp"
#include <cstdlib>


#include "stringhelper.hpp"
#include "soundfile.hpp"
#include "filter.hpp"
#include "resample.hpp"

//for memcpy
#include <cstring>

#include <queue>

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
        Resampler22kHzMono resampler;
        
        SoundFile file;
        file.open("./testdata/test.mp3");
        int16_t* buffer = NULL;
        int sampleCount = file.getSampleCount();
        int oldSampleCount = sampleCount;
        buffer = new int16_t[sampleCount];
        CHECK(buffer != NULL);
        file.readSamples(buffer, sampleCount);
        
        resampler.resample(file.getSampleRate(), &buffer, sampleCount, file.getChannelCount());
        
        //we need to have less samples. better check see below.
        CHECK(sampleCount < oldSampleCount);
        //half the sample rate, mono instead of stereo -> 1/4 the sample count
        //CHECK_EQ(sampleCount, oldSampleCount/4);
        
        //write our filtered data to disk
        SF_INFO sfinfo;
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = 22050;
        sfinfo.channels = 1;
        
        SNDFILE* sndfileHandle = sf_open("./test-resampled-22khz-mono.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, buffer, sampleCount);
        sf_close(sndfileHandle);
        
        //file.open("./testdata/test-32khz.mp3");
        file.open("./testdata/sine-9000-32khz-long.wav");
        delete[] buffer;
        buffer = NULL;
        sampleCount = file.getSampleCount();
        oldSampleCount = sampleCount;
        buffer = new int16_t[sampleCount];
        CHECK(buffer != NULL);
        file.readSamples(buffer, sampleCount);
        
        std::cerr << "now resampling..." << std::endl;
        resampler.resample(file.getSampleRate(), &buffer, sampleCount, file.getChannelCount());
        std::cerr << "resampled..." << std::endl;
        
        //we need to have less samples. better check see below.
        //CHECK(sampleCount < oldSampleCount);
        //half the sample rate, mono instead of stereo -> 1/4 the sample count
        //CHECK_EQ_TYPE(sampleCount, 1033566/2.0*22050.0/32000.0 + 1, int);
        
        //write our filtered data to disk
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = 32000;
        sfinfo.channels = 1;
        
        sndfileHandle = sf_open("./test-resampled-from32khz-22khz-mono.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, buffer, sampleCount);
        sf_close(sndfileHandle);
        
        std::cerr << "testing now with different sample rates and different sine waves..." << std::endl;
        std::queue<std::string> files;
        files.push("sine-8000-22khz.wav");
        files.push("sine-8000-32khz.wav");
        files.push("sine-8000-44khz.wav");
        
        files.push("sine-8900-22khz.wav");
        files.push("sine-8900-32khz.wav");
        files.push("sine-8900-44khz.wav");
        
        files.push("sine-9000-22khz.wav");
        files.push("sine-9000-32khz.wav");
        files.push("sine-9000-44khz.wav");
        
        files.push("sine-9900-22khz.wav");
        files.push("sine-9900-32khz.wav");
        files.push("sine-9900-44khz.wav");
        
        //do some batch resampling
        SoundFile soundfile;
        while (!files.empty())
        {
            std::string filename = files.front();
            files.pop();
            
            std::cerr << "resampling file \"" << std::string("./testdata/") + filename << "\" to 22khz..." << std::endl;
            CHECK(soundfile.open(std::string("./testdata/") + filename));
            
            CHECK(soundfile.getSampleCount() > 0);
            CHECK_EQ(soundfile.getChannelCount(), 1);
            CHECK_OP(soundfile.getSampleRate(), >=, 22050);
            CHECK_OP(soundfile.getSampleRate(), <=, 44100);
            
            if (buffer)
                delete[] buffer;
            buffer = new int16_t[soundfile.getSampleCount()];
            soundfile.readSamples(buffer, soundfile.getSampleCount());
            
            std::cerr << "resampling..." << std::endl;
            int sampleCount = soundfile.getSampleCount();
            resampler.resample(soundfile.getSampleRate(), &buffer, sampleCount, soundfile.getChannelCount());
            
            //TODO: Do some checks.
            std::cerr << "need to manually check these files if they are correct...: \""
                << "./resampled-to22khz-from" << filename << "\"" << std::endl;
            
            //write our filtered data to disk
            SF_INFO sfinfo;
            sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
            sfinfo.samplerate = 22050;
            sfinfo.channels = 1;
            
            SNDFILE* sndfileHandle = sf_open((std::string("./resampled-to22khz-from") + filename).c_str(), SFM_WRITE, &sfinfo);
            sf_writef_short(sndfileHandle, buffer, sampleCount);
            sf_close(sndfileHandle);
            soundfile.close();
        }
        
        return EXIT_SUCCESS;
    }
    
    int testIIRFilter()
    {
        SoundFile file;
        file.open("./testdata/test.mp3");
        int16_t* buffer = NULL;
        int16_t* buffer2 = NULL;
        
        CHECK_EQ(file.getSampleCount(), 1424384u);
        
        buffer = new int16_t[file.getSampleCount()];
        CHECK(buffer != NULL);
        int monoSampleCount = file.getSampleCount()/2;
        buffer2 = new int16_t[monoSampleCount];
        CHECK(buffer2 != NULL);
        file.readSamples(buffer, file.getSampleCount());
        
        std::cerr << "converting file to mono..." << std::endl;
        for(int i=0; i< file.getSampleCount(); i+=2)
        {
            buffer2[i/2] = (int32_t(buffer[i]) + int32_t(buffer[i+1])) / 2;
        }
        delete[] buffer;
        buffer = NULL;
        buffer = new int16_t[monoSampleCount];
        CHECK(buffer != NULL);
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
        
        //okay, we know that our filter did "something".
        
        
        //write our filtered data to disk
        SF_INFO sfinfo;
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = 44100;
        sfinfo.channels = 1;
        
        SNDFILE* sndfileHandle = sf_open("./test-iirfilter-filtered.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, buffer, monoSampleCount);
        sf_close(sndfileHandle);
        
        
        std::cerr << "reading 32khz file..." << std::endl;
        file.open("./testdata/test-32khz.mp3");
        //file.open("./testdata/sine-9900.mp3");
        delete[] buffer;
        buffer = NULL;
        int sampleCount = file.getSampleCount();
        buffer = new int16_t[sampleCount];
        CHECK(buffer != NULL);
        file.readSamples(buffer, sampleCount);
        
        if (file.getChannelCount() == 2)
        {
            std::cerr << "converting file to mono..." << std::endl;
            monoSampleCount = file.getSampleCount()/2;
            buffer2 = new int16_t[monoSampleCount];
            CHECK(buffer2 != NULL);
            for(int i=0; i< file.getSampleCount(); i+=2)
            {
                buffer2[i/2] = (int32_t(buffer[i]) + int32_t(buffer[i+1])) / 2;
            }
            delete[] buffer;
            buffer = NULL;
            buffer = new int16_t[monoSampleCount];
            CHECK(buffer != NULL);
            memcpy(buffer, buffer2, monoSampleCount*sizeof(int16_t));
            arraysAreEqual=true;
            CHECK(buffer != buffer2);
        }
        else
        {
            monoSampleCount = sampleCount;
        }
        
        std::cerr << "applying filter to 32khz signal..." << std::endl;
        IIRFilter* lowpassFilter2 = NULL;
        lowpassFilter2 = IIRFilter::createLowpassFilter(11025, file.getSampleRate());
        CHECK(lowpassFilter2 != NULL);
        lowpassFilter2->apply(buffer, monoSampleCount);
        
        for (int i = 0; i < monoSampleCount; i++)
        {
            arraysAreEqual = arraysAreEqual && (buffer[i] == buffer2[i]);
        }
        CHECK(!arraysAreEqual);
        
        //okay, we know that our filter did "something".
        
        
        //write our filtered data to disk
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        sfinfo.samplerate = 32000;
        sfinfo.channels = 1;
        
        sndfileHandle = sf_open("./test-iirfilter-filtered-32khz.wav", SFM_WRITE, &sfinfo);
        sf_writef_short(sndfileHandle, buffer, monoSampleCount);
        sf_close(sndfileHandle);
        
        //testing works somehow manual. this is not optimal, but I don't
        //have anything better for now.
        
        std::cerr << "testing now with different sample rates and different sine waves..." << std::endl;
        std::queue<std::string> files;
        files.push("sine-8000-22khz.wav");
        files.push("sine-8000-32khz.wav");
        files.push("sine-8000-44khz.wav");
        
        files.push("sine-8900-22khz.wav");
        files.push("sine-8900-32khz.wav");
        files.push("sine-8900-44khz.wav");
        
        files.push("sine-9000-22khz.wav");
        files.push("sine-9000-32khz.wav");
        files.push("sine-9000-44khz.wav");
        
        files.push("sine-9900-22khz.wav");
        files.push("sine-9900-32khz.wav");
        files.push("sine-9900-44khz.wav");
        
        delete lowpassFilter;
        lowpassFilter = NULL;
        file.close();
        
        SoundFile soundfile;
        
        while (!files.empty())
        {
            std::string filename = files.front();
            files.pop();
            
            std::cerr << "applying filter to file \"" << std::string("./testdata/") + filename << "\"..." << std::endl;
            CHECK(soundfile.open(std::string("./testdata/") + filename));
            
            CHECK(soundfile.getSampleCount() > 0);
            CHECK_EQ(soundfile.getChannelCount(), 1);
            CHECK_OP(soundfile.getSampleRate(), >=, 22050);
            CHECK_OP(soundfile.getSampleRate(), <=, 44100);
            
            if (buffer)
                delete[] buffer;
            buffer = new int16_t[soundfile.getSampleCount()];
            soundfile.readSamples(buffer, soundfile.getSampleCount());
            
            std::cerr << "applying low-pass filter..." << std::endl;
            if(lowpassFilter)
                delete lowpassFilter;
            lowpassFilter = IIRFilter::createLowpassFilter(11025, soundfile.getSampleRate());
            lowpassFilter->apply(buffer, soundfile.getSampleCount());
            
            //TODO: Do some checks.
            std::cerr << "need to manually check these files if they are correct...: \""
                << "./filtered-" << filename << "\"" << std::endl;
            
            //write our filtered data to disk
            SF_INFO sfinfo;
            sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
            sfinfo.samplerate = soundfile.getSampleRate();
            sfinfo.channels = 1;
            
            SNDFILE* sndfileHandle = sf_open((std::string("./filtered-") + filename).c_str(), SFM_WRITE, &sfinfo);
            sf_writef_short(sndfileHandle, buffer, soundfile.getSampleCount());
            sf_close(sndfileHandle);
            soundfile.close();
        }
        
        return EXIT_SUCCESS;
    }
}
