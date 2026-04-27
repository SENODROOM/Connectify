#pragma once
#include <QWidget>

class ChatBubble : public QWidget {
    Q_OBJECT
public:
    ChatBubble(const QString& text, bool isMine, QWidget* parent = nullptr);
};
