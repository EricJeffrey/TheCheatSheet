#if !defined(USER_HPP)
#define USER_HPP

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using std::string;
using std::vector;

class User {

public:
    static constexpr char KEY_ID[] = "_id";
    static constexpr char KEY_NAME[] = "name";
    static constexpr char KEY_EMAIL[] = "email";
    static constexpr char KEY_PASSWORD[] = "password";
    static constexpr char KEY_FAVORS[] = "favors";

    string mId;
    string mName;
    string mEmail;
    string mPassword;
    vector<string> mFavorIds;

    User() = default;
    ~User() = default;
    User(const string &name, const string &email, const string &password)
        : mName(name), mEmail(email), mPassword(password) {}
    bool operator==(const User &t) const {
        return mName == t.mName && mEmail == t.mEmail && mPassword == t.mPassword &&
               mFavorIds == t.mFavorIds;
    }

    nlohmann::json toJson() {
        return nlohmann::json{
            {KEY_ID, mId},           {KEY_NAME, mName},
            {KEY_EMAIL, mEmail},     {KEY_PASSWORD, mPassword},
            {KEY_FAVORS, mFavorIds},
        };
    }

    void setId(const string &id) { mId = id; }
    void setName(const string &name) { mName = name; }
    void setEmail(const string &email) { mEmail = email; }
    void setPassword(const string &password) { mPassword = password; }
    void setFavors(const vector<string> &favors) { mFavorIds = favors; }
};

#endif // USER_HPP
