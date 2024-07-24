# add subdirectories
add_subdirectory(include)
add_subdirectory(git)
add_subdirectory(config)
add_subdirectory(service)
add_subdirectory(tests)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Include)
target_link_libraries(${PROJECT_NAME} PRIVATE Git)
target_link_libraries(${PROJECT_NAME} PRIVATE Tests)
target_link_libraries(${PROJECT_NAME} PRIVATE Service)

# make the executable installable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)