#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "../../models/Post.h"
#include "../../models/User.h"

class PostCard : public QWidget {
    Q_OBJECT
public:
    PostCard(Post* post, User* currentUser, QWidget* parent = nullptr);
signals:
    void likeToggled(int postID, bool liked);
private:
    void setupUI();
    void updateLike();
    Post*        post_;
    User*        currentUser_;
    QPushButton* likeBtn_;
};
