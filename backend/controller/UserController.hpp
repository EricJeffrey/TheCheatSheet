#if !defined(USER_CONTROLLER_HPP)
#define USER_CONTROLLER_HPP

#include <optional>
#include <string>

#include "BasicController.hpp"

using std::string;

namespace controller {

using std::optional;

HandlerResult userLogin(const optional<string> &email, const optional<string> &password);

HandlerResult userRegister(const optional<string> &email, const optional<string> &name,
                           const optional<string> &password);

} // namespace controller

#endif // USER_CONTROLLER_HPP
