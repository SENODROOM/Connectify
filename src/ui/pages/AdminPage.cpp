#include "AdminPage.h"
#include "../../managers/AuthManager.h"
#include "../../managers/FileManager.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidgetItem>

AdminPage::AdminPage(QWidget* parent)
    : QWidget(parent), tabs_(nullptr), usersTable_(nullptr), postsTable_(nullptr)
{ setupUI(); }

void AdminPage::setupUI() {
    auto* root = new QVBoxLayout(this); root->setContentsMargins(24,24,24,24); root->setSpacing(16);
    auto* topRow = new QHBoxLayout();
    auto* title = new QLabel("Admin Command Center"); title->setObjectName("title");
    topRow->addWidget(title); topRow->addStretch();
    auto* logoutBtn = new QPushButton("Logout"); logoutBtn->setObjectName("dangerBtn");
    topRow->addWidget(logoutBtn);
    root->addLayout(topRow);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminPage::logout);

    tabs_ = new QTabWidget();
    usersTable_ = new QTableWidget();
    usersTable_->setColumnCount(4);
    usersTable_->setHorizontalHeaderLabels({"ID","Name","Email","Action"});
    usersTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    usersTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabs_->addTab(usersTable_, "Users");

    postsTable_ = new QTableWidget();
    postsTable_->setColumnCount(4);
    postsTable_->setHorizontalHeaderLabels({"ID","Owner","Content Preview","Action"});
    postsTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    postsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabs_->addTab(postsTable_, "Posts");

    root->addWidget(tabs_, 1);
}

void AdminPage::refresh() { buildUsersTab(); buildPostsTab(); }

void AdminPage::buildUsersTab() {
    usersTable_->setRowCount(0);
    const UserTable& users = AuthManager::instance().getUsers();
    for (int i = 0; i < users.size(); ++i) {
        User* u = users[i];
        int row = usersTable_->rowCount(); usersTable_->insertRow(row);
        usersTable_->setItem(row, 0, new QTableWidgetItem(QString::number(u->getID())));
        usersTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(u->getName())));
        usersTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(u->getEmail())));
        auto* btn = new QPushButton(u->isBanned() ? "Unban" : "Ban");
        btn->setObjectName(u->isBanned() ? "secondaryBtn" : "dangerBtn");
        int uid = u->getID();
        connect(btn, &QPushButton::clicked, this, [this, uid]{ onBanUser(uid); });
        usersTable_->setCellWidget(row, 3, btn);
    }
}

void AdminPage::buildPostsTab() {
    postsTable_->setRowCount(0);
    const UserTable& users = AuthManager::instance().getUsers();
    for (int i = 0; i < users.size(); ++i) {
        PostNode* node = users[i]->getPosts().head();
        while (node) {
            Post* p = node->post;
            int row = postsTable_->rowCount(); postsTable_->insertRow(row);
            postsTable_->setItem(row, 0, new QTableWidgetItem(QString::number(p->getPostID())));
            postsTable_->setItem(row, 1, new QTableWidgetItem(QString::number(p->getOwnerID())));
            std::string preview;
            if (auto* tp = dynamic_cast<TextPost*>(p))  preview = tp->getContent();
            else if (auto* ip = dynamic_cast<ImagePost*>(p)) preview = "[Image] " + ip->getCaption();
            if (preview.size() > 60) preview = preview.substr(0,60) + "…";
            postsTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(preview)));
            auto* btn = new QPushButton("Delete"); btn->setObjectName("dangerBtn");
            int pid = p->getPostID();
            connect(btn, &QPushButton::clicked, this, [this, pid]{ onDeletePost(pid); });
            postsTable_->setCellWidget(row, 3, btn);
            node = node->next;
        }
    }
}

void AdminPage::onBanUser(int userID) {
    User* u = AuthManager::instance().findUser(userID); if (!u) return;
    auto reply = QMessageBox::question(this, u->isBanned() ? "Unban" : "Ban",
        QString("Are you sure you want to %1 %2?")
            .arg(u->isBanned() ? "unban" : "ban")
            .arg(QString::fromStdString(u->getName())));
    if (reply != QMessageBox::Yes) return;
    if (u->isBanned()) AuthManager::instance().getAdmin()->unbanUser(u);
    else               AuthManager::instance().getAdmin()->banUser(u);
    AuthManager::instance().saveAll();
    refresh();
}

void AdminPage::onDeletePost(int postID) {
    auto reply = QMessageBox::question(this, "Delete Post", QString("Delete post #%1?").arg(postID));
    if (reply != QMessageBox::Yes) return;
    AuthManager::instance().getAdmin()->deletePost(postID, AuthManager::instance().getUsers());
    FileManager::instance().saveAllPosts(AuthManager::instance().getUsers());
    refresh();
}
