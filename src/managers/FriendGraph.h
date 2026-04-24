#pragma once
#include "../models/User.h"
#include <vector>
#include <string>

struct FriendRequest {
    int         requestID;
    int         fromID;
    int         toID;
    std::string status;   // "PENDING" | "ACCEPTED" | "REJECTED"
};

class FriendGraph {
public:
    static FriendGraph& instance();

    void sendRequest(int fromID, int toID, std::vector<User*>& allUsers);
    void acceptRequest(int requestID, std::vector<User*>& allUsers);
    void rejectRequest(int requestID);

    std::vector<FriendRequest> getPendingFor(int userID) const;
    std::vector<User*>         getFriends(int userID, const std::vector<User*>& allUsers) const;

    void loadRequests();
    void saveRequests() const;

private:
    FriendGraph() = default;
    FriendGraph(const FriendGraph&) = delete;
    FriendGraph& operator=(const FriendGraph&) = delete;

    std::vector<FriendRequest> requests_;
    int lastRequestID_ = 5000;

    static const std::string REQUESTS_FILE;
    User* findUser(int id, const std::vector<User*>& users) const;
};
