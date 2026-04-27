#pragma once
#include "../models/UserTable.h"
#include "../models/Post.h"
#include <string>

// Results are raw pointer arrays — caller must delete[] (NOT the items)
class SearchEngine {
public:
    // Returns heap array of matching User* — caller must delete[]
    static User** searchUsers(const std::string& keyword,
                              const UserTable& allUsers,
                              int& count);

    // Returns heap array of matching Post* — caller must delete[]
    static Post** searchPosts(const std::string& keyword,
                              const UserTable& allUsers,
                              int& count);

private:
    static std::string toLower(const std::string& s);
    static bool        contains(const std::string& haystack,
                                const std::string& needle);
};
