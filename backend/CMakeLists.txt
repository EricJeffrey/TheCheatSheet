cmake_minimum_required(VERSION 3.0.0)

project(cheatsheet_backend VERSION 0.1.0)

add_definitions("-std=c++17")
add_subdirectory(lib)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# message("--${CMAKE_BINARY_DIR}")
# message("--${CONAN_LIBS}")

add_executable(cheatsheet_backend main.cc)

target_link_libraries(${PROJECT_NAME} httplib)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
