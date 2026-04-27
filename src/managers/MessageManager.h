#pragma once
#include "../models/Message.h"
#include <string>

class MessageManager {
public:
    static MessageManager& instance();

    void sendMessage(int senderID, int receiverID, const std::string& content);

    // Returns heap-allocated array of MsgNode* for the conversation.
    // Caller must delete[] the array (NOT the nodes themselves).
    MsgNode** getConversation(int userA, int userB, int& count) const;

    // Returns heap-allocated int array of unique partner IDs.
    // Caller must delete[] the array.
    int* getInboxPartners(int userID, int& count) const;

    void loadAll();

private:
    MessageManager() : lastMsgID_(3000) {}
    MessageManager(const MessageManager&)            = delete;
    MessageManager& operator=(const MessageManager&) = delete;

    MessageList messages_;
    int         lastMsgID_;
};
