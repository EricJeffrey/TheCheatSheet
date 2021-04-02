#if !defined(TAG_HPP)
#define TAG_HPP

#include <string>

using std::string;

struct Tag {
    static constexpr char KEY_ID[] = "_id";
    static constexpr char KEY_VALUE[] = "value";

    string mId;
    string mValue;

    void setId(const string &id) { mId = id; }
    void setValue(const string &value) { mValue = value; }
};

#endif // TAG_HPP
