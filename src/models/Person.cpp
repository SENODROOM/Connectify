#include "Person.h"
#include <sstream>

Person::Person(int id, const std::string& name, const std::string& email, const std::string& hashedPassword)
    : id_(id), name_(name), email_(email), hashedPassword_(hashedPassword)
{}

bool Person::login(const std::string& email, const std::string& password) const {
    return (email_ == email && hashedPassword_ == password);
}

std::string Person::getProfile() const {
    std::ostringstream oss;
    oss << "[" << getRole() << "] " << name_ << " <" << email_ << "> (ID: " << id_ << ")";
    return oss.str();
}
