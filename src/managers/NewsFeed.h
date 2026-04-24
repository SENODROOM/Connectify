#pragma once
#include "../models/Post.h"
#include "../models/User.h"
#include <vector>

class NewsFeed {
public:
    // Returns posts from users that currentUser follows, sorted newest first
    std::vector<Post*> generateFeed(const User* currentUser,
                                    const std::vector<User*>& allUsers) const;

    // Engagement helpers (delegates to Post + fires FileManager save)
    void like  (int postID, int userID, std::vector<User*>& allUsers);
    void unlike(int postID, int userID, std::vector<User*>& allUsers);
    void comment(int postID, int userID, const std::string& username,
                 const std::string& text, std::vector<User*>& allUsers);

    Post* findPost(int postID, const std::vector<User*>& allUsers) const;

private:
    int nextPostID(const std::vector<User*>& allUsers) const;
};
