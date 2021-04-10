#if !defined(UTILITY_CC)
#define UTILITY_CC

#include <optional>
#include <string>

#include "Utility.hpp"

#include "openssl/aes.h"

using std::string;

const string &CONTENT_TYPE_JSON() {
    const static string _CONTENT_TYPE_JSON = "application/json";
    return _CONTENT_TYPE_JSON;
}
const string &CONTENT_TYPE_PLAIN() {
    const static string _CONTENT_TYPE_PLAIN = "text/plain";
    return _CONTENT_TYPE_PLAIN;
}
const string &CONTENT_HTML() {
    const static string _CONTENT_HTML = "text/html";
    return _CONTENT_HTML;
}

#endif // UTILITY_CC
