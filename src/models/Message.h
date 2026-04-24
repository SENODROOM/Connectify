#pragma once
#include <string>
#include <ctime>

struct Message {
    int         msgID;
    int         senderID;
    int         receiverID;
    std::string content;
    time_t      timestamp;
};
