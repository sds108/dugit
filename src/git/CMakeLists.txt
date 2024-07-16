add_library(Git STATIC git.cpp git.h)
set_target_properties(Git PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Git PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Git PUBLIC Include)