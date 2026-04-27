#pragma once
#include "Person.h"
#include "PostList.h"
#include "FollowArray.h"

class User : public Person {
public:
    User(int id, const std::string& name,
         const std::string& email,
         const std::string& hashedPassword);
    ~User() override = default;  // PostList destructor handles cleanup

    std::string getRole()    const override { return "USER"; }
    std::string getProfile() const override;

    // ── Social graph ──────────────────────────────────────────
    void follow(int userID)        { following_.add(userID); }
    void unfollow(int userID)      { following_.remove(userID); }
    bool isFollowing(int id) const { return following_.has(id); }
    void addFollower(int userID)   { followers_.add(userID); }
    void removeFollower(int id)    { followers_.remove(id); }

    // ── Posts ──────────────────────────────────────────────────
    void  addPost(Post* p)      { posts_.append(p); }
    bool  removePost(int pid)   { return posts_.remove(pid); }
    Post* findPost(int pid) const { return posts_.find(pid); }

    // ── Engagement ─────────────────────────────────────────────
    void likePost(Post* p)   { if (p) p->like(id_); }
    void unlikePost(Post* p) { if (p) p->unlike(id_); }
    void commentOn(Post* p, const std::string& text) {
        if (p) p->addComment(id_, name_, text);
    }

    // ── Status ─────────────────────────────────────────────────
    bool isBanned()             const { return banned_; }
    void setBanned(bool b)            { banned_ = b; }
    std::string getProfilePic() const { return profilePicPath_; }
    void setProfilePic(const std::string& p) { profilePicPath_ = p; }

    // ── Accessors ──────────────────────────────────────────────
    const FollowArray& getFollowing() const { return following_; }
    const FollowArray& getFollowers() const { return followers_; }
    const PostList&    getPosts()     const { return posts_; }

private:
    FollowArray following_;
    FollowArray followers_;
    PostList    posts_;
    bool        banned_         = false;
    std::string profilePicPath_;
};
