#include "MainWindow.h"
#include "Session.h"
#include "pages/LoginPage.h"
#include "pages/SignupPage.h"
#include "pages/FeedPage.h"
#include "pages/ProfilePage.h"
#include "pages/MessagesPage.h"
#include "pages/SearchPage.h"
#include "pages/NotifsPage.h"
#include "pages/AdminPage.h"
#include "../managers/AuthManager.h"
#include "../managers/MessageManager.h"
#include "../managers/NotificationManager.h"
#include "../managers/FriendGraph.h"
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Connectify");
    setMinimumSize(900, 650);

    // Load all data on startup
    AuthManager::instance().loadAll();
    MessageManager::instance().loadAll();
    NotificationManager::instance().loadAll();
    FriendGraph::instance().loadRequests();

    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    setupPages();
    showLogin();
}

MainWindow::~MainWindow() {
    AuthManager::instance().saveAll();
}

void MainWindow::setupPages() {
    loginPage_    = new LoginPage(this);
    signupPage_   = new SignupPage(this);
    feedPage_     = new FeedPage(this);
    profilePage_  = new ProfilePage(this);
    messagesPage_ = new MessagesPage(this);
    searchPage_   = new SearchPage(this);
    notifsPage_   = new NotifsPage(this);
    adminPage_    = new AdminPage(this);

    stack_->addWidget(loginPage_);
    stack_->addWidget(signupPage_);
    stack_->addWidget(feedPage_);
    stack_->addWidget(profilePage_);
    stack_->addWidget(messagesPage_);
    stack_->addWidget(searchPage_);
    stack_->addWidget(notifsPage_);
    stack_->addWidget(adminPage_);

    // Wire navigation signals
    connect(loginPage_,    &LoginPage::goToSignup,   this, &MainWindow::showSignup);
    connect(loginPage_,    &LoginPage::loginSuccess,  this, &MainWindow::showFeed);
    connect(loginPage_,    &LoginPage::adminLogin,    this, &MainWindow::showAdmin);
    connect(signupPage_,   &SignupPage::goToLogin,    this, &MainWindow::showLogin);
    connect(signupPage_,   &SignupPage::signupSuccess, this, &MainWindow::showFeed);

    connect(feedPage_,     &FeedPage::goToProfile,   this, &MainWindow::showProfile);
    connect(feedPage_,     &FeedPage::goToMessages,  this, &MainWindow::showMessages);
    connect(feedPage_,     &FeedPage::goToSearch,    this, &MainWindow::showSearch);
    connect(feedPage_,     &FeedPage::goToNotifs,    this, &MainWindow::showNotifs);
    connect(feedPage_,     &FeedPage::logout,        this, &MainWindow::logout);

    connect(profilePage_,  &ProfilePage::goBack,     this, &MainWindow::showFeed);
    connect(messagesPage_, &MessagesPage::goBack,    this, &MainWindow::showFeed);
    connect(searchPage_,   &SearchPage::goBack,      this, &MainWindow::showFeed);
    connect(notifsPage_,   &NotifsPage::goBack,      this, &MainWindow::showFeed);
    connect(adminPage_,    &AdminPage::logout,       this, &MainWindow::logout);
}

void MainWindow::showLogin() {
    Session::instance().clear();
    loginPage_->reset();
    stack_->setCurrentWidget(loginPage_);
}

void MainWindow::showSignup() {
    signupPage_->reset();
    stack_->setCurrentWidget(signupPage_);
}

void MainWindow::showFeed() {
    feedPage_->refresh();
    stack_->setCurrentWidget(feedPage_);
}

void MainWindow::showProfile(int userID) {
    profilePage_->loadUser(userID == -1
        ? Session::instance().current()->getID()
        : userID);
    stack_->setCurrentWidget(profilePage_);
}

void MainWindow::showMessages() {
    messagesPage_->refresh();
    stack_->setCurrentWidget(messagesPage_);
}

void MainWindow::showSearch() {
    searchPage_->clear();
    stack_->setCurrentWidget(searchPage_);
}

void MainWindow::showNotifs() {
    notifsPage_->refresh();
    stack_->setCurrentWidget(notifsPage_);
}

void MainWindow::showAdmin() {
    adminPage_->refresh();
    stack_->setCurrentWidget(adminPage_);
}

void MainWindow::logout() {
    AuthManager::instance().saveAll();
    showLogin();
}
