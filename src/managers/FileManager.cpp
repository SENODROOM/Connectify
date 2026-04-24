#include "FileManager.h"
#include "../models/TextPost.h"
#include "../models/ImagePost.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

// --- File paths ---
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

// Simple djb2-based hash (good enough for a course project)
std::string FileManager::hashPassword(const std::string& password) {
    unsigned long hash = 5381;
    for (char c : password)
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    std::ostringstream oss;
    oss << std::hex << hash;
    return oss.str();
}

User* FileManager::findUser(const std::vector<User*>& users, int id) const {
    for (User* u : users)
        if (u->getID() == id) return u;
    return nullptr;
}

// ─── Users ────────────────────────────────────────────────────────────────────

void FileManager::saveAllUsers(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(USERS_FILE);
    f << "userID|name|email|hashedPassword|isBanned|profilePicPath\n";
    for (User* u : users) {
        f << u->getID()      << '|'
          << u->getName()    << '|'
          << u->getEmail()   << '|'
          << u->getHashedPassword() << '|'
          << (u->isBanned() ? 1 : 0) << '|'
          << u->getProfilePic() << '\n';
    }
}

std::vector<User*> FileManager::loadAllUsers() {
    std::vector<User*> users;
    std::ifstream f(USERS_FILE);
    if (!f.is_open()) return users;

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
            users.push_back(u);
        } catch (...) {
            std::cerr << "[FileManager] Skipping malformed user line\n";
        }
    }
    return users;
}

// ─── Posts ────────────────────────────────────────────────────────────────────

void FileManager::saveAllPosts(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(POSTS_FILE);
    f << "postID|ownerID|type|content|imagePath|timestamp|likes\n";
    for (User* u : users) {
        for (Post* p : u->getPosts()) {
            std::string content, imagePath;
            if (p->getType() == "TEXT") {
                auto* tp = dynamic_cast<TextPost*>(p);
                content = tp ? tp->getContent() : "";
            } else {
                auto* ip = dynamic_cast<ImagePost*>(p);
                if (ip) { imagePath = ip->getImagePath(); content = ip->getCaption(); }
            }
            // Replace pipe chars in content to avoid parsing issues
            auto sanitize = [](std::string s) {
                for (char& c : s) if (c == '|') c = ';';
                return s;
            };
            f << p->getPostID()    << '|'
              << p->getOwnerID()   << '|'
              << p->getType()      << '|'
              << sanitize(content) << '|'
              << sanitize(imagePath) << '|'
              << p->getTimestamp() << '|'
              << p->getLikes()     << '\n';
        }
    }
}

void FileManager::loadAllPosts(std::vector<User*>& users) {
    std::ifstream f(POSTS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // skip header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string postID, ownerID, type, content, imagePath, timestamp, likes;
        std::getline(ss, postID,     '|');
        std::getline(ss, ownerID,    '|');
        std::getline(ss, type,       '|');
        std::getline(ss, content,    '|');
        std::getline(ss, imagePath,  '|');
        std::getline(ss, timestamp,  '|');
        std::getline(ss, likes,      '|');
        try {
            int pid = std::stoi(postID), oid = std::stoi(ownerID);
            time_t ts = static_cast<time_t>(std::stoll(timestamp));
            Post* post = nullptr;
            if (type == "TEXT")
                post = new TextPost(pid, oid, content, ts);
            else
                post = new ImagePost(pid, oid, imagePath, content, ts);

            User* owner = findUser(users, oid);
            if (owner && post) owner->addPost(post);
            else delete post;
        } catch (...) {
            std::cerr << "[FileManager] Skipping malformed post line\n";
        }
    }
}

// ─── Friends ──────────────────────────────────────────────────────────────────

void FileManager::saveFriends(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(FRIENDS_FILE);
    f << "fromID|toID|status\n";
    for (User* u : users) {
        for (int fid : u->getFollowing()) {
            f << u->getID() << '|' << fid << "|ACCEPTED\n";
        }
    }
}

void FileManager::loadFriends(std::vector<User*>& users) {
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
            int fromID = std::stoi(from), toID = std::stoi(to);
            User* follower = findUser(users, fromID);
            User* followed = findUser(users, toID);
            if (follower && followed) {
                follower->follow(toID);
                followed->addFollower(fromID);
            }
        } catch (...) {}
    }
}

// ─── Messages ─────────────────────────────────────────────────────────────────

void FileManager::saveMessage(const Message& msg) {
    ensureDataDir();
    std::ofstream f(MESSAGES_FILE, std::ios::app);
    if (f.tellp() == 0)
        f << "msgID|senderID|receiverID|content|timestamp\n";
    auto sanitize = [](std::string s) {
        for (char& c : s) if (c == '|') c = ';';
        return s;
    };
    f << msg.msgID      << '|'
      << msg.senderID   << '|'
      << msg.receiverID << '|'
      << sanitize(msg.content) << '|'
      << msg.timestamp  << '\n';
}

std::vector<Message> FileManager::loadAllMessages() {
    std::vector<Message> msgs;
    std::ifstream f(MESSAGES_FILE);
    if (!f.is_open()) return msgs;

    std::string line;
    std::getline(f, line);
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
            msgs.push_back({ std::stoi(mid), std::stoi(sid), std::stoi(rid),
                             content, static_cast<time_t>(std::stoll(ts)) });
        } catch (...) {}
    }
    return msgs;
}

// ─── Notifications ────────────────────────────────────────────────────────────

void FileManager::saveNotification(const Notification& n) {
    ensureDataDir();
    std::ofstream f(NOTIFS_FILE, std::ios::app);
    auto sanitize = [](std::string s) {
        for (char& c : s) if (c == '|') c = ';';
        return s;
    };
    f << n.notifID    << '|'
      << n.ownerID    << '|'
      << n.typeToString() << '|'
      << sanitize(n.message) << '|'
      << (n.isRead ? 1 : 0) << '|'
      << n.timestamp  << '\n';
}

void FileManager::saveAllNotifications(const std::vector<Notification>& notifs) {
    ensureDataDir();
    std::ofstream f(NOTIFS_FILE);
    f << "notifID|ownerID|type|message|isRead|timestamp\n";
    for (const auto& n : notifs) {
        auto sanitize = [](std::string s) {
            for (char& c : s) if (c == '|') c = ';';
            return s;
        };
        f << n.notifID    << '|'
          << n.ownerID    << '|'
          << n.typeToString() << '|'
          << sanitize(n.message) << '|'
          << (n.isRead ? 1 : 0) << '|'
          << n.timestamp  << '\n';
    }
}

std::vector<Notification> FileManager::loadAllNotifications() {
    std::vector<Notification> notifs;
    std::ifstream f(NOTIFS_FILE);
    if (!f.is_open()) return notifs;

    std::string line;
    std::getline(f, line);
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
            Notification n;
            n.notifID   = std::stoi(nid);
            n.ownerID   = std::stoi(oid);
            n.type      = Notification::typeFromString(type);
            n.message   = msg;
            n.isRead    = (read == "1");
            n.timestamp = static_cast<time_t>(std::stoll(ts));
            notifs.push_back(n);
        } catch (...) {}
    }
    return notifs;
}
