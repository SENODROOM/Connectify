#pragma once
#include "../models/Message.h"
#include <vector>
#include <string>
#include <set>

class MessageManager {
public:
    static MessageManager& instance();

    void sendMessage(int senderID, int receiverID, const std::string& content);
    std::vector<Message> getConversation(int userA, int userB) const;
    std::vector<int>     getInbox(int userID) const;  // unique conversation partner IDs

    void loadAll();

private:
    MessageManager() = default;
    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;

    std::vector<Message> messages_;
    int lastMsgID_ = 3000;
};
