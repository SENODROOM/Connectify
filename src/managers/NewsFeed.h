#pragma once
#include "../models/User.h"
#include "../models/UserTable.h"

// Holds a raw pointer array of Post* for the rendered feed.
// Does NOT own the posts — they belong to their User's PostList.
class FeedSnapshot {
public:
    FeedSnapshot() : data_(nullptr), size_(0), capacity_(0) {}
    ~FeedSnapshot() { delete[] data_; }

    void add(Post* p) {
        if (size_ == capacity_) {
            int nc    = (capacity_ == 0) ? 16 : capacity_ * 2;
            Post** nd = new Post*[nc];
            for (int i = 0; i < size_; ++i) nd[i] = data_[i];
            delete[] data_; data_ = nd; capacity_ = nc;
        }
        data_[size_++] = p;
    }

    // Simple insertion sort — newest first by timestamp
    void sortNewestFirst() {
        for (int i = 1; i < size_; ++i) {
            Post* key = data_[i];
            int   j   = i - 1;
            while (j >= 0 && data_[j]->getTimestamp() < key->getTimestamp()) {
                data_[j + 1] = data_[j];
                --j;
            }
            data_[j + 1] = key;
        }
    }

    Post* operator[](int i) const { return data_[i]; }
    int   size()            const { return size_; }

private:
    Post** data_;
    int    size_;
    int    capacity_;
};

class NewsFeed {
public:
    // Fills snapshot with posts from users the currentUser follows
    void generate(const User* currentUser,
                  const UserTable& allUsers,
                  FeedSnapshot& out) const;

    static int nextPostID(const UserTable& allUsers);
};
