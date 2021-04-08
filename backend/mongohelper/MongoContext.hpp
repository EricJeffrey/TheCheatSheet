#if !defined(CONTEXT_HPP)
#define CONTEXT_HPP

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <string>

using std::string;

// helpers for mongodb
namespace mongohelper {
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

mongocxx::pool::entry mongoClientEntry();

// Get collection, the lifetime of collection should be within the lifetime of client.
mongocxx::collection mongoCollection(const mongocxx::pool::entry &clientEntry,
                                     const string &collectionName);
} // namespace mongohelper
#endif // CONTEXT_HPP
