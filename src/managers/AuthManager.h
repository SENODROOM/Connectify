#pragma once
#include "../models/User.h"
#include "../models/Admin.h"
#include <vector>
#include <string>

class AuthManager {
public:
    static AuthManager& instance();

    // Returns User* on success, nullptr on failure
    User*  login(const std::string& email, const std::string& password);
    bool   isAdminLogin(const std::string& email, const std::string& password);

    User*  signup(const std::string& name, const std::string& email, const std::string& password);
    bool   deleteAccount(int userID);

    void   banUser(int userID);
    void   unbanUser(int userID);

    void   loadAll();   // call on app start
    void   saveAll();   // call on app exit / after mutations

    const std::vector<User*>& getUsers() const { return users_; }
    User* findUser(int id) const;
    User* findUserByEmail(const std::string& email) const;

    Admin* getAdmin() const { return admin_; }

    int nextUserID() { return ++lastUserID_; }

private:
    AuthManager();
    ~AuthManager();
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;

    std::vector<User*> users_;
    Admin*             admin_    = nullptr;
    int                lastUserID_ = 1000;
};
