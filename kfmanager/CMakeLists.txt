cmake_minimum_required(VERSION 2.8)
project(kfrouter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#add_definitions(-D_MAXOS)
add_definitions(-DUSE_ZLOG)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libs" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libs/json" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libs/net/http" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/common" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/kfmd5" )

if(MTK)
add_definitions(-DMTK)
endif()

if(MIPS)
include_directories( ${cm_include} )
LINK_DIRECTORIES( ${cm_link} )
add_definitions( -Os -pipe -mno-branch-likely -mips32r2 -mtune=24kec -mdsp -fno-caller-saves -fhonour-copts -Wno-error=unused-but-set-variable -msoft-float -DNDEBUG )
endif()

if(PC)
link_libraries("/usr/lib/x86_64-linux-gnu")
include_directories("/usr/include/lua5.1")
endif()


add_subdirectory(common)
add_subdirectory(kfmd5)
add_subdirectory(libs)
add_subdirectory(kfmanager)

if(GOOGLETEST)
add_subdirectory(googletest)
endif()

#add_library(kfprogram STATIC)
#target_link_libraries(kfprogram kfcommon dl)

install(FILES kfmanager/kfmanager DESTINATION /usr/bin)

