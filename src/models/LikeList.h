#pragma once

// Fixed-ceiling likes tracker using a raw int* array
// Grows dynamically but stays pointer-only (no STL)
class LikeList {
public:
    LikeList() : data_(nullptr), size_(0), capacity_(0) {}

    ~LikeList() { delete[] data_; }

    LikeList(const LikeList& o) : data_(nullptr), size_(0), capacity_(0) {
        for (int i = 0; i < o.size_; ++i) add(o.data_[i]);
    }
    LikeList& operator=(const LikeList& o) {
        if (this == &o) return *this;
        delete[] data_; data_ = nullptr; size_ = 0; capacity_ = 0;
        for (int i = 0; i < o.size_; ++i) add(o.data_[i]);
        return *this;
    }

    bool add(int userID) {
        if (has(userID)) return false;
        if (size_ == capacity_) grow();
        data_[size_++] = userID;
        return true;
    }

    bool remove(int userID) {
        for (int i = 0; i < size_; ++i) {
            if (data_[i] == userID) {
                for (int j = i; j < size_ - 1; ++j)
                    data_[j] = data_[j + 1];
                --size_;
                return true;
            }
        }
        return false;
    }

    bool has(int userID) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i] == userID) return true;
        return false;
    }

    int count() const { return size_; }

private:
    void grow() {
        int newCap = (capacity_ == 0) ? 4 : capacity_ * 2;
        int* nd    = new int[newCap];
        for (int i = 0; i < size_; ++i) nd[i] = data_[i];
        delete[] data_;
        data_     = nd;
        capacity_ = newCap;
    }

    int* data_;
    int  size_;
    int  capacity_;
};
