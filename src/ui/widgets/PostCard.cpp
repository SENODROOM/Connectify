#include "PostCard.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QDateTime>

PostCard::PostCard(Post* post, User* currentUser, QWidget* parent)
    : QWidget(parent), post_(post), currentUser_(currentUser), likeBtn_(nullptr)
{ setupUI(); }

void PostCard::setupUI() {
    auto* card = new QFrame(); card->setObjectName("postCard");
    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(0,0,0,0); outer->addWidget(card);
    auto* lay = new QVBoxLayout(card); lay->setContentsMargins(16,12,16,12); lay->setSpacing(8);

    User* owner = AuthManager::instance().findUser(post_->getOwnerID());
    QString author = owner ? QString::fromStdString(owner->getName()) : "Unknown";
    QString ts     = QDateTime::fromSecsSinceEpoch(post_->getTimestamp()).toString("MMM d · hh:mm");
    auto* meta = new QLabel(author + "  ·  " + ts); meta->setObjectName("postMeta");
    lay->addWidget(meta);

    std::string content;
    if (auto* tp = dynamic_cast<TextPost*>(post_))       content = tp->getContent();
    else if (auto* ip = dynamic_cast<ImagePost*>(post_)) content = "📷  " + ip->getCaption();
    auto* contentLbl = new QLabel(QString::fromStdString(content));
    contentLbl->setObjectName("postContent"); contentLbl->setWordWrap(true);
    lay->addWidget(contentLbl);

    auto* actions = new QHBoxLayout(); actions->setSpacing(8);
    likeBtn_ = new QPushButton(); likeBtn_->setObjectName("likeBtn"); likeBtn_->setCheckable(true);
    updateLike();
    actions->addWidget(likeBtn_);

    auto* commentBtn = new QPushButton(QString("💬  %1").arg(post_->getComments().size()));
    commentBtn->setObjectName("commentBtn");
    actions->addWidget(commentBtn);
    actions->addStretch();
    lay->addLayout(actions);

    connect(likeBtn_, &QPushButton::toggled, this, [this](bool checked){
        emit likeToggled(post_->getPostID(), checked);
        updateLike();
    });
}

void PostCard::updateLike() {
    bool liked = currentUser_ && post_->isLikedBy(currentUser_->getID());
    likeBtn_->setChecked(liked);
    likeBtn_->setText(liked
        ? QString("❤️  %1").arg(post_->getLikes())
        : QString("🤍  %1").arg(post_->getLikes()));
}
