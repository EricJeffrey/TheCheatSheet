#if !defined(CONTROLLER_MAPPING_HPP)
#define CONTROLLER_MAPPING_HPP

#include <memory>
#include <string>
#include <vector>

#include "../controller/BasicController.hpp"

using std::string;
using std::vector;

// specify what to parse during handling a request.
// note that only one of _PARAM and _BODY should be  set
enum ParseOption { _PARAM = 1, _BODY = 0b10, _HEADER = 0b100 };

enum RequestMethod { M_GET, M_POST, M_PUT, M_DELETE };

struct ControllerItem {
    RequestMethod mMethod;
    string mPath;
    int32_t mParseOptionMask;
    std::shared_ptr<controller::BasicController> mController;

    // create the controller and set its parameters' default values
    ControllerItem(RequestMethod method, const string &path, int32_t parseOptionMask,
                   std::shared_ptr<controller::BasicController> controller,
                   const vector<std::pair<int32_t, string>> &defaultValues = {})
        : mMethod(method), mPath(path), mParseOptionMask(parseOptionMask), mController(controller) {
        // set default values
        for (auto &&tmpPair : defaultValues) {
            mController->setParamValue(tmpPair.first, tmpPair.second);
        }
    }
    ~ControllerItem() = default;
};

#endif // CONTROLLER_MAPPING_HPP
