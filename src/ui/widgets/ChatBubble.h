#pragma once
#include <QWidget>
#include <QLabel>

class ChatBubble : public QWidget {
    Q_OBJECT
public:
    ChatBubble(const QString& text, bool isMine, QWidget* parent = nullptr);

private:
    void setupUI(const QString& text, bool isMine);
};
