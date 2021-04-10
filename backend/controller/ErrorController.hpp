#if !defined(ERROR_CONTROLLER)
#define ERROR_CONTROLLER

#include <cstdint>

namespace controller {

struct ErrorController {
    static const int32_t CODE_SUCCESS = 100;
    static const int32_t CODE_AUTHENTIC_FAILED = 200;
    static const int32_t CODE_CONFLICT = 200;

    static constexpr char MSG_SUCCESS[] = "operation success";
    static constexpr char MSG_WRONG_PASSWORD[] = "wrong password";
    static constexpr char MSG_WRONG_USER[] = "no such user";
    static constexpr char MSG_NEED_LOGIN[] = "login needed";
    static constexpr char MSG_TITLE_CONFLICT[] = "title conflict";
    static constexpr char MSG_EMAIL_USED[] = "email already used";
};

} // namespace controller

#endif // ERROR_CONTROLLER
