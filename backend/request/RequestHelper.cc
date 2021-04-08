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
    const static string _PARAM_KEY_CODE_SEGMENT = "codeSegment";
    return _PARAM_KEY_CODE_SEGMENT;
}

const string &RequestHelper::PATH_GET_CODE_SEGMENTS() {
    const static string _PATH_GET_CODE_SEGMENTS = "/getCodeSegments";
    return _PATH_GET_CODE_SEGMENTS;
}

const string &RequestHelper::PATH_ADD_CODE_SEGMENT() {
    const static string _PATH_ADD_CODE_SEGMENT = "/addCodeSegment";
    return _PATH_ADD_CODE_SEGMENT;
}

const string &RequestHelper::PATH_UPDATE_CODE_SEGMENT() {
    const static string _PATH_UPDATE_CODE_SEGMENT = "/updateCodeSegment";
    return _PATH_UPDATE_CODE_SEGMENT;
}
const string &RequestHelper::PATH_SEARCH_CODE_SEGMENT() {
    const static string _PATH_SEARCH_CODE_SEGMENT = "/search";
    return _PATH_SEARCH_CODE_SEGMENT;
}
const string &RequestHelper::PATH_ADD_TAG() {
    const static string _PATH_ADD_TAG = "/addTag";
    return _PATH_ADD_TAG;
}
const string &RequestHelper::PATH_DELETE_TAG() {
    const static string _PATH_DELETE_TAG = "/deleteTag";
    return _PATH_DELETE_TAG;
}
const string &RequestHelper::PATH_GET_TAG_LIST() {
    const static string _PATH_GET_TAG_LIST = "/tagList";
    return _PATH_GET_TAG_LIST;
}
const string &RequestHelper::PATH_FAVOR() {
    const static string _PATH_FAVOR = "/favor";
    return _PATH_FAVOR;
}
const string &RequestHelper::PATH_LOGIN() {
    const static string _PATH_LOGIN = "/login";
    return _PATH_LOGIN;
}
const string &RequestHelper::PATH_REGISTER() {
    const static string _PATH_REGISTER = "/register";
    return _PATH_REGISTER;
}
const string &RequestHelper::PATH_UPDATE_PASSWORD() {
    const static string _PATH_UPDATE_PASSWORD = "/updatePassword";
    return _PATH_UPDATE_PASSWORD;
}
const string &RequestHelper::PATH_GET_USER_FAVORS() {
    const static string _PATH_GET_USER_FAVORS = "/userFavors";
    return _PATH_GET_USER_FAVORS;
}
const string &RequestHelper::PATH_GET_USER_FAVOR_IDS() {
    const static string _PATH_GET_USER_FAVOR_IDS = "userFavorIds";
    return _PATH_GET_USER_FAVOR_IDS;
}

#endif // KEY_HELPER_CC
