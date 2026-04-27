#include "FileManager.h"
#include "../models/TextPost.h"
#include "../models/ImagePost.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

const std::string FileManager::DATA_DIR      = "data/";
const std::string FileManager::USERS_FILE    = "data/users.dat";
const std::string FileManager::POSTS_FILE    = "data/posts.dat";
const std::string FileManager::FRIENDS_FILE  = "data/friends.dat";
const std::string FileManager::MESSAGES_FILE = "data/messages.dat";
const std::string FileManager::NOTIFS_FILE   = "data/notifications.dat";

FileManager& FileManager::instance() {
    static FileManager fm;
    return fm;
}

void FileManager::ensureDataDir() {
    std::filesystem::create_directories(DATA_DIR);
}

std::string FileManager::sanitize(const std::string& s) {
    std::string out = s;
    for (char& c : out) if (c == '|') c = ';';
    return out;
}

// djb2 hash — no STL maps, no external libs
std::string FileManager::hashPassword(const std::string& pw) {
    unsigned long h = 5381;
    for (unsigned char c : pw)
        h = ((h << 5) + h) + c;
    std::ostringstream oss;
    oss << std::hex << h;
    return oss.str();
}

// ─── Users ────────────────────────────────────────────────────────────────────

void FileManager::saveAllUsers(const UserTable& users) {
    ensureDataDir();
    std::ofstream f(USERS_FILE);
    f << "userID|name|email|hashedPassword|isBanned|profilePicPath\n";
    for (int i = 0; i < users.size(); ++i) {
        User* u = users[i];
        f << u->getID()             << '|'
          << sanitize(u->getName()) << '|'
          << sanitize(u->getEmail())<< '|'
          << u->getHashedPassword() << '|'
          << (u->isBanned() ? 1 : 0)<< '|'
          << sanitize(u->getProfilePic()) << '\n';
    }
}

void FileManager::loadAllUsers(UserTable& users) {
    // delete existing users first
    for (int i = 0; i < users.size(); ++i) delete users[i];
    users.clear();

    std::ifstream f(USERS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string id, name, email, hash, banned, pic;
        std::getline(ss, id,     '|');
        std::getline(ss, name,   '|');
        std::getline(ss, email,  '|');
        std::getline(ss, hash,   '|');
        std::getline(ss, banned, '|');
        std::getline(ss, pic,    '|');
        try {
            User* u = new User(std::stoi(id), name, email, hash);
            u->setBanned(banned == "1");
            u->setProfilePic(pic);
            users.add(u);
        } catch (...) {
            std::cerr << "[FileManager] Skipping malformed user line\n";
        }
    }
}

// ─── Posts ────────────────────────────────────────────────────────────────────

void FileManager::saveAllPosts(const UserTable& users) {
    ensureDataDir();
    std::ofstream f(POSTS_FILE);
    f << "postID|ownerID|type|content|imagePath|timestamp|likes\n";

    for (int i = 0; i < users.size(); ++i) {
        User* u = users[i];
        PostNode* node = u->getPosts().head();
        while (node) {
            Post* p = node->post;
            std::string content, imagePath;
            if (p->getType() == "TEXT") {
                content = dynamic_cast<TextPost*>(p)->getContent();
            } else {
                auto* ip = dynamic_cast<ImagePost*>(p);
                if (ip) { imagePath = ip->getImagePath(); content = ip->getCaption(); }
            }
            f << p->getPostID()          << '|'
              << p->getOwnerID()         << '|'
              << p->getType()            << '|'
              << sanitize(content)       << '|'
              << sanitize(imagePath)     << '|'
              << p->getTimestamp()       << '|'
              << p->getLikes()           << '\n';
            node = node->next;
        }
    }
}

void FileManager::loadAllPosts(UserTable& users) {
    std::ifstream f(POSTS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string postID, ownerID, type, content, imagePath, timestamp, likes;
        std::getline(ss, postID,    '|');
        std::getline(ss, ownerID,   '|');
        std::getline(ss, type,      '|');
        std::getline(ss, content,   '|');
        std::getline(ss, imagePath, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, likes,     '|');
        try {
            int    pid = std::stoi(postID);
            int    oid = std::stoi(ownerID);
            time_t ts  = static_cast<time_t>(std::stoll(timestamp));

            Post* post = nullptr;
            if (type == "TEXT")
                post = new TextPost(pid, oid, content, ts);
            else
                post = new ImagePost(pid, oid, imagePath, content, ts);

            User* owner = users.findByID(oid);
            if (owner && post) owner->addPost(post);
            else               delete post;
        } catch (...) {
            std::cerr << "[FileManager] Skipping malformed post line\n";
        }
    }
}

// ─── Friends ──────────────────────────────────────────────────────────────────

void FileManager::saveFriends(const UserTable& users) {
    ensureDataDir();
    std::ofstream f(FRIENDS_FILE);
    f << "fromID|toID|status\n";
    for (int i = 0; i < users.size(); ++i) {
        User* u = users[i];
        const FollowArray& following = u->getFollowing();
        for (int j = 0; j < following.size(); ++j)
            f << u->getID() << '|' << following[j] << "|ACCEPTED\n";
    }
}

void FileManager::loadFriends(UserTable& users) {
    std::ifstream f(FRIENDS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string from, to, status;
        std::getline(ss, from,   '|');
        std::getline(ss, to,     '|');
        std::getline(ss, status, '|');
        if (status != "ACCEPTED") continue;
        try {
            int fromID = std::stoi(from);
            int toID   = std::stoi(to);
            User* follower = users.findByID(fromID);
            User* followed = users.findByID(toID);
            if (follower && followed) {
                follower->follow(toID);
                followed->addFollower(fromID);
            }
        } catch (...) {}
    }
}

// ─── Messages ─────────────────────────────────────────────────────────────────

void FileManager::saveMessage(int msgID, int senderID, int receiverID,
                               const std::string& content, time_t ts) {
    ensureDataDir();
    // Append mode — check if header line needed
    bool needsHeader = false;
    {
        std::ifstream check(MESSAGES_FILE);
        needsHeader = !check.is_open() || check.peek() == std::ifstream::traits_type::eof();
    }
    std::ofstream f(MESSAGES_FILE, std::ios::app);
    if (needsHeader) f << "msgID|senderID|receiverID|content|timestamp\n";
    f << msgID      << '|'
      << senderID   << '|'
      << receiverID << '|'
      << sanitize(content) << '|'
      << ts         << '\n';
}

void FileManager::loadAllMessages(MessageList& list) {
    list.clear();
    std::ifstream f(MESSAGES_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string mid, sid, rid, content, ts;
        std::getline(ss, mid,     '|');
        std::getline(ss, sid,     '|');
        std::getline(ss, rid,     '|');
        std::getline(ss, content, '|');
        std::getline(ss, ts,      '|');
        try {
            list.append(std::stoi(mid), std::stoi(sid), std::stoi(rid),
                        content, static_cast<time_t>(std::stoll(ts)));
        } catch (...) {}
    }
}

// ─── Notifications ────────────────────────────────────────────────────────────

void FileManager::appendNotification(const NotifNode* node) {
    ensureDataDir();
    bool needsHeader = false;
    {
        std::ifstream check(NOTIFS_FILE);
        needsHeader = !check.is_open() || check.peek() == std::ifstream::traits_type::eof();
    }
    std::ofstream f(NOTIFS_FILE, std::ios::app);
    if (needsHeader) f << "notifID|ownerID|type|message|isRead|timestamp\n";
    f << node->notifID        << '|'
      << node->ownerID        << '|'
      << node->typeString()   << '|'
      << sanitize(node->message) << '|'
      << (node->isRead ? 1 : 0)  << '|'
      << node->timestamp      << '\n';
}

void FileManager::saveAllNotifications(const NotifList& list) {
    ensureDataDir();
    std::ofstream f(NOTIFS_FILE);
    f << "notifID|ownerID|type|message|isRead|timestamp\n";
    NotifNode* cur = list.head();
    while (cur) {
        f << cur->notifID        << '|'
          << cur->ownerID        << '|'
          << cur->typeString()   << '|'
          << sanitize(cur->message) << '|'
          << (cur->isRead ? 1 : 0)  << '|'
          << cur->timestamp      << '\n';
        cur = cur->next;
    }
}

void FileManager::loadAllNotifications(NotifList& list) {
    list.clear();
    std::ifstream f(NOTIFS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string nid, oid, type, msg, read, ts;
        std::getline(ss, nid,  '|');
        std::getline(ss, oid,  '|');
        std::getline(ss, type, '|');
        std::getline(ss, msg,  '|');
        std::getline(ss, read, '|');
        std::getline(ss, ts,   '|');
        try {
            NotifNode* node = new NotifNode(
                std::stoi(nid), std::stoi(oid),
                NotifNode::typeFromString(type),
                msg, static_cast<time_t>(std::stoll(ts)));
            node->isRead = (read == "1");
            list.append(node);
        } catch (...) {}
    }
}
