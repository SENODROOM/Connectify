#include "MessagesPage.h"
#include "../Session.h"
#include "../widgets/ChatBubble.h"
#include "../../managers/AuthManager.h"
#include "../../managers/MessageManager.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QSplitter>

MessagesPage::MessagesPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void MessagesPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Top bar
    auto* topBar = new QFrame();
    topBar->setObjectName("navbar");
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(16, 8, 16, 8);
    backBtn_ = new QPushButton("← Back");
    backBtn_->setObjectName("linkBtn");
    topLayout->addWidget(backBtn_);
    topLayout->addWidget(new QLabel("Messages"), 1, Qt::AlignCenter);
    topLayout->addStretch();
    root->addWidget(topBar);

    connect(backBtn_, &QPushButton::clicked, this, &MessagesPage::goBack);

    // Split: inbox list | chat panel
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(1);

    inboxList_ = new QListWidget();
    inboxList_->setObjectName("inboxList");
    inboxList_->setFixedWidth(240);
    splitter->addWidget(inboxList_);

    // Chat panel
    chatPanel_ = new QWidget();
    auto* chatLayout = new QVBoxLayout(chatPanel_);
    chatLayout->setContentsMargins(0, 0, 0, 0);
    chatLayout->setSpacing(0);

    chatHeader_ = new QLabel("Select a conversation");
    chatHeader_->setObjectName("chatHeader");
    chatHeader_->setAlignment(Qt::AlignCenter);
    chatHeader_->setContentsMargins(16, 12, 16, 12);
    chatLayout->addWidget(chatHeader_);

    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    bubbleContainer_ = new QWidget();
    bubbleLayout_    = new QVBoxLayout(bubbleContainer_);
    bubbleLayout_->setAlignment(Qt::AlignTop);
    bubbleLayout_->setSpacing(6);
    bubbleLayout_->setContentsMargins(16, 16, 16, 16);
    scroll->setWidget(bubbleContainer_);
    chatLayout->addWidget(scroll, 1);

    auto* inputRow = new QFrame();
    inputRow->setObjectName("inputRow");
    auto* inputLayout = new QHBoxLayout(inputRow);
    inputLayout->setContentsMargins(12, 8, 12, 8);
    messageInput_ = new QLineEdit();
    messageInput_->setPlaceholderText("Type a message…");
    messageInput_->setObjectName("inputField");
    sendBtn_ = new QPushButton("Send");
    sendBtn_->setObjectName("primaryBtn");
    inputLayout->addWidget(messageInput_);
    inputLayout->addWidget(sendBtn_);
    chatLayout->addWidget(inputRow);

    splitter->addWidget(chatPanel_);
    splitter->setStretchFactor(1, 1);
    root->addWidget(splitter, 1);

    connect(inboxList_, &QListWidget::currentRowChanged, this, &MessagesPage::onConversationSelected);
    connect(sendBtn_,   &QPushButton::clicked,           this, &MessagesPage::onSendMessage);
    connect(messageInput_, &QLineEdit::returnPressed,    this, &MessagesPage::onSendMessage);
}

void MessagesPage::refresh() {
    buildInbox();
}

void MessagesPage::buildInbox() {
    inboxList_->clear();
    User* me = Session::instance().current();
    if (!me) return;

    auto partners = MessageManager::instance().getInbox(me->getID());
    for (int pid : partners) {
        User* partner = AuthManager::instance().findUser(pid);
        if (!partner) continue;
        auto* item = new QListWidgetItem(QString::fromStdString(partner->getName()));
        item->setData(Qt::UserRole, pid);
        inboxList_->addItem(item);
    }
}

void MessagesPage::onConversationSelected(int row) {
    if (row < 0) return;
    auto* item = inboxList_->item(row);
    if (!item) return;
    currentPartnerID_ = item->data(Qt::UserRole).toInt();
    loadConversation(currentPartnerID_);
}

void MessagesPage::loadConversation(int partnerID) {
    // Clear bubbles
    QLayoutItem* i;
    while ((i = bubbleLayout_->takeAt(0)) != nullptr) {
        if (i->widget()) i->widget()->deleteLater();
        delete i;
    }

    User* me      = Session::instance().current();
    User* partner = AuthManager::instance().findUser(partnerID);
    if (!me || !partner) return;

    chatHeader_->setText(QString("Conversation with %1").arg(
        QString::fromStdString(partner->getName())));

    auto msgs = MessageManager::instance().getConversation(me->getID(), partnerID);
    for (const auto& msg : msgs) {
        bool isMine = (msg.senderID == me->getID());
        auto* bubble = new ChatBubble(QString::fromStdString(msg.content), isMine, this);
        bubbleLayout_->addWidget(bubble);
    }
}

void MessagesPage::onSendMessage() {
    if (currentPartnerID_ < 0) return;
    QString text = messageInput_->text().trimmed();
    if (text.isEmpty()) return;

    User* me = Session::instance().current();
    if (!me) return;

    MessageManager::instance().sendMessage(me->getID(), currentPartnerID_, text.toStdString());
    messageInput_->clear();
    loadConversation(currentPartnerID_);
}
