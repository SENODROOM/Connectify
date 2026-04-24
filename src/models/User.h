#pragma once
#include "Person.h"
#include "Post.h"
#include <vector>
#include <string>

class User : public Person {
public:
    User(int id, const std::string& name, const std::string& email,
         const std::string& hashedPassword);
    ~User() override;

    std::string getRole() const override { return "USER"; }
    std::string getProfile() const override;

    // Social graph
    void follow(int userID);
    void unfollow(int userID);
    bool isFollowing(int userID) const;
    void addFollower(int userID);
    void removeFollower(int userID);

    // Posts
    void   addPost(Post* post);
    void   removePost(int postID);
    Post*  getPost(int postID) const;

    // Engagement
    void likePost(Post* post);
    void unlikePost(Post* post);
    void commentOnPost(Post* post, const std::string& text);

    // Status
    bool isBanned() const     { return banned_; }
    void setBanned(bool b)    { banned_ = b; }

    std::string getProfilePic() const              { return profilePicPath_; }
    void        setProfilePic(const std::string& p){ profilePicPath_ = p; }

    // Accessors
    const std::vector<int>&    getFollowing()  const { return following_; }
    const std::vector<int>&    getFollowers()  const { return followers_; }
    const std::vector<Post*>&  getPosts()      const { return posts_; }

private:
    std::vector<int>   following_;
    std::vector<int>   followers_;
    std::vector<Post*> posts_;        // owned by this user
    bool               banned_ = false;
    std::string        profilePicPath_;
};
