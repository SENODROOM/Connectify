#include "NewsFeed.h"
#include "FileManager.h"
#include <algorithm>

std::vector<Post*> NewsFeed::generateFeed(const User* currentUser,
                                          const std::vector<User*>& allUsers) const {
    std::vector<Post*> feed;
    for (int followedID : currentUser->getFollowing()) {
        for (User* u : allUsers) {
            if (u->getID() == followedID) {
                for (Post* p : u->getPosts())
                    feed.push_back(p);
                break;
            }
        }
    }
    // Sort newest first
    std::sort(feed.begin(), feed.end(), [](Post* a, Post* b){
        return a->getTimestamp() > b->getTimestamp();
    });
    return feed;
}

Post* NewsFeed::findPost(int postID, const std::vector<User*>& allUsers) const {
    for (User* u : allUsers)
        for (Post* p : u->getPosts())
            if (p->getPostID() == postID) return p;
    return nullptr;
}

void NewsFeed::like(int postID, int userID, std::vector<User*>& allUsers) {
    Post* p = findPost(postID, allUsers);
    if (p) {
        p->like(userID);
        FileManager::instance().saveAllPosts(allUsers);
    }
}

void NewsFeed::unlike(int postID, int userID, std::vector<User*>& allUsers) {
    Post* p = findPost(postID, allUsers);
    if (p) {
        p->unlike(userID);
        FileManager::instance().saveAllPosts(allUsers);
    }
}

void NewsFeed::comment(int postID, int userID, const std::string& username,
                       const std::string& text, std::vector<User*>& allUsers) {
    Post* p = findPost(postID, allUsers);
    if (p) {
        p->addComment(Comment{userID, username, text, time(nullptr)});
        FileManager::instance().saveAllPosts(allUsers);
    }
}

int NewsFeed::nextPostID(const std::vector<User*>& allUsers) const {
    int maxID = 2000;
    for (User* u : allUsers)
        for (Post* p : u->getPosts())
            if (p->getPostID() > maxID) maxID = p->getPostID();
    return maxID + 1;
}
