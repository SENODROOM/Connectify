#include "ImagePost.h"
#include <iostream>

ImagePost::ImagePost(int postID, int ownerID,
                     const std::string& imagePath,
                     const std::string& caption,
                     time_t timestamp)
    : Post(postID, ownerID, timestamp),
      imagePath_(imagePath), caption_(caption)
{}

void ImagePost::display() const {
    std::cout << "[ImagePost #" << postID_
              << " | Owner: " << ownerID_
              << " | Likes: " << getLikes() << "]\n"
              << "Image: " << imagePath_ << "\n"
              << "Caption: " << caption_ << "\n";
}
