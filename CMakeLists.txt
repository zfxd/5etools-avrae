# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("5etools-avrae")

# Include sub-projects.
add_subdirectory ("src")

# Add include directories
include_directories("include")

#nlohmann json

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Add executable
add_executable(5etools-avrae "src/main.cpp" "include/Avrae.h")

target_link_libraries(5etools-avrae PRIVATE nlohmann_json::nlohmann_json)
