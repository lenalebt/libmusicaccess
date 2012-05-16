#ifndef SOUNDFILE_HPP
#define SOUNDFILE_HPP

#include <stdint.h>
#include <string>

enum SOUNDFILE_DATATYPE
{
    DATATYPE_MPG123,
    DATATYPE_SNDFILE,
    DATATYPE_UNKNOWN
};

/**
 * @brief This is a class for music files which allows to read
 *      data from a number of different music file formats.
 * 
 * Endianess if automatically converted to the correct format for the CPU.
 * 
 * @remarks Using this class, you will get the data in an integer format,
 * regardless of the possibilities in the underlying format.
 */
class SoundFile
{
private:
    int channelCount;
    int sampleSize;   //may be any of 1, 2, 4 bytes.
    long frameCount;
    uint32_t sampleRate;
    
    SOUNDFILE_DATATYPE dataType;
public:
    void open(std::string filename);
    void close();
    
    int getChannelCount() {return channelCount;}
    long getFrameCount() {return frameCount;}
    int getSampleSize() {return sampleSize;}
    
    uint32_t getSample();
};

#endif  //SOUNDFILE_HPP 
