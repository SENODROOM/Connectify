#pragma once
#include <string>
#include <ctime>

// Doubly-linked list node for messages (allows forward & backward traversal)
struct MsgNode {
    int         msgID;
    int         senderID;
    int         receiverID;
    std::string content;
    time_t      timestamp;
    MsgNode*    prev;
    MsgNode*    next;

    MsgNode(int mid, int sid, int rid,
            const std::string& c, time_t ts)
        : msgID(mid), senderID(sid), receiverID(rid),
          content(c), timestamp(ts), prev(nullptr), next(nullptr) {}
};

// Doubly-linked list of messages
class MessageList {
public:
    MessageList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~MessageList() { clear(); }

    MessageList(const MessageList&)            = delete;
    MessageList& operator=(const MessageList&) = delete;

    void append(int msgID, int senderID, int receiverID,
                const std::string& content, time_t ts) {
        MsgNode* node = new MsgNode(msgID, senderID, receiverID, content, ts);
        if (!tail_) { head_ = tail_ = node; }
        else {
            tail_->next = node;
            node->prev  = tail_;
            tail_       = node;
        }
        ++size_;
    }

    void clear() {
        MsgNode* cur = head_;
        while (cur) { MsgNode* n = cur->next; delete cur; cur = n; }
        head_ = tail_ = nullptr; size_ = 0;
    }

    MsgNode* head() const { return head_; }
    MsgNode* tail() const { return tail_; }
    int      size() const { return size_; }

private:
    MsgNode* head_;
    MsgNode* tail_;
    int      size_;
};
