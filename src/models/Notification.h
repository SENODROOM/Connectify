#pragma once
#include <string>
#include <ctime>

enum class NotifType { LIKE, COMMENT, REQUEST, FOLLOW, SYSTEM };

// Linked-list node
struct NotifNode {
    int         notifID;
    int         ownerID;
    NotifType   type;
    std::string message;
    bool        isRead;
    time_t      timestamp;
    NotifNode*  next;

    NotifNode(int nid, int oid, NotifType t,
              const std::string& msg, time_t ts)
        : notifID(nid), ownerID(oid), type(t),
          message(msg), isRead(false), timestamp(ts), next(nullptr) {}

    std::string typeString() const {
        switch (type) {
            case NotifType::LIKE:    return "LIKE";
            case NotifType::COMMENT: return "COMMENT";
            case NotifType::REQUEST: return "REQUEST";
            case NotifType::FOLLOW:  return "FOLLOW";
            default:                 return "SYSTEM";
        }
    }

    static NotifType typeFromString(const std::string& s) {
        if (s == "LIKE")    return NotifType::LIKE;
        if (s == "COMMENT") return NotifType::COMMENT;
        if (s == "REQUEST") return NotifType::REQUEST;
        if (s == "FOLLOW")  return NotifType::FOLLOW;
        return NotifType::SYSTEM;
    }
};

// Singly-linked list of notifications
class NotifList {
public:
    NotifList()  : head_(nullptr), size_(0) {}
    ~NotifList() { clear(); }

    NotifList(const NotifList&)            = delete;
    NotifList& operator=(const NotifList&) = delete;

    void append(NotifNode* node) {
        node->next = nullptr;
        if (!head_) { head_ = node; }
        else {
            NotifNode* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size_;
    }

    void clear() {
        NotifNode* cur = head_;
        while (cur) { NotifNode* n = cur->next; delete cur; cur = n; }
        head_ = nullptr; size_ = 0;
    }

    NotifNode* head() const { return head_; }
    int        size() const { return size_; }

private:
    NotifNode* head_;
    int        size_;
};
