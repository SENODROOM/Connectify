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
    void commentClicked(int postID);

private:
    void setupUI();
    void updateLikeButton();

    Post* post_;
    User* currentUser_;

    QLabel*      contentLabel_;
    QLabel*      metaLabel_;
    QPushButton* likeBtn_;
    QPushButton* commentBtn_;
};
