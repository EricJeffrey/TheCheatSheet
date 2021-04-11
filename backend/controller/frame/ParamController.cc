#if !defined(PARAM_ONLY_CONTROLLER_CC)
#define PARAM_ONLY_CONTROLLER_CC

#include "ParamController.hpp"

namespace controller {

// template <typename... ParamTypes>
// std::shared_ptr<BasicController>
// makeParamController(const string &path, const vector<string> &paramNames,
//                     const vector<string> &headerNames,
//                     typename ParamController<ParamTypes...>::HandlerType handler) {

//     auto *pointer = new ParamController<ParamTypes...>(
//         path, paramNames, headerNames, std::make_tuple(std::optional<ParamTypes>{}...), handler);
//     return std::shared_ptr<BasicController>(dynamic_cast<BasicController *>(pointer));
// }
} // namespace controller

#endif // PARAM_ONLY_CONTROLLER_CC
