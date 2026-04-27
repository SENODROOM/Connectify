#pragma once
#include "../models/User.h"

class Session {
public:
    static Session& instance();

    void  set(User* u)       { currentUser_ = u; isAdmin_ = false; }
    void  setAdmin(bool a)   { isAdmin_ = a; if (a) currentUser_ = nullptr; }
    void  clear()            { currentUser_ = nullptr; isAdmin_ = false; }

    User* current()    const { return currentUser_; }
    bool  isAdmin()    const { return isAdmin_; }
    bool  isLoggedIn() const { return currentUser_ != nullptr || isAdmin_; }

private:
    Session() : currentUser_(nullptr), isAdmin_(false) {}
    Session(const Session&)            = delete;
    Session& operator=(const Session&) = delete;

    User* currentUser_;
    bool  isAdmin_;
};
