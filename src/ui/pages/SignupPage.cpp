#include "SignupPage.h"
#include "../Session.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QFrame>

SignupPage::SignupPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void SignupPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame();
    card->setObjectName("card");
    card->setFixedWidth(380);
    auto* form = new QVBoxLayout(card);
    form->setSpacing(12);
    form->setContentsMargins(32, 32, 32, 32);

    auto* title = new QLabel("Create Account");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);
    form->addWidget(title);

    nameEdit_ = new QLineEdit();
    nameEdit_->setPlaceholderText("Full Name");
    nameEdit_->setObjectName("inputField");
    form->addWidget(nameEdit_);

    emailEdit_ = new QLineEdit();
    emailEdit_->setPlaceholderText("Email");
    emailEdit_->setObjectName("inputField");
    form->addWidget(emailEdit_);

    passwordEdit_ = new QLineEdit();
    passwordEdit_->setPlaceholderText("Password (min 6 characters)");
    passwordEdit_->setEchoMode(QLineEdit::Password);
    passwordEdit_->setObjectName("inputField");
    form->addWidget(passwordEdit_);

    confirmEdit_ = new QLineEdit();
    confirmEdit_->setPlaceholderText("Confirm Password");
    confirmEdit_->setEchoMode(QLineEdit::Password);
    confirmEdit_->setObjectName("inputField");
    form->addWidget(confirmEdit_);

    errorLabel_ = new QLabel();
    errorLabel_->setObjectName("errorLabel");
    errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setWordWrap(true);
    errorLabel_->setVisible(false);
    form->addWidget(errorLabel_);

    signupBtn_ = new QPushButton("Sign Up");
    signupBtn_->setObjectName("primaryBtn");
    form->addWidget(signupBtn_);

    loginBtn_ = new QPushButton("Already have an account? Log in");
    loginBtn_->setObjectName("linkBtn");
    form->addWidget(loginBtn_);

    root->addWidget(card, 0, Qt::AlignCenter);

    connect(signupBtn_, &QPushButton::clicked, this, &SignupPage::onSignup);
    connect(loginBtn_,  &QPushButton::clicked, this, &SignupPage::goToLogin);
    connect(confirmEdit_, &QLineEdit::returnPressed, this, &SignupPage::onSignup);
}

void SignupPage::reset() {
    nameEdit_->clear();
    emailEdit_->clear();
    passwordEdit_->clear();
    confirmEdit_->clear();
    errorLabel_->setVisible(false);
}

void SignupPage::onSignup() {
    QString name     = nameEdit_->text().trimmed();
    QString email    = emailEdit_->text().trimmed();
    QString password = passwordEdit_->text();
    QString confirm  = confirmEdit_->text();

    if (name.isEmpty() || email.isEmpty() || password.isEmpty()) {
        errorLabel_->setText("All fields are required.");
        errorLabel_->setVisible(true);
        return;
    }
    if (password != confirm) {
        errorLabel_->setText("Passwords do not match.");
        errorLabel_->setVisible(true);
        return;
    }
    if (password.length() < 6) {
        errorLabel_->setText("Password must be at least 6 characters.");
        errorLabel_->setVisible(true);
        return;
    }

    User* user = AuthManager::instance().signup(
        name.toStdString(), email.toStdString(), password.toStdString());

    if (user) {
        Session::instance().set(user);
        emit signupSuccess();
    } else {
        errorLabel_->setText("Email already registered. Try logging in.");
        errorLabel_->setVisible(true);
    }
}
