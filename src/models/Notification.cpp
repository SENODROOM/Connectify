#include "Notification.h"

std::string Notification::typeToString() const {
    switch (type) {
        case NotifType::LIKE:    return "LIKE";
        case NotifType::COMMENT: return "COMMENT";
        case NotifType::REQUEST: return "REQUEST";
        case NotifType::FOLLOW:  return "FOLLOW";
        default:                 return "SYSTEM";
    }
}

NotifType Notification::typeFromString(const std::string& s) {
    if (s == "LIKE")    return NotifType::LIKE;
    if (s == "COMMENT") return NotifType::COMMENT;
    if (s == "REQUEST") return NotifType::REQUEST;
    if (s == "FOLLOW")  return NotifType::FOLLOW;
    return NotifType::SYSTEM;
}
