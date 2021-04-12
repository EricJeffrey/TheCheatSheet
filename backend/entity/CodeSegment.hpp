#if !defined(CODE_SEGMENT_HPP)
#define CODE_SEGMENT_HPP

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

class CodeSegment {
public:
    static constexpr char KEY_ID[] = "_id";
    static constexpr char KEY_MONGO_ID[] = "mongoId";
    static constexpr char KEY_ES_ID[] = "esId";
    static constexpr char KEY_TITLE[] = "title";
    static constexpr char KEY_DESCRIPTION[] = "description";
    static constexpr char KEY_CONTENT[] = "content";
    static constexpr char KEY_CREATED_AT[] = "createdAt";
    static constexpr char KEY_LAST_MODIFIED[] = "lastModified";
    static constexpr char KEY_FAVOR_NUMBER[] = "favorNumber";
    static constexpr char KEY_TAG_LIST[] = "tagList";

    // mid is actually _id in mongo
    string mId;
    // id for elasticsearch
    string mEsId;
    string mTitle;
    string mDescription;
    string mContent;
    int64_t mCreatedAt;
    int64_t mLastModified;
    int32_t mFavorNumber;
    vector<string> mTagList;

    CodeSegment() {}
    CodeSegment(const string &title, const string &description, const string &content,
                int64_t createdAt, int64_t lastModified, int32_t favorNumber,
                const vector<string> &tagList)
        : mTitle(title), mDescription(description), mContent(content), mCreatedAt(createdAt),
          mLastModified(lastModified), mFavorNumber(favorNumber), mTagList(tagList) {}

    ~CodeSegment() {}

    CodeSegment(const CodeSegment &segment) = default;

    CodeSegment &operator=(const CodeSegment &segment) = default;

    void swap(CodeSegment &&segment);

    CodeSegment(CodeSegment &&segment) { swap(std::forward<CodeSegment>(segment)); }

    CodeSegment &operator=(CodeSegment &&segment) {
        swap(std::forward<CodeSegment>(segment));
        return *this;
    }

    // construct from a json object, existence of id is not checked when converting
    CodeSegment(const nlohmann::json &valJson, bool ignoreMId = false, bool ignoreEsId = false);

    // convert to nlohmann::json. use toJson if id fields are not needed
    operator nlohmann::json() const { return toJson(false, false); }

    // mTitle and mDescription and mContent and mTagList equal
    bool operator==(const CodeSegment &segment) const {
        return mTitle == segment.mTitle && mDescription == segment.mDescription &&
               mContent == segment.mContent && mTagList == segment.mTagList;
    }

    string toString() const { return toJson(false, false).dump(); }

    nlohmann::json toJson(bool ignoreMId, bool ignoreEsId) const;

    void setId(const string &id) { mId = id; }
    void setEsId(const string &esId) { mEsId = esId; }
    void setTitle(const string &title) { mTitle = title; }
    void setDescription(const string &description) { mDescription = description; }
    void setContent(const string &content) { mContent = content; }
    void setCreatedAt(const int64_t createdAt) { mCreatedAt = createdAt; }
    void setLastModified(const int64_t lastModified) { mLastModified = lastModified; }
    void setFavorNumber(const int32_t favorNumber) { mFavorNumber = favorNumber; }
    void setTagList(const vector<string> &tagList) { mTagList = tagList; }
};

#endif // CODE_SEGMENT_HPP