
enable_testing()

aux_source_directory(../mongohelper/ TEST_SRC_DIRS)
add_executable(MongoTest MongoTest.cc ${TEST_SRC_DIRS})
target_link_libraries(MongoTest ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(MongoTest)