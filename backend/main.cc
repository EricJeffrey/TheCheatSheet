#include "lib/httplib.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

void mongoTest() {
    mongocxx::instance instance{};
    mongocxx::uri tmpUri("mongodb://172.17.0.2:27017");
    mongocxx::client client(tmpUri);
    auto db = client.database("cheatsheet");
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value =
        builder << "name"
                << "MongoDB"
                << "type"
                << "database"
                << "count" << 1 << "versions" << bsoncxx::builder::stream::open_array << "v3.2"
                << "v3.0"
                << "v2.6" << bsoncxx::builder::stream::close_array << "info"
                << bsoncxx::builder::stream::open_document << "x" << 203 << "y" << 102
                << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    auto collection = db.collection("test");
    collection.insert_one(doc_value.view());
    mongocxx::cursor cursor = collection.find({});
    for (auto doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << "\n";
    }
}

void serverTest() {
    httplib::Server server;
    server.Get("/", [&](const httplib::Request &request, httplib::Response &response) {
        response.set_content("hello world", "text/plain");
    });
    server.listen("0.0.0.0", 8000);
}

int main(int argc, char const *argv[]) {
    mongoTest();
    // serverTest();
    return 0;
}
