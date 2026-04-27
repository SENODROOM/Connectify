#pragma once
#include "User.h"

// Dynamic raw array of User* pointers.
// Does NOT own the User objects — callers manage lifetime.
class UserTable {
public:
    UserTable() : data_(nullptr), size_(0), capacity_(0) {}
    ~UserTable() { delete[] data_; }

    UserTable(const UserTable&)            = delete;
    UserTable& operator=(const UserTable&) = delete;

    void add(User* u) {
        if (size_ == capacity_) grow();
        data_[size_++] = u;
    }

    bool remove(int userID) {
        for (int i = 0; i < size_; ++i) {
            if (data_[i]->getID() == userID) {
                for (int j = i; j < size_ - 1; ++j)
                    data_[j] = data_[j + 1];
                --size_;
                return true;
            }
        }
        return false;
    }

    User* findByID(int id) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i]->getID() == id) return data_[i];
        return nullptr;
    }

    User* findByEmail(const std::string& email) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i]->getEmail() == email) return data_[i];
        return nullptr;
    }

    User*  operator[](int i) const { return data_[i]; }
    int    size()             const { return size_; }
    bool   empty()            const { return size_ == 0; }

    void clear() { size_ = 0; }   // does NOT delete User objects

private:
    void grow() {
        int newCap  = (capacity_ == 0) ? 8 : capacity_ * 2;
        User** nd   = new User*[newCap];
        for (int i = 0; i < size_; ++i) nd[i] = data_[i];
        delete[] data_;
        data_     = nd;
        capacity_ = newCap;
    }

    User** data_;
    int    size_;
    int    capacity_;
};
