cmake_minimum_required(VERSION 3.10)
project(dugit LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)

# specify build type
set(CMAKE_BUILD_TYPE Debug)

# add subdirectory
add_subdirectory(src)

# install targets
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
