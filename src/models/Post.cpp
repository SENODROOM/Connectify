#include "Post.h"

Post::Post(int postID, int ownerID, time_t timestamp)
    : postID_(postID), ownerID_(ownerID), timestamp_(timestamp)
{}

void Post::addComment(int userID, const std::string& username,
                      const std::string& text, time_t ts) {
    comments_.add(userID, username, text, ts);
}
