#pragma once
#include <string>
#include <ctime>

// Singly-linked list node — each Comment owns the next pointer
struct Comment {
    int         userID;
    std::string username;
    std::string text;
    time_t      timestamp;
    Comment*    next;

    Comment(int uid, const std::string& uname,
            const std::string& t, time_t ts)
        : userID(uid), username(uname), text(t),
          timestamp(ts), next(nullptr) {}
};

// Linked list wrapper so Post doesn't manage raw head/tail manually
class CommentList {
public:
    CommentList()  : head_(nullptr), size_(0) {}
    ~CommentList() { clear(); }

    // No copy (deep copy not needed for this project scope)
    CommentList(const CommentList&)            = delete;
    CommentList& operator=(const CommentList&) = delete;

    void add(int userID, const std::string& username,
             const std::string& text, time_t ts = time(nullptr)) {
        Comment* node = new Comment(userID, username, text, ts);
        // Append to end
        if (!head_) { head_ = node; }
        else {
            Comment* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size_;
    }

    void clear() {
        Comment* cur = head_;
        while (cur) {
            Comment* next = cur->next;
            delete cur;
            cur = next;
        }
        head_ = nullptr;
        size_ = 0;
    }

    Comment* head() const { return head_; }
    int      size() const { return size_; }

private:
    Comment* head_;
    int      size_;
};
