#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class FeedPage : public QWidget {
    Q_OBJECT
public:
    explicit FeedPage(QWidget* parent = nullptr);
    void refresh();

signals:
    void goToProfile(int userID);
    void goToMessages();
    void goToSearch();
    void goToNotifs();
    void logout();

private slots:
    void onCreatePost();

private:
    void setupUI();
    void buildFeed();
    void clearFeed();

    QWidget*     feedContainer_;
    QVBoxLayout* feedLayout_;
    QLineEdit*   postInput_;
    QPushButton* postBtn_;
    QLabel*      emptyLabel_;
};
