cmake_minimum_required(VERSION 2.6)
# name and language
project (SFMLSpriter CXX)

add_definitions(-Wall)
# set(CMAKE_C_FLAGS "-Wall")

# add cmake_modules directory to module path (contains magic SFML finder)
set(CMAKE_MODULE_PATH "cmake_modules" ${CMAKE_MODULE_PATH})
# magic SFML locating, sets SFML_LIBRARIES and SFML_INCLUDE_DIR, amongst others
find_package(SFML 2 REQUIRED system window graphics)

#  Library
# adding include path
include_directories("include" ${SFML_INCLUDE_DIRS})
# adding source files, compiling to library - <executable> <source 1> <source 2> ... <source n>
add_library(SFMLSpriter SHARED src/SFMLSpriter/Character.cpp)
add_library(SFMLSpriter_s STATIC src/SFMLSpriter/Character.cpp)

# copying library to lib/ on install
install(TARGETS SFMLSpriter SFMLSpriter_s DESTINATION "lib")

# also execute test/CMakeList.txt
add_subdirectory(test)