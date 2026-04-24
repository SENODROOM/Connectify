#pragma once
#include <string>
#include <ctime>
#include <vector>

struct Comment {
    int         userID;
    std::string username;
    std::string text;
    time_t      timestamp;
};

class Post {
public:
    Post(int postID, int ownerID, time_t timestamp);
    virtual ~Post() = default;

    // Pure virtual — each post type renders differently
    virtual void        display()   const = 0;
    virtual std::string getType()   const = 0;

    // Engagement
    void like(int userID);
    void unlike(int userID);
    bool isLikedBy(int userID) const;
    void addComment(const Comment& c);

    // Getters
    int                        getPostID()    const { return postID_; }
    int                        getOwnerID()   const { return ownerID_; }
    time_t                     getTimestamp() const { return timestamp_; }
    int                        getLikes()     const { return static_cast<int>(likedBy_.size()); }
    const std::vector<Comment>& getComments() const { return comments_; }

protected:
    int                  postID_;
    int                  ownerID_;
    time_t               timestamp_;
    std::vector<int>     likedBy_;   // userIDs who liked
    std::vector<Comment> comments_;
};
