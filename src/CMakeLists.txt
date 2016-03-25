
include(ExternalProject)
include(TestHelpers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# SET SOURCES AND STUFF
file(GLOB_RECURSE LIB_SRC lib/*)

add_library(artecshppcore ${LIB_SRC})
add_library(xstf STATIC IMPORTED)
set_property(TARGET xstf PROPERTY IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/module/xstf/lib/libxstf.a)

file(GLOB tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

if(NOT EXISTS "${CMAKE_HOME_DIRECTORY}/module/xstf/lib/libxstf.a")
	message(SEND_ERROR "You must build the xstf submodule first in order to create test targets."
					   " Do the following to update the project submodules:\n"
					   "git submodule init\n"
					   "git submodule update\n"
					   "Then, build the xstf project as a normal CMake project inside its folder: module/xstf")
	return()
endif()


foreach(test ${tests})
	get_filename_component(testName ${test} NAME_WE)
	addTests(${testName} ${test} "lib;${CMAKE_HOME_DIRECTORY}/module/xstf/include" "artecshppcore;xstf")
endforeach()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

