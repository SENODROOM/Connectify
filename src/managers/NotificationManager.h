#pragma once
#include "../models/Notification.h"
#include <vector>
#include <string>

class NotificationManager {
public:
    static NotificationManager& instance();

    void notify(int ownerID, NotifType type, const std::string& message);
    std::vector<Notification> getUnread(int userID) const;
    std::vector<Notification> getAll(int userID) const;
    void markAllRead(int userID);

    void loadAll();

private:
    NotificationManager() = default;
    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;

    std::vector<Notification> notifs_;
    int lastID_ = 4000;
};
