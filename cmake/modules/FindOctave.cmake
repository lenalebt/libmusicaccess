# Find octave (no mex and other things)
#
# OCTAVE_FOUND      - True if octave found.
# OCTAVE_EXECUTABLE - contains the name and path to the octave executable

IF(OCTAVE_EXECUTABLE)
    # be silent, file already in cache
    SET(OCTAVE_FIND_QUIETLY TRUE)
ENDIF(OCTAVE_EXECUTABLE)

FIND_PROGRAM(OCTAVE_EXECUTABLE
    NAMES octave
    PATHS /usr/bin /bin
    DOC "GNU Octave"
)


# Handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if
# all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OCTAVE DEFAULT_MSG OCTAVE_EXECUTABLE)

MARK_AS_ADVANCED(OCTAVE_EXECUTABLE)
