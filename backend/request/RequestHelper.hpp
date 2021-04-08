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
    static const string &PATH_ADD_CODE_SEGMENT();
    static const string &PATH_UPDATE_CODE_SEGMENT();

    static const string &PATH_SEARCH_CODE_SEGMENT();

    static const string &PATH_ADD_TAG();
    static const string &PATH_DELETE_TAG();
    static const string &PATH_GET_TAG_LIST();

    static const string &PATH_FAVOR();

    static const string &PATH_LOGIN();
    static const string &PATH_REGISTER();
    static const string &PATH_UPDATE_PASSWORD();
    static const string &PATH_GET_USER_FAVORS();
    static const string &PATH_GET_USER_FAVOR_IDS();
};
#endif // KEY_HELPER_HPP
