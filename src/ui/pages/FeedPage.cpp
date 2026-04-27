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
#include <QPushButton>

FeedPage::FeedPage(QWidget* parent) : QWidget(parent) { setupUI(); }

void FeedPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0); root->setSpacing(0);

    // Navbar
    auto* nav = new QFrame(); nav->setObjectName("navbar");
    auto* navL = new QHBoxLayout(nav); navL->setContentsMargins(16,8,16,8);
    auto* brand = new QLabel("Connectify"); brand->setObjectName("brand");
    navL->addWidget(brand); navL->addStretch();
    auto* mkBtn = [](const QString& t, const QString& obj) {
        auto* b = new QPushButton(t); b->setObjectName(obj); return b;
    };
    auto* searchBtn   = mkBtn("Search",        "navBtn");
    auto* msgsBtn     = mkBtn("Messages",      "navBtn");
    auto* notifsBtn   = mkBtn("Notifications", "navBtn");
    auto* profileBtn  = mkBtn("Profile",       "navBtn");
    auto* logoutBtn   = mkBtn("Logout",        "dangerBtn");
    for (auto* b : {searchBtn, msgsBtn, notifsBtn, profileBtn}) navL->addWidget(b);
    navL->addWidget(logoutBtn);
    root->addWidget(nav);

    connect(searchBtn,  &QPushButton::clicked, this, &FeedPage::goToSearch);
    connect(msgsBtn,    &QPushButton::clicked, this, &FeedPage::goToMessages);
    connect(notifsBtn,  &QPushButton::clicked, this, &FeedPage::goToNotifs);
    connect(profileBtn, &QPushButton::clicked, this, [this]{ emit goToProfile(-1); });
    connect(logoutBtn,  &QPushButton::clicked, this, &FeedPage::logout);

    // Composer
    auto* composer = new QFrame(); composer->setObjectName("card");
    auto* compL = new QHBoxLayout(composer); compL->setContentsMargins(16,12,16,12);
    postInput_ = new QLineEdit(); postInput_->setPlaceholderText("What's on your mind?"); postInput_->setObjectName("inputField");
    auto* postBtn = new QPushButton("Post"); postBtn->setObjectName("primaryBtn");
    compL->addWidget(postInput_); compL->addWidget(postBtn);

    // Center column
    auto* center = new QWidget();
    auto* centerL = new QVBoxLayout(center);
    centerL->setContentsMargins(120,16,120,16); centerL->setSpacing(12);
    centerL->addWidget(composer);

    // Scroll
    auto* scroll = new QScrollArea(); scroll->setWidgetResizable(true); scroll->setFrameShape(QFrame::NoFrame);
    feedContainer_ = new QWidget();
    feedLayout_    = new QVBoxLayout(feedContainer_);
    feedLayout_->setAlignment(Qt::AlignTop); feedLayout_->setSpacing(12); feedLayout_->setContentsMargins(0,0,0,16);
    emptyLabel_ = new QLabel("No posts yet. Follow someone!");
    emptyLabel_->setObjectName("emptyLabel"); emptyLabel_->setAlignment(Qt::AlignCenter);
    feedLayout_->addWidget(emptyLabel_);
    scroll->setWidget(feedContainer_);
    centerL->addWidget(scroll);
    root->addWidget(center, 1);

    connect(postBtn,   &QPushButton::clicked,      this, &FeedPage::onCreatePost);
    connect(postInput_,&QLineEdit::returnPressed,  this, &FeedPage::onCreatePost);
}

void FeedPage::refresh() { buildFeed(); }

void FeedPage::clearFeed() {
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

    FeedSnapshot snap;
    NewsFeed nf;
    nf.generate(me, AuthManager::instance().getUsers(), snap);

    if (snap.size() == 0) {
        feedLayout_->addWidget(emptyLabel_);
        emptyLabel_->setVisible(true);
        return;
    }
    emptyLabel_->setVisible(false);

    for (int i = 0; i < snap.size(); ++i) {
        Post* p    = snap[i];
        auto* card = new PostCard(p, me, this);
        connect(card, &PostCard::likeToggled, this, [this, p, me](int /*pid*/, bool liked) {
            if (liked) {
                p->like(me->getID());
                User* owner = AuthManager::instance().findUser(p->getOwnerID());
                if (owner && owner->getID() != me->getID())
                    NotificationManager::instance().notify(
                        owner->getID(), NotifType::LIKE, me->getName() + " liked your post.");
            } else {
                p->unlike(me->getID());
            }
            FileManager::instance().saveAllPosts(AuthManager::instance().getUsers());
        });
        feedLayout_->addWidget(card);
    }
}

void FeedPage::onCreatePost() {
    QString text = postInput_->text().trimmed();
    if (text.isEmpty()) return;
    User* me = Session::instance().current();
    if (!me) return;

    int pid = NewsFeed::nextPostID(AuthManager::instance().getUsers());
    Post* p = new TextPost(pid, me->getID(), text.toStdString());
    me->addPost(p);
    FileManager::instance().saveAllPosts(AuthManager::instance().getUsers());
    postInput_->clear();
    buildFeed();
}
