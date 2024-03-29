#if !defined(CONTEXT_CC)
#define CONTEXT_CC

#include "MongoContext.hpp"

namespace mongohelper {

mongocxx::instance MongoContext::Instance;
mongocxx::uri MongoContext::Uri("mongodb://172.17.0.4:27017");
string MongoContext::DBName = "cheatsheet";
string MongoContext::COLLECTION_CODE_SEGMENT = "codesegment";
string MongoContext::COLLECTION_USER = "user";
string MongoContext::COLLECTION_TAG = "tag";

void MongoContext::config(const string &host, const int32_t port) {
    MongoContext::Uri = mongocxx::uri{"mongodb://" + host + ':' + std::to_string(port)};
}

mongocxx::pool::entry mongoClientEntry() {
    static mongocxx::pool connectionPool(MongoContext::Uri);
    return connectionPool.acquire();
}

mongocxx::collection mongoCollection(const mongocxx::pool::entry &clientEntry,
                                     const string &collectionName) {
    return (*clientEntry)[MongoContext::DBName][collectionName];
}

} // namespace mongohelper
#endif // CONTEXT_CC
