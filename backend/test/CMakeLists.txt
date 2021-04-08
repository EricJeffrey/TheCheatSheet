
enable_testing()

aux_source_directory(../mongohelper/ TEST_MONGO_DIR)
aux_source_directory(../eshelper/ TEST_ES_DIR)
aux_source_directory(../util/ TEST_ES_DIR)
aux_source_directory(../entity/ TEST_ES_DIR)

add_executable(MongoTest MongoTest.cc ${TEST_MONGO_DIR})
target_link_libraries(MongoTest ${CONAN_LIBS})


add_executable(EsTest EsTest.cc ${TEST_ES_DIR})
target_link_libraries(EsTest ${CONAN_LIBS})
target_link_libraries(EsTest httplib)

include(GoogleTest)
gtest_discover_tests(MongoTest)
gtest_discover_tests(EsTest)