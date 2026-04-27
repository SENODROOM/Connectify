#pragma once
#include "Comment.h"
#include "LikeList.h"
#include <string>
#include <ctime>

class Post {
public:
    Post(int postID, int ownerID, time_t timestamp);
    virtual ~Post() = default;

    // Pure virtual — subclasses define how they display and what type they are
    virtual void        display()  const = 0;
    virtual std::string getType()  const = 0;

    // Engagement
    bool like(int userID)   { return likes_.add(userID); }
    bool unlike(int userID) { return likes_.remove(userID); }
    bool isLikedBy(int userID) const { return likes_.has(userID); }
    int  getLikes()            const { return likes_.count(); }

    void addComment(int userID, const std::string& username,
                    const std::string& text, time_t ts = time(nullptr));

    // Getters
    int      getPostID()    const { return postID_; }
    int      getOwnerID()   const { return ownerID_; }
    time_t   getTimestamp() const { return timestamp_; }
    const CommentList& getComments() const { return comments_; }

protected:
    int         postID_;
    int         ownerID_;
    time_t      timestamp_;
    LikeList    likes_;
    CommentList comments_;
};
