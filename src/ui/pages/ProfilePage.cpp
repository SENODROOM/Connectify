#include "ProfilePage.h"
#include "../Session.h"
#include "../widgets/PostCard.h"
#include "../../managers/AuthManager.h"
#include "../../managers/FileManager.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>

ProfilePage::ProfilePage(QWidget* parent)
    : QWidget(parent), viewedUserID_(-1),
      nameLabel_(nullptr), emailLabel_(nullptr), statsLabel_(nullptr),
      followBtn_(nullptr), postsLayout_(nullptr), postsContainer_(nullptr)
{ setupUI(); }

void ProfilePage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(120,24,120,24); root->setSpacing(16);

    auto* backBtn = new QPushButton("← Back to Feed"); backBtn->setObjectName("linkBtn");
    root->addWidget(backBtn, 0, Qt::AlignLeft);
    connect(backBtn, &QPushButton::clicked, this, &ProfilePage::goBack);

    auto* card = new QFrame(); card->setObjectName("card");
    auto* cardL = new QVBoxLayout(card);
    cardL->setContentsMargins(24,24,24,24); cardL->setSpacing(8);
    nameLabel_  = new QLabel(); nameLabel_->setObjectName("profileName");
    emailLabel_ = new QLabel(); emailLabel_->setObjectName("profileEmail");
    statsLabel_ = new QLabel(); statsLabel_->setObjectName("profileStats");
    cardL->addWidget(nameLabel_); cardL->addWidget(emailLabel_); cardL->addWidget(statsLabel_);
    followBtn_ = new QPushButton("Follow"); followBtn_->setObjectName("primaryBtn"); followBtn_->setFixedWidth(120);
    cardL->addWidget(followBtn_, 0, Qt::AlignLeft);
    root->addWidget(card);

    auto* scroll = new QScrollArea(); scroll->setWidgetResizable(true); scroll->setFrameShape(QFrame::NoFrame);
    postsContainer_ = new QWidget();
    postsLayout_    = new QVBoxLayout(postsContainer_);
    postsLayout_->setAlignment(Qt::AlignTop); postsLayout_->setSpacing(12);
    scroll->setWidget(postsContainer_);
    root->addWidget(scroll, 1);

    connect(followBtn_, &QPushButton::clicked, this, [this]() {
        User* me     = Session::instance().current();
        User* target = AuthManager::instance().findUser(viewedUserID_);
        if (!me || !target || me->getID() == target->getID()) return;
        if (me->isFollowing(target->getID())) {
            me->unfollow(target->getID());
            target->removeFollower(me->getID());
        } else {
            me->follow(target->getID());
            target->addFollower(me->getID());
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
    statsLabel_->setText(QString("%1 posts · %2 followers · %3 following")
        .arg(target->getPosts().size())
        .arg(target->getFollowers().size())
        .arg(target->getFollowing().size()));

    bool own = me && me->getID() == userID;
    followBtn_->setVisible(!own);
    if (!own && me)
        followBtn_->setText(me->isFollowing(userID) ? "Unfollow" : "Follow");
    buildPosts();
}

void ProfilePage::buildPosts() {
    QLayoutItem* item;
    while ((item = postsLayout_->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    User* target = AuthManager::instance().findUser(viewedUserID_);
    User* me     = Session::instance().current();
    if (!target) return;

    if (target->getPosts().size() == 0) {
        auto* lbl = new QLabel("No posts yet."); lbl->setObjectName("emptyLabel"); lbl->setAlignment(Qt::AlignCenter);
        postsLayout_->addWidget(lbl); return;
    }
    PostNode* node = target->getPosts().head();
    while (node) {
        postsLayout_->addWidget(new PostCard(node->post, me, this));
        node = node->next;
    }
}
