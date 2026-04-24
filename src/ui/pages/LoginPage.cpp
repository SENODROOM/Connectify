#include "LoginPage.h"
#include "../Session.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>

LoginPage::LoginPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void LoginPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame();
    card->setObjectName("card");
    card->setFixedWidth(380);
    auto* form = new QVBoxLayout(card);
    form->setSpacing(12);
    form->setContentsMargins(32, 32, 32, 32);

    auto* title = new QLabel("Welcome to Connectify");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);
    form->addWidget(title);

    emailEdit_ = new QLineEdit();
    emailEdit_->setPlaceholderText("Email");
    emailEdit_->setObjectName("inputField");
    form->addWidget(emailEdit_);

    passwordEdit_ = new QLineEdit();
    passwordEdit_->setPlaceholderText("Password");
    passwordEdit_->setEchoMode(QLineEdit::Password);
    passwordEdit_->setObjectName("inputField");
    form->addWidget(passwordEdit_);

    errorLabel_ = new QLabel();
    errorLabel_->setObjectName("errorLabel");
    errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setVisible(false);
    form->addWidget(errorLabel_);

    loginBtn_ = new QPushButton("Log In");
    loginBtn_->setObjectName("primaryBtn");
    form->addWidget(loginBtn_);

    signupBtn_ = new QPushButton("Create an account");
    signupBtn_->setObjectName("linkBtn");
    form->addWidget(signupBtn_);

    root->addWidget(card, 0, Qt::AlignCenter);

    connect(loginBtn_,  &QPushButton::clicked, this, &LoginPage::onLogin);
    connect(signupBtn_, &QPushButton::clicked, this, &LoginPage::goToSignup);
    connect(passwordEdit_, &QLineEdit::returnPressed, this, &LoginPage::onLogin);
}

void LoginPage::reset() {
    emailEdit_->clear();
    passwordEdit_->clear();
    errorLabel_->setVisible(false);
}

void LoginPage::onLogin() {
    QString email    = emailEdit_->text().trimmed();
    QString password = passwordEdit_->text();

    if (email.isEmpty() || password.isEmpty()) {
        errorLabel_->setText("Please fill in all fields.");
        errorLabel_->setVisible(true);
        return;
    }

    auto& auth = AuthManager::instance();

    // Check admin first
    if (auth.isAdminLogin(email.toStdString(), password.toStdString())) {
        Session::instance().setAdmin(true);
        emit adminLogin();
        return;
    }

    User* user = auth.login(email.toStdString(), password.toStdString());
    if (user) {
        Session::instance().set(user);
        emit loginSuccess();
    } else {
        errorLabel_->setText("Invalid credentials or account banned.");
        errorLabel_->setVisible(true);
    }
}
