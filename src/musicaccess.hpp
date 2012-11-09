#ifndef MUSICACCESS_HPP
#define MUSICACCESS_HPP

#include "musicaccess/soundfile.hpp"
#include "musicaccess/resample.hpp"
#include "musicaccess/filter.hpp"


/**
 * @mainpage libmusicaccess
 * libmusicaccess is a helper library which helps opening various sound
 * formats and presenting them in a standard format (22khz, mono, 16bit).
 * libmusicaccess allows opening/reading, filtering and resampling tasks.
 * 
 * @section mainpage_install Install
 * @subsection mainpage_install_prequisites Prequisites
 * To install the library, you need to install some other libraries and tools:
 *  * <code>doxygen</code>, if you want to build the documentation. This package is optional.
 *  * <code>cmake</code>, which is the build system and needed to compile the software.
 *  * <code>libmpg123</code>, which is used to load mp3 files and read their metadata
 *  * <code>libsndfile</code>, used to load all file types that cannot be handled by libmpg123.
 *      <code>libsndfile</code> has some dependencies if you want to read e.g. FLAC or ogg/Vorbis files -
 *      if you need them, install the dependencies. They are not needed to run the software.
 *  * <code>libvorbisfile</code>, used to read metadata from ogg/Vorbis files. This package is optional.
 *  * <code>matlab</code> or <code>octave</code>, which is used for some software tests. This is optional if
 *      you do not need the software tests; otherwise you only need one of the dependencies, not both.
 *  * <code>libsamplerate</code> if you do not want to use the builtin (buggy) resampling algorithms.
 *      To use this library, you need to call cmake with the <code>-DUSE_GPL_SRC</code> flag.
 *      The resulting software will be GPL, do not publish the binaries if you do not want to
 *      publish the sources.
 * @subsection mainpage_normal_install Installing on a normal *nix system
 * 
 * If you want to install the library in a normal, 
 * non-cross-compile-environment, just go to the "build/" directory and
 * type
 * 
 * @code{.sh}
 * cmake -DCMAKE_BUILD_TYPE=Release -DUSE_GPL_SRC ..
 * make -j5
 * make install
 * @endcode
 * 
 * If the code may not contain GPL code, type instead:
 * @code{.sh}
 * cmake -DCMAKE_BUILD_TYPE=Release ..
 * make -j5
 * make install
 * @endcode
 * 
 * The last step of both listings might require root privileges. If you want to build 
 * the docs, type
 * 
 * @code{.sh}
 * make doc
 * @endcode
 * 
 * in the very same directory. You will find the docs in the api-doc/ 
 * directory. There are two formats available, for the more common HTML
 * version open the <code>file api-doc/html/index.html</code>.
 * 
 * @subsection mainpage_crosscompilation_install Installing for Cross-Compilation
 * 
 * If you want to cross-compile and install the library, you need to 
 * build the library with a cmake toolchain file and afterwards copy 
 * the files manually to the right directories. This step highly 
 * depends on the environment you are using.
 * 
 * Example: Blackberry Playbook NDK 2.0.1
 * 
 * Assuming you installed the NDK in <code>/opt/bbndk-2.0.1/</code>, you need to:
 * - build the library:
 *     * go to the <code>build/</code> directory. Type:
 *         @code{.sh}
 *         cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/modules/Toolchain-QNX-6.5.0-armv7.cmake ..
 *         make -j4
 *         @endcode
 * - copy the file <code>libmusicaccess.so</code> to <code>/opt/bbndk-2.0.1/target/qnx6/armle-v7/usr/lib</code>
 * - copy all header files (<code>*.hpp</code>) from the <code>../src/</code> directory to 
 *   <code>/opt/bbndk-2.0.1/target/qnx6/usr/include</code>. Also copy the headers 
 *   in the subdirectories!
 * 
 * All libraries depending on this library should now be able to find it.
 * 
 * @section mainpage_finding Finding the library on a system
 * If you want to search for the library on a system, and you are using CMake,
 * you could use the supplied CMake module <code>FindMusicaccess.cmake</code>
 * in <code>cmake/modules/</code>. It is documented in its source code,
 * but essentially it is used in the very same way as every other CMake
 * find module.
 * 
 * @section mainpage_usage Standard usage
 * If you want to open and read a file, you would use musicaccess::SoundFile. You will find some examples
 * there.
 * 
 * To resample a given piece of audio, feed it to musicaccess::Resampler22kHzMono.
 * 
 * @section mainpage_libsamplerate Libsamplerate
 * This library may be compiled with support for libsamplerate
 * (aka secret rabbit code) for resampling,
 * as the internal resampling algorithms do not work very good. If you do so,
 * and you want to publish your binaries, you need to publish your sources, too, and
 * they will be licensed as GNU GPL! You can find the sources of the library here:
 * http://www.mega-nerd.com/SRC/ .
 * 
 * 
 */

#endif 
