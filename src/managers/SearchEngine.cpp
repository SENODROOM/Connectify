#include "SearchEngine.h"
#include "../models/TextPost.h"
#include "../models/ImagePost.h"
#include <algorithm>
#include <cctype>

std::string SearchEngine::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

std::vector<User*> SearchEngine::searchUsers(const std::string& keyword,
                                              const std::vector<User*>& allUsers) {
    std::vector<User*> results;
    if (keyword.empty()) return results;
    std::string kw = toLower(keyword);
    for (User* u : allUsers) {
        if (toLower(u->getName()).find(kw) != std::string::npos ||
            toLower(u->getEmail()).find(kw) != std::string::npos)
            results.push_back(u);
    }
    return results;
}

std::vector<Post*> SearchEngine::searchPosts(const std::string& keyword,
                                              const std::vector<User*>& allUsers) {
    std::vector<Post*> results;
    if (keyword.empty()) return results;
    std::string kw = toLower(keyword);
    for (User* u : allUsers) {
        for (Post* p : u->getPosts()) {
            std::string content;
            if (auto* tp = dynamic_cast<TextPost*>(p))
                content = toLower(tp->getContent());
            else if (auto* ip = dynamic_cast<ImagePost*>(p))
                content = toLower(ip->getCaption());
            if (content.find(kw) != std::string::npos)
                results.push_back(p);
        }
    }
    return results;
}
