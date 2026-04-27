#include "LoginPage.h"
#include "../Session.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QFrame>

LoginPage::LoginPage(QWidget* parent) : QWidget(parent) { setupUI(); }

void LoginPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame(); card->setObjectName("card"); card->setFixedWidth(380);
    auto* lay  = new QVBoxLayout(card);
    lay->setSpacing(12); lay->setContentsMargins(32,32,32,32);

    auto* title = new QLabel("Welcome to Connectify");
    title->setObjectName("title"); title->setAlignment(Qt::AlignCenter);
    lay->addWidget(title);

    emailEdit_ = new QLineEdit(); emailEdit_->setPlaceholderText("Email"); emailEdit_->setObjectName("inputField");
    lay->addWidget(emailEdit_);

    passwordEdit_ = new QLineEdit(); passwordEdit_->setPlaceholderText("Password");
    passwordEdit_->setEchoMode(QLineEdit::Password); passwordEdit_->setObjectName("inputField");
    lay->addWidget(passwordEdit_);

    errorLabel_ = new QLabel(); errorLabel_->setObjectName("errorLabel");
    errorLabel_->setAlignment(Qt::AlignCenter); errorLabel_->setVisible(false);
    lay->addWidget(errorLabel_);

    auto* loginBtn = new QPushButton("Log In"); loginBtn->setObjectName("primaryBtn");
    lay->addWidget(loginBtn);
    auto* signupBtn = new QPushButton("Create an account"); signupBtn->setObjectName("linkBtn");
    lay->addWidget(signupBtn);

    root->addWidget(card, 0, Qt::AlignCenter);
    connect(loginBtn,    &QPushButton::clicked,         this, &LoginPage::onLogin);
    connect(signupBtn,   &QPushButton::clicked,         this, &LoginPage::goToSignup);
    connect(passwordEdit_,&QLineEdit::returnPressed,    this, &LoginPage::onLogin);
}

void LoginPage::reset() { emailEdit_->clear(); passwordEdit_->clear(); errorLabel_->setVisible(false); }

void LoginPage::onLogin() {
    QString email = emailEdit_->text().trimmed();
    QString pass  = passwordEdit_->text();
    if (email.isEmpty() || pass.isEmpty()) {
        errorLabel_->setText("Please fill in all fields."); errorLabel_->setVisible(true); return;
    }
    auto& auth = AuthManager::instance();
    if (auth.isAdminLogin(email.toStdString(), pass.toStdString())) {
        Session::instance().setAdmin(true); emit adminLogin(); return;
    }
    User* user = auth.login(email.toStdString(), pass.toStdString());
    if (user) { Session::instance().set(user); emit loginSuccess(); }
    else { errorLabel_->setText("Invalid credentials or account banned."); errorLabel_->setVisible(true); }
}
