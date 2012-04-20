#  Test Program

# name and source files
add_executable(SFMLSpriterTest main.cpp)
# link SFML and SFMLSpriter
target_link_libraries(SFMLSpriterTest ${SFML_LIBRARIES} SFMLSpriter)
# add include directories
include_directories(SFMLSpriterTest ${SFML_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/include")
# set where to install
install(TARGETS SFMLSpriterTest DESTINATION "${CMAKE_SOURCE_DIR}/bin")