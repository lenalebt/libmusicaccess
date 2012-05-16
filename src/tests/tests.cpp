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
        CHECK(!file.isFileOpen());
        CHECK(file.open("testdata/test.mp3"));
        CHECK(file.isFileOpen());
        CHECK_EQ(file.getPosition(), 0u);
        CHECK_EQ(file.getChannelCount(), 2);
        CHECK_EQ(file.getFrameCount(), 2);
        CHECK_EQ(file.getSampleSize(), 2);
        CHECK_EQ(file.getSampleRate(), 2u);
        CHECK_EQ(file.getSample(), 2);
        CHECK_EQ(file.getPosition(), 1u);
        file.close();
        CHECK(!file.isFileOpen());
        return EXIT_SUCCESS;
    }
}
