#if !defined(COOKIE_HELPER)
#define COOKIE_HELPER

#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

struct CookieHelper {
    using CookiePair = std::pair<string, string>;
    static constexpr char KEY_SET_COOKIE[] = "Set-Cookie";
    static constexpr char KEY_COOKIE_UID[] = "uid";
    static constexpr char KEY_COOKIE[] = "Cookie";

    static constexpr int32_t DEFAULT_MAX_AGE = 30 * 24 * 60 * 60;
    static string wrapCookie(const string &key, const string &value,
                             int32_t maxAge = DEFAULT_MAX_AGE);

    static string getCookieValue(const string& key, const string& cookie);
};

#endif // COOKIE_HELPER
