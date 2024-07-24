add_library(Service STATIC service.cpp service.h)
set_target_properties(Service PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Service PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Service PUBLIC Include)