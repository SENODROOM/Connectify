#pragma once
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>

class MessagesPage : public QWidget {
    Q_OBJECT
public:
    explicit MessagesPage(QWidget* parent = nullptr);
    void refresh();
signals:
    void goBack();
private slots:
    void onConversationSelected(int row);
    void onSendMessage();
private:
    void setupUI();
    void loadConversation(int partnerID);
    void buildInbox();
    int          currentPartnerID_;
    QListWidget* inboxList_;
    QVBoxLayout* bubbleLayout_;
    QWidget*     bubbleContainer_;
    QLabel*      chatHeader_;
    QLineEdit*   messageInput_;
};
