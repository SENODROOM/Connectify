#pragma once
#include "../models/UserTable.h"
#include "../models/Admin.h"

class AuthManager {
public:
    static AuthManager& instance();

    User*  login(const std::string& email, const std::string& password);
    bool   isAdminLogin(const std::string& email, const std::string& password);
    User*  signup(const std::string& name, const std::string& email,
                  const std::string& password);
    bool   deleteAccount(int userID);

    void loadAll();
    void saveAll();

    const UserTable& getUsers() const { return users_; }
    UserTable&       getUsers()       { return users_; }

    User*  findUser(int id)                  const { return users_.findByID(id); }
    User*  findByEmail(const std::string& e) const { return users_.findByEmail(e); }
    Admin* getAdmin()                        const { return admin_; }

    int nextUserID() { return ++lastUserID_; }

private:
    AuthManager();
    ~AuthManager();
    AuthManager(const AuthManager&)            = delete;
    AuthManager& operator=(const AuthManager&) = delete;

    UserTable users_;
    Admin*    admin_      = nullptr;
    int       lastUserID_ = 1000;
};
