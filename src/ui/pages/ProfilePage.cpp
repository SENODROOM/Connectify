#include "ProfilePage.h"
#include "../Session.h"
#include "../widgets/PostCard.h"
#include "../../managers/AuthManager.h"
#include "../../managers/FileManager.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>

ProfilePage::ProfilePage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ProfilePage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(120, 24, 120, 24);
    root->setSpacing(16);

    backBtn_ = new QPushButton("← Back to Feed");
    backBtn_->setObjectName("linkBtn");
    root->addWidget(backBtn_, 0, Qt::AlignLeft);
    connect(backBtn_, &QPushButton::clicked, this, &ProfilePage::goBack);

    // Profile card
    auto* card = new QFrame();
    card->setObjectName("card");
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(8);

    nameLabel_  = new QLabel();
    nameLabel_->setObjectName("profileName");
    emailLabel_ = new QLabel();
    emailLabel_->setObjectName("profileEmail");
    statsLabel_ = new QLabel();
    statsLabel_->setObjectName("profileStats");

    cardLayout->addWidget(nameLabel_);
    cardLayout->addWidget(emailLabel_);
    cardLayout->addWidget(statsLabel_);

    followBtn_ = new QPushButton("Follow");
    followBtn_->setObjectName("primaryBtn");
    followBtn_->setFixedWidth(120);
    cardLayout->addWidget(followBtn_, 0, Qt::AlignLeft);

    root->addWidget(card);

    // Posts scroll
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    postsContainer_ = new QWidget();
    postsLayout_    = new QVBoxLayout(postsContainer_);
    postsLayout_->setAlignment(Qt::AlignTop);
    postsLayout_->setSpacing(12);

    scroll->setWidget(postsContainer_);
    root->addWidget(scroll, 1);

    connect(followBtn_, &QPushButton::clicked, this, [this]() {
        User* me     = Session::instance().current();
        User* target = AuthManager::instance().findUser(viewedUserID_);
        if (!me || !target || me->getID() == target->getID()) return;

        if (me->isFollowing(target->getID())) {
            me->unfollow(target->getID());
            target->removeFollower(me->getID());
            followBtn_->setText("Follow");
        } else {
            me->follow(target->getID());
            target->addFollower(me->getID());
            followBtn_->setText("Unfollow");
        }
        FileManager::instance().saveFriends(AuthManager::instance().getUsers());
        loadUser(viewedUserID_);
    });
}

void ProfilePage::loadUser(int userID) {
    viewedUserID_ = userID;
    User* me     = Session::instance().current();
    User* target = AuthManager::instance().findUser(userID);
    if (!target) return;

    nameLabel_->setText(QString::fromStdString(target->getName()));
    emailLabel_->setText(QString::fromStdString(target->getEmail()));
    statsLabel_->setText(
        QString("%1 posts · %2 followers · %3 following")
            .arg(target->getPosts().size())
            .arg(target->getFollowers().size())
            .arg(target->getFollowing().size()));

    bool isOwnProfile = me && me->getID() == userID;
    followBtn_->setVisible(!isOwnProfile);
    if (!isOwnProfile && me) {
        followBtn_->setText(me->isFollowing(userID) ? "Unfollow" : "Follow");
    }

    buildPosts();
}

void ProfilePage::buildPosts() {
    // Clear existing
    QLayoutItem* item;
    while ((item = postsLayout_->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    User* target = AuthManager::instance().findUser(viewedUserID_);
    User* me     = Session::instance().current();
    if (!target) return;

    auto posts = target->getPosts();
    if (posts.empty()) {
        auto* lbl = new QLabel("No posts yet.");
        lbl->setObjectName("emptyLabel");
        lbl->setAlignment(Qt::AlignCenter);
        postsLayout_->addWidget(lbl);
        return;
    }

    for (Post* p : posts) {
        auto* card = new PostCard(p, me, this);
        postsLayout_->addWidget(card);
    }
}
