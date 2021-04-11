#if !defined(COOKIE_HELPER_CC)
#define COOKIE_HELPER_CC

#include "CookieHelper.hpp"

string CookieHelper::wrapCookie(const string &key, const string &value, int32_t maxAge) {
    return key + "=" + value + ";" + std::to_string(maxAge);
}

vector<CookieHelper::CookiePair> CookieHelper::parseCookie(const string &cookie) {
    string s = cookie;
    size_t pos = 0;
    string delimiter = ";";
    vector<string> resv;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        resv.emplace_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    resv.push_back(s);
    vector<CookiePair> res;
    for (auto &&v : resv) {
        pos = cookie.find('=');
        if (pos != string::npos && pos != 0 && pos != v.size() - 1) {
            res.push_back({v.substr(0, pos), v.substr(pos + 1)});
        }
    }
    return res;
}

string CookieHelper::getCookieValue(const string &key, const string &cookie) {
    string res;
    auto cookieKVs = parseCookie(cookie);
    for (int32_t i = cookieKVs.size() - 1; i >= 0; i--) {
        if (cookieKVs[i].first == key) {
            res = cookieKVs[i].second;
            break;
        }
    }
    return res;
}

#endif // COOKIE_HELPER_CC
