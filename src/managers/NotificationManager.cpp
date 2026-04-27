#include "NotificationManager.h"
#include "FileManager.h"

NotificationManager& NotificationManager::instance() {
    static NotificationManager nm;
    return nm;
}

void NotificationManager::loadAll() {
    FileManager::instance().loadAllNotifications(notifs_);
    NotifNode* cur = notifs_.head();
    while (cur) {
        if (cur->notifID > lastID_) lastID_ = cur->notifID;
        cur = cur->next;
    }
}

void NotificationManager::notify(int ownerID, NotifType type,
                                  const std::string& message) {
    NotifNode* node = new NotifNode(++lastID_, ownerID, type, message, time(nullptr));
    notifs_.append(node);
    FileManager::instance().appendNotification(node);
}

void NotificationManager::markAllRead(int userID) {
    NotifNode* cur = notifs_.head();
    while (cur) {
        if (cur->ownerID == userID) cur->isRead = true;
        cur = cur->next;
    }
    FileManager::instance().saveAllNotifications(notifs_);
}

NotifNode** NotificationManager::getAll(int userID, int& count) const {
    count = 0;
    NotifNode* cur = notifs_.head();
    while (cur) { if (cur->ownerID == userID) ++count; cur = cur->next; }
    if (count == 0) return nullptr;

    NotifNode** arr = new NotifNode*[count];
    int idx = 0;
    cur = notifs_.head();
    while (cur) {
        if (cur->ownerID == userID) arr[idx++] = cur;
        cur = cur->next;
    }
    return arr;
}

NotifNode** NotificationManager::getUnread(int userID, int& count) const {
    count = 0;
    NotifNode* cur = notifs_.head();
    while (cur) {
        if (cur->ownerID == userID && !cur->isRead) ++count;
        cur = cur->next;
    }
    if (count == 0) return nullptr;

    NotifNode** arr = new NotifNode*[count];
    int idx = 0;
    cur = notifs_.head();
    while (cur) {
        if (cur->ownerID == userID && !cur->isRead) arr[idx++] = cur;
        cur = cur->next;
    }
    return arr;
}
