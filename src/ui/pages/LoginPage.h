#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class LoginPage : public QWidget {
    Q_OBJECT
public:
    explicit LoginPage(QWidget* parent = nullptr);
    void reset();

signals:
    void loginSuccess();
    void adminLogin();
    void goToSignup();

private slots:
    void onLogin();

private:
    void setupUI();

    QLineEdit*  emailEdit_;
    QLineEdit*  passwordEdit_;
    QLabel*     errorLabel_;
    QPushButton* loginBtn_;
    QPushButton* signupBtn_;
};
