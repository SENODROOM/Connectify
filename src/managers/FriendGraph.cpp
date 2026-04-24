#include "FriendGraph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <filesystem>

const std::string FriendGraph::REQUESTS_FILE = "data/friend_requests.dat";

FriendGraph& FriendGraph::instance() {
    static FriendGraph fg;
    return fg;
}

User* FriendGraph::findUser(int id, const std::vector<User*>& users) const {
    for (User* u : users) if (u->getID() == id) return u;
    return nullptr;
}

void FriendGraph::sendRequest(int fromID, int toID, std::vector<User*>& allUsers) {
    // Avoid duplicate pending requests
    for (const auto& r : requests_)
        if (r.fromID == fromID && r.toID == toID && r.status == "PENDING") return;

    FriendRequest req{ ++lastRequestID_, fromID, toID, "PENDING" };
    requests_.push_back(req);
    saveRequests();
    std::cout << "[FriendGraph] Request #" << req.requestID << " sent.\n";
}

void FriendGraph::acceptRequest(int requestID, std::vector<User*>& allUsers) {
    for (auto& r : requests_) {
        if (r.requestID == requestID && r.status == "PENDING") {
            r.status = "ACCEPTED";
            User* from = findUser(r.fromID, allUsers);
            User* to   = findUser(r.toID,   allUsers);
            if (from && to) {
                from->follow(to->getID());
                to->addFollower(from->getID());
            }
            saveRequests();
            return;
        }
    }
}

void FriendGraph::rejectRequest(int requestID) {
    for (auto& r : requests_) {
        if (r.requestID == requestID) { r.status = "REJECTED"; break; }
    }
    saveRequests();
}

std::vector<FriendRequest> FriendGraph::getPendingFor(int userID) const {
    std::vector<FriendRequest> pending;
    for (const auto& r : requests_)
        if (r.toID == userID && r.status == "PENDING") pending.push_back(r);
    return pending;
}

std::vector<User*> FriendGraph::getFriends(int userID, const std::vector<User*>& allUsers) const {
    User* u = findUser(userID, allUsers);
    if (!u) return {};
    std::vector<User*> friends;
    for (int fid : u->getFollowing()) {
        User* f = findUser(fid, allUsers);
        if (f) friends.push_back(f);
    }
    return friends;
}

void FriendGraph::loadRequests() {
    requests_.clear();
    std::ifstream f(REQUESTS_FILE);
    if (!f.is_open()) return;

    std::string line;
    std::getline(f, line); // header
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string rid, from, to, status;
        std::getline(ss, rid,    '|');
        std::getline(ss, from,   '|');
        std::getline(ss, to,     '|');
        std::getline(ss, status, '|');
        try {
            FriendRequest r{ std::stoi(rid), std::stoi(from), std::stoi(to), status };
            requests_.push_back(r);
            if (r.requestID > lastRequestID_) lastRequestID_ = r.requestID;
        } catch (...) {}
    }
}

void FriendGraph::saveRequests() const {
    std::filesystem::create_directories("data");
    std::ofstream f(REQUESTS_FILE);
    f << "requestID|fromID|toID|status\n";
    for (const auto& r : requests_)
        f << r.requestID << '|' << r.fromID << '|' << r.toID << '|' << r.status << '\n';
}
