#if !defined(KEY_HELPER_HPP)
#define KEY_HELPER_HPP
#include <string>

using std::string;

struct RequestHelper {

    static const string &PARAM_KEY_SORTBY();
    static const string &PARAM_KEY_TAG();
    static const string &PARAM_KEY_PAGE();
    static const string &PARAM_KEY_PAGE_SIZE();
    static const string &PARAM_KEY_CODE_SEGMENT();

    static const string &PATH_GET_CODE_SEGMENTS();
};
#endif // KEY_HELPER_HPP
