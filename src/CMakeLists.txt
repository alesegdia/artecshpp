set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# SET SOURCES AND STUFF
file(GLOB_RECURSE LIB_SRC lib/*)
file(GLOB_RECURSE TEST_SRC test/main.cpp)
file(GLOB_RECURSE TEST2_SRC test/viewtest.cpp)

add_executable(artecshpp-test ${TEST_SRC})
add_executable(artecshpp-test2 ${TEST2_SRC})
add_library(artecshppcore ${LIB_SRC})

include_directories(artecshppcore lib)
include_directories(artecshpp-test lib)
include_directories(artecshpp-test2 lib)

target_link_libraries(artecshpp-test artecshppcore)
target_link_libraries(artecshpp-test2 artecshppcore)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

