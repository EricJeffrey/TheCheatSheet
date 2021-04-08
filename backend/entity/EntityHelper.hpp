#if !defined(ENTITY_HELPER_HPP)
#define ENTITY_HELPER_HPP

#include "nlohmann/json.hpp"

#include "CodeSegment.hpp"

// segment.id and segment.esId is ignored
nlohmann::json toJson(const CodeSegment &segment);
// segment.id and segment.esId is ignored
CodeSegment toCodeSegment(const nlohmann::json &segmentJson);

#endif // ENTITY_HELPER_HPP
