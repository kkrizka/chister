# - Test for National Instruments IMAQ
# Once loaded this will define
#   NIIMAQ_FOUND        - system has NI-IMAQ
#   NIIMAQ_INCLUDE_DIR  - include directory for NI-IMAQ
#   NIIMAQ_LIBRARIES    - libraries you need to link to

FIND_PATH(NIIMAQ_INCLUDE_DIR niimaq.h
  PATHS "${NIIMAQ_DIR}/Include"
  DOC "Diretory containing niimaq.h include file")

FIND_LIBRARY(NIIMAQ_LIBRARIES NAMES imaq
  PATHS "${NIIMAQ_DIR}/Lib/MSVC"
  DOC "niimaq library for linking")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NIIMAQ DEFAULT_MSG NIIMAQ_INCLUDE_DIR NIIMAQ_LIBRARIES)

MARK_AS_ADVANCED(NIIMAQ_INCLUDE_DIR NIIMAQ_LIBRARIES)
