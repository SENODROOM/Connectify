#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
public:
    int userID;
    string password;
    string userName;
    string role;

    User** friends;
    int friendCount;

    User** request;
    int requestCount;

    User** follower;
    int followerCount;

    User** following;
    int followingCount;

    User();
    ~User();

    User(const User& other);
    User& operator=(const User& other);

    void resize(User**& u, int count);

    void sendRequest(User* u);
    void follow(User* to);
    void acceptRequest(User* u);
    void rejectRequest(User* u);
};

// Global user system
extern User* user;
extern int userCount;

// Utility functions
bool userNameExist(string n);
bool adminExist();

void resizeUsers();

void signup(int id, string usern, string pass, string rol);

int login(string u, string pass);

void removeUserReferences(int targetID);

void deleteAccount(int index);

void adminDelete(int adminIndex, string u);

void display(int adminIndex);

#endif