#include "Admin.h"

Admin::Admin(int id, const std::string& name,
             const std::string& email,
             const std::string& hashedPassword)
    : Person(id, name, email, hashedPassword)
{}

bool Admin::deletePost(int postID, UserTable& allUsers) {
    for (int i = 0; i < allUsers.size(); ++i) {
        User* u = allUsers[i];
        if (u->findPost(postID)) {
            return u->removePost(postID);
        }
    }
    return false;
}
