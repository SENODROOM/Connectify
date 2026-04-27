#pragma once
#include "../models/UserTable.h"
#include "../models/Message.h"
#include "../models/Notification.h"
#include <string>

class FileManager {
public:
    static FileManager& instance();

    // Users
    void saveAllUsers(const UserTable& users);
    void loadAllUsers(UserTable& users);

    // Posts (loads into existing users)
    void saveAllPosts(const UserTable& users);
    void loadAllPosts(UserTable& users);

    // Follow graph
    void saveFriends(const UserTable& users);
    void loadFriends(UserTable& users);

    // Messages
    void saveMessage(int msgID, int senderID, int receiverID,
                     const std::string& content, time_t ts);
    void loadAllMessages(MessageList& list);

    // Notifications
    void saveAllNotifications(const NotifList& list);
    void appendNotification(const NotifNode* node);
    void loadAllNotifications(NotifList& list);

    // Utility
    static std::string hashPassword(const std::string& pw);
    void ensureDataDir();

private:
    FileManager() = default;
    FileManager(const FileManager&)            = delete;
    FileManager& operator=(const FileManager&) = delete;

    static const std::string DATA_DIR;
    static const std::string USERS_FILE;
    static const std::string POSTS_FILE;
    static const std::string FRIENDS_FILE;
    static const std::string MESSAGES_FILE;
    static const std::string NOTIFS_FILE;

    static std::string sanitize(const std::string& s);
};
