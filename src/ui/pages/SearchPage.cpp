#include "SearchPage.h"
#include "../../managers/AuthManager.h"
#include "../../managers/SearchEngine.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QListWidgetItem>

SearchPage::SearchPage(QWidget* parent)
    : QWidget(parent), searchInput_(nullptr), userResults_(nullptr), postResults_(nullptr)
{ setupUI(); }

void SearchPage::setupUI() {
    auto* root = new QVBoxLayout(this); root->setContentsMargins(120,24,120,24); root->setSpacing(16);
    auto* topRow = new QHBoxLayout();
    auto* backBtn = new QPushButton("← Back"); backBtn->setObjectName("linkBtn");
    topRow->addWidget(backBtn); topRow->addStretch();
    root->addLayout(topRow);
    connect(backBtn, &QPushButton::clicked, this, &SearchPage::goBack);

    auto* bar = new QHBoxLayout();
    searchInput_ = new QLineEdit(); searchInput_->setPlaceholderText("Search users or posts…"); searchInput_->setObjectName("inputField");
    auto* searchBtn = new QPushButton("Search"); searchBtn->setObjectName("primaryBtn");
    bar->addWidget(searchInput_); bar->addWidget(searchBtn);
    root->addLayout(bar);
    connect(searchBtn,   &QPushButton::clicked,      this, &SearchPage::onSearch);
    connect(searchInput_,&QLineEdit::returnPressed,  this, &SearchPage::onSearch);

    auto* tabs = new QTabWidget();
    userResults_ = new QListWidget(); userResults_->setObjectName("resultList"); tabs->addTab(userResults_, "Users");
    postResults_ = new QListWidget(); postResults_->setObjectName("resultList"); tabs->addTab(postResults_, "Posts");
    root->addWidget(tabs, 1);

    connect(userResults_, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item){
        emit openProfile(item->data(Qt::UserRole).toInt());
    });
}

void SearchPage::clear() { if (searchInput_) searchInput_->clear(); if (userResults_) userResults_->clear(); if (postResults_) postResults_->clear(); }

void SearchPage::onSearch() {
    QString kw = searchInput_->text().trimmed();
    userResults_->clear(); postResults_->clear();
    if (kw.isEmpty()) return;

    const UserTable& users = AuthManager::instance().getUsers();

    int     uCount = 0;
    User**  uArr   = SearchEngine::searchUsers(kw.toStdString(), users, uCount);
    for (int i = 0; i < uCount; ++i) {
        auto* item = new QListWidgetItem(
            QString::fromStdString(uArr[i]->getName()) + " — " +
            QString::fromStdString(uArr[i]->getEmail()));
        item->setData(Qt::UserRole, uArr[i]->getID());
        userResults_->addItem(item);
    }
    delete[] uArr;
    if (uCount == 0) userResults_->addItem("No users found.");

    int    pCount = 0;
    Post** pArr   = SearchEngine::searchPosts(kw.toStdString(), users, pCount);
    for (int i = 0; i < pCount; ++i) {
        std::string preview;
        if (auto* tp = dynamic_cast<TextPost*>(pArr[i]))  preview = tp->getContent();
        else if (auto* ip = dynamic_cast<ImagePost*>(pArr[i])) preview = "[Image] " + ip->getCaption();
        if (preview.size() > 80) preview = preview.substr(0, 80) + "…";
        auto* item = new QListWidgetItem(QString::fromStdString(preview));
        item->setData(Qt::UserRole, pArr[i]->getPostID());
        postResults_->addItem(item);
    }
    delete[] pArr;
    if (pCount == 0) postResults_->addItem("No posts found.");
}
