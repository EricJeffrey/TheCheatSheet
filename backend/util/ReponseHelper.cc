#if !defined(RESPONSE_HELPER_CC)
#define RESPONSE_HELPER_CC

#include "ResponseHelper.hpp"

const string &ResponseHelper::HEADER_KEY_SET_COOKIE() {
    const static string _SET_COOKIE = "Set-Cookie";
    return _SET_COOKIE;
}

#endif // RESPONSE_HELPER_CC
