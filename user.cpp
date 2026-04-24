
#include <iostream>
#include <cstring>
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

	User() {
		userID = 0;
		requestCount = 0;
		followerCount = 0;
		followingCount = 0;
		friendCount = 0;
		friends = new User * [1];
		request = new User * [1];
		follower = new User * [1];
		following = new User * [1];
	}
	~User() {
		delete[] friends;
		delete[] request;
		delete[] follower;
		delete[] following;
	}
	User(const User& other) {
		userID = other.userID;
		password = other.password;
		userName = other.userName;
		role = other.role;

		friendCount = other.friendCount;
		requestCount = other.requestCount;
		followerCount = other.followerCount;
		followingCount = other.followingCount;

		friends = new User * [friendCount + 1];
		request = new User * [requestCount + 1];
		follower = new User * [followerCount + 1];
		following = new User * [followingCount + 1];

		for (int i = 0; i < friendCount; i++) friends[i] = other.friends[i];
		for (int i = 0; i < requestCount; i++) request[i] = other.request[i];
		for (int i = 0; i < followerCount; i++) follower[i] = other.follower[i];
		for (int i = 0; i < followingCount; i++) following[i] = other.following[i];
	}
	User& operator=(const User& other) {
		if (this != &other) {
			delete[] friends;
			delete[] request;
			delete[] follower;
			delete[] following;
			userID = other.userID;
			password = other.password;
			userName = other.userName;
			role = other.role;
			friendCount = other.friendCount;
			requestCount = other.requestCount;
			followerCount = other.followerCount;
			followingCount = other.followingCount;
			friends = new User * [friendCount + 1];
			request = new User * [requestCount + 1];
			follower = new User * [followerCount + 1];
			following = new User * [followingCount + 1];
			for (int i = 0; i < friendCount; i++) friends[i] = other.friends[i];
			for (int i = 0; i < requestCount; i++) request[i] = other.request[i];
			for (int i = 0; i < followerCount; i++) follower[i] = other.follower[i];
			for (int i = 0; i < followingCount; i++) following[i] = other.following[i];
		}
		return *this;
	}
	// for resizing dynamic pointers
	void resize(User**& u, int count) {
		int ns = count + 1;
		User** temp = new User * [ns];
		for (int i = 0; i < count; i++)
			temp[i] = u[i];
		delete[] u;
		u = temp;
	}

	void sendRequest(User* u) {
		if (u == NULL) {
			cout << "user not found" << endl;
			return;
		}
		if (u == this) {
			cout << "cant request yourself" << endl;
			return;
		}
		for (int i = 0; i < friendCount; i++) {
			if (friends[i] == u) {
				cout << "already friends" << endl;
				return;
			}
		}
		// Check if u already sent a request to this user
		for (int i = 0; i < requestCount; i++) {
			if (request[i] == u) {
				cout << "they already sent you a request, accept it instead" << endl;
				return;
			}
		}
		for (int i = 0; i < u->requestCount; i++) {
			if (u->request[i] == this) {
				cout << "already requested" << endl;
				return;
			}
		}
		u->resize(u->request, u->requestCount);
		u->request[u->requestCount++] = this;
		cout << "request sent successfully" << endl;
	}

	void follow(User* to) {
		if (to == NULL || to == this) return;
		for (int i = 0; i < followerCount; i++)
			if (follower[i] == to) return;

		// A gains follower  B
		resize(follower, followerCount);
		this->follower[this->followerCount++] = to;

		// B is following  A
		to->resize(to->following, to->followingCount);
		to->following[to->followingCount++] = this;
	}

	void acceptRequest(User* u) {
		for (int i = 0; i < friendCount; i++)
			if (friends[i] == u) {
				cout << "already friends" << endl;
				return;
			}
		// check if both follow each other
		bool mutual = false;
		for (int i = 0; i < u->followingCount; i++) {
			if (u->following[i] == this) {
				mutual = true;
				break;
			}
		}

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
	// delete request in user request list
	void rejectRequest(User* u) {
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
};

User* user = nullptr;
int userCount = 0;

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
	if (userNameExist(usern)) {
		cout << "username already exists" << endl;
		return;
	}
	if ((rol == "Admin" || rol == "admin") && adminExist()) {
		cout << "admin already exists" << endl;
		return;
	}
	resizeUsers();
	user[userCount].userID = id;
	user[userCount].userName = usern;
	user[userCount].password = pass;
	user[userCount].role = rol;
	userCount++;
	cout << "account created successfully" << endl;
}

// Returns index instead of pointer to avoid dangling pointer after resize
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

		// Remove from friends
		for (int j = 0; j < u->friendCount; j++) {
			if (u->friends[j]->userID == targetID) {
				for (int k = j; k < u->friendCount - 1; k++)
					u->friends[k] = u->friends[k + 1];
				u->friendCount--;
				break;
			}
		}
		// Remove from requests
		for (int j = 0; j < u->requestCount; j++) {
			if (u->request[j]->userID == targetID) {
				for (int k = j; k < u->requestCount - 1; k++)
					u->request[k] = u->request[k + 1];
				u->requestCount--;
				break;
			}
		}
		// Remove from followers
		for (int j = 0; j < u->followerCount; j++) {
			if (u->follower[j]->userID == targetID) {
				for (int k = j; k < u->followerCount - 1; k++)
					u->follower[k] = u->follower[k + 1];
				u->followerCount--;
				break;
			}
		}
		// Remove from following
		for (int j = 0; j < u->followingCount; j++) {
			if (u->following[j]->userID == targetID) {
				for (int k = j; k < u->followingCount - 1; k++)
					u->following[k] = u->following[k + 1];
				u->followingCount--;
				break;
			}
		}
	}
}

void deleteAccount(int index) {
	if (index < 0 || index >= userCount) {
		cout << "no such user account found" << endl;
		return;
	}
	// FIX: save the userID before shifting, then match by ID not pointer
	int targetID = user[index].userID;
	removeUserReferences(targetID);
	for (int i = index; i < userCount - 1; i++)
		user[i] = user[i + 1];
	userCount--;
	cout << "account deleted successfully" << endl;
}

void adminDelete(int adminIndex, string u) {
	if (user[adminIndex].role != "admin" && user[adminIndex].role != "Admin") {
		cout << "access denied" << endl;
		return;
	}
	int index = -1;
	for (int i = 0; i < userCount; i++) {
		if (user[i].userName == u) { index = i; break; }
	}
	if (index == -1) {
		cout << "no such user account found" << endl;
		return;
	}
	int targetID = user[index].userID;
	removeUserReferences(targetID);
	for (int i = index; i < userCount - 1; i++)
		user[i] = user[i + 1];
	userCount--;
	cout << "account deleted successfully" << endl;
}

void display(int adminIndex) {
	if (user[adminIndex].role != "admin" && user[adminIndex].role != "Admin") {
		cout << "access denied" << endl;
		return;
	}
	for (int i = 0; i < userCount; i++) {
		cout << "user id   : " << user[i].userID << endl;
		cout << "user name : " << user[i].userName << endl;
		cout << "role      : " << user[i].role << endl;
		cout << "-------------------------" << endl;
	}
}