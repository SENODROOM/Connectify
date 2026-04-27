#include <iostream>
#include <cstring>
#include "user.h"
using namespace std;

// ==================== GLOBAL VARIABLES ====================
User* user = nullptr;
int userCount = 0;

// ==================== USER METHODS ====================

void User::resize(User**& u, int count) {
    User** temp = new User * [count + 1];
    for (int i = 0; i < count; i++)
        temp[i] = u[i];
    delete[] u;
    u = temp;
}

void User::createPost(Post* p) {
    Post** temp = new Post * [postCount + 1];
    for (int i = 0; i < postCount; i++)
        temp[i] = posts[i];
    temp[postCount] = p;
    postCount++;
    delete[] posts;
    posts = temp;
    cout << userName << " created a post\n";
}

void User::showPosts() {
    cout << "\n--- Posts of " << userName << " ---\n";
    for (int i = 0; i < postCount; i++) {
        posts[i]->display();
        cout << "-----------------\n";
    }
}

void User::sendRequest(User* u) {
    if (u == NULL) { cout << "user not found" << endl; return; }
    if (u == this) { cout << "cant request yourself" << endl; return; }

    for (int i = 0; i < friendCount; i++)
        if (friends[i] == u) { cout << "already friends" << endl; return; }

    for (int i = 0; i < requestCount; i++)
        if (request[i] == u) { cout << "they already sent you a request, accept it instead" << endl; return; }

    for (int i = 0; i < u->requestCount; i++)
        if (u->request[i] == this) { cout << "already requested" << endl; return; }

    u->resize(u->request, u->requestCount);
    u->request[u->requestCount++] = this;
    cout << "request sent successfully" << endl;
}

void User::follow(User* to) {
    if (to == NULL || to == this) return;
    for (int i = 0; i < followingCount; i++)
        if (following[i] == to) return;

    resize(following, followingCount);
    following[followingCount++] = to;

    to->resize(to->follower, to->followerCount);
    to->follower[to->followerCount++] = this;
}

void User::acceptRequest(User* u) {
    for (int i = 0; i < friendCount; i++)
        if (friends[i] == u) { cout << "already friends" << endl; return; }

    bool found = false;
    for (int i = 0; i < requestCount; i++)
        if (request[i] == u) { found = true; break; }
    if (!found) { cout << "no request from this user" << endl; return; }

    bool mutual = false;
    for (int i = 0; i < u->followingCount; i++)
        if (u->following[i] == this) { mutual = true; break; }

    follow(u);

    if (mutual) {
        resize(friends, friendCount);
        friends[friendCount++] = u;
        u->resize(u->friends, u->friendCount);
        u->friends[u->friendCount++] = this;
        cout << "request accepted, you are now friends" << endl;
    }
    else {
        cout << "request accepted" << endl;
    }

    rejectRequest(u);
}

void User::rejectRequest(User* u) {
    for (int i = 0; i < requestCount; i++) {
        if (request[i] == u) {
            for (int j = i; j < requestCount - 1; j++)
                request[j] = request[j + 1];
            requestCount--;
            cout << "request rejected" << endl;
            return;
        }
    }
}

// ==================== GLOBAL FUNCTIONS ====================

bool userNameExist(string n) {
    for (int i = 0; i < userCount; i++)
        if (user[i].userName == n) return true;
    return false;
}

bool adminExist() {
    for (int i = 0; i < userCount; i++)
        if (user[i].role == "Admin" || user[i].role == "admin") return true;
    return false;
}

void resizeUsers() {
    int ns = userCount + 1;
    User* temp = new User[ns];
    for (int i = 0; i < userCount; i++)
        temp[i] = user[i];
    delete[] user;
    user = temp;
}

void signup(int id, string usern, string pass, string rol) {
    if (userNameExist(usern)) { cout << "username already exists" << endl; return; }
    if ((rol == "Admin" || rol == "admin") && adminExist()) { cout << "admin already exists" << endl; return; }
    resizeUsers();
    user[userCount].userID = id;
    user[userCount].userName = usern;
    user[userCount].password = pass;
    user[userCount].role = rol;
    userCount++;
    cout << "account created successfully" << endl;
}

int login(string u, string pass) {
    for (int i = 0; i < userCount; i++)
        if (user[i].userName == u && user[i].password == pass)
            return i;
    return -1;
}

void removeUserReferences(int targetID) {
    for (int i = 0; i < userCount; i++) {
        User* u = &user[i];
        if (u->userID == targetID) continue;

        for (int j = 0; j < u->friendCount; j++)
            if (u->friends[j]->userID == targetID) {
                for (int k = j; k < u->friendCount - 1; k++) u->friends[k] = u->friends[k + 1];
                u->friendCount--; break;
            }
        for (int j = 0; j < u->requestCount; j++)
            if (u->request[j]->userID == targetID) {
                for (int k = j; k < u->requestCount - 1; k++) u->request[k] = u->request[k + 1];
                u->requestCount--; break;
            }
        for (int j = 0; j < u->followerCount; j++)
            if (u->follower[j]->userID == targetID) {
                for (int k = j; k < u->followerCount - 1; k++) u->follower[k] = u->follower[k + 1];
                u->followerCount--; break;
            }
        for (int j = 0; j < u->followingCount; j++)
            if (u->following[j]->userID == targetID) {
                for (int k = j; k < u->followingCount - 1; k++) u->following[k] = u->following[k + 1];
                u->followingCount--; break;
            }
    }
}

void deleteAccount(int index) {
    if (index < 0 || index >= userCount) { cout << "no such user account found" << endl; return; }
    removeUserReferences(user[index].userID);
    for (int i = index; i < userCount - 1; i++)
        user[i] = user[i + 1];
    userCount--;
    cout << "account deleted successfully" << endl;
}

void adminDelete(int adminIndex, string u) {
    if (user[adminIndex].role != "admin" && user[adminIndex].role != "Admin") {
        cout << "access denied" << endl; return;
    }
    int index = -1;
    for (int i = 0; i < userCount; i++)
        if (user[i].userName == u) { index = i; break; }
    if (index == -1) { cout << "no such user account found" << endl; return; }
    removeUserReferences(user[index].userID);
    for (int i = index; i < userCount - 1; i++)
        user[i] = user[i + 1];
    userCount--;
    cout << "account deleted successfully" << endl;
}

void display(int adminIndex) {
    if (user[adminIndex].role != "admin" && user[adminIndex].role != "Admin") {
        cout << "access denied" << endl; return;
    }
    for (int i = 0; i < userCount; i++) {
        cout << "user id   : " << user[i].userID << endl;
        cout << "user name : " << user[i].userName << endl;
        cout << "role      : " << user[i].role << endl;
        cout << "-------------------------" << endl;
    }
}
