#pragma once
#include "../models/User.h"
#include "../models/Admin.h"
#include "../models/Post.h"
#include "../models/Message.h"
#include "../models/Notification.h"
#include <vector>
#include <string>

class FileManager {
public:
    static FileManager& instance();

    // Users
    void             saveAllUsers(const std::vector<User*>& users);
    std::vector<User*> loadAllUsers();

    // Posts
    void             saveAllPosts(const std::vector<User*>& users);
    void             loadAllPosts(std::vector<User*>& users);

    // Friends graph
    void             saveFriends(const std::vector<User*>& users);
    void             loadFriends(std::vector<User*>& users);

    // Messages
    void               saveMessage(const Message& msg);
    std::vector<Message> loadAllMessages();

    // Notifications
    void                      saveNotification(const Notification& n);
    void                      saveAllNotifications(const std::vector<Notification>& notifs);
    std::vector<Notification> loadAllNotifications();

    // Utility
    static std::string hashPassword(const std::string& password);
    void ensureDataDir();

private:
    FileManager() = default;
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    static const std::string DATA_DIR;
    static const std::string USERS_FILE;
    static const std::string POSTS_FILE;
    static const std::string FRIENDS_FILE;
    static const std::string MESSAGES_FILE;
    static const std::string NOTIFS_FILE;

    User* findUser(const std::vector<User*>& users, int id) const;
};
