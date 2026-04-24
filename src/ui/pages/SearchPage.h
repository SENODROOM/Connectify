#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>

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
    QPushButton* searchBtn_;
    QPushButton* backBtn_;
    QListWidget* userResults_;
    QListWidget* postResults_;
};
