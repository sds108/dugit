add_library(Config STATIC config.h)
set_target_properties(Config PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Config PUBLIC Include)