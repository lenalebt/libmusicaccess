# Find libsndfile
#
# SNDFILE_INCLUDE_DIR - where to find sndfile.h, etc.
# SNDFILE_LIBRARIES - List of libraries when using libsndfile.
# SNDFILE_FOUND - True if libsndfile found.

IF(SNDFILE_INCLUDE_DIR)
    # be silent, file already in cache
    SET(SNDFILE_FIND_QUIETLY TRUE)
ENDIF(SNDFILE_INCLUDE_DIR)

FIND_PATH(SNDFILE_INCLUDE_DIR sndfile.h PATHS /usr/include)

FIND_LIBRARY(SNDFILE_LIBRARY NAMES sndfile sndfile-1 PATHS /usr/lib /armle-v7/usr/lib)

# Handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if
# all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SNDFILE DEFAULT_MSG SNDFILE_LIBRARY SNDFILE_INCLUDE_DIR)

IF(SNDFILE_FOUND)
  SET(SNDFILE_LIBRARIES ${SNDFILE_LIBRARY})
ELSE(SNDFILE_FOUND)
  SET(SNDFILE_LIBRARIES)
ENDIF(SNDFILE_FOUND)

MARK_AS_ADVANCED(SNDFILE_INCLUDE_DIR SNDFILE_LIBRARY)
