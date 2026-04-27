#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>

class AdminPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminPage(QWidget* parent = nullptr);
    void refresh();
signals:
    void logout();
private:
    void setupUI();
    void buildUsersTab();
    void buildPostsTab();
    void onBanUser(int userID);
    void onDeletePost(int postID);
    QTabWidget*   tabs_;
    QTableWidget* usersTable_;
    QTableWidget* postsTable_;
};
