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
    
    uint32_t position;
    
    bool fileOpen;
    
    SOUNDFILE_DATATYPE dataType;
public:
    SoundFile();
    
    /**
     * @brief Opens a music file.
     * 
     * This function first tries to determine which underlying library it should
     * use to open the file dependend on the extension of the file.
     * if the extension is <code>.mp3</code>, it uses libmpg123, otherwise it
     * uses libsndfile.
     * 
     * @param filename The filename you want to open.
     * @return <code>true</code>, if opening the file was successful, <code>false</code> otherwise.
     * 
     * @todo implementation
     */
    bool open(std::string filename);
    
    /**
     * @brief Closes an opened music file.
     * 
     * 
     * 
     * 
     * @remarks 
     * @todo 
     * @bug 
     */
    void close();
    
    /**
     * @brief Returns if a file has been opened, or not.
     * 
     * @return if a file has been opened
     */
    bool isFileOpen() {return fileOpen;}
    
    /**
     * @brief Returns the channel count of the music file.
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @return The channel count of the music file.
     */
    int getChannelCount() {return channelCount;}
    
    /**
     * @brief Retuns the frame count of the music file.
     * 
     * This value is the count of all frames together. A file
     * with 10 frames and 2 channels has 5 frames per channel.
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @return the frame count
     */
    long getFrameCount() {return frameCount;}
    
    /**
     * @brief Returns the size of a sample, in bytes.
     * 
     * This effectively returns what the maximal and minimal values are
     * that you can expect from getSample(). You should be able to
     * choose the best data type depended on this value.
     * 
     * 1 means 8 bit samples, and so on.
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @return 1, 2 or 4 - dependend on the sample size in the file.
     */
    int getSampleSize() {return sampleSize;}
    
    /**
     * @brief Returns the sample rate of the file, in Hz.
     * 
     * 
     * 
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @remarks There are some file types out there which are able to change the
     *      sample rate over time. This function does not reflect this fact.
     * @return the sample rate in Hz.
     */
    uint32_t getSampleRate() {return sampleRate;}
    
    /**
     * @brief Returns the sample reader position within the file.
     * 
     * Use this to determine how far you have gotten while reading the file contents.
     * That position is an absolute position. It does not take care of
     * how many channels you have, and so on.
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @return The position within the file.
     */
    uint32_t getPosition() {return position;}
    
    /**
     * @brief Returns the next sample
     * 
     * This functions returns the next sample from the opened music file.
     * It does not take care of channels and so on, so you have to take
     * care of that on your own. The samples of the different samples
     * are interleaved, so you will get the samples for the different channles of the
     * the same time slot one after another.
     * 
     * @remarks If no file has been opened, the return value is undefined.
     * @remarks This function will return 32bit integers, regardless of the input
     *      format. Take a look at getSampleSize() to see what resolution the samples
     *      have.
     * @return the next sample
     */
    int32_t getSample();
};

#endif  //SOUNDFILE_HPP 
