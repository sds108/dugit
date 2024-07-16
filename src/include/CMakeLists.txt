add_library(Include STATIC include.h)
set_target_properties(Include PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})