#if !defined(TAG_HPP)
#define TAG_HPP

#include <string>

#include "nlohmann/json.hpp"

using std::string;

struct Tag {
    static constexpr char KEY_ID[] = "_id";
    static constexpr char KEY_VALUE[] = "value";

    string mId;
    string mValue;

    Tag() = default;
    ~Tag() = default;

    Tag(const string &value) : mValue(value) {}

    bool operator==(const Tag &t) const { return mValue == t.mValue; }

    // convert to json
    operator nlohmann::json() const { return toJson(); }

    void setId(const string &id) { mId = id; }
    void setValue(const string &value) { mValue = value; }

    nlohmann::json toJson() const {
        return nlohmann::json{
            {KEY_ID, mId},
            {KEY_VALUE, mValue},
        };
    }
};

#endif // TAG_HPP
