#include "PostCard.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QDateTime>

PostCard::PostCard(Post* post, User* currentUser, QWidget* parent)
    : QWidget(parent), post_(post), currentUser_(currentUser)
{
    setupUI();
}

void PostCard::setupUI() {
    auto* card = new QFrame();
    card->setObjectName("postCard");
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->addWidget(card);

    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(8);

    // Author + timestamp
    User* owner = AuthManager::instance().findUser(post_->getOwnerID());
    QString authorName = owner ? QString::fromStdString(owner->getName()) : "Unknown";
    QString ts = QDateTime::fromSecsSinceEpoch(post_->getTimestamp()).toString("MMM d · hh:mm");

    metaLabel_ = new QLabel(authorName + "  ·  " + ts);
    metaLabel_->setObjectName("postMeta");
    layout->addWidget(metaLabel_);

    // Content
    std::string content;
    if (auto* tp = dynamic_cast<TextPost*>(post_))
        content = tp->getContent();
    else if (auto* ip = dynamic_cast<ImagePost*>(post_))
        content = "📷  " + ip->getCaption();

    contentLabel_ = new QLabel(QString::fromStdString(content));
    contentLabel_->setObjectName("postContent");
    contentLabel_->setWordWrap(true);
    layout->addWidget(contentLabel_);

    // Action row
    auto* actionRow = new QHBoxLayout();
    actionRow->setSpacing(8);

    likeBtn_ = new QPushButton();
    likeBtn_->setObjectName("likeBtn");
    likeBtn_->setCheckable(true);
    updateLikeButton();
    actionRow->addWidget(likeBtn_);

    commentBtn_ = new QPushButton(QString("💬  %1").arg(post_->getComments().size()));
    commentBtn_->setObjectName("commentBtn");
    actionRow->addWidget(commentBtn_);

    actionRow->addStretch();
    layout->addLayout(actionRow);

    connect(likeBtn_, &QPushButton::toggled, this, [this](bool checked) {
        emit likeToggled(post_->getPostID(), checked);
        updateLikeButton();
    });
    connect(commentBtn_, &QPushButton::clicked, this, [this]() {
        emit commentClicked(post_->getPostID());
    });
}

void PostCard::updateLikeButton() {
    bool liked = currentUser_ && post_->isLikedBy(currentUser_->getID());
    likeBtn_->setChecked(liked);
    likeBtn_->setText(liked
        ? QString("❤️  %1").arg(post_->getLikes())
        : QString("🤍  %1").arg(post_->getLikes()));
}
