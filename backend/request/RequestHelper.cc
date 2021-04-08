#if !defined(KEY_HELPER_CC)
#define KEY_HELPER_CC

#include "RequestHelper.hpp"

const string &RequestHelper::PARAM_KEY_SORTBY() {
    const static string _PARAM_KEY_SORTBY = "sortBy";
    return _PARAM_KEY_SORTBY;
}
const string &RequestHelper::PARAM_KEY_TAG() {
    const static string _PARAM_KEY_TAG = "tag";
    return _PARAM_KEY_TAG;
}
const string &RequestHelper::PARAM_KEY_PAGE() {
    const static string _PARAM_KEY_PAGE = "page";
    return _PARAM_KEY_PAGE;
}
const string &RequestHelper::PARAM_KEY_PAGE_SIZE() {
    const static string _PARAM_KEY_PAGE_SIZE = "pageSize";
    return _PARAM_KEY_PAGE_SIZE;
}
const string &RequestHelper::PARAM_KEY_CODE_SEGMENT() {
    const static string _PARAM_KEY_CODE_SEGMENT = "codesegment";
    return _PARAM_KEY_CODE_SEGMENT;
}

const string &RequestHelper::PATH_GET_CODE_SEGMENTS() {
    const static string _PATH_GET_CODE_SEGMENTS = "/getcodesegments";
    return _PATH_GET_CODE_SEGMENTS;
}

#endif // KEY_HELPER_CC
