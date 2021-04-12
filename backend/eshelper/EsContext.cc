#if !defined(ES_CONTEXT_CC)
#define ES_CONTEXT_CC

#include "EsContext.hpp"
#include "../entity/CodeSegment.hpp"

const char EsContext::HOST[] = "172.17.0.5";
const int EsContext::port = 9200;
const char EsContext::INDEX_CODE_SEGMENT[] = "codesegment";

#endif // ES_CONTEXT_CC

const string &EsContext::INDEX_MAPPING_CODE_SEGMENT() {
    static const string mapping = "{"
                                  "\"mappings\":{"
                                  "\"properties\":{"
                                  "\"" +
                                  string{CodeSegment::KEY_TITLE} +
                                  "\""
                                  ":{"
                                  "\"type\":\"text\","
                                  "\"analyzer\":\"ik_max_word\","
                                  "\"search_analyzer\":\"ik_smart\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_DESCRIPTION} +
                                  "\""
                                  ":{"
                                  "\"type\":\"text\","
                                  "\"analyzer\":\"ik_max_word\","
                                  "\"search_analyzer\":\"ik_smart\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_CONTENT} +
                                  "\""
                                  ":{"
                                  "\"type\":\"text\","
                                  "\"analyzer\":\"ik_max_word\","
                                  "\"search_analyzer\":\"ik_smart\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_CREATED_AT} +
                                  "\""
                                  ":{"
                                  "\"type\":\"long\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_LAST_MODIFIED} +
                                  "\""
                                  ":{"
                                  "\"type\":\"long\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_FAVOR_NUMBER} +
                                  "\""
                                  ":{"
                                  "\"type\":\"long\""
                                  "},"
                                  "\"" +
                                  string{CodeSegment::KEY_TAG_LIST} +
                                  "\""
                                  ":{"
                                  "\"type\":\"text\""
                                  "}"
                                  "}"
                                  "}"
                                  "}";
    return mapping;
}
