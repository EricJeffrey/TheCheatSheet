#if !defined(ES_CONTEXT_HPP)
#define ES_CONTEXT_HPP

#include <string>

using std::string;

struct EsContext {
    static const char HOST[];
    static const int port;
    static const char INDEX_CODE_SEGMENT[];
    static const char CONTENT_TYPE_JSON[];

    static const string& INDEX_MAPPING_CODE_SEGMENT();

    EsContext() {}
    ~EsContext() {}
};

#endif // ES_CONTEXT_HPP
