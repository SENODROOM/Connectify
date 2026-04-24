#pragma once
#include <string>
#include <ctime>

enum class NotifType { LIKE, COMMENT, REQUEST, FOLLOW, SYSTEM };

struct Notification {
    int        notifID;
    int        ownerID;
    NotifType  type;
    std::string message;
    bool       isRead;
    time_t     timestamp;

    std::string typeToString() const;
    static NotifType typeFromString(const std::string& s);
};
