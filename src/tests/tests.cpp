#include "tests.hpp"
#include "testframework.hpp"
#include <cstdlib>


#include "stringhelper.hpp"
#include "soundfile.hpp"

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
        int16_t buffer[1424384+2];
        CHECK_EQ(file.readSamples(buffer, 1424384+2), 1424384u);
        CHECK_EQ(file.getPosition(), 1424384u);
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
        //CHECK_EQ(file.getSample(), 2);
        CHECK_EQ(file.getPosition(), 1u);
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
        //CHECK_EQ(file.getSample(), 2);
        CHECK_EQ(file.getPosition(), 1u);
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
        //CHECK_EQ(file.getSample(), 2);
        CHECK_EQ(file.getPosition(), 1u);
        CHECK(file.close());
        CHECK(!file.isFileOpen());
        return EXIT_SUCCESS;
    }
}
