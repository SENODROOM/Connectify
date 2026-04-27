#include "MessagesPage.h"
#include "../Session.h"
#include "../widgets/ChatBubble.h"
#include "../../managers/AuthManager.h"
#include "../../managers/MessageManager.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QSplitter>
#include <QPushButton>

MessagesPage::MessagesPage(QWidget* parent)
    : QWidget(parent), currentPartnerID_(-1),
      inboxList_(nullptr), bubbleLayout_(nullptr), bubbleContainer_(nullptr),
      chatHeader_(nullptr), messageInput_(nullptr)
{ setupUI(); }

void MessagesPage::setupUI() {
    auto* root = new QVBoxLayout(this); root->setContentsMargins(0,0,0,0); root->setSpacing(0);

    auto* top = new QFrame(); top->setObjectName("navbar");
    auto* topL = new QHBoxLayout(top); topL->setContentsMargins(16,8,16,8);
    auto* backBtn = new QPushButton("← Back"); backBtn->setObjectName("linkBtn");
    topL->addWidget(backBtn);
    topL->addWidget(new QLabel("Messages"), 1, Qt::AlignCenter);
    topL->addStretch();
    root->addWidget(top);
    connect(backBtn, &QPushButton::clicked, this, &MessagesPage::goBack);

    auto* splitter = new QSplitter(Qt::Horizontal); splitter->setHandleWidth(1);
    inboxList_ = new QListWidget(); inboxList_->setObjectName("inboxList"); inboxList_->setFixedWidth(240);
    splitter->addWidget(inboxList_);

    auto* chatPanel = new QWidget();
    auto* chatL = new QVBoxLayout(chatPanel); chatL->setContentsMargins(0,0,0,0); chatL->setSpacing(0);
    chatHeader_ = new QLabel("Select a conversation"); chatHeader_->setObjectName("chatHeader");
    chatHeader_->setAlignment(Qt::AlignCenter); chatHeader_->setContentsMargins(16,12,16,12);
    chatL->addWidget(chatHeader_);

    auto* scroll = new QScrollArea(); scroll->setWidgetResizable(true); scroll->setFrameShape(QFrame::NoFrame);
    bubbleContainer_ = new QWidget();
    bubbleLayout_ = new QVBoxLayout(bubbleContainer_);
    bubbleLayout_->setAlignment(Qt::AlignTop); bubbleLayout_->setSpacing(6); bubbleLayout_->setContentsMargins(16,16,16,16);
    scroll->setWidget(bubbleContainer_);
    chatL->addWidget(scroll, 1);

    auto* inputRow = new QFrame(); inputRow->setObjectName("inputRow");
    auto* inputL = new QHBoxLayout(inputRow); inputL->setContentsMargins(12,8,12,8);
    messageInput_ = new QLineEdit(); messageInput_->setPlaceholderText("Type a message…"); messageInput_->setObjectName("inputField");
    auto* sendBtn = new QPushButton("Send"); sendBtn->setObjectName("primaryBtn");
    inputL->addWidget(messageInput_); inputL->addWidget(sendBtn);
    chatL->addWidget(inputRow);

    splitter->addWidget(chatPanel); splitter->setStretchFactor(1,1);
    root->addWidget(splitter, 1);

    connect(inboxList_,   &QListWidget::currentRowChanged, this, &MessagesPage::onConversationSelected);
    connect(sendBtn,      &QPushButton::clicked,           this, &MessagesPage::onSendMessage);
    connect(messageInput_,&QLineEdit::returnPressed,       this, &MessagesPage::onSendMessage);
}

void MessagesPage::refresh() { buildInbox(); }

void MessagesPage::buildInbox() {
    inboxList_->clear();
    User* me = Session::instance().current(); if (!me) return;
    int count = 0;
    int* partners = MessageManager::instance().getInboxPartners(me->getID(), count);
    for (int i = 0; i < count; ++i) {
        User* p = AuthManager::instance().findUser(partners[i]);
        if (!p) continue;
        auto* item = new QListWidgetItem(QString::fromStdString(p->getName()));
        item->setData(Qt::UserRole, partners[i]);
        inboxList_->addItem(item);
    }
    delete[] partners;
}

void MessagesPage::onConversationSelected(int row) {
    if (row < 0) return;
    auto* item = inboxList_->item(row); if (!item) return;
    currentPartnerID_ = item->data(Qt::UserRole).toInt();
    loadConversation(currentPartnerID_);
}

void MessagesPage::loadConversation(int partnerID) {
    QLayoutItem* li;
    while ((li = bubbleLayout_->takeAt(0)) != nullptr) { if (li->widget()) li->widget()->deleteLater(); delete li; }
    User* me = Session::instance().current();
    User* partner = AuthManager::instance().findUser(partnerID);
    if (!me || !partner) return;
    chatHeader_->setText(QString("Conversation with %1").arg(QString::fromStdString(partner->getName())));
    int count = 0;
    MsgNode** msgs = MessageManager::instance().getConversation(me->getID(), partnerID, count);
    for (int i = 0; i < count; ++i)
        bubbleLayout_->addWidget(new ChatBubble(QString::fromStdString(msgs[i]->content), msgs[i]->senderID == me->getID(), this));
    delete[] msgs;
}

void MessagesPage::onSendMessage() {
    if (currentPartnerID_ < 0) return;
    QString text = messageInput_->text().trimmed(); if (text.isEmpty()) return;
    User* me = Session::instance().current(); if (!me) return;
    MessageManager::instance().sendMessage(me->getID(), currentPartnerID_, text.toStdString());
    messageInput_->clear();
    loadConversation(currentPartnerID_);
}
