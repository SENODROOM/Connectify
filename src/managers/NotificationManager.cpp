#include "NotificationManager.h"
#include "FileManager.h"
#include <algorithm>

NotificationManager& NotificationManager::instance() {
    static NotificationManager nm;
    return nm;
}

void NotificationManager::loadAll() {
    notifs_ = FileManager::instance().loadAllNotifications();
    for (const auto& n : notifs_)
        if (n.notifID > lastID_) lastID_ = n.notifID;
}

void NotificationManager::notify(int ownerID, NotifType type, const std::string& message) {
    Notification n{ ++lastID_, ownerID, type, message, false, time(nullptr) };
    notifs_.push_back(n);
    FileManager::instance().saveNotification(n);
}

std::vector<Notification> NotificationManager::getUnread(int userID) const {
    std::vector<Notification> out;
    for (const auto& n : notifs_)
        if (n.ownerID == userID && !n.isRead) out.push_back(n);
    return out;
}

std::vector<Notification> NotificationManager::getAll(int userID) const {
    std::vector<Notification> out;
    for (const auto& n : notifs_)
        if (n.ownerID == userID) out.push_back(n);
    return out;
}

void NotificationManager::markAllRead(int userID) {
    for (auto& n : notifs_)
        if (n.ownerID == userID) n.isRead = true;
    FileManager::instance().saveAllNotifications(notifs_);
}
