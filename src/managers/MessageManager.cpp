#include "MessageManager.h"
#include "FileManager.h"

MessageManager& MessageManager::instance() {
    static MessageManager mm;
    return mm;
}

void MessageManager::loadAll() {
    FileManager::instance().loadAllMessages(messages_);
    MsgNode* cur = messages_.head();
    while (cur) {
        if (cur->msgID > lastMsgID_) lastMsgID_ = cur->msgID;
        cur = cur->next;
    }
}

void MessageManager::sendMessage(int senderID, int receiverID,
                                  const std::string& content) {
    int    id = ++lastMsgID_;
    time_t ts = time(nullptr);
    messages_.append(id, senderID, receiverID, content, ts);
    FileManager::instance().saveMessage(id, senderID, receiverID, content, ts);
}

MsgNode** MessageManager::getConversation(int userA, int userB, int& count) const {
    // Count matching messages
    count = 0;
    MsgNode* cur = messages_.head();
    while (cur) {
        if ((cur->senderID == userA && cur->receiverID == userB) ||
            (cur->senderID == userB && cur->receiverID == userA))
            ++count;
        cur = cur->next;
    }
    if (count == 0) return nullptr;

    MsgNode** arr = new MsgNode*[count];
    int idx = 0;
    cur = messages_.head();
    while (cur) {
        if ((cur->senderID == userA && cur->receiverID == userB) ||
            (cur->senderID == userB && cur->receiverID == userA))
            arr[idx++] = cur;
        cur = cur->next;
    }
    // Insertion sort by timestamp ascending (messages already roughly ordered)
    for (int i = 1; i < count; ++i) {
        MsgNode* key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j]->timestamp > key->timestamp) {
            arr[j + 1] = arr[j]; --j;
        }
        arr[j + 1] = key;
    }
    return arr;
}

int* MessageManager::getInboxPartners(int userID, int& count) const {
    // Collect unique partner IDs using a small raw int array
    int  tempCap  = 32;
    int* partners = new int[tempCap];
    int  tempSize = 0;

    MsgNode* cur = messages_.head();
    while (cur) {
        int partner = -1;
        if (cur->senderID   == userID) partner = cur->receiverID;
        if (cur->receiverID == userID) partner = cur->senderID;
        if (partner != -1) {
            // Check if already in partners
            bool found = false;
            for (int i = 0; i < tempSize; ++i)
                if (partners[i] == partner) { found = true; break; }
            if (!found) {
                if (tempSize == tempCap) {
                    // Grow
                    int  nc  = tempCap * 2;
                    int* nd  = new int[nc];
                    for (int i = 0; i < tempSize; ++i) nd[i] = partners[i];
                    delete[] partners;
                    partners = nd; tempCap = nc;
                }
                partners[tempSize++] = partner;
            }
        }
        cur = cur->next;
    }
    count = tempSize;
    return partners; // caller must delete[]
}
