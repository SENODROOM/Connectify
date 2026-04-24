#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTabWidget>

class AdminPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminPage(QWidget* parent = nullptr);
    void refresh();

signals:
    void logout();

private slots:
    void onBanUser(int userID);
    void onDeletePost(int postID);

private:
    void setupUI();
    void buildUsersTab();
    void buildPostsTab();

    QTabWidget*   tabs_;
    QTableWidget* usersTable_;
    QTableWidget* postsTable_;
    QPushButton*  logoutBtn_;
};
