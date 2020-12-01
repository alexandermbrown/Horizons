#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::iconv::libcharset" for configuration "Debug"
set_property(TARGET unofficial::iconv::libcharset APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::iconv::libcharset PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libcharset.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS unofficial::iconv::libcharset )
list(APPEND _IMPORT_CHECK_FILES_FOR_unofficial::iconv::libcharset "${_IMPORT_PREFIX}/debug/lib/libcharset.lib" )

# Import target "unofficial::iconv::libiconv" for configuration "Debug"
set_property(TARGET unofficial::iconv::libiconv APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::iconv::libiconv PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libiconv.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS unofficial::iconv::libiconv )
list(APPEND _IMPORT_CHECK_FILES_FOR_unofficial::iconv::libiconv "${_IMPORT_PREFIX}/debug/lib/libiconv.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
