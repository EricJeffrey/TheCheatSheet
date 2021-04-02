#if !defined(CONTEXT_CC)
#define CONTEXT_CC

#include "Context.hpp"

mongocxx::instance MongoContext::Instance;
mongocxx::uri MongoContext::Uri("mongodb://172.17.0.4:27017");
string MongoContext::DBName = "cheatsheet";
string MongoContext::COLLECTION_CODE_SEGMENT = "codesegment";
string MongoContext::COLLECTION_USER = "user";
string MongoContext::COLLECTION_TAG = "tag";

#endif // CONTEXT_CC
