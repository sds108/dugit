add_library(Tests STATIC tests.cpp tests.h)
set_target_properties(Tests PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Tests PUBLIC Include)
target_link_libraries(Tests PUBLIC Git)

# Tester
add_executable(${PROJECT_NAME}_tester main.cpp)
target_link_libraries(${PROJECT_NAME}_tester PRIVATE Include)
target_link_libraries(${PROJECT_NAME}_tester PRIVATE Git)
target_link_libraries(${PROJECT_NAME}_tester PRIVATE Tests)
target_link_libraries(${PROJECT_NAME}_tester PRIVATE Service)