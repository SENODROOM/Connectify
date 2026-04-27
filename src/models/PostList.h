#pragma once
#include "Post.h"

// Singly-linked list node wrapping a Post*
struct PostNode {
    Post*     post;
    PostNode* next;
    explicit PostNode(Post* p) : post(p), next(nullptr) {}
};

// Singly-linked list of Post pointers.
// The list OWNS the Post objects — destructor deletes them.
class PostList {
public:
    PostList()  : head_(nullptr), size_(0) {}
    ~PostList() { clear(); }

    PostList(const PostList&)            = delete;
    PostList& operator=(const PostList&) = delete;

    void append(Post* p) {
        PostNode* node = new PostNode(p);
        if (!head_) { head_ = node; }
        else {
            PostNode* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size_;
    }

    // Remove by postID — also deletes the Post object
    bool remove(int postID) {
        PostNode* prev = nullptr;
        PostNode* cur  = head_;
        while (cur) {
            if (cur->post->getPostID() == postID) {
                if (prev) prev->next = cur->next;
                else      head_      = cur->next;
                delete cur->post;
                delete cur;
                --size_;
                return true;
            }
            prev = cur;
            cur  = cur->next;
        }
        return false;
    }

    Post* find(int postID) const {
        PostNode* cur = head_;
        while (cur) {
            if (cur->post->getPostID() == postID) return cur->post;
            cur = cur->next;
        }
        return nullptr;
    }

    void clear() {
        PostNode* cur = head_;
        while (cur) {
            PostNode* next = cur->next;
            delete cur->post;
            delete cur;
            cur = next;
        }
        head_ = nullptr;
        size_ = 0;
    }

    PostNode* head() const { return head_; }
    int       size() const { return size_; }

private:
    PostNode* head_;
    int       size_;
};
