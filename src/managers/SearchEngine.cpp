#include "SearchEngine.h"
#include "../models/TextPost.h"
#include "../models/ImagePost.h"
#include <cctype>

std::string SearchEngine::toLower(const std::string& s) {
    std::string out = s;
    for (char& c : out) c = static_cast<char>(std::tolower(c));
    return out;
}

bool SearchEngine::contains(const std::string& haystack,
                             const std::string& needle) {
    if (needle.empty()) return false;
    // Manual substring search — no STL algorithms
    for (int i = 0; i <= (int)haystack.size() - (int)needle.size(); ++i) {
        bool match = true;
        for (int j = 0; j < (int)needle.size(); ++j) {
            if (haystack[i + j] != needle[j]) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

User** SearchEngine::searchUsers(const std::string& keyword,
                                  const UserTable& allUsers,
                                  int& count) {
    count = 0;
    std::string kw = toLower(keyword);
    if (kw.empty()) return nullptr;

    // First pass: count matches
    for (int i = 0; i < allUsers.size(); ++i) {
        User* u = allUsers[i];
        if (contains(toLower(u->getName()),  kw) ||
            contains(toLower(u->getEmail()), kw))
            ++count;
    }
    if (count == 0) return nullptr;

    User** arr = new User*[count];
    int    idx = 0;
    for (int i = 0; i < allUsers.size(); ++i) {
        User* u = allUsers[i];
        if (contains(toLower(u->getName()),  kw) ||
            contains(toLower(u->getEmail()), kw))
            arr[idx++] = u;
    }
    return arr;
}

Post** SearchEngine::searchPosts(const std::string& keyword,
                                  const UserTable& allUsers,
                                  int& count) {
    count = 0;
    std::string kw = toLower(keyword);
    if (kw.empty()) return nullptr;

    // Count pass
    for (int i = 0; i < allUsers.size(); ++i) {
        PostNode* node = allUsers[i]->getPosts().head();
        while (node) {
            std::string content;
            if (auto* tp = dynamic_cast<TextPost*>(node->post))
                content = toLower(tp->getContent());
            else if (auto* ip = dynamic_cast<ImagePost*>(node->post))
                content = toLower(ip->getCaption());
            if (contains(content, kw)) ++count;
            node = node->next;
        }
    }
    if (count == 0) return nullptr;

    Post** arr = new Post*[count];
    int    idx = 0;
    for (int i = 0; i < allUsers.size(); ++i) {
        PostNode* node = allUsers[i]->getPosts().head();
        while (node) {
            std::string content;
            if (auto* tp = dynamic_cast<TextPost*>(node->post))
                content = toLower(tp->getContent());
            else if (auto* ip = dynamic_cast<ImagePost*>(node->post))
                content = toLower(ip->getCaption());
            if (contains(content, kw)) arr[idx++] = node->post;
            node = node->next;
        }
    }
    return arr;
}
