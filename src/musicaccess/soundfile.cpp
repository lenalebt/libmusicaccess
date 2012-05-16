#include "soundfile.hpp"
#include "stringhelper.hpp"
#include <sndfile.h>
#include <mpg123.h>

void SoundFile::open(std::string filename)
{
    if (endsWith(filename, ".mp3"))
    {
        //TODO: MP3 reading
        dataType = DATATYPE_MPG123;
    }
    else
    {
        //TODO: try sndfile
        dataType = DATATYPE_SNDFILE;
    }
}
void SoundFile::close()
{
}

uint32_t SoundFile::getSample()
{
    if (dataType == DATATYPE_MPG123)
    {
        
    }
    else if (dataType == DATATYPE_SNDFILE)
    {
        
    }
    else
    {
        std::cerr << "trying to read unknown datatype!" << std::endl;
        return 0;
    }
}
