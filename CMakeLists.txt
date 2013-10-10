cmake_minimum_required(VERSION 2.8)

project(geec)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")

find_package(ENET REQUIRED)

if(ENET_FOUND)
	include_directories(${ENET_INCLUDE_DIRS})
	set(LIBRARIES ${LIBRARIES} ${ENET_LIBRARIES})
endif()

find_package(SFML REQUIRED COMPONENTS system window graphics audio network)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	set(LIBRARIES ${LIBRARIES} ${SFML_LIBRARIES})
endif()

find_package(GERM REQUIRED)

if(GERM_FOUND)
	include_directories(${GERM_INCLUDE_DIRS})
	set(LIBRARIES ${LIBRARIES} ${GERM_LIBRARIES})
endif()

include_directories("include")

#Folders to be searched
set(FOLDERS "src")

#Adding source files in each of the FOLDERS to EXECUTABLES
foreach(FOLDER ${FOLDERS})
	file(GLOB GLOBFILES "${FOLDER}/*.cpp")
	set(EXECUTABLES ${EXECUTABLES} ${GLOBFILES})
endforeach(FOLDER)

add_library(geec SHARED ${EXECUTABLES})
target_link_libraries(geec ${LIBRARIES})
