#pragma once
#include "../models/User.h"
#include "../models/Post.h"
#include <vector>
#include <string>

class SearchEngine {
public:
    static std::vector<User*> searchUsers(const std::string& keyword,
                                          const std::vector<User*>& allUsers);

    static std::vector<Post*> searchPosts(const std::string& keyword,
                                          const std::vector<User*>& allUsers);

private:
    static std::string toLower(const std::string& s);
};
