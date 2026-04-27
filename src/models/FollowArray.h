#pragma once

// Dynamic raw int array — stores user IDs for follow lists
class FollowArray {
public:
    FollowArray() : data_(nullptr), size_(0), capacity_(0) {}

    ~FollowArray() { delete[] data_; }

    FollowArray(const FollowArray& o)
        : data_(nullptr), size_(0), capacity_(0) {
        for (int i = 0; i < o.size_; ++i) add(o.data_[i]);
    }
    FollowArray& operator=(const FollowArray& o) {
        if (this == &o) return *this;
        delete[] data_; data_ = nullptr; size_ = 0; capacity_ = 0;
        for (int i = 0; i < o.size_; ++i) add(o.data_[i]);
        return *this;
    }

    bool add(int id) {
        if (has(id)) return false;
        if (size_ == capacity_) grow();
        data_[size_++] = id;
        return true;
    }

    bool remove(int id) {
        for (int i = 0; i < size_; ++i) {
            if (data_[i] == id) {
                for (int j = i; j < size_ - 1; ++j)
                    data_[j] = data_[j + 1];
                --size_;
                return true;
            }
        }
        return false;
    }

    bool has(int id) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i] == id) return true;
        return false;
    }

    int  operator[](int i) const { return data_[i]; }
    int  size()            const { return size_; }
    bool empty()           const { return size_ == 0; }

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
