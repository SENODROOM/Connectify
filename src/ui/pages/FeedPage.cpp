#include "FeedPage.h"
#include "../Session.h"
#include "../widgets/PostCard.h"
#include "../../managers/AuthManager.h"
#include "../../managers/NewsFeed.h"
#include "../../managers/FileManager.h"
#include "../../managers/NotificationManager.h"
#include "../../models/TextPost.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>

FeedPage::FeedPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void FeedPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── NavBar ──────────────────────────────────────────────────────────
    auto* nav = new QFrame();
    nav->setObjectName("navbar");
    auto* navLayout = new QHBoxLayout(nav);
    navLayout->setContentsMargins(16, 8, 16, 8);

    auto* brand = new QLabel("Connectify");
    brand->setObjectName("brand");
    navLayout->addWidget(brand);
    navLayout->addStretch();

    auto* feedBtn     = new QPushButton("Feed");
    auto* searchBtn   = new QPushButton("Search");
    auto* messagesBtn = new QPushButton("Messages");
    auto* notifsBtn   = new QPushButton("Notifications");
    auto* profileBtn  = new QPushButton("Profile");
    auto* logoutBtn   = new QPushButton("Logout");

    for (auto* b : {feedBtn, searchBtn, messagesBtn, notifsBtn, profileBtn})
        b->setObjectName("navBtn");
    logoutBtn->setObjectName("dangerBtn");

    navLayout->addWidget(feedBtn);
    navLayout->addWidget(searchBtn);
    navLayout->addWidget(messagesBtn);
    navLayout->addWidget(notifsBtn);
    navLayout->addWidget(profileBtn);
    navLayout->addWidget(logoutBtn);

    root->addWidget(nav);

    connect(searchBtn,   &QPushButton::clicked, this, &FeedPage::goToSearch);
    connect(messagesBtn, &QPushButton::clicked, this, &FeedPage::goToMessages);
    connect(notifsBtn,   &QPushButton::clicked, this, &FeedPage::goToNotifs);
    connect(profileBtn,  &QPushButton::clicked, this, [this](){
        emit goToProfile(-1);
    });
    connect(logoutBtn,   &QPushButton::clicked, this, &FeedPage::logout);

    // ── Create Post ─────────────────────────────────────────────────────
    auto* composer = new QFrame();
    composer->setObjectName("card");
    auto* compLayout = new QHBoxLayout(composer);
    compLayout->setContentsMargins(16, 12, 16, 12);

    postInput_ = new QLineEdit();
    postInput_->setPlaceholderText("What's on your mind?");
    postInput_->setObjectName("inputField");
    compLayout->addWidget(postInput_);

    postBtn_ = new QPushButton("Post");
    postBtn_->setObjectName("primaryBtn");
    compLayout->addWidget(postBtn_);

    auto* centerWidget = new QWidget();
    auto* centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setContentsMargins(120, 16, 120, 16);
    centerLayout->setSpacing(12);
    centerLayout->addWidget(composer);

    // ── Feed scroll area ────────────────────────────────────────────────
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setObjectName("feedScroll");
    scroll->setFrameShape(QFrame::NoFrame);

    feedContainer_ = new QWidget();
    feedLayout_    = new QVBoxLayout(feedContainer_);
    feedLayout_->setAlignment(Qt::AlignTop);
    feedLayout_->setSpacing(12);
    feedLayout_->setContentsMargins(120, 0, 120, 16);

    emptyLabel_ = new QLabel("No posts yet. Follow someone!");
    emptyLabel_->setObjectName("emptyLabel");
    emptyLabel_->setAlignment(Qt::AlignCenter);
    feedLayout_->addWidget(emptyLabel_);

    scroll->setWidget(feedContainer_);

    centerLayout->addWidget(scroll);
    root->addWidget(centerWidget, 1);

    connect(postBtn_,  &QPushButton::clicked,       this, &FeedPage::onCreatePost);
    connect(postInput_, &QLineEdit::returnPressed,  this, &FeedPage::onCreatePost);
}

void FeedPage::refresh() {
    buildFeed();
}

void FeedPage::clearFeed() {
    // Remove all PostCard widgets but keep emptyLabel_
    QLayoutItem* item;
    while ((item = feedLayout_->takeAt(0)) != nullptr) {
        if (item->widget() && item->widget() != emptyLabel_)
            item->widget()->deleteLater();
        delete item;
    }
}

void FeedPage::buildFeed() {
    clearFeed();

    User* me = Session::instance().current();
    if (!me) return;

    auto& users = AuthManager::instance().getUsers();
    NewsFeed nf;
    auto feed = nf.generateFeed(me, users);

    if (feed.empty()) {
        feedLayout_->addWidget(emptyLabel_);
        emptyLabel_->setVisible(true);
        return;
    }

    emptyLabel_->setVisible(false);
    for (Post* p : feed) {
        auto* card = new PostCard(p, me, this);
        connect(card, &PostCard::likeToggled, this, [this, p, me](int postID, bool liked) {
            auto& us = AuthManager::instance().getUsers();
            NewsFeed nf2;
            if (liked) {
                nf2.like(postID, me->getID(), const_cast<std::vector<User*>&>(us));
                // notify post owner
                User* owner = AuthManager::instance().findUser(p->getOwnerID());
                if (owner && owner->getID() != me->getID())
                    NotificationManager::instance().notify(
                        owner->getID(), NotifType::LIKE,
                        me->getName() + " liked your post.");
            } else {
                nf2.unlike(postID, me->getID(), const_cast<std::vector<User*>&>(us));
            }
        });
        feedLayout_->addWidget(card);
    }
}

void FeedPage::onCreatePost() {
    QString text = postInput_->text().trimmed();
    if (text.isEmpty()) return;

    User* me = Session::instance().current();
    if (!me) return;

    auto& users = const_cast<std::vector<User*>&>(AuthManager::instance().getUsers());

    int maxID = 2000;
    for (User* u : users)
        for (Post* p : u->getPosts())
            if (p->getPostID() > maxID) maxID = p->getPostID();

    Post* newPost = new TextPost(maxID + 1, me->getID(), text.toStdString());
    me->addPost(newPost);
    FileManager::instance().saveAllPosts(users);

    postInput_->clear();
    buildFeed();
}
