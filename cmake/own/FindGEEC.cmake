find_path(GEEC_INCLUDE_DIR NAMES EntityManager.h)
find_library(GEEC_LIBRARY NAMES geec)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GEEC DEFAULT_MSG GEEC_INCLUDE_DIR GEEC_LIBRARY)

if(GEEC_FOUND)
	set(GEEC_INCLUDE_DIRS ${GEEC_INCLUDE_DIR})
	set(GEEC_LIBRARIES ${GEEC_LIBRARY})
endif()

mark_as_advanced(GEEC_INCLUDE_DIR GEEC_LIBRARY)