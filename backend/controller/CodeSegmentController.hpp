#if !defined(CODE_SEGMENT_CONTROLLER_HPP)
#define CODE_SEGMENT_CONTROLLER_HPP

#include <optional>
#include <string>
#include <tuple>

using std::string;

namespace controller {

string getCodeSegments(std::optional<int32_t> page, std::optional<int32_t> pageSize,
                       std::optional<string> sortOrder, std::optional<string> tag);
} // namespace controller

#endif // CODE_SEGMENT_CONTROLLER_HPP
