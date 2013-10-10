find_path(GERM_INCLUDE_DIR NAMES ResourceManager.h)
find_library(GERM_LIBRARY NAMES germ)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GERM DEFAULT_MSG GERM_INCLUDE_DIR GERM_LIBRARY)

if(GERM_FOUND)
	set(GERM_INCLUDE_DIRS ${GERM_INCLUDE_DIR})
	set(GERM_LIBRARIES ${GERM_LIBRARY})
endif()

mark_as_advanced(GERM_INCLUDE_DIR GERM_LIBRARY)