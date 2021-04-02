#if !defined(CONTEXT_HPP)
#define CONTEXT_HPP

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <string>

using std::string;

// MongoDB related stuff
struct MongoContext {
    // This should be done only once
    static mongocxx::instance Instance;
    static mongocxx::uri Uri;
    static string DBName;
    static string COLLECTION_CODE_SEGMENT;
    static string COLLECTION_USER;
    static string COLLECTION_TAG;
};

struct Context {

    Context() {}
    ~Context() {}
};

inline mongocxx::pool::entry mongoClient() {
    static mongocxx::pool connectionPool(MongoContext::Uri);
    return connectionPool.acquire();
}

inline mongocxx::collection mongoCollection(const string &collectionName) {
    auto clientEntry = mongoClient();
    auto &client = *clientEntry;
    auto collection = client[MongoContext::DBName][collectionName];
    return collection;
}

#endif // CONTEXT_HPP
