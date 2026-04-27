#include "SignupPage.h"
#include "../Session.h"
#include "../../managers/AuthManager.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

SignupPage::SignupPage(QWidget* parent) : QWidget(parent) { setupUI(); }

void SignupPage::setupUI() {
    auto* root = new QVBoxLayout(this); root->setAlignment(Qt::AlignCenter);
    auto* card = new QFrame(); card->setObjectName("card"); card->setFixedWidth(380);
    auto* lay  = new QVBoxLayout(card);
    lay->setSpacing(12); lay->setContentsMargins(32,32,32,32);

    auto* title = new QLabel("Create Account"); title->setObjectName("title"); title->setAlignment(Qt::AlignCenter);
    lay->addWidget(title);

    nameEdit_     = new QLineEdit(); nameEdit_->setPlaceholderText("Full Name");            nameEdit_->setObjectName("inputField");     lay->addWidget(nameEdit_);
    emailEdit_    = new QLineEdit(); emailEdit_->setPlaceholderText("Email");                emailEdit_->setObjectName("inputField");    lay->addWidget(emailEdit_);
    passwordEdit_ = new QLineEdit(); passwordEdit_->setPlaceholderText("Password (min 6)"); passwordEdit_->setEchoMode(QLineEdit::Password); passwordEdit_->setObjectName("inputField"); lay->addWidget(passwordEdit_);
    confirmEdit_  = new QLineEdit(); confirmEdit_->setPlaceholderText("Confirm Password");  confirmEdit_->setEchoMode(QLineEdit::Password);  confirmEdit_->setObjectName("inputField");  lay->addWidget(confirmEdit_);

    errorLabel_ = new QLabel(); errorLabel_->setObjectName("errorLabel"); errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setWordWrap(true); errorLabel_->setVisible(false); lay->addWidget(errorLabel_);

    auto* signupBtn = new QPushButton("Sign Up"); signupBtn->setObjectName("primaryBtn"); lay->addWidget(signupBtn);
    auto* loginBtn  = new QPushButton("Already have an account? Log in"); loginBtn->setObjectName("linkBtn"); lay->addWidget(loginBtn);

    root->addWidget(card, 0, Qt::AlignCenter);
    connect(signupBtn,   &QPushButton::clicked,      this, &SignupPage::onSignup);
    connect(loginBtn,    &QPushButton::clicked,      this, &SignupPage::goToLogin);
    connect(confirmEdit_,&QLineEdit::returnPressed,  this, &SignupPage::onSignup);
}

void SignupPage::reset() { nameEdit_->clear(); emailEdit_->clear(); passwordEdit_->clear(); confirmEdit_->clear(); errorLabel_->setVisible(false); }

void SignupPage::onSignup() {
    QString name = nameEdit_->text().trimmed(), email = emailEdit_->text().trimmed();
    QString pass = passwordEdit_->text(), confirm = confirmEdit_->text();
    if (name.isEmpty() || email.isEmpty() || pass.isEmpty()) { errorLabel_->setText("All fields are required."); errorLabel_->setVisible(true); return; }
    if (pass != confirm) { errorLabel_->setText("Passwords do not match."); errorLabel_->setVisible(true); return; }
    if (pass.length() < 6) { errorLabel_->setText("Password must be at least 6 characters."); errorLabel_->setVisible(true); return; }
    User* u = AuthManager::instance().signup(name.toStdString(), email.toStdString(), pass.toStdString());
    if (u) { Session::instance().set(u); emit signupSuccess(); }
    else   { errorLabel_->setText("Email already registered."); errorLabel_->setVisible(true); }
}
