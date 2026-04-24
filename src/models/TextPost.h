#pragma once
#include "Post.h"

class TextPost : public Post {
public:
    TextPost(int postID, int ownerID, const std::string& content, time_t timestamp = time(nullptr));

    void        display() const override;
    std::string getType() const override { return "TEXT"; }

    std::string getContent() const { return content_; }

private:
    std::string content_;
};
