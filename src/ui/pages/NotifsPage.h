#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class NotifsPage : public QWidget {
    Q_OBJECT
public:
    explicit NotifsPage(QWidget* parent = nullptr);
    void refresh();

signals:
    void goBack();

private:
    void setupUI();

    QListWidget* notifList_;
    QPushButton* markReadBtn_;
    QPushButton* backBtn_;
};
