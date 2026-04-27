#pragma once
#include "../models/UserTable.h"
#include <string>

struct RequestNode {
    int          requestID;
    int          fromID;
    int          toID;
    std::string  status;   // "PENDING" | "ACCEPTED" | "REJECTED"
    RequestNode* next;

    RequestNode(int rid, int fid, int tid, const std::string& s)
        : requestID(rid), fromID(fid), toID(tid), status(s), next(nullptr) {}
};

// Singly-linked list of friend requests
class RequestList {
public:
    RequestList()  : head_(nullptr), size_(0) {}
    ~RequestList() { clear(); }

    RequestList(const RequestList&)            = delete;
    RequestList& operator=(const RequestList&) = delete;

    void append(RequestNode* node) {
        node->next = nullptr;
        if (!head_) { head_ = node; }
        else {
            RequestNode* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size_;
    }

    RequestNode* findByID(int requestID) const {
        RequestNode* cur = head_;
        while (cur) {
            if (cur->requestID == requestID) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void clear() {
        RequestNode* cur = head_;
        while (cur) { RequestNode* n = cur->next; delete cur; cur = n; }
        head_ = nullptr; size_ = 0;
    }

    RequestNode* head() const { return head_; }
    int          size() const { return size_; }

private:
    RequestNode* head_;
    int          size_;
};

class FriendGraph {
public:
    static FriendGraph& instance();

    void sendRequest(int fromID, int toID);
    void acceptRequest(int requestID, UserTable& allUsers);
    void rejectRequest(int requestID);

    // Returns raw pointer array of pending requests for userID
    // Caller must delete[] the returned array
    RequestNode** getPendingFor(int userID, int& count) const;

    void loadRequests();
    void saveRequests() const;

private:
    FriendGraph() : lastRequestID_(5000) {}
    FriendGraph(const FriendGraph&)            = delete;
    FriendGraph& operator=(const FriendGraph&) = delete;

    RequestList requests_;
    int         lastRequestID_;

    static const std::string REQUESTS_FILE;
};
