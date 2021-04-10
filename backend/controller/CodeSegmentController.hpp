#if !defined(CODE_SEGMENT_CONTROLLER_HPP)
#define CODE_SEGMENT_CONTROLLER_HPP

#include <optional>
#include <string>
#include <tuple>

#include "../entity/CodeSegment.hpp"
#include "../util/RequestHelper.hpp"
#include "BasicController.hpp"

using std::string;

namespace controller {
using std::optional;

HandlerResult getCodeSegments(const optional<int32_t> &page, const optional<int32_t> &pageSize,
                       const optional<string> &sortOrder, const optional<string> &tag);

HandlerResult addCodeSegment(const std::optional<CodeSegment> &, const RequestHelper::Headers &headers);

} // namespace controller

#endif // CODE_SEGMENT_CONTROLLER_HPP
