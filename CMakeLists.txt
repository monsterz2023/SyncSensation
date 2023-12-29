cmake_minimum_required(VERSION 3.15)
# add dependencies
include(cmake/CPM.cmake)

project(SyncSensation VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options: Turn on with 'cmake -DBUILD_TESTS=ON'.
option(BUILD_TESTS "Build all tests." OFF) # Turn off by default

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.cpp" "src/domain/*.cpp")

# add_subdirectory(src)

# If testing is enabled, include the tests subdirectory
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.11.3
  GITHUB_REPOSITORY nlohmann/json
  OPTIONS
    "JSON_BuildTests OFF"
)
# boost is a huge project and will take a while to download
# using `CPM_SOURCE_CACHE` is strongly recommended
CPMAddPackage(
  NAME Boost
  VERSION 1.84.0
  GITHUB_REPOSITORY "boostorg/boost"
  GIT_TAG "boost-1.84.0"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(SyncSensation PRIVATE nlohmann_json::nlohmann_json)
