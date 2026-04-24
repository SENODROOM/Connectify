#include "AuthManager.h"
#include "FileManager.h"
#include <algorithm>
#include <iostream>

// Default admin credentials
static const std::string ADMIN_EMAIL = "admin@connectify.com";
static const std::string ADMIN_PASS_HASH = ""; // set after hashing "admin123"

AuthManager::AuthManager() {
    // Create the hardcoded admin account
    std::string adminHash = FileManager::hashPassword("admin123");
    admin_ = new Admin(1, "Administrator", ADMIN_EMAIL, adminHash);
}

AuthManager::~AuthManager() {
    delete admin_;
    for (User* u : users_) delete u;
}

AuthManager& AuthManager::instance() {
    static AuthManager am;
    return am;
}

void AuthManager::loadAll() {
    for (User* u : users_) delete u;
    users_ = FileManager::instance().loadAllUsers();
    FileManager::instance().loadAllPosts(users_);
    FileManager::instance().loadFriends(users_);

    // Determine highest existing ID
    for (User* u : users_)
        if (u->getID() > lastUserID_) lastUserID_ = u->getID();
}

void AuthManager::saveAll() {
    FileManager::instance().saveAllUsers(users_);
    FileManager::instance().saveAllPosts(users_);
    FileManager::instance().saveFriends(users_);
}

User* AuthManager::login(const std::string& email, const std::string& password) {
    std::string hash = FileManager::hashPassword(password);
    for (User* u : users_) {
        if (u->getEmail() == email && u->getHashedPassword() == hash) {
            if (u->isBanned()) {
                std::cerr << "[Auth] Account is banned.\n";
                return nullptr;
            }
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
    // Check for duplicate email
    if (findUserByEmail(email)) {
        std::cerr << "[Auth] Email already registered.\n";
        return nullptr;
    }
    if (name.empty() || email.empty() || password.size() < 6) {
        std::cerr << "[Auth] Invalid signup data.\n";
        return nullptr;
    }
    int id = nextUserID();
    std::string hash = FileManager::hashPassword(password);
    User* u = new User(id, name, email, hash);
    users_.push_back(u);
    saveAll();
    return u;
}

bool AuthManager::deleteAccount(int userID) {
    auto it = std::find_if(users_.begin(), users_.end(),
                           [userID](User* u){ return u->getID() == userID; });
    if (it == users_.end()) return false;
    delete *it;
    users_.erase(it);
    saveAll();
    return true;
}

void AuthManager::banUser(int userID) {
    User* u = findUser(userID);
    if (u) { u->setBanned(true); saveAll(); }
}

void AuthManager::unbanUser(int userID) {
    User* u = findUser(userID);
    if (u) { u->setBanned(false); saveAll(); }
}

User* AuthManager::findUser(int id) const {
    for (User* u : users_)
        if (u->getID() == id) return u;
    return nullptr;
}

User* AuthManager::findUserByEmail(const std::string& email) const {
    for (User* u : users_)
        if (u->getEmail() == email) return u;
    return nullptr;
}
