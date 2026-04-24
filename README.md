<div align="center">

```
 ██████╗ ██████╗ ███╗   ██╗███╗   ██╗███████╗ ██████╗████████╗██╗███████╗██╗   ██╗
██╔════╝██╔═══██╗████╗  ██║████╗  ██║██╔════╝██╔════╝╚══██╔══╝██║██╔════╝╚██╗ ██╔╝
██║     ██║   ██║██╔██╗ ██║██╔██╗ ██║█████╗  ██║        ██║   ██║█████╗   ╚████╔╝
██║     ██║   ██║██║╚██╗██║██║╚██╗██║██╔══╝  ██║        ██║   ██║██╔══╝    ╚██╔╝
╚██████╗╚██████╔╝██║ ╚████║██║ ╚████║███████╗╚██████╗   ██║   ██║██║        ██║
 ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═══╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝╚═╝        ╚═╝
```

**A C++ Social Media Management System**

_Built with Object-Oriented Programming · WinForms GUI · File Persistence_

---

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![WinForms](https://img.shields.io/badge/WinForms-GUI-512BD4?style=for-the-badge&logo=windows&logoColor=white)
![Status](https://img.shields.io/badge/Status-In%20Development-22c55e?style=for-the-badge)
![Semester](https://img.shields.io/badge/Spring-2nd%20Semester-f59e0b?style=for-the-badge)
![Team](https://img.shields.io/badge/Team-05%20%7C%20BSE--2B-ef4444?style=for-the-badge)

</div>

---

## 📋 Table of Contents

- [About](#-about)
- [Features](#-features)
- [OOP Concepts](#-oop-concepts)
- [Class Architecture](#-class-architecture)
- [Complete Development Roadmap](#-complete-development-roadmap)
  - [Phase 1 — Foundation](#phase-1--foundation-week-1-2)
  - [Phase 2 — Core Features](#phase-2--core-features-week-3-4)
  - [Phase 3 — Social Engine](#phase-3--social-engine-week-5-6)
  - [Phase 4 — GUI & Polish](#phase-4--gui--polish-week-7-8)
  - [Phase 5 — Testing & Submission](#phase-5--testing--submission-week-9)
- [File Structure](#-file-structure)
- [Data Storage Format](#-data-storage-format)
- [How to Build & Run](#-how-to-build--run)
- [Team](#-team)

---

## 📖 About

**Connectify** is a C++ WinForms application that simulates a modern digital social networking platform. Built on rock-solid OOP principles, it handles complex user relationships, content flow, and private communication — all wrapped in an interactive GUI.

> _"Bridging theoretical OOP concepts with practical software engineering."_

The system implements a **Role-Based Access Control (RBAC)** model with distinct `Admin` and `User` roles derived from a common `Person` base class, ensuring clean separation of privileges throughout the entire codebase.

---

## ✨ Features

| #   | Feature                     | Description                             | OOP Concept              |
| --- | --------------------------- | --------------------------------------- | ------------------------ |
| 1   | 🔐 **User Management**      | Secure Login, Signup & Account Deletion | Encapsulation            |
| 2   | 🛡️ **Admin Command Center** | View users, delete posts, ban accounts  | Inheritance + RBAC       |
| 3   | 📰 **Dynamic News Feed**    | Real-time posts from followed users     | Pointers, Polymorphism   |
| 4   | 🖼️ **Multimedia Posts**     | Text & image-based posts                | Inheritance, Abstraction |
| 5   | 🤝 **Social Graph**         | Friend requests, followers, connections | Data Structures          |
| 6   | ❤️ **Engagement Module**    | Likes & Comments on posts               | Composition              |
| 7   | 💬 **Private Messaging**    | Secure one-to-one user chat             | Encapsulation            |
| 8   | 🔍 **Search & Discovery**   | Search users or posts by keyword        | Algorithms               |
| 9   | 💾 **Persistent Storage**   | Full save/load via `fstream`            | File Handling            |
| 10  | 🔔 **Notification Engine**  | Real-time alerts for activity           | Observer Pattern         |

---

## 🧠 OOP Concepts

```
┌─────────────────────┬──────────────────────────────────────────────────────────────────────┐
│   Concept           │   Implementation in Connectify                                       │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🔒 Encapsulation    │ Passwords & personal data stored as private members.                 │
│                     │ Access only via public getUsername(), getEmail() methods.             │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🧬 Inheritance      │ Person (base) → Admin, User (derived).                               │
│                     │ Post (base) → TextPost, ImagePost (derived).                         │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🎭 Abstraction      │ Abstract Post class exposes display() interface.                     │
│                     │ News Feed renders posts without knowing their concrete type.          │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🔄 Polymorphism     │ display() overridden in TextPost & ImagePost.                        │
│                     │ Polymorphic calls via Post* pointer vectors.                         │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 💾 File Handling    │ fstream serializes all objects: users, posts, messages,               │
│                     │ friendship graphs → .dat files for persistence.                      │
└─────────────────────┴──────────────────────────────────────────────────────────────────────┘
```

---

## 🏗️ Class Architecture

```
                           ┌─────────────────────┐
                           │       Person        │  ← Abstract Base
                           │─────────────────────│
                           │ - name: string      │
                           │ - email: string     │
                           │ - password: string  │
                           │─────────────────────│
                           │ + login(): bool     │
                           │ + getProfile(): str │
                           └──────────┬──────────┘
                                      │
               ┌──────────────────────┴──────────────────────┐
               │                                             │
    ┌──────────▼──────────┐                      ┌──────────▼──────────┐
    │        Admin        │                      │        User         │
    │─────────────────────│                      │─────────────────────│
    │ + banUser()         │                      │ - followers: []     │
    │ + deletePost()      │                      │ - following: []     │
    │ + viewAllUsers()    │                      │ - posts: Post*[]    │
    └─────────────────────┘                      │─────────────────────│
                                                 │ + sendRequest()     │
                                                 │ + likePost()        │
                                                 │ + comment()         │
                                                 │ + sendMessage()     │
                                                 └─────────────────────┘

                           ┌─────────────────────┐
                           │     Post (ABC)      │  ← Abstract Base Class
                           │─────────────────────│
                           │ - postID: int       │
                           │ - timestamp: time_t │
                           │ - likes: int        │
                           │─────────────────────│
                           │ + display() = 0     │  ← Pure Virtual
                           │ + getLikes(): int   │
                           └──────────┬──────────┘
                                      │
               ┌──────────────────────┴──────────────────────┐
               │                                             │
    ┌──────────▼──────────┐                      ┌──────────▼──────────┐
    │      TextPost       │                      │      ImagePost      │
    │─────────────────────│                      │─────────────────────│
    │ - content: string   │                      │ - imagePath: string │
    │ - caption: string   │                      │ - caption: string   │
    │─────────────────────│                      │─────────────────────│
    │ + display() overr.  │                      │ + display() overr.  │
    └─────────────────────┘                      └─────────────────────┘

    ┌─────────────────────┐     ┌─────────────────────┐
    │      Message        │     │    Notification     │
    │─────────────────────│     │─────────────────────│
    │ - sender: User*     │     │ - type: NotifType   │
    │ - receiver: User*   │     │ - message: string   │
    │ - content: string   │     │ - isRead: bool      │
    │ - timestamp: time_t │     │─────────────────────│
    └─────────────────────┘     │ + markRead()        │
                                └─────────────────────┘
```

---

## 🗺️ Complete Development Roadmap

> **Total Duration:** 9 Weeks | **Team Size:** 6 | **Stack:** C++17 · WinForms · fstream

---

### Phase 1 — Foundation `Week 1-2`

> _Set up the project, define all classes, implement the data layer._

#### ✅ Week 1 — Project Setup & Class Design

**Goal:** Every team member working in the same repo with agreed-upon class contracts.

```
Tasks:
├── [ ] Create Visual Studio WinForms project (C++/CLI)
├── [ ] Initialize GitHub repo + set branching strategy
│         main ← stable releases only
│         dev  ← integration branch
│         feature/* ← one branch per feature
├── [ ] Write Person.h       — base class: name, email, password (private)
├── [ ] Write User.h         — derived from Person, stub all methods
├── [ ] Write Admin.h        — derived from Person, stub all methods
├── [ ] Write Post.h         — abstract class with pure virtual display()
├── [ ] Write TextPost.h / ImagePost.h  — concrete post classes
├── [ ] Write Message.h      — one-to-one message struct
├── [ ] Write Notification.h — notification struct with NotifType enum
└── [ ] Team: review and agree on ALL header files before coding bodies
```

> **Deliverable:** All `.h` files finalized. Every member compiled a "Hello World" WinForms window.

---

#### ✅ Week 2 — Core Logic (Console Only, No GUI)

**Goal:** All business logic working and testable in a console harness.

```
Tasks:
├── [ ] Implement Person.cpp    — constructor, login(), getProfile()
├── [ ] Implement User.cpp      — follow(), unfollow(), createPost(), sendRequest()
├── [ ] Implement Admin.cpp     — banUser(), deletePost(), viewAllUsers()
├── [ ] Implement TextPost.cpp  — override display() to print text content
├── [ ] Implement ImagePost.cpp — override display() to print image path + caption
├── [ ] Implement Message.cpp   — send(), receive(), display()
├── [ ] Write test main.cpp     — create users + posts, print feed to console
└── [ ] Code review session — all 6 members review each other's .cpp files
```

> **Deliverable:** Console prints "User created → Post created → Feed displayed" with zero crashes.

---

### Phase 2 — Core Features `Week 3-4`

> _Authentication system, file persistence, and the news feed engine._

#### ✅ Week 3 — Auth System + File Handling

**Goal:** Users can sign up, log in, and data persists across runs.

```
Tasks:
├── [ ] Implement AuthManager class
│   ├── signup(name, email, password)   → validates + writes to users.dat
│   ├── login(email, password)          → reads users.dat, validates hash
│   └── deleteAccount(userID)           → removes entry from users.dat
│
├── [ ] Implement FileManager class (fstream wrapper)
│   ├── saveUser(User&)         → serialize User to users.dat
│   ├── loadAllUsers()          → deserialize → vector<User*>
│   ├── savePost(Post&)         → serialize Post to posts.dat
│   ├── loadAllPosts()          → deserialize → vector<Post*>
│   └── saveGraph()             → serialize friendship adjacency list
│
├── [ ] Agree on serialization format (pipe-delimited, see Data section)
├── [ ] Hash passwords before storing (XOR cipher or custom hash)
└── [ ] Test: create 3 users → close app → reopen → all 3 still exist ✔
```

> **Deliverable:** Sign up, close the app, reopen, log in successfully with persisted data.

---

#### ✅ Week 4 — News Feed Engine

**Goal:** A logged-in user sees posts from people they follow, sorted by time.

```
Tasks:
├── [ ] Implement NewsFeed class
│   ├── generateFeed(User* currentUser)
│   │     → iterate following[], collect their posts, sort by timestamp DESC
│   ├── addPost(Post*)
│   └── removePost(postID)
│
├── [ ] Polymorphic rendering loop (KEY OOP MOMENT)
│         for (Post* p : feed) p->display();   ← runtime polymorphism!
│
├── [ ] Implement like(postID, userID) and unlike(postID, userID)
├── [ ] Implement addComment(postID, Comment{userID, text, timestamp})
└── [ ] Test: A follows B → B creates post → A's feed shows it; C not following B → C's feed is empty
```

> **Deliverable:** Feed correctly shows only posts from followed users, newest first.

---

### Phase 3 — Social Engine `Week 5-6`

> _Friend graph, private messaging, search, and notifications._

#### ✅ Week 5 — Friend Graph + Messaging

**Goal:** Friend requests work end-to-end. Messages send and persist.

```
Tasks:
├── [ ] Implement FriendGraph class
│   ├── sendRequest(fromID, toID)   → writes pending entry to friends.dat
│   ├── acceptRequest(requestID)    → updates status PENDING → ACCEPTED
│   │                                  adds to both users' followers[] list
│   ├── rejectRequest(requestID)    → removes entry from friends.dat
│   └── getFriends(userID)          → returns vector<User*>
│
├── [ ] Save/load friendship graph on startup (adjacency list format)
│
├── [ ] Implement MessageManager class
│   ├── sendMessage(from, to, content)         → appends to messages.dat
│   ├── getConversation(userA, userB)          → returns sorted vector<Message>
│   └── getInbox(userID)                       → all unique conversation partners
│
└── [ ] Test: A sends request → B accepts → both in each other's friends list ✔
         A sends "hey" to B → stored in messages.dat → B can read it ✔
```

> **Deliverable:** Complete friend request flow with persistent storage.

---

#### ✅ Week 6 — Search, Notifications & Admin Tools

**Goal:** Search works. Admins can moderate. Notifications fire on events.

```
Tasks:
├── [ ] Implement SearchEngine class
│   ├── searchUsers(keyword)  → scan name/username fields → vector<User*>
│   └── searchPosts(keyword)  → scan post content/captions → vector<Post*>
│
├── [ ] Implement NotificationManager
│   ├── notify(userID, type, message)  → appends to notifications.dat
│   ├── getUnread(userID)              → vector<Notification> (isRead == false)
│   └── markAllRead(userID)            → updates all entries in file
│
├── [ ] Hook notifications to system events:
│   ├── Someone likes your post     → notify(ownerID, LIKE,    "X liked your post")
│   ├── Someone comments            → notify(ownerID, COMMENT, "X commented: ...")
│   └── Friend request received     → notify(toID,    REQUEST, "X sent you a request")
│
├── [ ] Admin::banUser(userID)    — set isBanned=1 in users.dat, block their login
├── [ ] Admin::deletePost(postID) — remove entry from posts.dat + update feed
└── [ ] Admin::viewAllUsers()     — load and return full vector<User*>
```

> **Deliverable:** Searching "ali" returns User Ali. Liking a post triggers a notification entry.

---

### Phase 4 — GUI & Polish `Week 7-8`

> _Wire all backend logic to WinForms. Every feature gets a screen._

#### ✅ Week 7 — All WinForms Screens Built

**Goal:** Every feature has a working, navigable WinForms screen.

```
Screens to build:
│
├── LoginForm          — email field + password field + Login / Go to Signup buttons
├── SignupForm         — name, email, password, confirm password + Signup button
├── MainFeedForm       — scrollable Panel of posts; Like / Comment buttons per post
│                         + top nav: Home | Search | Messages | Notifications | Profile
├── ProfileForm        — avatar, bio, post grid, Follow/Unfollow button
│                         (shows own profile OR another user's profile)
├── CreatePostForm     — TabControl: "Text Post" tab | "Image Post" tab
│                         Text tab: multiline TextBox + Post button
│                         Image tab: OpenFileDialog picker + caption + Post button
├── MessagesForm       — ListBox of conversations (left) + chat Panel (right)
│                         TextBox at bottom + Send button
├── SearchForm         — SearchBar (TextBox + Search button) + results ListBox
│                         clicking a result opens ProfileForm
├── NotificationsForm  — ListBox of all notifications, unread ones highlighted
│                         "Mark all read" button at top
└── AdminDashboard     — TabControl: "Users" tab | "Posts" tab
                          Users: DataGridView with Ban button per row
                          Posts: DataGridView with Delete button per row

Architecture tips:
  - Use a single MainForm with a Panel container; swap child panels for navigation
    (avoids opening/closing windows constantly)
  - Keep ALL business logic OUT of Form classes — forms only call Manager methods
  - Pass the logged-in User* through forms using a Session singleton (Session::currentUser)
  - Admins get redirected to AdminDashboard on login instead of MainFeedForm
```

> **Deliverable:** Can click through every screen without exceptions or crashes.

---

#### ✅ Week 8 — Full Integration, Styling & Edge Cases

**Goal:** Data flows end-to-end between all screens. UI is clean and consistent.

```
Tasks:
├── [ ] LoginForm → AuthManager::login() → opens MainFeedForm or AdminDashboard
├── [ ] CreatePost → FileManager::savePost() → NewsFeed::addPost() → refresh feed panel
├── [ ] Like button → Post::addLike() → update file → NotificationManager::notify()
├── [ ] Comment → addComment() → update file → notify owner
├── [ ] MessagesForm → MessageManager::sendMessage() → persist → refresh conversation
├── [ ] SearchForm → SearchEngine → populate ListBox → click → open ProfileForm
├── [ ] NotificationsForm → NotificationManager::getUnread() → display + markRead
├── [ ] Admin Ban → AuthManager::banUser() → confirm dialog → refresh user list
├── [ ] Admin Delete Post → FileManager delete → NewsFeed remove → refresh
│
├── [ ] Styling pass:
│   ├── Consistent font (e.g. Segoe UI 10pt) across all forms
│   ├── Color scheme: choose primary + accent color + background
│   ├── Padding/margins on all controls (no elements touching edges)
│   └── Hover cursor on buttons (Cursors::Hand)
│
├── [ ] Edge cases to handle:
│   ├── Login with wrong password → show error label (don't crash)
│   ├── Duplicate email on signup → "Email already registered"
│   ├── Empty feed → show "No posts yet. Follow someone!"
│   ├── No messages → show "No conversations yet"
│   ├── Search returns nothing → show "No results found"
│   └── Corrupted .dat file → catch exception, show error, don't crash
│
└── [ ] Memory: delete all heap Post* and User* pointers before app closes
```

> **Deliverable:** Full end-to-end demo. Sign up → post → follow → message → admin bans.

---

### Phase 5 — Testing & Submission `Week 9`

> _Harden the system, write final docs, record demo, submit._

#### ✅ Week 9 — Testing, Docs & Submission

```
Testing checklist:
├── [ ] Auth:        wrong password, duplicate signup, banned user tries login
├── [ ] Feed:        follow/unfollow immediately affects feed output
├── [ ] FileManager: delete .dat file manually → app recreates it gracefully
├── [ ] Search:      empty query, special characters (!@#), no results case
├── [ ] Notifications: like/comment/request all fire correct notification type
├── [ ] Admin:       banning a user blocks their next login attempt
└── [ ] Memory:      run full session, check no access violations on exit

Documentation:
├── [ ] Write inline comments on all class headers (public method docs)
├── [ ] Finalize this README (add screenshots of each screen)
├── [ ] Record 3-5 min demo video (screen recording)
├── [ ] Write presentation slides (problem → design → demo → OOP concepts used)
└── [ ] GitHub: push final build → tag release v1.0 → share repo link
```

---

## 📁 File Structure

```
Connectify/
│
├── src/
│   ├── models/                  ← all class definitions
│   │   ├── Person.h / .cpp
│   │   ├── User.h / .cpp
│   │   ├── Admin.h / .cpp
│   │   ├── Post.h               ← abstract, no .cpp
│   │   ├── TextPost.h / .cpp
│   │   ├── ImagePost.h / .cpp
│   │   ├── Message.h / .cpp
│   │   └── Notification.h / .cpp
│   │
│   ├── managers/                ← business logic layer
│   │   ├── AuthManager.h / .cpp
│   │   ├── FileManager.h / .cpp
│   │   ├── NewsFeed.h / .cpp
│   │   ├── FriendGraph.h / .cpp
│   │   ├── MessageManager.h / .cpp
│   │   ├── SearchEngine.h / .cpp
│   │   └── NotificationManager.h / .cpp
│   │
│   ├── forms/                   ← WinForms UI layer (never put logic here)
│   │   ├── LoginForm.h / .cpp
│   │   ├── SignupForm.h / .cpp
│   │   ├── MainFeedForm.h / .cpp
│   │   ├── ProfileForm.h / .cpp
│   │   ├── CreatePostForm.h / .cpp
│   │   ├── MessagesForm.h / .cpp
│   │   ├── SearchForm.h / .cpp
│   │   ├── NotificationsForm.h / .cpp
│   │   └── AdminDashboard.h / .cpp
│   │
│   └── utils/
│       ├── Session.h            ← singleton: holds current User* across forms
│       └── Hasher.h             ← simple password hashing utility
│
├── data/                        ← auto-created on first run, never commit to git
│   ├── users.dat
│   ├── posts.dat
│   ├── friends.dat
│   ├── messages.dat
│   └── notifications.dat
│
├── assets/
│   └── images/                  ← uploaded post images stored here
│
├── Connectify.sln
├── .gitignore                   ← add /data/* and /Debug/* and /Release/*
└── README.md
```

---

## 💾 Data Storage Format

Each `.dat` file uses **pipe-delimited plain text** for easy `fstream` parsing with `getline` and `stringstream`.

**`users.dat`**

```
userID|name|email|hashedPassword|isBanned|profilePicPath
1001|Ali Abdullah|ali@email.com|a3f2b1c9|0|assets/pfp/1001.png
1002|Farzam Zeeshan|farzam@email.com|b7d4e2f1|0|
```

**`posts.dat`**

```
postID|ownerID|type|content|imagePath|timestamp|likes
2001|1001|TEXT|Hello Connectify!||1714900000|12
2002|1002|IMAGE|My first photo!|assets/images/img1.png|1714901000|8
```

**`friends.dat`**

```
fromID|toID|status
1001|1002|ACCEPTED
1001|1003|PENDING
```

**`messages.dat`**

```
msgID|senderID|receiverID|content|timestamp
3001|1001|1002|Hey Farzam!|1714902000
3002|1002|1001|Hey Ali!|1714902060
```

**`notifications.dat`**

```
notifID|ownerID|type|message|isRead|timestamp
4001|1002|LIKE|Ali liked your post|0|1714903000
4002|1001|REQUEST|Farzam sent you a friend request|1|1714904000
```

**Reading example (C++):**

```cpp
ifstream file("data/users.dat");
string line;
getline(file, line); // skip header
while (getline(file, line)) {
    stringstream ss(line);
    string id, name, email, hash, banned, pic;
    getline(ss, id,     '|');
    getline(ss, name,   '|');
    getline(ss, email,  '|');
    getline(ss, hash,   '|');
    getline(ss, banned, '|');
    getline(ss, pic,    '|');
    User* u = new User(stoi(id), name, email, hash);
    u->setBanned(banned == "1");
    users.push_back(u);
}
```

---

## 🚀 How to Build & Run

### Prerequisites

- **Visual Studio 2022** with "Desktop development with C++" workload
- **.NET Framework 4.8** (for WinForms/CLI)
- **Windows 10 or 11**

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/your-team/connectify.git
cd connectify

# 2. Open solution in Visual Studio
start Connectify.sln

# 3. Set build configuration
#    Build menu → Configuration Manager → Debug | x64

# 4. Build the solution
#    Ctrl + Shift + B

# 5. Run
#    F5  (with debugger)
#    Ctrl + F5  (without debugger, recommended for demo)
```

> The `/data` directory is auto-created on first launch.  
> Default admin credentials: `admin@connectify.com` / `admin123`

---

## 👥 Team

**Team 05 — Section BSE-2B**  
**Course:** Object Oriented Programming (OOP) — Spring 2025  
**Submitted To:** Mam Hina Iqbal & Syed Saad Ali  
**Institution:** NUCES — National University of Computer and Emerging Sciences

| Role           | Name             | Roll No  | Primary Responsibilities                  |
| -------------- | ---------------- | -------- | ----------------------------------------- |
| 👑 Team Leader | Ali Abdullah     | 25L-3022 | Architecture, AuthManager, Integration    |
| 👨‍💻 Member      | Farzam Zeeshan   | 25L-3049 | NewsFeed, Post classes, Polymorphism      |
| 👨‍💻 Member      | Mustafa Amir     | 25L-3107 | FriendGraph, MessageManager               |
| 👨‍💻 Member      | M. Arslan        | 25L-3080 | FileManager, Data Persistence             |
| 👨‍💻 Member      | Saad Amin        | 25L-3045 | WinForms GUI, SearchEngine                |
| 👨‍💻 Member      | Muhammad Khubaib | 25L-3026 | NotificationManager, Admin Tools, Testing |

---

## 📊 Progress Tracker

| Phase   | Description                          | Week | Status         |
| ------- | ------------------------------------ | ---- | -------------- |
| Phase 1 | Foundation — Classes & Project Setup | 1-2  | 🔲 Not Started |
| Phase 2 | Core — Auth, File I/O, News Feed     | 3-4  | 🔲 Not Started |
| Phase 3 | Social — Friends, Messages, Search   | 5-6  | 🔲 Not Started |
| Phase 4 | GUI — All WinForms Screens           | 7-8  | 🔲 Not Started |
| Phase 5 | Testing, Docs & Submission           | 9    | 🔲 Not Started |

> Replace `🔲 Not Started` with `🔄 In Progress` or `✅ Done` as you go!

---

<div align="center">

```
Built with ❤️ and way too many pointer errors
```

**NUCES — National University of Computer and Emerging Sciences**
_BS Software Engineering · 2nd Semester · Spring 2025_

</div>
