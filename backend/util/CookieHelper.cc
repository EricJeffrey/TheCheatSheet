#if !defined(COOKIE_HELPER_CC)
#define COOKIE_HELPER_CC

#include "CookieHelper.hpp"

string CookieHelper::wrapCookie(const string &key, const string &value, int32_t maxAge) {
    return key + "=" + value + ";" + std::to_string(maxAge);
}

string CookieHelper::getCookieValue(const string &key, const string &cookie) {
    string res;
    size_t pos = cookie.find('=');
    if (pos != 0 && pos != string::npos) {
        auto tmpKey = cookie.substr(0, pos);
        if (tmpKey == key) {
            res = cookie.substr(pos + 1);
        }
    }
    return res;
}

#endif // COOKIE_HELPER_CC
