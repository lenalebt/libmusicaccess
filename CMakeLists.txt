CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#project name
PROJECT(musicaccess)

#version number
SET (musicaccess_VERSION_MAJOR 0)
SET (musicaccess_VERSION_MINOR 8)

# you can choose to include secret rabbit code (aka libsamplerate) in this build.
# keep in mind: this makes the build being licensed as GNU GPL, which in turn
# means you need to open the source code and allow modifications!
OPTION(USE_GPL_SRC "build with support for libsamplerate. Do not publish binary if you want to keep your sources closed (infects source with GPL)." OFF)

#tell the compiler to include the version number as preprocessor directive
ADD_DEFINITIONS(-DVERSION=${musicaccess_VERSION_MAJOR}.${musicaccess_VERSION_MINOR})

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    MESSAGE("CMAKE_BUILD_TYPE set to Debug.")
ELSEIF (CMAKE_BUILD_TYPE STREQUAL Release)
    MESSAGE("CMAKE_BUILD_TYPE set to Release.")
    #für assert.h
    ADD_DEFINITIONS(-DNDEBUG)
ELSEIF (CMAKE_BUILD_TYPE STREQUAL Profiling)
    MESSAGE("CMAKE_BUILD_TYPE set to Profiling.")
    SET(CMAKE_CXX_FLAGS_PROFILING "-pg")
#    SET(CMAKE_EXE_LINKER_FLAGS_PROFILING "-pg")
    #for assert.h
    ADD_DEFINITIONS(-DNDEBUG)
ELSEIF (CMAKE_BUILD_TYPE STREQUAL Valgrind)
    MESSAGE("CMAKE_BUILD_TYPE set to Valgrind.")
    SET(CMAKE_CXX_FLAGS_VALGRIND "-g -O3")
#    SET(CMAKE_EXE_LINKER_FLAGS_PROFILING "-pg")
    #for assert.h
    ADD_DEFINITIONS(-DNDEBUG)
ELSE()
    MESSAGE ( "Choose build type by typing \"cmake -DCMAKE_BUILD_TYPE=Debug\" or \"cmake -DCMAKE_BUILD_TYPE=Release\" or \"cmake -DCMAKE_BUILD_TYPE=Profiling\" or \"cmake -DCMAKE_BUILD_TYPE=Valgrind\".")
ENDIF ()

IF (DEBUG_LEVEL)
    ADD_DEFINITIONS(-DDEBUG_LEVEL=${DEBUG_LEVEL})
    MESSAGE( "DEBUG_LEVEL set to " ${DEBUG_LEVEL})
ELSE(DEBUG_LEVEL)
    IF (CMAKE_BUILD_TYPE STREQUAL Debug)
        ADD_DEFINITIONS(-DDEBUG_LEVEL=200)
        MESSAGE( "DEBUG_LEVEL set to 200.")
    ELSE()
        MESSAGE( "DEBUG_LEVEL cleared.")
    ENDIF()
ENDIF()

#here, you would set some compiler specific things
IF ( CMAKE_COMPILER_IS_GNUCXX )
    MESSAGE ("Using Gnu g++ compiler...")
    #gcc should be more pedantic.
    ADD_DEFINITIONS(-Wall -pedantic -Wno-long-long)
ELSE ( MSVC )
    MESSAGE ("Using MS Visual Studio compiler...")
ENDIF()

#set cmake module directory, which is needed to find other libs
SET(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/modules/"
    )

FIND_PACKAGE(Sndfile REQUIRED)
FIND_PACKAGE(Mpg123 REQUIRED)
#use doxygen for documentation. don't need this for compilation.
FIND_PACKAGE(Doxygen)
#octave or matlab will be used for some test cases
FIND_PACKAGE(Octave)
FIND_PACKAGE(Matlab)
FIND_PACKAGE(Vorbisfile)
IF(VORBISFILE_FOUND)
    ADD_DEFINITIONS(-DHAVE_VORBISFILE=1)
    INCLUDE_DIRECTORIES(${VORBISFILE_INCLUDE_DIR})
ENDIF()

# if wanted by the user, compile with secret rabbit code support.
# this should not be enabled by default!
IF(USE_GPL_SRC)
    MESSAGE ("Building with libsamplerate, do not publish binary if you do not intent to open your sources...")
    FIND_PACKAGE(SRC REQUIRED)
    ADD_DEFINITIONS(-DUSE_GPL_SRC=1)
ENDIF(USE_GPL_SRC)


#place source files here
SET(
    musicaccess_SOURCES
    src/musicaccess/resample.cpp
    src/musicaccess/filter.cpp
    src/musicaccess/soundfile.cpp
    )

#place header here
SET(
    musicaccess_HEADERS
    src/musicaccess.hpp
    src/musicaccess/resample.hpp
    src/musicaccess/filter.hpp
    src/musicaccess/soundfile.hpp
    src/musicaccess/stringhelper.hpp
    )

#put all (possibly autocreated) headers and source files here
SET(
    musicaccess_TEST_SOURCES
    ${musicaccess_SOURCES}
    src/tests/tests.cpp
    src/tests/testframework.cpp
    src/tests/debug.cpp
    src/tests/main.cpp
    src/tools/console_colors.cpp
    )

SET(
    musicaccess_TEST_HEADERS
    ${musicaccess_HEADERS}
    src/tests/tests.hpp
    src/tests/testframework.hpp
    src/tests/debug.hpp
    src/tests/main.hpp
    src/tools/console_colors.hpp
    )    

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SQLITE_INCLUDE_DIRS}
    ${SRC_INCLUDE_DIR}      #note: SRC will only be included if explicitly wanted on the command line.
    ${MPG123_INCLUDE_DIR_UNIX}
    src/
    src/musicaccess
    src/tests
    src/tools
    )

#to copy a directory (e.g. test files) to another.
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF(DOXYGEN_FOUND)
    #convert semicolon-seperated list to whitespace-seperated one
    FOREACH(ARG ${musicaccess_HEADERS})
        SET(musicaccess_HEADERS_WHITESPACE "${musicaccess_HEADERS_WHITESPACE} ${CMAKE_CURRENT_SOURCE_DIR}/${ARG}")
    ENDFOREACH(ARG ${musicaccess_HEADERS})
    FOREACH(ARG ${musicaccess_SOURCES})
        SET(musicaccess_SOURCES_WHITESPACE "${musicaccess_SOURCES_WHITESPACE} ${CMAKE_CURRENT_SOURCE_DIR}/${ARG}")
    ENDFOREACH(ARG ${musicaccess_SOURCES})
    GET_DIRECTORY_PROPERTY(DIRINC INCLUDE_DIRECTORIES)
    FOREACH(ARG ${DIRINC})
        SET(musicaccess_INCLUDES_WHITESPACE "${musicaccess_INCLUDES_WHITESPACE} ${ARG}")
    ENDFOREACH(ARG ${DIRINC})
    
    #add configuration file
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    #add doxygen as new target
    ADD_CUSTOM_TARGET(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    #"make clean" should also clean doxygen
    SET_PROPERTY(DIRECTORY APPEND PROPERTY
	     ADDITIONAL_MAKE_CLEAN_FILES api-doc)
    GET_TARGET_PROPERTY(DOC_TARGET doc TYPE)
    IF(NOT DOC_TARGET)
        ADD_CUSTOM_TARGET(doc)
    ENDIF()
    ADD_DEPENDENCIES(doc doxygen)
ENDIF()

#add executable files for our project
ADD_LIBRARY(
    musicaccess SHARED
    ${musicaccess_SOURCES}
    )

IF(VORBISFILE_FOUND)
    TARGET_LINK_LIBRARIES(musicaccess
    ${VORBISFILE_LIBRARIES}
    )
ENDIF(VORBISFILE_FOUND)

#tell the linker which libs need to be linked
TARGET_LINK_LIBRARIES(musicaccess
    ${SNDFILE_LIBRARIES}
    ${MPG123_LIBRARIES}
    ${SRC_LIBRARIES}              #note: SRC will only be included if explicitly wanted on the command line.
    m
    )
SET_TARGET_PROPERTIES(musicaccess
    PROPERTIES COMPILE_FLAGS "-fPIC -shared"
    )

#add unit tests here
ENABLE_TESTING()
ADD_TEST(basename                  "musicaccesstests" "basename")
ADD_TEST(endswith                  "musicaccesstests" "endswith")
ADD_TEST(soundfile                 "musicaccesstests" "soundfile")

ADD_TEST(iirfilter                 "musicaccesstests" "iirfilter")
SET_TESTS_PROPERTIES(iirfilter PROPERTIES DEPENDS soundfile)
ADD_TEST(sortingiirfilter          "musicaccesstests" "sortingiirfilter")
SET_TESTS_PROPERTIES(sortingiirfilter PROPERTIES DEPENDS soundfile)

ADD_TEST(resampling                "musicaccesstests" "resampling")
SET_TESTS_PROPERTIES(resampling PROPERTIES DEPENDS soundfile)

SET_PROPERTY(DIRECTORY
        APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES "*.wav")

IF(MATLAB_FOUND)
    ADD_TEST(fft-iirfilter-matlab                 "matlab" "wav2fft.m" "filtered*.wav")
    ADD_TEST(fft-sortingiirfilter-matlab          "matlab" "wav2fft.m" "sortfiltered*.wav")
    ADD_TEST(fft-resampling-matlab                "matlab" "wav2fft.m" "resampled*.wav")
    SET_TESTS_PROPERTIES(fft-iirfilter-matlab PROPERTIES DEPENDS iirfilter)
    SET_TESTS_PROPERTIES(fft-resampling-matlab PROPERTIES DEPENDS resampling)
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/wav2fft.m DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE(MATLAB_FOUND)
    IF(OCTAVE_FOUND)
        ADD_TEST(fft-iirfilter-octave                ${OCTAVE_EXECUTABLE} "--path" ${CMAKE_CURRENT_BINARY_DIR} "wav2fft.m" "filtered*.wav")
        ADD_TEST(fft-sortingiirfilter-octave         ${OCTAVE_EXECUTABLE} "--path" ${CMAKE_CURRENT_BINARY_DIR} "wav2fft.m" "sortfiltered*.wav")
        ADD_TEST(fft-resampling-octave               ${OCTAVE_EXECUTABLE} "--path" ${CMAKE_CURRENT_BINARY_DIR} "wav2fft.m" "resampled*.wav")
        SET_TESTS_PROPERTIES(fft-iirfilter-octave PROPERTIES DEPENDS iirfilter)
        SET_TESTS_PROPERTIES(fft-resampling-octave PROPERTIES DEPENDS resampling)
        FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/wav2fft.m DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    ENDIF(OCTAVE_FOUND)
ENDIF(MATLAB_FOUND)

ADD_EXECUTABLE(musicaccesstests
    ${musicaccess_TEST_SOURCES}
    )

IF(VORBISFILE_FOUND)
    TARGET_LINK_LIBRARIES(musicaccesstests
    ${VORBISFILE_LIBRARIES}
    )
ENDIF(VORBISFILE_FOUND)

#tell the linker which libs need to be linked
TARGET_LINK_LIBRARIES(musicaccesstests
    ${SNDFILE_LIBRARIES}
    ${MPG123_LIBRARIES}
    ${SRC_LIBRARIES}              #note: SRC will only be included if explicitly wanted on the command line.
    m
    )
SET_TARGET_PROPERTIES(musicaccesstests
    PROPERTIES COMPILE_FLAGS "-fPIC -shared"
    )
#use "make test" to start all tests.

#use this to automatically create installation packages for different platforms
IF(UNIX)
    SET(CPACK_SOURCE_GENERATOR
        TGZ
        TBZ2
    )
    SET(CPACK_GENERATOR
        TGZ
        TBZ2
        RPM
        DEB
    )
ELSE(UNIX)
    SET(CPACK_SOURCE_GENERATOR
        ZIP
    )
    SET(CPACK_GENERATOR
        NSIS
    )
ENDIF(UNIX)

#meta data
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "musicaccess is a library to centralize access to different music formats"
    )
SET(CPACK_PACKAGE_VENDOR
    "-"
    )
SET(CPACK_PACKAGE_DESCRIPTION_FILE
    #"${CMAKE_CURRENT_SOURCE_DIR}/README.TXT"
    )
SET(CPACK_RESOURCE_FILE_LICENSE
    #"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.TXT"
    )
SET(CPACK_PACKAGE_VERSION_MAJOR
    ${musicaccess_VERSION_MAJOR}
    )
SET(CPACK_PACKAGE_VERSION_MINOR
    ${musicaccess_VERSION_MINOR}
    )
SET(CPACK_PACKAGE_CONTACT
    "lbruder@rim.com"
    )
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER
    "Lena Brueder"
    )
#maybe set something better here
SET(CPACK_PACKAGE_SECTION
    "music"
    )
SET(CPACK_SOURCE_IGNORE_FILES
    ${CMAKE_BINARY_DIR}/*
    ${CMAKE_CURRENT_BINARY_DIR}/*
    ${CMAKE_CURRENT_SOURCE_DIR}/.gitignore
    ${CMAKE_CURRENT_SOURCE_DIR}/.git
    ${CMAKE_CURRENT_SOURCE_DIR}/build/*
    )
INCLUDE(CPack)

INSTALL (TARGETS musicaccess
    LIBRARY DESTINATION lib
    )
FILE(GLOB music_global_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
INSTALL (FILES ${music_global_headers}
    DESTINATION include
    )
FILE(GLOB music_special_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/musicaccess/*.hpp")
INSTALL (FILES ${music_special_headers}
    DESTINATION include/musicaccess
    )
