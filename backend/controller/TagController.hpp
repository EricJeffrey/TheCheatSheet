#if !defined(TAG_CONTROLLER_HPP)
#define TAG_CONTROLLER_HPP

#include <string>

#include "../util/RequestHelper.hpp"
#include "frame/HandlerResult.hpp"

using std::string;

using Headers = RequestHelper::Headers;

namespace controller {

HandlerResult getTagList();

HandlerResult addTag(const std::optional<string> &tagValue, const Headers& headers);

} // namespace controller

#endif // TAG_CONTROLLER_HPP
