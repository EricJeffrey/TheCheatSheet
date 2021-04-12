#if !defined(USER_CONTROLLER_HPP)
#define USER_CONTROLLER_HPP

#include <optional>
#include <string>

#include "../util/RequestHelper.hpp"
#include "frame/HandlerResult.hpp"

using Headers = RequestHelper::Headers;

using std::string;

namespace controller {

using std::optional;

HandlerResult userLogin(const optional<string> &email, const optional<string> &password);

HandlerResult userRegister(const optional<string> &email, const optional<string> &name,
                           const optional<string> &password);

HandlerResult updatePassword();

HandlerResult getUserFavoredSegmentIds(const Headers &headers);

HandlerResult getUserFavoredSegments(const optional<int32_t> &page,
                                     const optional<int32_t> pageSize, const Headers &headers);

HandlerResult favorCodeSegment(const optional<string> &segmentId, const Headers &headers);

HandlerResult userProfile(const Headers &headers);

} // namespace controller

#endif // USER_CONTROLLER_HPP
