#pragma once
#include "Person.h"
#include "User.h"
#include "Post.h"
#include <vector>
#include <string>

class Admin : public Person {
public:
    Admin(int id, const std::string& name, const std::string& email,
          const std::string& hashedPassword);

    std::string getRole() const override { return "ADMIN"; }

    // Admin-only actions
    void banUser(User* user);
    void unbanUser(User* user);
    bool deletePost(int postID, std::vector<User*>& allUsers);
    std::vector<User*> viewAllUsers(const std::vector<User*>& allUsers) const;
};
