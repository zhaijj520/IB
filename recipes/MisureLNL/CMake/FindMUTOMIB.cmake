# - Try to find MUTOMIB
# Once done this will define
#
#  MUTOMIB_FOUND - system has MUTOMIB
#  MUTOMIB_INCLUDE_DIR - the MUTOMIB include directory
#  MUTOMIB_LIBRARIES - The libraries needed to use MUTOMIB
#  MUTOMIB_DEFINITIONS - Compiler switches required for using MUTOMIB
#

if (NOT(MUTOMIB_INCLUDE_DIR AND MUTOMIB_LIBRARY_DIR AND MUTOMIB_CORE_DIR AND MUTOMIB_ROOT_DIR))
  SET (MUTOMIB_INCLUDE_DIR MUTOMIB_INCLUDE_DIR-NOTFOUND)
  SET (MUTOMIB_LIB_DIR MUTOMIB_LIB_DIR-NOTFOUND)
  SET (MUTOMIB_CORE_DIR MUTOMIB_CORE_DIR-NOTFOUND)
  SET (MUTOMIB_ROOT_DIR MUTOMIB_ROOT_DIR-NOTFOUND)
  SET (MUTOMIB_PLISTS_LIB_DIR MUTOMIB_PLISTS_LIB_DIR-NOTFOUND)
endif (NOT(MUTOMIB_INCLUDE_DIR AND MUTOMIB_LIBRARY_DIR AND MUTOMIB_CORE_DIR AND MUTOMIB_ROOT_DIR))

MESSAGE(STATUS "Looking for MUTOMIB...")

#________________________
#---- MuTomIB
#/home/$ENV{USER}
SET (MUTOMIB_DIR "../../../../MuTomIB/trunk/")

FIND_PATH(MUTOMIB_INCLUDE_DIR NAMES FileReader.hh PATHS
  ${MUTOMIB_DIR}/include/
    NO_DEFAULT_PATH
  )

SET(MUTOMIB_INCLUDE_DIR
  ${MUTOMIB_DIR}/include/
  )

FIND_PATH(MUTOMIB_LIB_DIR NAMES libMuTomIB.so PATHS
  ${MUTOMIB_DIR}/build/lib/
  NO_DEFAULT_PATH
  )

#________________________
#---- ROOTLink objects
FIND_PATH(MUTOMIB_ROOTLINK_DIR NAMES dict.h PATHS
  ${MUTOMIB_DIR}/ROOTLink/
  NO_DEFAULT_PATH
  )

SET(MUTOMIB_ROOTLINK_DIR
  ${MUTOMIB_DIR}/ROOTLink/
  )

#________________________
#---- Core objects
FIND_PATH(MUTOMIB_CORE_DIR NAMES Options.hh PATHS
  ${MUTOMIB_DIR}/Core/
  NO_DEFAULT_PATH
  )

SET(MUTOMIB_CORE_DIR
  ${MUTOMIB_DIR}/Core/
  )


#________________________
#---- Get the library paths
IF (MUTOMIB_LIB_DIR)
  SET(MUTOMIB_LIBRARY_DIR ${MUTOMIB_LIB_DIR})
  SET(MUTOMIB_LIBRARIES ${MUTOMIB_LIB_DIR}/libMuTomIB.so)
ENDIF (MUTOMIB_LIB_DIR)

#________________________
#---- Check status and exit
if (MUTOMIB_INCLUDE_DIR AND MUTOMIB_LIBRARY_DIR AND MUTOMIB_ROOTLINK_DIR)
   set(MUTOMIB_FOUND TRUE)
endif (MUTOMIB_INCLUDE_DIR AND MUTOMIB_LIBRARY_DIR AND MUTOMIB_ROOTLINK_DIR)

if (MUTOMIB_FOUND)
  MESSAGE(STATUS "Looking for MUTOMIB... - found ${MUTOMIB_LIBRARY_DIR}")
  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${MUTOMIB_LIBRARY_DIR})
else (MUTOMIB_FOUND)
  if (MUTOMIB_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for MUTOMIB... - Not found")
  endif (MUTOMIB_FIND_REQUIRED)
endif (MUTOMIB_FOUND)

