#include "FriendGraph.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

const std::string FriendGraph::REQUESTS_FILE = "data/friend_requests.dat";

FriendGraph& FriendGraph::instance() {
    static FriendGraph fg;
    return fg;
}

void FriendGraph::sendRequest(int fromID, int toID) {
    // Avoid duplicate pending
    RequestNode* cur = requests_.head();
    while (cur) {
        if (cur->fromID == fromID && cur->toID == toID && cur->status == "PENDING")
            return;
        cur = cur->next;
    }
    RequestNode* node = new RequestNode(++lastRequestID_, fromID, toID, "PENDING");
    requests_.append(node);
    saveRequests();
}

void FriendGraph::acceptRequest(int requestID, UserTable& allUsers) {
    RequestNode* node = requests_.findByID(requestID);
    if (!node || node->status != "PENDING") return;
    node->status = "ACCEPTED";

    User* from = allUsers.findByID(node->fromID);
    User* to   = allUsers.findByID(node->toID);
    if (from && to) {
        from->follow(to->getID());
        to->addFollower(from->getID());
    }
    saveRequests();
}

void FriendGraph::rejectRequest(int requestID) {
    RequestNode* node = requests_.findByID(requestID);
    if (node) node->status = "REJECTED";
    saveRequests();
}

RequestNode** FriendGraph::getPendingFor(int userID, int& count) const {
    // Count first
    count = 0;
    RequestNode* cur = requests_.head();
    while (cur) {
        if (cur->toID == userID && cur->status == "PENDING") ++count;
        cur = cur->next;
    }
    if (count == 0) return nullptr;

    RequestNode** arr = new RequestNode*[count];
    int idx = 0;
    cur = requests_.head();
    while (cur) {
        if (cur->toID == userID && cur->status == "PENDING")
            arr[idx++] = cur;
        cur = cur->next;
    }
    return arr;
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
            int ridi = std::stoi(rid);
            RequestNode* node = new RequestNode(ridi, std::stoi(from),
                                                std::stoi(to), status);
            requests_.append(node);
            if (ridi > lastRequestID_) lastRequestID_ = ridi;
        } catch (...) {}
    }
}

void FriendGraph::saveRequests() const {
    std::filesystem::create_directories("data");
    std::ofstream f(REQUESTS_FILE);
    f << "requestID|fromID|toID|status\n";
    RequestNode* cur = requests_.head();
    while (cur) {
        f << cur->requestID << '|' << cur->fromID << '|'
          << cur->toID      << '|' << cur->status << '\n';
        cur = cur->next;
    }
}
