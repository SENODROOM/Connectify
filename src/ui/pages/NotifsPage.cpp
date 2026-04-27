#include "NotifsPage.h"
#include "../Session.h"
#include "../../managers/NotificationManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QFont>

NotifsPage::NotifsPage(QWidget* parent) : QWidget(parent), notifList_(nullptr) { setupUI(); }

void NotifsPage::setupUI() {
    auto* root = new QVBoxLayout(this); root->setContentsMargins(120,24,120,24); root->setSpacing(16);

    auto* topRow = new QHBoxLayout();
    auto* backBtn = new QPushButton("← Back"); backBtn->setObjectName("linkBtn");
    topRow->addWidget(backBtn);
    topRow->addWidget(new QLabel("Notifications"), 1, Qt::AlignCenter);
    auto* markBtn = new QPushButton("Mark all read"); markBtn->setObjectName("secondaryBtn");
    topRow->addWidget(markBtn);
    root->addLayout(topRow);

    notifList_ = new QListWidget(); notifList_->setObjectName("notifList");
    root->addWidget(notifList_, 1);

    connect(backBtn, &QPushButton::clicked, this, &NotifsPage::goBack);
    connect(markBtn, &QPushButton::clicked, this, [this](){
        User* me = Session::instance().current();
        if (me) { NotificationManager::instance().markAllRead(me->getID()); refresh(); }
    });
}

void NotifsPage::refresh() {
    notifList_->clear();
    User* me = Session::instance().current(); if (!me) return;
    int count = 0;
    NotifNode** arr = NotificationManager::instance().getAll(me->getID(), count);
    if (count == 0) { notifList_->addItem("No notifications yet."); delete[] arr; return; }
    // Show newest first — walk backwards using index
    for (int i = count - 1; i >= 0; --i) {
        NotifNode* n = arr[i];
        QString time = QDateTime::fromSecsSinceEpoch(n->timestamp).toString("MMM d, hh:mm");
        auto* item = new QListWidgetItem(
            QString("[%1]  %2  —  %3")
                .arg(QString::fromStdString(n->typeString()))
                .arg(QString::fromStdString(n->message))
                .arg(time));
        if (!n->isRead) { QFont f = item->font(); f.setBold(true); item->setFont(f); }
        notifList_->addItem(item);
    }
    delete[] arr;
}
