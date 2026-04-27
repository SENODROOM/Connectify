#pragma once
#include <stdexcept>
#include <cstring>

// Generic resizable array of pointers (no STL containers used)
// Used everywhere instead of std::vector
template<typename T>
class Array {
public:
    Array() : data_(nullptr), size_(0), capacity_(0) {}

    ~Array() {
        delete[] data_;
    }

    // Copy
    Array(const Array& other) : data_(nullptr), size_(0), capacity_(0) {
        for (int i = 0; i < other.size_; ++i)
            push(other.data_[i]);
    }

    Array& operator=(const Array& other) {
        if (this == &other) return *this;
        delete[] data_;
        data_ = nullptr; size_ = 0; capacity_ = 0;
        for (int i = 0; i < other.size_; ++i)
            push(other.data_[i]);
        return *this;
    }

    void push(T item) {
        if (size_ == capacity_) grow();
        data_[size_++] = item;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size_) return;
        for (int i = index; i < size_ - 1; ++i)
            data_[i] = data_[i + 1];
        --size_;
    }

    bool remove(T item) {
        for (int i = 0; i < size_; ++i) {
            if (data_[i] == item) { removeAt(i); return true; }
        }
        return false;
    }

    bool contains(T item) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i] == item) return true;
        return false;
    }

    T operator[](int i) const { return data_[i]; }
    T& operator[](int i)     { return data_[i]; }

    int  size()    const { return size_; }
    bool empty()   const { return size_ == 0; }
    void clear()         { size_ = 0; }

private:
    void grow() {
        int newCap = (capacity_ == 0) ? 4 : capacity_ * 2;
        T* newData  = new T[newCap];
        for (int i = 0; i < size_; ++i) newData[i] = data_[i];
        delete[] data_;
        data_     = newData;
        capacity_ = newCap;
    }

    T*  data_;
    int size_;
    int capacity_;
};
