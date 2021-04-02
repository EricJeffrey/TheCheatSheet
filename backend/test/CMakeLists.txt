
enable_testing()

add_executable(MongoTest MongoTest.cc ../MongoHelper.cc ../Context.cc)
target_link_libraries(MongoTest ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(MongoTest)