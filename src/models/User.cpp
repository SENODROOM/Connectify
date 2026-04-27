#include "User.h"
#include <sstream>

User::User(int id, const std::string& name,
           const std::string& email,
           const std::string& hashedPassword)
    : Person(id, name, email, hashedPassword)
{}

std::string User::getProfile() const {
    std::ostringstream oss;
    oss << Person::getProfile()
        << " | Following: " << following_.size()
        << " | Followers: " << followers_.size()
        << " | Posts: "     << posts_.size();
    if (banned_) oss << " [BANNED]";
    return oss.str();
}
