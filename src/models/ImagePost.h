#pragma once
#include "Post.h"

class ImagePost : public Post {
public:
    ImagePost(int postID, int ownerID,
              const std::string& imagePath,
              const std::string& caption,
              time_t timestamp = time(nullptr));

    void        display() const override;
    std::string getType()      const override { return "IMAGE"; }
    std::string getImagePath() const { return imagePath_; }
    std::string getCaption()   const { return caption_; }

private:
    std::string imagePath_;
    std::string caption_;
};
