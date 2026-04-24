#include "AdminPage.h"
#include "../../managers/AuthManager.h"
#include "../../managers/FileManager.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>

AdminPage::AdminPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void AdminPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(16);

    auto* topRow = new QHBoxLayout();
    auto* title  = new QLabel("Admin Command Center");
    title->setObjectName("title");
    topRow->addWidget(title);
    topRow->addStretch();
    logoutBtn_ = new QPushButton("Logout");
    logoutBtn_->setObjectName("dangerBtn");
    topRow->addWidget(logoutBtn_);
    root->addLayout(topRow);

    connect(logoutBtn_, &QPushButton::clicked, this, &AdminPage::logout);

    tabs_ = new QTabWidget();

    usersTable_ = new QTableWidget();
    usersTable_->setColumnCount(4);
    usersTable_->setHorizontalHeaderLabels({"ID", "Name", "Email", "Action"});
    usersTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    usersTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usersTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabs_->addTab(usersTable_, "Users");

    postsTable_ = new QTableWidget();
    postsTable_->setColumnCount(4);
    postsTable_->setHorizontalHeaderLabels({"ID", "Owner ID", "Content Preview", "Action"});
    postsTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    postsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    postsTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabs_->addTab(postsTable_, "Posts");

    root->addWidget(tabs_, 1);
}

void AdminPage::refresh() {
    buildUsersTab();
    buildPostsTab();
}

void AdminPage::buildUsersTab() {
    usersTable_->setRowCount(0);
    const auto& users = AuthManager::instance().getUsers();
    for (User* u : users) {
        int row = usersTable_->rowCount();
        usersTable_->insertRow(row);
        usersTable_->setItem(row, 0, new QTableWidgetItem(QString::number(u->getID())));
        usersTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(u->getName())));
        usersTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(u->getEmail())));

        auto* btn = new QPushButton(u->isBanned() ? "Unban" : "Ban");
        btn->setObjectName(u->isBanned() ? "secondaryBtn" : "dangerBtn");
        int uid = u->getID();
        connect(btn, &QPushButton::clicked, this, [this, uid]() { onBanUser(uid); });
        usersTable_->setCellWidget(row, 3, btn);
    }
}

void AdminPage::buildPostsTab() {
    postsTable_->setRowCount(0);
    const auto& users = AuthManager::instance().getUsers();
    for (User* u : users) {
        for (Post* p : u->getPosts()) {
            int row = postsTable_->rowCount();
            postsTable_->insertRow(row);
            postsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(p->getPostID())));
            postsTable_->setItem(row, 1, new QTableWidgetItem(QString::number(p->getOwnerID())));

            std::string preview;
            if (auto* tp = dynamic_cast<TextPost*>(p))
                preview = tp->getContent();
            else if (auto* ip = dynamic_cast<ImagePost*>(p))
                preview = "[Image] " + ip->getCaption();
            if (preview.size() > 60) preview = preview.substr(0, 60) + "…";
            postsTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(preview)));

            auto* btn = new QPushButton("Delete");
            btn->setObjectName("dangerBtn");
            int pid = p->getPostID();
            connect(btn, &QPushButton::clicked, this, [this, pid]() { onDeletePost(pid); });
            postsTable_->setCellWidget(row, 3, btn);
        }
    }
}

void AdminPage::onBanUser(int userID) {
    User* u = AuthManager::instance().findUser(userID);
    if (!u) return;
    auto reply = QMessageBox::question(this,
        u->isBanned() ? "Unban User" : "Ban User",
        QString("Are you sure you want to %1 %2?")
            .arg(u->isBanned() ? "unban" : "ban")
            .arg(QString::fromStdString(u->getName())));
    if (reply != QMessageBox::Yes) return;

    if (u->isBanned()) AuthManager::instance().unbanUser(userID);
    else               AuthManager::instance().banUser(userID);
    refresh();
}

void AdminPage::onDeletePost(int postID) {
    auto reply = QMessageBox::question(this, "Delete Post",
        QString("Delete post #%1?").arg(postID));
    if (reply != QMessageBox::Yes) return;

    auto& users = const_cast<std::vector<User*>&>(AuthManager::instance().getUsers());
    for (User* u : users) {
        if (u->getPost(postID)) {
            u->removePost(postID);
            FileManager::instance().saveAllPosts(users);
            refresh();
            return;
        }
    }
}
