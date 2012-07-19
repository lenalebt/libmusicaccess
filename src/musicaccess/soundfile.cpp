#include "soundfile.hpp"
#include "stringhelper.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

namespace musicaccess
{
    SoundFile::SoundFile() :
        channelCount(0), sampleSize(0), sampleCount(0),
        sampleRate(0), position(0), fileOpen(false),
        dataType(DATATYPE_UNKNOWN),
        mpg123Handle(NULL), sndfileHandle(NULL),
        metadata(NULL)
    {
        //will init mpg123 and sndfile if necessary
        SingletonInitializer::initialize();
    }
    SoundFile::~SoundFile()
    {
        if (fileOpen)
            close();
        //will destroy mpg123 and sndfile if necessary
        SingletonInitializer::destroy();
    }

    bool SoundFile::open(const std::string& filename, bool decodeToFloat)
    {
        //first: close open files, if any.
        if (fileOpen)
        {
            if (!close())
                return false;
        }
        
        this->decodeToFloat = decodeToFloat;
        
        std::string loweredFilename(filename);
        std::transform(loweredFilename.begin(), loweredFilename.end(), loweredFilename.begin(), ::tolower);
        
        //std::cerr << loweredFilename << std::endl;
        
        //now open the file - take libmpg123 or libsndfile.
        if (endsWith(loweredFilename, ".mp3"))
        {   //use libmpg123
            dataType = DATATYPE_MPG123;
            int error;
            mpg123Handle = mpg123_new(NULL, &error);
            if (error != MPG123_OK)
            {
                std::cerr << "mpg123: setup failed, " << mpg123_plain_strerror(error) << std::endl;
                fileOpen = false;
                return false;
            }
            
            //force mpg123 to use float if chosen.
            if (decodeToFloat)
                mpg123_param(mpg123Handle, MPG123_ADD_FLAGS, MPG123_FORCE_FLOAT, 0.0);
            
            //open file
            error = mpg123_open(mpg123Handle, filename.c_str());
            if (error != MPG123_OK)
            {
                std::cerr << "mpg123: open failed, " << mpg123_plain_strerror(error) << std::endl;
                fileOpen = false;
                return false;
            }
            
            int encoding;
            error = mpg123_getformat(mpg123Handle, &sampleRate, &channelCount, &encoding);
            if (error != MPG123_OK)
            {
                std::cerr << "mpg123: getting file info failed, " << mpg123_plain_strerror(error) << std::endl;
                fileOpen = false;
                return false;
            }
            
            if (decodeToFloat)
            {
                if (encoding != MPG123_ENC_FLOAT_32)
                {
                    std::cerr << "mpg123: wrong data encoding, requested float. got 0x" << std::hex << encoding << std::endl;
                    fileOpen = false;
                    return false;
                }
            }
            else
            {
                if (encoding != MPG123_ENC_SIGNED_16)
                {
                    std::cerr << "mpg123: wrong data encoding, requested 16bit signed integer. got 0x" << std::hex << encoding << std::endl;
                    fileOpen = false;
                    return false;
                }
            }

            
            //force mpg123 to not change the encoding or other things while decoding
            mpg123_format_none(mpg123Handle);
            mpg123_format(mpg123Handle, sampleRate, channelCount, encoding);
            
            //get the length of the file (if available!)
            sampleCount = mpg123_length(mpg123Handle);
            if (sampleCount == MPG123_ERR)
            {
                std::cerr << "mpg123: was not able to get the length of the file. aborting." << std::endl;
                fileOpen = false;
                return false;
            }
            sampleCount *= channelCount;    //fix the count. could not multiply before if().
            
            //get the size of one sample
            sampleSize = mpg123_encsize(encoding);
            
            position = 0;
            
            //okay, done now. file is open!
            
            fileOpen = true;
            return true;
        }
        else
        {   //use libsndfile
            dataType = DATATYPE_SNDFILE;
            
            SF_INFO sfinfo;
            sfinfo.format = 0;  //documentation says I need to do so
            sndfileHandle = sf_open(filename.c_str(), SFM_READ, &sfinfo);
            
            position = 0;
            sampleRate = sfinfo.samplerate;
            channelCount = sfinfo.channels;
            sampleCount = sfinfo.frames * channelCount;
            
            if (decodeToFloat)
                sampleSize = 4;
            else
                sampleSize = 2;
            
            //okay, done now. file is open! somehow less work than with libmpg123.
            
            fileOpen = true;
            return true;
        }
        
        fileOpen = false;
        return false;
    }

    bool SoundFile::close()
    {
        if (metadata != NULL)
        {
            delete metadata;
            metadata = NULL;
        }
        
        if (fileOpen)
        {
            if (dataType == DATATYPE_MPG123)
            {
                mpg123_close(mpg123Handle);
                mpg123_delete(mpg123Handle);
                
                mpg123Handle = NULL;
                sndfileHandle = NULL;
                dataType = DATATYPE_UNKNOWN;
                fileOpen = false;
                
                return true;
            }
            else if (dataType == DATATYPE_SNDFILE)
            {
                sf_close(sndfileHandle);
                
                mpg123Handle = NULL;
                sndfileHandle = NULL;
                dataType = DATATYPE_UNKNOWN;
                fileOpen = false;
                
                return true;
            }
            else
            {
                //do nothing.
                mpg123Handle = NULL;
                sndfileHandle = NULL;
                dataType = DATATYPE_UNKNOWN;
                fileOpen = false;
                return true;
            }
        }
        else
            return true;
    }

    size_t SoundFile::readSamples(int16_t* buffer, int count)
    {
        if (dataType == DATATYPE_MPG123)
        {   //return data from libmpg123
            int error;
            size_t bytesRead;
            size_t framesRead;
            
            if (!decodeToFloat)
            {
                error = mpg123_read( mpg123Handle, (unsigned char*)buffer, count*sampleSize, &bytesRead );
                framesRead = bytesRead / sampleSize;
            }
            else
            {
                //read as float and reformat to int16_t
                float* floatBuffer = new float[count];
                mpg123_read( mpg123Handle, (unsigned char*)floatBuffer, count*sizeof(float), &bytesRead );
                for (int i=0; i<count; i++)
                {
                    buffer[i] = 32768.0 * floatBuffer[i];
                }
                delete[] floatBuffer;
                framesRead = bytesRead / sizeof(float);
            }
            
            position += framesRead;
            
            if (error == MPG123_DONE)
            {   //okay, decoding finished
                return framesRead;
            }
            else if (error == MPG123_OK)
            {   //okay, more bytes follow
                return framesRead;
            }
            else
            {
                std::cerr << "mpg123: error while reading: " << mpg123_plain_strerror(error) << std::endl;
                return 0;
            }
        }
        else if (dataType == DATATYPE_SNDFILE)
        {   //return data from libsndfile
            int itemsRead = sf_read_short(sndfileHandle, buffer, count);
            position += itemsRead;
            return itemsRead;
        }
        else
        {
            std::cerr << "trying to read unknown datatype!" << std::endl;
            return 0;
        }
    }
    
    size_t SoundFile::readSamples(float* buffer, int count)
    {
        if (dataType == DATATYPE_MPG123)
        {   //return data from libmpg123
            int error;
            size_t bytesRead;
            size_t framesRead;
            
            if (decodeToFloat)
            {
                error = mpg123_read( mpg123Handle, (unsigned char*)buffer, count*sampleSize, &bytesRead );
                framesRead = bytesRead / sampleSize;
            }
            else
            {
                //read as int16_t and reformat as float
                int16_t* intBuffer = new int16_t[count];
                mpg123_read( mpg123Handle, (unsigned char*)intBuffer, count*sizeof(int16_t), &bytesRead );
                for (int i=0; i<count; i++)
                {
                    buffer[i] = float(intBuffer[i]) / 32768.0;
                }
                delete[] intBuffer;
                framesRead = bytesRead / sizeof(int16_t);
            }
            
            position += framesRead;
            
            if (error == MPG123_DONE)
            {   //okay, decoding finished
                return framesRead;
            }
            else if (error == MPG123_OK)
            {   //okay, more bytes follow
                return framesRead;
            }
            else
            {
                std::cerr << "mpg123: error while reading: " << mpg123_plain_strerror(error) << std::endl;
                return 0;
            }
        }
        else if (dataType == DATATYPE_SNDFILE)
        {   //return data from libsndfile
            int itemsRead = sf_read_float(sndfileHandle, buffer, count);
            position += itemsRead;
            return itemsRead;
        }
        else
        {
            std::cerr << "trying to read unknown datatype!" << std::endl;
            return 0;
        }
    }



    SoundFile::SingletonInitializer* SoundFile::SingletonInitializer::instance = NULL;

    SoundFile::SingletonInitializer::SingletonInitializer() :
        counter(0)
    {
        
    }
    void SoundFile::SingletonInitializer::initialize()
    {
        if (instance == NULL)
            instance = new SingletonInitializer();
        
        if (instance->counter == 0)
        {
            mpg123_init();
            if(mpg123_init() != MPG123_OK)
                std::cerr << "mpg123: basic setup impossible." << std::endl;
            
            //TODO: init sndfile
        }
        instance->counter++;
    }
    void SoundFile::SingletonInitializer::destroy()
    {
        instance->counter--;
        if (instance->counter <= 0)
        {
            instance->counter = 0;
            
            mpg123_exit();
            //TODO: destroy sndfile
        }
    }
    SoundFile::SingletonInitializer::~SingletonInitializer()
    {
        
    }
    
    SoundFileMetadata* getMetadata()
    {
        return metadata;
    }
    
    std::string SoundFileMetadata::getTitle() const                     {return title;}
    std::string SoundFileMetadata::getArtist() const                    {return artist;}
    std::string SoundFileMetadata::getAlbum() const                     {return album;}
    std::string SoundFileMetadata::getGenre() const                     {return genre;}
    std::string SoundFileMetadata::getFilename() const                  {return filename;}
    
    void SoundFileMetadata::getTitle(const std::string& title)          {this->title = title;}
    void SoundFileMetadata::getArtist(const std::string& artist)        {this->artist = artist;}
    void SoundFileMetadata::getAlbum(const std::string& album)          {this->album = album;}
    void SoundFileMetadata::getGenre(const std::string& genre)          {this->genre = genre;}
    void SoundFileMetadata::getFilename(const std::string& filename)    {this->filename = filename;}
}
