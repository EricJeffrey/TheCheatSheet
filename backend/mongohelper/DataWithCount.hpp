#if !defined(MONGO_RESULT_TYPE)
#define MONGO_RESULT_TYPE

#include <cstdint>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace mongohelper {

template <typename T> struct DataWithTotalCount {
    int32_t mCount;
    vector<T> mData;

    DataWithTotalCount() : mCount(0), mData() {}

    DataWithTotalCount(int32_t count, const vector<T> data) : mCount(count), mData(data) {}
    DataWithTotalCount(int32_t count, vector<T> &&data) : mCount(count), mData(data) {}

    DataWithTotalCount(const DataWithTotalCount &) = default;

    DataWithTotalCount &operator=(DataWithTotalCount &&t) {
        mCount = t.mCount;
        mData = std::move(t.mData);
        return *this;
    }
};

} // namespace mongohelper

#endif // MONGO_RESULT_TYPE
