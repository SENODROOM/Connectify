#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    explicit ProfilePage(QWidget* parent = nullptr);
    void loadUser(int userID);
signals:
    void goBack();
private:
    void setupUI();
    void buildPosts();
    int          viewedUserID_;
    QLabel*      nameLabel_;
    QLabel*      emailLabel_;
    QLabel*      statsLabel_;
    QPushButton* followBtn_;
    QVBoxLayout* postsLayout_;
    QWidget*     postsContainer_;
};
