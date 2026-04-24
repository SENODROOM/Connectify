#include "ChatBubble.h"
#include <QHBoxLayout>

ChatBubble::ChatBubble(const QString& text, bool isMine, QWidget* parent)
    : QWidget(parent)
{
    setupUI(text, isMine);
}

void ChatBubble::setupUI(const QString& text, bool isMine) {
    auto* row = new QHBoxLayout(this);
    row->setContentsMargins(0, 2, 0, 2);

    auto* label = new QLabel(text);
    label->setWordWrap(true);
    label->setMaximumWidth(400);
    label->setObjectName(isMine ? "bubbleMine" : "bubbleTheirs");

    if (isMine) {
        row->addStretch();
        row->addWidget(label);
    } else {
        row->addWidget(label);
        row->addStretch();
    }
}
