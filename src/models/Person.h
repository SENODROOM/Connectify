#pragma once
#include <string>

class Person {
public:
    Person(int id, const std::string& name,
           const std::string& email, const std::string& hashedPassword);
    virtual ~Person() = default;

    virtual std::string getRole()    const = 0;
    virtual std::string getProfile() const;

    bool login(const std::string& email, const std::string& password) const;

    int         getID()             const { return id_; }
    std::string getName()           const { return name_; }
    std::string getEmail()          const { return email_; }
    std::string getHashedPassword() const { return hashedPassword_; }

    void setName(const std::string& n)  { name_  = n; }
    void setEmail(const std::string& e) { email_ = e; }

protected:
    int         id_;
    std::string name_;
    std::string email_;
    std::string hashedPassword_;
};
