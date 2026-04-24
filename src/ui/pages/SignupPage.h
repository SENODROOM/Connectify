#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class SignupPage : public QWidget {
    Q_OBJECT
public:
    explicit SignupPage(QWidget* parent = nullptr);
    void reset();

signals:
    void signupSuccess();
    void goToLogin();

private slots:
    void onSignup();

private:
    void setupUI();

    QLineEdit*   nameEdit_;
    QLineEdit*   emailEdit_;
    QLineEdit*   passwordEdit_;
    QLineEdit*   confirmEdit_;
    QLabel*      errorLabel_;
    QPushButton* signupBtn_;
    QPushButton* loginBtn_;
};
