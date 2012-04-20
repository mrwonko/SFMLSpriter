cmake_minimum_required(VERSION 2.6)
# name and language
project (SFMLSpriter CXX)

set(SFML_SPRITER_SOURCE_FILES "src/SFMLSpriter/Character.cpp" "src/SFMLSpriter/SimpleTextureManager.cpp")
set(SFML_SPRITER_INCLUDE_FILES "include/SFMLSpriter/Character.hpp" "include/SFMLSpriter/API.hpp" "include/SFMLSpriter/TextureManager.hpp" "include/SFMLSpriter/SimpleTextureManager.hpp")

add_definitions(-Wall -DSPRITER)
# set(CMAKE_C_FLAGS "-Wall")

# add cmake_modules directory to module path (contains magic SFML finder)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
# magic SFML locating, sets SFML_LIBRARIES and SFML_INCLUDE_DIR, amongst others
find_package(SFML 2 REQUIRED system window graphics)

#  Library
# adding include path
include_directories("${CMAKE_SOURCE_DIR}/include" ${SFML_INCLUDE_DIR})
# adding source files, compiling to library - <executable> <source 1> <source 2> ... <source n>
add_library(SFMLSpriter SHARED ${SFML_SPRITER_SOURCE_FILES}  ${SFML_SPRITER_INCLUDE_FILES})
add_library(SFMLSpriter_s STATIC ${SFML_SPRITER_SOURCE_FILES}  ${SFML_SPRITER_INCLUDE_FILES})
# add #define SFML_SPRITER_BUILD
set_target_properties(SFMLSpriter SFMLSpriter_s PROPERTIES COMPILE_DEFINITIONS "SFML_SPRITER_BUILD")
# link SFML libraries to dynamic build
target_link_libraries(SFMLSpriter ${SFML_LIBRARIES})

# copying library to lib / bin on install (.dll being a runtime, the corresponding .lib an archive, just like static libraries, while on other platforms dynamic libraries like .so are of type library)
install(TARGETS SFMLSpriter SFMLSpriter_s
		ARCHIVE DESTINATION "${CMAKE_SOURCE_DIR}/lib"
		LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/lib"
		RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")

# also execute test/CMakeList.txt
add_subdirectory(test)