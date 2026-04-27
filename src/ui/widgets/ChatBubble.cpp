#include "ChatBubble.h"
#include <QHBoxLayout>
#include <QLabel>

ChatBubble::ChatBubble(const QString& text, bool isMine, QWidget* parent)
    : QWidget(parent)
{
    auto* row = new QHBoxLayout(this); row->setContentsMargins(0,2,0,2);
    auto* lbl = new QLabel(text); lbl->setWordWrap(true); lbl->setMaximumWidth(400);
    lbl->setObjectName(isMine ? "bubbleMine" : "bubbleTheirs");
    if (isMine) { row->addStretch(); row->addWidget(lbl); }
    else        { row->addWidget(lbl); row->addStretch(); }
}
