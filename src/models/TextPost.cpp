#include "TextPost.h"
#include <iostream>

TextPost::TextPost(int postID, int ownerID, const std::string& content, time_t timestamp)
    : Post(postID, ownerID, timestamp), content_(content)
{}

void TextPost::display() const {
    std::cout << "[TextPost #" << postID_ << " | Owner: " << ownerID_
              << " | Likes: " << getLikes() << "]\n"
              << content_ << "\n";
}
