#include "Admin.h"
#include <iostream>

Admin::Admin(int id, const std::string& name, const std::string& email,
             const std::string& hashedPassword)
    : Person(id, name, email, hashedPassword)
{}

void Admin::banUser(User* user) {
    if (user) {
        user->setBanned(true);
        std::cout << "[Admin] Banned user: " << user->getName() << "\n";
    }
}

void Admin::unbanUser(User* user) {
    if (user) {
        user->setBanned(false);
        std::cout << "[Admin] Unbanned user: " << user->getName() << "\n";
    }
}

bool Admin::deletePost(int postID, std::vector<User*>& allUsers) {
    for (User* u : allUsers) {
        Post* p = u->getPost(postID);
        if (p) {
            u->removePost(postID);
            std::cout << "[Admin] Deleted post #" << postID << "\n";
            return true;
        }
    }
    return false;
}

std::vector<User*> Admin::viewAllUsers(const std::vector<User*>& allUsers) const {
    return allUsers;  // Admin sees everyone; caller decides how to display
}
