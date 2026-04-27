#pragma once
#include <QWidget>
#include <QListWidget>

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
};
