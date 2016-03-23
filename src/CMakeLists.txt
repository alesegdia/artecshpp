
include(TestHelpers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# SET SOURCES AND STUFF
file(GLOB_RECURSE LIB_SRC lib/*)
file(GLOB_RECURSE TEST_SRC test/main.cpp)
file(GLOB_RECURSE TEST2_SRC test/viewtest.cpp)

add_library(artecshppcore ${LIB_SRC})

file(GLOB tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

foreach(test ${tests})
	get_filename_component(testName ${test} NAME_WE)
	addTests(${testName} ${test} lib artecshppcore)
endforeach()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

