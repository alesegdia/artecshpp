set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# SET SOURCES AND STUFF
file(GLOB_RECURSE LIB_SRC lib/*)
file(GLOB_RECURSE TEST_SRC test/*)

add_executable(artecshpp-test ${TEST_SRC})
add_library(artecshppcore ${LIB_SRC})

include_directories(artecshppcore lib)
include_directories(artecshpp-test lib)

include_directories(artecshppcore ../modules/rztl/include)

target_link_libraries(artecshpp-test artecshppcore)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

