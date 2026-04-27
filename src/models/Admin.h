#pragma once
#include "Person.h"
#include "UserTable.h"

class Admin : public Person {
public:
    Admin(int id, const std::string& name,
          const std::string& email,
          const std::string& hashedPassword);

    std::string getRole() const override { return "ADMIN"; }

    // Admin-only actions
    void banUser(User* u)   { if (u) u->setBanned(true);  }
    void unbanUser(User* u) { if (u) u->setBanned(false); }

    bool deletePost(int postID, UserTable& allUsers);
};
