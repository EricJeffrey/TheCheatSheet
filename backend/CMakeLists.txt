cmake_minimum_required(VERSION 3.0.0)

project(cheatsheet_backend VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(lib)
add_subdirectory(test)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./mongohelper DIR_SRCS)
aux_source_directory(./eshelper DIR_SRCS)
aux_source_directory(./entity DIR_SRCS)
aux_source_directory(./controller DIR_SRCS)
aux_source_directory(./controller/frame DIR_SRCS)
aux_source_directory(./util DIR_SRCS)
aux_source_directory(./httpserver DIR_SRCS)
aux_source_directory(./request DIR_SRCS)
aux_source_directory(./config DIR_SRCS)

add_executable(cheatsheet_backend ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} httplib)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
