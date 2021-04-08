#if !defined(ES_HELPER_CC)
#define ES_HELPER_CC

#include "../entity/EntityHelper.hpp"
#include "../util/Utility.hpp"
#include "EsHelper.hpp"
#include "../lib/httplib.h"
#include "EsContext.hpp"
#include "nlohmann/json.hpp"

using HttpError = httplib::Error;
using HttpClient = httplib::Client;
using HttpParams = httplib::Params;
using HttpHeaders = httplib::Headers;

const static string slash = "/";

namespace eshelper {

std::optional<string> addCodeSegment(const CodeSegment &segment) {
    httplib::Client client{EsContext::HOST, EsContext::port};
    auto resp = client.Post((slash + EsContext::INDEX_CODE_SEGMENT + slash + "_doc").c_str(),
                            toJson(segment).dump(), CONTENT_TYPE_JSON().c_str());
    std::optional<string> res;
    if (resp.error() == HttpError::Success) {
        auto respJson = NlohmannJson::parse(resp.value().body);
        if (!respJson.contains("error") && respJson.contains("result"))
            res = respJson["_id"];
    }
    return res;
}

bool updateCodeSegment(const CodeSegment &segment) {
    bool res = false;
    if (!segment.mEsId.empty()) {
        httplib::Client client{EsContext::HOST, EsContext::port};
        auto resp = client.Put(
            (slash + EsContext::INDEX_CODE_SEGMENT + slash + "_doc" + slash + segment.mEsId)
                .c_str(),
            toJson(segment).dump(), CONTENT_TYPE_JSON().c_str());
        if (resp.error() == HttpError::Success) {
            auto respJson = NlohmannJson::parse(resp.value().body);
            if (!respJson.contains("error") && respJson.contains("result"))
                res = true;
        }
    }
    return res;
}

vector<CodeSegment> search(const string &text, int32_t page, int32_t pagesSize) {
    vector<CodeSegment> res;
    res.reserve(pagesSize);
    httplib::Client client{EsContext::HOST, EsContext::port};
    httplib::Request request;
    request.path = slash + EsContext::INDEX_CODE_SEGMENT + slash + "_search";
    request.method = "GET";
    request.headers = {{"Content-Type", CONTENT_TYPE_JSON().c_str()}};
    request.body =
        NlohmannJson{
            {"from", std::to_string((page - 1) * pagesSize)},
            {"size", std::to_string(pagesSize)},
            {"query",
             NlohmannJson{{"multi_match",
                           NlohmannJson{{"query", text},
                                        {"fields", NlohmannJson{CodeSegment::KEY_TITLE,
                                                                CodeSegment::KEY_DESCRIPTION,
                                                                CodeSegment::KEY_CONTENT}}}}}}}
            .dump();
    auto resp = client.send(request);

    if (resp.error() == HttpError::Success) {
        auto respJson = NlohmannJson::parse(resp.value().body);
        if (!respJson.contains("error")) {
            int num = respJson["hits"]["total"]["value"].get<int32_t>();
            if (num > 0)
                res.reserve(num);
            auto hitsJson = respJson["hits"]["hits"];
            for (auto &&v : hitsJson) {
                res.emplace_back(toCodeSegment(v["_source"]));
            }
        }
    }
    return res;
}

bool createIndex() {
    httplib::Client client{EsContext::HOST, EsContext::port};
    auto resp =
        client.Put((slash + EsContext::INDEX_CODE_SEGMENT).c_str(),
                   EsContext::INDEX_MAPPING_CODE_SEGMENT().c_str(), CONTENT_TYPE_JSON().c_str());
    bool res = false;
    if (resp.error() == HttpError::Success) {
        auto respJson = NlohmannJson::parse(resp.value().body);
        // acknowledged will be false if timed-out, so just check error
        if (!respJson.contains("error"))
            res = true;
    }
    return res;
}

} // namespace eshelper

#endif // ES_HELPER_CC
