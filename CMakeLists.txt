cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME dr-web-test)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only if it's the main project
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#    include(CTest)
#endif()

add_subdirectory(src)
add_subdirectory(apps)

#option(${PROJECT_NAME}_BUILD_TESTING "Build tests for '${PROJECT_NAME}' when it is used as a subdirectory" OFF)
#if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR ${PROJECT_NAME}_BUILD_TESTING) AND BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
