#pragma once
#include <QMainWindow>
#include <QStackedWidget>

class LoginPage;
class SignupPage;
class FeedPage;
class ProfilePage;
class MessagesPage;
class SearchPage;
class NotifsPage;
class AdminPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void showLogin();
    void showSignup();
    void showFeed();
    void showProfile(int userID = -1);
    void showMessages();
    void showSearch();
    void showNotifs();
    void showAdmin();
    void logout();

private:
    void setupPages();

    QStackedWidget* stack_;
    LoginPage*      loginPage_;
    SignupPage*     signupPage_;
    FeedPage*       feedPage_;
    ProfilePage*    profilePage_;
    MessagesPage*   messagesPage_;
    SearchPage*     searchPage_;
    NotifsPage*     notifsPage_;
    AdminPage*      adminPage_;
};
