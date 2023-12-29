cmake_minimum_required(VERSION 3.15)
include(FetchContent)
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

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(boost URL https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.gz)
FetchContent_MakeAvailable(boost)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(SyncSensation PRIVATE nlohmann_json::nlohmann_json)
