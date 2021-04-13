#if !defined(ES_CONTEXT_HPP)
#define ES_CONTEXT_HPP

#include <string>

using std::string;
namespace eshelper {
struct EsContext {
    static string HOST;
    static int32_t port;
    static const string INDEX_CODE_SEGMENT;

    static const string &INDEX_MAPPING_CODE_SEGMENT();

    static void config(const string &host, const int32_t port);

    EsContext() {}
    ~EsContext() {}
};

} // namespace eshelper

#endif // ES_CONTEXT_HPP
