find_path(ENET_INCLUDE_DIR NAMES enet.h)
find_library(ENET_LIBRARY NAMES enet)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(ENET DEFAULT_MSG ENET_INCLUDE_DIR ENET_LIBRARY)

if(ENET_FOUND)
	set(ENET_INCLUDE_DIRS ${ENET_INCLUDE_DIR})
	set(ENET_LIBRARIES ${ENET_LIBRARY})
	if(WIN32)
		set(ENET_LIBRARIES ${ENET_LIBRARIES} ws2_32 winmm)
	endif()
endif()

mark_as_advanced(ENET_INCLUDE_DIRS ENET_LIBRARIES)