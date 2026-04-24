#pragma once
#include "../models/User.h"

// Holds the currently logged-in user for the duration of the session.
class Session {
public:
    static Session& instance();

    void  set(User* user)    { currentUser_ = user; isAdmin_ = false; }
    void  setAdmin(bool a)   { isAdmin_ = a; }
    void  clear()            { currentUser_ = nullptr; isAdmin_ = false; }

    User* current()   const  { return currentUser_; }
    bool  isAdmin()   const  { return isAdmin_; }
    bool  isLoggedIn() const { return currentUser_ != nullptr || isAdmin_; }

private:
    Session() = default;
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    User* currentUser_ = nullptr;
    bool  isAdmin_     = false;
};
