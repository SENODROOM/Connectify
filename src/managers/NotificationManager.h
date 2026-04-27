#pragma once
#include "../models/Notification.h"
#include <string>

class NotificationManager {
public:
    static NotificationManager& instance();

    void notify(int ownerID, NotifType type, const std::string& message);
    void markAllRead(int userID);

    // Returns heap array of NotifNode* for a user — caller must delete[]
    NotifNode** getAll(int userID, int& count) const;
    NotifNode** getUnread(int userID, int& count) const;

    void loadAll();

private:
    NotificationManager() : lastID_(4000) {}
    NotificationManager(const NotificationManager&)            = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;

    NotifList notifs_;
    int       lastID_;
};
