#  Test Program
add_executable(SFMLSpriterTest main.cpp)
target_link_libraries(SFMLSpriterTest ${SFML_LIBRARIES})
include_directories(SFMLSpriterTest ${SFML_INCLUDE_DIR})
install(TARGETS SFMLSpriterTest DESTINATION "../bin")