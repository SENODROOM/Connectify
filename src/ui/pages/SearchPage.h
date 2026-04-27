#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>

class SearchPage : public QWidget {
    Q_OBJECT
public:
    explicit SearchPage(QWidget* parent = nullptr);
    void clear();
signals:
    void goBack();
    void openProfile(int userID);
private slots:
    void onSearch();
private:
    void setupUI();
    QLineEdit*   searchInput_;
    QListWidget* userResults_;
    QListWidget* postResults_;
};
