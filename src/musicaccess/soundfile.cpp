#include "soundfile.hpp"
#include "stringhelper.hpp"
#include <sndfile.h>
#include <mpg123.h>
#include <iostream>
#include <algorithm>
#include <cctype>

SoundFile::SoundFile() :
    channelCount(0), sampleSize(0), frameCount(0),
    sampleRate(0), position(0), fileOpen(false),
    dataType(DATATYPE_UNKNOWN)
{
    
}

bool SoundFile::open(std::string filename)
{
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    
    if (endsWith(filename, ".mp3"))
    {
        //TODO: MP3 reading
        dataType = DATATYPE_MPG123;
        
        return false;
    }
    else
    {
        //TODO: try sndfile
        dataType = DATATYPE_SNDFILE;
        
        return false;
    }
    
    return false;
}

void SoundFile::close()
{
    if (dataType == DATATYPE_MPG123)
    {
        //TODO: close mpg123 stream
    }
    else if (dataType == DATATYPE_SNDFILE)
    {
        //TODO: close sndfile stream
    }
    else
    {
        //do nothing.
    }
}

int32_t SoundFile::getSample()
{
    position++;
    
    if (dataType == DATATYPE_MPG123)
    {
        //TODO: return mpg123 sample
        return 0;
    }
    else if (dataType == DATATYPE_SNDFILE)
    {
        //TODO: return sndfile sample
        return 0;
    }
    else
    {
        std::cerr << "trying to read unknown datatype!" << std::endl;
        return 0;
    }
}
