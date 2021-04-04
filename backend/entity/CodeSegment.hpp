#if !defined(CODE_SEGMENT_HPP)
#define CODE_SEGMENT_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

class CodeSegment {
public:
    static constexpr char KEY_ID[] = "_id";
    static constexpr char KEY_TITLE[] = "title";
    static constexpr char KEY_DESCRIPTION[] = "description";
    static constexpr char KEY_CONTENT[] = "content";
    static constexpr char KEY_CREATED_AT[] = "createdAt";
    static constexpr char KEY_LAST_MODIFIED[] = "lastModified";
    static constexpr char KEY_FAVOR_NUMBER[] = "favorNumber";
    static constexpr char KEY_TAG_LIST[] = "tagList";

    // mid is actually _id in mongo
    string mId;
    string mTitle;
    string mDescription;
    string mContent;
    int64_t mCreatedAt;
    int64_t mLastModified;
    int32_t mFavorNumber;
    vector<string> mTagList;

    CodeSegment() {}
    CodeSegment(const string &id, const string &title, const string &description,
                const string &content, int64_t createdAt, int64_t lastModified, int32_t favorNumber,
                const vector<string> &tagList)
        : mId(id), mTitle(title), mDescription(description), mContent(content),
          mCreatedAt(createdAt), mLastModified(lastModified), mFavorNumber(favorNumber),
          mTagList(tagList) {}
    ~CodeSegment() {}

    // mTitle and mDescription and mContent and mTagList equal
    bool operator==(const CodeSegment &segment) const {
        return mTitle == segment.mTitle && mDescription == segment.mDescription &&
               mContent == segment.mContent && mTagList == segment.mTagList;
    }

    void setId(const string &id) { mId = id; }
    void setTitle(const string &title) { mTitle = title; }
    void setDescription(const string &description) { mDescription = description; }
    void setContent(const string &content) { mContent = content; }
    void setCreatedAt(const int64_t createdAt) { mCreatedAt = createdAt; }
    void setLastModified(const int64_t lastModified) { mLastModified = lastModified; }
    void setFavorNumber(const int32_t favorNumber) { mFavorNumber = favorNumber; }
    void setTagList(const vector<string> &tagList) { mTagList = tagList; }
};
#endif // CODE_SEGMENT_HPP