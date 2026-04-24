#pragma once
#include <string>

class Person {
public:
    Person(int id, const std::string& name, const std::string& email, const std::string& hashedPassword);
    virtual ~Person() = default;

    // Pure virtual — forces subclasses to define role
    virtual std::string getRole() const = 0;

    bool login(const std::string& email, const std::string& password) const;
    virtual std::string getProfile() const;

    // Getters
    int         getID()       const { return id_; }
    std::string getName()     const { return name_; }
    std::string getEmail()    const { return email_; }
    std::string getHashedPassword() const { return hashedPassword_; }

    // Setters
    void setName(const std::string& name)   { name_ = name; }
    void setEmail(const std::string& email) { email_ = email; }

protected:
    int         id_;
    std::string name_;
    std::string email_;
    std::string hashedPassword_;  // never stored in plain text
};
