#include "MessageManager.h"
#include "FileManager.h"
#include <algorithm>

MessageManager& MessageManager::instance() {
    static MessageManager mm;
    return mm;
}

void MessageManager::loadAll() {
    messages_ = FileManager::instance().loadAllMessages();
    for (const auto& m : messages_)
        if (m.msgID > lastMsgID_) lastMsgID_ = m.msgID;
}

void MessageManager::sendMessage(int senderID, int receiverID, const std::string& content) {
    Message msg{ ++lastMsgID_, senderID, receiverID, content, time(nullptr) };
    messages_.push_back(msg);
    FileManager::instance().saveMessage(msg);
}

std::vector<Message> MessageManager::getConversation(int userA, int userB) const {
    std::vector<Message> conv;
    for (const auto& m : messages_) {
        if ((m.senderID == userA && m.receiverID == userB) ||
            (m.senderID == userB && m.receiverID == userA))
            conv.push_back(m);
    }
    std::sort(conv.begin(), conv.end(), [](const Message& a, const Message& b){
        return a.timestamp < b.timestamp;
    });
    return conv;
}

std::vector<int> MessageManager::getInbox(int userID) const {
    std::set<int> partners;
    for (const auto& m : messages_) {
        if (m.senderID   == userID) partners.insert(m.receiverID);
        if (m.receiverID == userID) partners.insert(m.senderID);
    }
    return std::vector<int>(partners.begin(), partners.end());
}
