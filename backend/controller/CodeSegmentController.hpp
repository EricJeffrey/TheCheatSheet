#if !defined(CODE_SEGMENT_CONTROLLER_HPP)
#define CODE_SEGMENT_CONTROLLER_HPP

#include <optional>
#include <string>
#include <tuple>

#include "../entity/CodeSegment.hpp"
#include "../util/RequestHelper.hpp"
#include "frame/HandlerResult.hpp"
#include "frame/HttpException.hpp"

using Headers = RequestHelper::Headers;

using std::string;

namespace controller {
using std::optional;

HandlerResult getCodeSegments(const optional<int32_t> &page, const optional<int32_t> &pageSize,
                              const optional<string> &sortOrder, const optional<string> &tag);

HandlerResult search(const optional<string> &text, const optional<int32_t> &page,
                     const optional<int32_t> &pageSize);

HandlerResult addCodeSegment(const optional<CodeSegment> &, const Headers &headers);

HandlerResult updateCodeSegment(const optional<CodeSegment> &t, const Headers &headers);

} // namespace controller

#endif // CODE_SEGMENT_CONTROLLER_HPP
