#include "NotifsPage.h"
#include "../Session.h"
#include "../../managers/NotificationManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QListWidgetItem>
#include <QDateTime>

NotifsPage::NotifsPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void NotifsPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(120, 24, 120, 24);
    root->setSpacing(16);

    auto* topRow = new QHBoxLayout();
    backBtn_ = new QPushButton("← Back");
    backBtn_->setObjectName("linkBtn");
    topRow->addWidget(backBtn_);
    topRow->addWidget(new QLabel("Notifications"), 1, Qt::AlignCenter);
    markReadBtn_ = new QPushButton("Mark all read");
    markReadBtn_->setObjectName("secondaryBtn");
    topRow->addWidget(markReadBtn_);
    root->addLayout(topRow);

    notifList_ = new QListWidget();
    notifList_->setObjectName("notifList");
    root->addWidget(notifList_, 1);

    connect(backBtn_,     &QPushButton::clicked, this, &NotifsPage::goBack);
    connect(markReadBtn_, &QPushButton::clicked, this, [this]() {
        User* me = Session::instance().current();
        if (me) {
            NotificationManager::instance().markAllRead(me->getID());
            refresh();
        }
    });
}

void NotifsPage::refresh() {
    notifList_->clear();
    User* me = Session::instance().current();
    if (!me) return;

    auto notifs = NotificationManager::instance().getAll(me->getID());
    if (notifs.empty()) {
        notifList_->addItem("No notifications yet.");
        return;
    }

    // Newest first
    for (int i = static_cast<int>(notifs.size()) - 1; i >= 0; --i) {
        const auto& n = notifs[i];
        QString time = QDateTime::fromSecsSinceEpoch(n.timestamp).toString("MMM d, hh:mm");
        auto* item = new QListWidgetItem(
            QString("[%1]  %2  —  %3")
                .arg(QString::fromStdString(n.typeToString()))
                .arg(QString::fromStdString(n.message))
                .arg(time));
        if (!n.isRead) {
            QFont f = item->font();
            f.setBold(true);
            item->setFont(f);
        }
        notifList_->addItem(item);
    }
}
