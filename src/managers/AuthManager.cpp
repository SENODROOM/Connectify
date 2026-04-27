#include "AuthManager.h"
#include "FileManager.h"
#include <iostream>

AuthManager::AuthManager() {
    std::string adminHash = FileManager::hashPassword("admin123");
    admin_ = new Admin(1, "Administrator", "admin@connectify.com", adminHash);
}

AuthManager::~AuthManager() {
    delete admin_;
    for (int i = 0; i < users_.size(); ++i) delete users_[i];
}

AuthManager& AuthManager::instance() {
    static AuthManager am;
    return am;
}

void AuthManager::loadAll() {
    FileManager::instance().loadAllUsers(users_);
    FileManager::instance().loadAllPosts(users_);
    FileManager::instance().loadFriends(users_);

    for (int i = 0; i < users_.size(); ++i)
        if (users_[i]->getID() > lastUserID_)
            lastUserID_ = users_[i]->getID();
}

void AuthManager::saveAll() {
    FileManager::instance().saveAllUsers(users_);
    FileManager::instance().saveAllPosts(users_);
    FileManager::instance().saveFriends(users_);
}

User* AuthManager::login(const std::string& email, const std::string& password) {
    std::string hash = FileManager::hashPassword(password);
    for (int i = 0; i < users_.size(); ++i) {
        User* u = users_[i];
        if (u->getEmail() == email && u->getHashedPassword() == hash) {
            if (u->isBanned()) { std::cerr << "[Auth] Account banned.\n"; return nullptr; }
            return u;
        }
    }
    return nullptr;
}

bool AuthManager::isAdminLogin(const std::string& email, const std::string& password) {
    if (!admin_) return false;
    std::string hash = FileManager::hashPassword(password);
    return admin_->getEmail() == email && admin_->getHashedPassword() == hash;
}

User* AuthManager::signup(const std::string& name, const std::string& email,
                           const std::string& password) {
    if (findByEmail(email)) { std::cerr << "[Auth] Email exists.\n"; return nullptr; }
    if (name.empty() || email.empty() || password.size() < 6) return nullptr;

    int   id   = nextUserID();
    std::string hash = FileManager::hashPassword(password);
    User* u    = new User(id, name, email, hash);
    users_.add(u);
    saveAll();
    return u;
}

bool AuthManager::deleteAccount(int userID) {
    User* u = findUser(userID);
    if (!u) return false;
    users_.remove(userID);
    delete u;
    saveAll();
    return true;
}
