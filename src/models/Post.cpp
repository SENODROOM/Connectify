#include "Post.h"
#include <algorithm>

Post::Post(int postID, int ownerID, time_t timestamp)
    : postID_(postID), ownerID_(ownerID), timestamp_(timestamp)
{}

void Post::like(int userID) {
    if (!isLikedBy(userID))
        likedBy_.push_back(userID);
}

void Post::unlike(int userID) {
    likedBy_.erase(std::remove(likedBy_.begin(), likedBy_.end(), userID), likedBy_.end());
}

bool Post::isLikedBy(int userID) const {
    return std::find(likedBy_.begin(), likedBy_.end(), userID) != likedBy_.end();
}

void Post::addComment(const Comment& c) {
    comments_.push_back(c);
}
