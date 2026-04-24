#include "SearchPage.h"
#include "../../managers/AuthManager.h"
#include "../../managers/SearchEngine.h"
#include "../../models/TextPost.h"
#include "../../models/ImagePost.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QListWidgetItem>

SearchPage::SearchPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void SearchPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(120, 24, 120, 24);
    root->setSpacing(16);

    auto* topRow = new QHBoxLayout();
    backBtn_ = new QPushButton("← Back");
    backBtn_->setObjectName("linkBtn");
    topRow->addWidget(backBtn_);
    topRow->addStretch();
    root->addLayout(topRow);

    connect(backBtn_, &QPushButton::clicked, this, &SearchPage::goBack);

    // Search bar
    auto* bar = new QHBoxLayout();
    searchInput_ = new QLineEdit();
    searchInput_->setPlaceholderText("Search users or posts…");
    searchInput_->setObjectName("inputField");
    searchBtn_ = new QPushButton("Search");
    searchBtn_->setObjectName("primaryBtn");
    bar->addWidget(searchInput_);
    bar->addWidget(searchBtn_);
    root->addLayout(bar);

    connect(searchBtn_,  &QPushButton::clicked,      this, &SearchPage::onSearch);
    connect(searchInput_, &QLineEdit::returnPressed, this, &SearchPage::onSearch);

    // Results tabs
    auto* tabs = new QTabWidget();

    userResults_ = new QListWidget();
    userResults_->setObjectName("resultList");
    tabs->addTab(userResults_, "Users");

    postResults_ = new QListWidget();
    postResults_->setObjectName("resultList");
    tabs->addTab(postResults_, "Posts");

    root->addWidget(tabs, 1);

    connect(userResults_, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item){
        int uid = item->data(Qt::UserRole).toInt();
        emit openProfile(uid);
    });
}

void SearchPage::clear() {
    searchInput_->clear();
    userResults_->clear();
    postResults_->clear();
}

void SearchPage::onSearch() {
    QString kw = searchInput_->text().trimmed();
    userResults_->clear();
    postResults_->clear();
    if (kw.isEmpty()) return;

    auto& users = AuthManager::instance().getUsers();

    auto foundUsers = SearchEngine::searchUsers(kw.toStdString(), users);
    for (User* u : foundUsers) {
        auto* item = new QListWidgetItem(
            QString::fromStdString(u->getName()) + " — " +
            QString::fromStdString(u->getEmail()));
        item->setData(Qt::UserRole, u->getID());
        userResults_->addItem(item);
    }
    if (foundUsers.empty())
        userResults_->addItem("No users found.");

    auto foundPosts = SearchEngine::searchPosts(kw.toStdString(), users);
    for (Post* p : foundPosts) {
        std::string preview;
        if (auto* tp = dynamic_cast<TextPost*>(p))
            preview = tp->getContent();
        else if (auto* ip = dynamic_cast<ImagePost*>(p))
            preview = "[Image] " + ip->getCaption();
        if (preview.size() > 80) preview = preview.substr(0, 80) + "…";
        auto* item = new QListWidgetItem(QString::fromStdString(preview));
        item->setData(Qt::UserRole, p->getPostID());
        postResults_->addItem(item);
    }
    if (foundPosts.empty())
        postResults_->addItem("No posts found.");
}
