#include "user.h"
class Message {
public:
    int senderID;
    int receiverID;
    string text;

    Message(int s, int r, string t) {
        senderID = s;
        receiverID = r;
        text = t;
    }
};
class MessageSystem {
public:
    Message** msg;
    int msgCount;
    MessageSystem() {
        msg = new Message * [1];
        msgCount = 0;
    }
    void resize() {
        Message** temp = new Message * [msgCount + 1];
        for (int i = 0; i < msgCount; i++)
            temp[i] = msg[i];
        delete[] msg;
        msg = temp;
    }
    void sendMessage(User* from, User* to, string text) {
        if (!from || !to || from == to) {
            cout << "invalid users" << endl;
            return;
        }

        // check connection friends only
        bool connected = false;
        for (int i = 0; i < from->friendCount; i++) {
            if (from->friends[i] == to) {
                connected = true;
                break;
            }
        }
        if (!connected) {
            cout << "only connected users can message" << endl;
            return;
        }
        resize();
        msg[msgCount++] = new Message(from->userID, to->userID, text);
        cout << "message sent" << endl;
    }
    void viewInbox(User* u) {
        bool found = false;
        for (int i = 0; i < msgCount; i++) {
            if (msg[i]->receiverID == u->userID) {
                cout << "From ID: " << msg[i]->senderID << endl;
                cout << "Message: " << msg[i]->text << endl;
                cout << "-------------------" << endl;
                found = true;
            }
        }

        if (!found)
            cout << "no messages" << endl;
    }
};
