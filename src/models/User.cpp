#include "User.h"
#include <algorithm>
#include <sstream>

User::User(int id, const std::string& name, const std::string& email,
           const std::string& hashedPassword)
    : Person(id, name, email, hashedPassword)
{}

User::~User() {
    for (Post* p : posts_) delete p;
}

std::string User::getProfile() const {
    std::ostringstream oss;
    oss << Person::getProfile()
        << " | Following: " << following_.size()
        << " | Followers: " << followers_.size()
        << " | Posts: "     << posts_.size();
    if (banned_) oss << " [BANNED]";
    return oss.str();
}

// --- Social graph ---

void User::follow(int userID) {
    if (!isFollowing(userID))
        following_.push_back(userID);
}

void User::unfollow(int userID) {
    following_.erase(std::remove(following_.begin(), following_.end(), userID), following_.end());
}

bool User::isFollowing(int userID) const {
    return std::find(following_.begin(), following_.end(), userID) != following_.end();
}

void User::addFollower(int userID) {
    if (std::find(followers_.begin(), followers_.end(), userID) == followers_.end())
        followers_.push_back(userID);
}

void User::removeFollower(int userID) {
    followers_.erase(std::remove(followers_.begin(), followers_.end(), userID), followers_.end());
}

// --- Posts ---

void User::addPost(Post* post) {
    posts_.push_back(post);
}

void User::removePost(int postID) {
    auto it = std::find_if(posts_.begin(), posts_.end(),
                           [postID](Post* p){ return p->getPostID() == postID; });
    if (it != posts_.end()) {
        delete *it;
        posts_.erase(it);
    }
}

Post* User::getPost(int postID) const {
    for (Post* p : posts_)
        if (p->getPostID() == postID) return p;
    return nullptr;
}

// --- Engagement ---

void User::likePost(Post* post) {
    if (post) post->like(id_);
}

void User::unlikePost(Post* post) {
    if (post) post->unlike(id_);
}

void User::commentOnPost(Post* post, const std::string& text) {
    if (post) post->addComment(Comment{id_, name_, text, time(nullptr)});
}
