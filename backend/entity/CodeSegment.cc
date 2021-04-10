#if !defined(CODE_SEGMENT_CC)
#define CODE_SEGMENT_CC

#include "CodeSegment.hpp"
#include "EntityHelper.hpp"

#include "nlohmann/json.hpp"

using NlohmannJson = nlohmann::json;

CodeSegment::CodeSegment(const string &val) { CodeSegment(NlohmannJson::parse(val)); }
CodeSegment::CodeSegment(const NlohmannJson &valJson) { swap(toCodeSegment(valJson)); }

#endif // CODE_SEGMENT_CC
