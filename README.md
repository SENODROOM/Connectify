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

_Built with Object-Oriented Programming · Qt6 GUI · File Persistence_

---

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-6-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Status](https://img.shields.io/badge/Status-In%20Development-22c55e?style=for-the-badge)
![Team](https://img.shields.io/badge/Team-05%20%7C%20BSE--2B-ef4444?style=for-the-badge)

</div>

---

## 📋 Table of Contents

- [About](#-about)
- [Features](#-features)
- [OOP Concepts](#-oop-concepts)
- [Class Architecture](#-class-architecture)
- [Complete Development Roadmap](#-complete-development-roadmap)
  - [Phase 0 — Qt Setup](#phase-0--qt-setup-before-week-1)
  - [Phase 1 — Foundation](#phase-1--foundation-week-1-2)
  - [Phase 2 — Core Features](#phase-2--core-features-week-3-4)
  - [Phase 3 — Social Engine](#phase-3--social-engine-week-5-6)
  - [Phase 4 — Qt GUI & Polish](#phase-4--qt-gui--polish-week-7-8)
  - [Phase 5 — Testing & Submission](#phase-5--testing--submission-week-9)
- [Qt Concepts Used](#-qt-concepts-used-in-connectify)
- [File Structure](#-file-structure)
- [Data Storage Format](#-data-storage-format)
- [How to Build & Run](#-how-to-build--run)
- [Team](#-team)

---

## 📖 About

**Connectify** is a cross-platform C++ application built with **Qt6** that simulates a modern social networking platform. It combines rock-solid OOP principles with Qt's powerful signal-slot architecture, custom widgets, and QSS styling to deliver a professional, native-feeling GUI on Windows, macOS, and Linux.

> _"Bridging theoretical OOP concepts with practical software engineering."_

The system uses **Role-Based Access Control (RBAC)** via `Admin` and `User` classes derived from a common `Person` base, and leverages Qt's `QStackedWidget` for seamless screen navigation — no messy window management required.

---

## ✨ Features

| #   | Feature                     | Description                             | OOP / Qt Concept             |
| --- | --------------------------- | --------------------------------------- | ---------------------------- |
| 1   | 🔐 **User Management**      | Secure Login, Signup & Account Deletion | Encapsulation, `QLineEdit`   |
| 2   | 🛡️ **Admin Command Center** | View users, delete posts, ban accounts  | Inheritance + RBAC           |
| 3   | 📰 **Dynamic News Feed**    | Real-time posts from followed users     | Polymorphism, `QScrollArea`  |
| 4   | 🖼️ **Multimedia Posts**     | Text & image-based posts                | Abstraction, `QLabel` pixmap |
| 5   | 🤝 **Social Graph**         | Friend requests, followers, connections | Data Structures              |
| 6   | ❤️ **Engagement Module**    | Likes & Comments on posts               | Signals & Slots              |
| 7   | 💬 **Private Messaging**    | Secure one-to-one user chat             | `QListWidget`, `QTextEdit`   |
| 8   | 🔍 **Search & Discovery**   | Search users or posts by keyword        | `QLineEdit`, filtering       |
| 9   | 💾 **Persistent Storage**   | Full save/load via `fstream`            | File Handling                |
| 10  | 🔔 **Notification Engine**  | Real-time alerts for all activity       | Qt Signals & Slots           |

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
│                     │ QWidget → all custom Qt screens (LoginPage, FeedPage, etc.)          │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🎭 Abstraction      │ Abstract Post class exposes display() interface.                     │
│                     │ News Feed renders posts without knowing their concrete type.          │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 🔄 Polymorphism     │ display() overridden in TextPost & ImagePost.                        │
│                     │ Polymorphic calls via Post* pointer vectors.                         │
│                     │ Qt virtual paintEvent() for custom widget rendering.                 │
├─────────────────────┼──────────────────────────────────────────────────────────────────────┤
│ 💾 File Handling    │ fstream serializes all objects → .dat files for persistence.         │
│                     │ QFile / QTextStream usable as Qt-native alternative.                 │
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
    │─────────────────────│                      │ - caption: string   │
    │ + display() overr.  │                      │─────────────────────│
    └─────────────────────┘                      │ + display() overr.  │
                                                 └─────────────────────┘

    ┌─────────────────────┐     ┌─────────────────────┐
    │      Message        │     │    Notification     │
    │─────────────────────│     │─────────────────────│
    │ - sender: User*     │     │ - type: NotifType   │
    │ - receiver: User*   │     │ - message: string   │
    │ - content: string   │     │ - isRead: bool      │
    │ - timestamp: time_t │     └─────────────────────┘
    └─────────────────────┘

    ┌─────────────────────────────────────────────────────────┐
    │                   Qt GUI Layer                          │
    │─────────────────────────────────────────────────────────│
    │  MainWindow (QMainWindow)                               │
    │    └── QStackedWidget                                   │
    │          ├── LoginPage    : public QWidget              │
    │          ├── SignupPage   : public QWidget              │
    │          ├── FeedPage     : public QWidget              │
    │          ├── ProfilePage  : public QWidget              │
    │          ├── MessagesPage : public QWidget              │
    │          ├── SearchPage   : public QWidget              │
    │          ├── NotifsPage   : public QWidget              │
    │          └── AdminPage    : public QWidget              │
    └─────────────────────────────────────────────────────────┘
```

---

## 🗺️ Complete Development Roadmap

> **Total Duration:** 9 Weeks | **Team Size:** 6 | **Stack:** C++17 · Qt6 · fstream

---

### Phase 0 — Qt Setup `Before Week 1`

> _Everyone on the team must complete this before coding begins._

```
Every team member:
├── [ ] Install Qt 6.x (Community/Open Source — free)
│         Download: https://www.qt.io/download-open-source
│         During install, select:
│           ✔ Qt 6.x.x → Desktop (MinGW or MSVC depending on your compiler)
│           ✔ Qt Creator IDE
│           ✔ CMake (bundled)
│
├── [ ] Launch Qt Creator → New Project → Qt Widgets Application
│         Project name: Connectify
│         Build system: CMake
│         Class name: MainWindow   Base class: QMainWindow
│
├── [ ] Confirm the default project builds and runs (empty window appears)
│
├── [ ] Learn these Qt basics before Phase 1 (30 min each):
│         • Signals & Slots — how Qt widgets communicate
│           https://doc.qt.io/qt-6/signalsandslots.html
│         • QStackedWidget — swapping pages without opening new windows
│           https://doc.qt.io/qt-6/qstackedwidget.html
│         • Qt Stylesheet (QSS) — CSS-like styling for Qt widgets
│           https://doc.qt.io/qt-6/stylesheet.html
│
└── [ ] Initialize GitHub repo, push the empty Qt project, everyone pulls
```

---

### Phase 1 — Foundation `Week 1-2`

> _Define all classes, implement the data layer, no GUI yet._

#### ✅ Week 1 — Project Setup & Class Design

**Goal:** Agreed-upon class contracts. Every header file finalized before bodies are written.

```
Tasks:
├── [ ] Organize project into folders (see File Structure section)
├── [ ] Write Person.h       — base: name, email, password (private)
├── [ ] Write User.h         — derived from Person, stub all methods
├── [ ] Write Admin.h        — derived from Person, stub all methods
├── [ ] Write Post.h         — abstract class with pure virtual display()
├── [ ] Write TextPost.h / ImagePost.h  — concrete post classes
├── [ ] Write Message.h      — one-to-one message struct
├── [ ] Write Notification.h — NotifType enum + notification struct
└── [ ] Team review: ALL headers agreed before anyone writes a .cpp
```

> **Deliverable:** All `.h` files committed. Everyone has compiled the empty Qt window.

---

#### ✅ Week 2 — Core Logic (No GUI)

**Goal:** All business logic working and testable without touching Qt.

```
Tasks:
├── [ ] Implement Person.cpp    — constructor, login(), getProfile()
├── [ ] Implement User.cpp      — follow(), unfollow(), createPost()
├── [ ] Implement Admin.cpp     — banUser(), deletePost(), viewAllUsers()
├── [ ] Implement TextPost.cpp  — override display() → print text content
├── [ ] Implement ImagePost.cpp — override display() → print image path + caption
├── [ ] Implement Message.cpp   — send(), receive(), display()
├── [ ] Write a test main() (plain console, no Qt) that:
│         creates 2 users → user A follows B → B posts → print A's feed
└── [ ] Code review: all 6 members review each other's implementations
```

> **Deliverable:** `g++ -o test main.cpp *.cpp && ./test` prints feed correctly.

---

### Phase 2 — Core Features `Week 3-4`

> _Authentication, file persistence, and the news feed engine._

#### ✅ Week 3 — Auth System + File Handling

**Goal:** Users can sign up, log in, and their data survives a restart.

```
Tasks:
├── [ ] Implement AuthManager class
│   ├── signup(name, email, password)   → validate → save to users.dat
│   ├── login(email, password)          → read users.dat → return User* or nullptr
│   └── deleteAccount(userID)           → remove from users.dat
│
├── [ ] Implement FileManager class (fstream wrapper)
│   ├── saveUser(User&)          → serialize pipe-delimited line to users.dat
│   ├── loadAllUsers()           → deserialize → vector<User*>
│   ├── savePost(Post&)          → serialize to posts.dat
│   ├── loadAllPosts()           → deserialize → vector<Post*>
│   └── saveGraph()              → serialize friendship adjacency list
│
├── [ ] Agree on pipe-delimited format (see Data Storage section)
├── [ ] Hash passwords before storing (simple XOR or djb2 hash is fine)
└── [ ] Test: create 3 users → close → reopen → all 3 load from file ✔
```

> **Deliverable:** Persistence works end-to-end in console test. No GUI yet.

---

#### ✅ Week 4 — News Feed Engine

**Goal:** A user sees posts from people they follow, sorted newest first.

```
Tasks:
├── [ ] Implement NewsFeed class
│   ├── generateFeed(User* currentUser)
│   │     → iterate following[], collect posts, sort by timestamp DESC
│   ├── addPost(Post*)
│   └── removePost(postID)
│
├── [ ] Polymorphic rendering (KEY OOP MOMENT)
│         for (Post* p : feed) p->display();   ← runtime polymorphism!
│
├── [ ] Implement like(postID, userID) and unlike(postID, userID)
├── [ ] Implement addComment(postID, Comment{userID, text, timestamp})
└── [ ] Test: A follows B → B posts → A's feed has it; C not following → empty ✔
```

> **Deliverable:** Feed logic is correct and tested in console.

---

### Phase 3 — Social Engine `Week 5-6`

> _Friend graph, private messaging, search, notifications._

#### ✅ Week 5 — Friend Graph + Messaging

**Goal:** Friend requests and private messages work end-to-end with persistence.

```
Tasks:
├── [ ] Implement FriendGraph class
│   ├── sendRequest(fromID, toID)    → write PENDING entry to friends.dat
│   ├── acceptRequest(requestID)     → update to ACCEPTED, add to followers[]
│   ├── rejectRequest(requestID)     → remove entry
│   └── getFriends(userID)           → return vector<User*>
│
├── [ ] Implement MessageManager class
│   ├── sendMessage(from, to, content)      → append to messages.dat
│   ├── getConversation(userA, userB)       → return sorted vector<Message>
│   └── getInbox(userID)                    → all unique conversation partners
│
└── [ ] Test: A sends request → B accepts → both in friends list ✔
         A sends "hey" to B → persisted → B loads it on restart ✔
```

> **Deliverable:** Complete friend + message flow with file persistence.

---

#### ✅ Week 6 — Search, Notifications & Admin

**Goal:** Search, admin tools, and notifications all wired up.

```
Tasks:
├── [ ] Implement SearchEngine
│   ├── searchUsers(keyword)   → scan name/username → vector<User*>
│   └── searchPosts(keyword)   → scan content/captions → vector<Post*>
│
├── [ ] Implement NotificationManager
│   ├── notify(userID, type, message)  → append to notifications.dat
│   ├── getUnread(userID)              → vector<Notification>
│   └── markAllRead(userID)            → update file
│
├── [ ] Hook notifications to events:
│   ├── Post liked         → notify(owner, LIKE,    "X liked your post")
│   ├── Comment added      → notify(owner, COMMENT, "X commented: ...")
│   └── Friend request     → notify(target, REQUEST, "X sent you a request")
│
├── [ ] Admin::banUser(userID)    — set isBanned=1 in users.dat
├── [ ] Admin::deletePost(postID) — remove from posts.dat + feed
└── [ ] Admin::viewAllUsers()     — return full vector<User*>
```

> **Deliverable:** Full backend complete. All logic testable without GUI.

---

### Phase 4 — Qt GUI & Polish `Week 7-8`

> _Wire all backend logic to Qt widgets. Build every screen._

#### ✅ Week 7 — Qt Screens & Navigation

**Goal:** Every feature has a working Qt widget. Navigation via `QStackedWidget`.

```
MainWindow setup (do this first):
├── [ ] Create MainWindow : QMainWindow
│   ├── Add a QStackedWidget as the central widget
│   ├── Create a Session singleton → holds User* currentUser
│   └── Add a top nav bar (QToolBar or custom QWidget with QPushButton row)
│         Buttons: Home | Search | Messages | Notifications | Profile | Logout

Screens to build (each is a QWidget subclass):
│
├── LoginPage
│   ├── QLineEdit  → email
│   ├── QLineEdit  → password (setEchoMode(QLineEdit::Password))
│   ├── QPushButton "Login"   → calls AuthManager::login()
│   │     on success: Session::set(user) → stack->setCurrentWidget(feedPage)
│   │     on failure: QLabel shows "Incorrect email or password"
│   └── QPushButton "Sign Up" → stack->setCurrentWidget(signupPage)
│
├── SignupPage
│   ├── QLineEdit → name, email, password, confirm password
│   ├── QPushButton "Create Account" → AuthManager::signup()
│   └── QPushButton "Back to Login"
│
├── FeedPage
│   ├── QScrollArea containing a QVBoxLayout of PostCard widgets
│   │     PostCard is a custom QWidget: avatar | name | content | Like btn | Comment btn
│   │     Like QPushButton emits signal → slot calls NewsFeed::like() → refreshFeed()
│   └── QPushButton "New Post" → opens CreatePostDialog (QDialog)
│
├── CreatePostDialog (QDialog, not a full page)
│   ├── QTabWidget: "Text Post" tab | "Image Post" tab
│   │     Text tab: QTextEdit + QPushButton "Post"
│   │     Image tab: QPushButton "Choose Image" (QFileDialog::getOpenFileName)
│   │                + QLineEdit caption + QPushButton "Post"
│   └── on Post: creates TextPost* or ImagePost* → FileManager::savePost() → emit postCreated()
│
├── ProfilePage
│   ├── QLabel (avatar pixmap) + QLabel (name/bio)
│   ├── QPushButton "Follow" / "Unfollow" (hidden on own profile)
│   ├── QGridLayout of post thumbnails
│   └── Reused for own profile AND other users' profiles
│
├── MessagesPage
│   ├── QListWidget (left) → conversation partners
│   │     clicking a name loads that conversation
│   ├── QScrollArea (right) → chat bubbles (custom QWidget per message)
│   ├── QLineEdit + QPushButton "Send"
│   └── Send: MessageManager::sendMessage() → append bubble to scroll area
│
├── SearchPage
│   ├── QLineEdit + QPushButton "Search"
│   │     on search: SearchEngine::searchUsers() + searchPosts()
│   ├── QListWidget → user results (click → open ProfilePage)
│   └── QListWidget → post results
│
├── NotifsPage
│   ├── QListWidget → all notifications (unread bold)
│   └── QPushButton "Mark all read" → NotificationManager::markAllRead()
│
└── AdminPage (only shown when Admin logs in)
    ├── QTabWidget: "Users" tab | "Posts" tab
    │     Users tab: QTableWidget (name | email | banned) + "Ban" QPushButton per row
    └───── Posts tab: QTableWidget (author | content preview) + "Delete" per row
```

> **Deliverable:** Can navigate all screens. No crashes. Data displays correctly.

---

#### ✅ Week 8 — Full Integration, QSS Styling & Edge Cases

**Goal:** Everything connected. App looks polished. All edge cases handled.

```
Integration tasks:
├── [ ] LoginPage  → AuthManager → Session::set() → switch to FeedPage or AdminPage
├── [ ] FeedPage   → NewsFeed::generateFeed() → populate PostCard widgets
├── [ ] Like btn   → NewsFeed::like() → update file → NotificationManager::notify()
│                    → emit likeUpdated(postID) → PostCard refreshes count
├── [ ] CreatePost → FileManager::savePost() → emit postCreated() → FeedPage refreshes
├── [ ] Messages   → MessageManager::sendMessage() → append bubble → save to file
├── [ ] Search     → SearchEngine → populate QListWidgets
├── [ ] Notifs     → NotificationManager::getUnread() → populate list
├── [ ] Admin ban  → AuthManager::banUser() → QMessageBox::question confirm → refresh table
│
QSS Styling (style.qss loaded in main.cpp):
├── [ ] QMainWindow, QWidget → background color scheme
├── [ ] QPushButton → rounded corners, hover/pressed states
│         QPushButton { border-radius: 8px; padding: 8px 16px; }
│         QPushButton:hover { background-color: #0056b3; }
├── [ ] QLineEdit → clean border, focus highlight
├── [ ] PostCard custom widget → card shadow effect via border
├── [ ] "Like" button → changes color when liked (setProperty + style().unpolish/polish)
│
Edge cases:
├── [ ] Wrong password → show red QLabel, don't crash
├── [ ] Duplicate email signup → "Email already registered"
├── [ ] Empty feed → centered QLabel "No posts yet. Follow someone!"
├── [ ] No messages → "No conversations yet"
├── [ ] Corrupted .dat → catch std::exception, show QMessageBox::warning
└── [ ] Memory: call qDeleteAll() + clear() on all Post* / User* vectors at shutdown
```

> **Deliverable:** Full end-to-end demo. Signup → post → follow → message → admin bans.

---

### Phase 5 — Testing & Submission `Week 9`

> _Harden, document, record demo, submit._

#### ✅ Week 9 — Final Testing & Docs

```
Testing checklist:
├── [ ] Auth:    wrong password, banned user login, duplicate signup
├── [ ] Feed:    follow/unfollow immediately updates feed content
├── [ ] Files:   delete .dat manually → app recreates gracefully on next run
├── [ ] Search:  empty query, special characters, no-results case
├── [ ] Notifs:  like/comment/request each fire the correct notification type
├── [ ] Admin:   ban blocks the user's very next login attempt
└── [ ] Memory:  full session run → no Qt warnings about deleted objects

Documentation:
├── [ ] Inline comments on all class public methods
├── [ ] Finalize README: add screenshots of each Qt screen
├── [ ] Record 3–5 min demo video (screen recording, OBS or ShareX)
├── [ ] Presentation slides: problem → OOP design → Qt architecture → live demo
└── [ ] GitHub: final push → tag release v1.0 → share link with instructor
```

---

## ⚡ Qt Concepts Used in Connectify

```
┌────────────────────────┬──────────────────────────────────────────────────────────────┐
│   Qt Concept           │   Where It's Used                                            │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ Signals & Slots        │ Like button emits likeClicked(postID) → feed slot updates    │
│                        │ New post created → FeedPage slot refreshes the scroll area   │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QStackedWidget         │ MainWindow holds all pages; setCurrentWidget() for nav       │
│                        │ No window opens/closes — just page swaps                     │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ Custom QWidget         │ PostCard: a self-contained widget for each post in the feed  │
│                        │ ChatBubble: a message widget for the conversation panel      │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QScrollArea            │ News feed and chat window scroll to show all content         │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QFileDialog            │ Image picker in CreatePostDialog for ImagePost uploads       │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QSS Stylesheets        │ Global style.qss loaded at startup for consistent theming    │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QTableWidget           │ Admin panel: sortable tables of users and posts              │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QMessageBox            │ Confirmation dialogs for ban/delete; error dialogs for auth  │
├────────────────────────┼──────────────────────────────────────────────────────────────┤
│ QLabel + QPixmap       │ Displaying profile pictures and post images                  │
└────────────────────────┴──────────────────────────────────────────────────────────────┘
```

**Signals & Slots example** (the core Qt OOP pattern):

```cpp
// In PostCard.h
signals:
    void likeClicked(int postID);
    void commentClicked(int postID);

// In PostCard.cpp — wire the button
connect(likeBtn, &QPushButton::clicked, this, [this]() {
    emit likeClicked(postID);
});

// In FeedPage.cpp — listen to the signal
connect(card, &PostCard::likeClicked, this, &FeedPage::onLikeClicked);

void FeedPage::onLikeClicked(int postID) {
    newsFeed.like(postID, Session::current()->getUserID());
    FileManager::instance().savePost(*newsFeed.getPost(postID));
    NotificationManager::instance().notify(/* owner */, LIKE, "X liked your post");
    refreshFeed(); // rebuild PostCard list
}
```

---

## 📁 File Structure

```
Connectify/
│
├── CMakeLists.txt              ← Qt's build system (replaces .pro file in Qt6)
│
├── src/
│   ├── main.cpp                ← QApplication + load style.qss + show MainWindow
│   │
│   ├── models/                 ← pure C++ classes, zero Qt dependency
│   │   ├── Person.h / .cpp
│   │   ├── User.h / .cpp
│   │   ├── Admin.h / .cpp
│   │   ├── Post.h              ← abstract, no .cpp
│   │   ├── TextPost.h / .cpp
│   │   ├── ImagePost.h / .cpp
│   │   ├── Message.h / .cpp
│   │   └── Notification.h / .cpp
│   │
│   ├── managers/               ← business logic, also zero Qt dependency
│   │   ├── AuthManager.h / .cpp
│   │   ├── FileManager.h / .cpp
│   │   ├── NewsFeed.h / .cpp
│   │   ├── FriendGraph.h / .cpp
│   │   ├── MessageManager.h / .cpp
│   │   ├── SearchEngine.h / .cpp
│   │   └── NotificationManager.h / .cpp
│   │
│   ├── ui/                     ← all Qt-dependent code lives here
│   │   ├── MainWindow.h / .cpp      ← QMainWindow + QStackedWidget
│   │   ├── Session.h                ← singleton: User* currentUser
│   │   ├── pages/
│   │   │   ├── LoginPage.h / .cpp
│   │   │   ├── SignupPage.h / .cpp
│   │   │   ├── FeedPage.h / .cpp
│   │   │   ├── ProfilePage.h / .cpp
│   │   │   ├── MessagesPage.h / .cpp
│   │   │   ├── SearchPage.h / .cpp
│   │   │   ├── NotifsPage.h / .cpp
│   │   │   └── AdminPage.h / .cpp
│   │   └── widgets/
│   │       ├── PostCard.h / .cpp    ← custom QWidget for one post
│   │       ├── ChatBubble.h / .cpp  ← custom QWidget for one message
│   │       └── NavBar.h / .cpp      ← top navigation bar widget
│   │
│   └── resources/
│       ├── style.qss               ← global Qt stylesheet
│       └── resources.qrc           ← Qt resource file (icons, fonts)
│
├── data/                           ← auto-created on first run (add to .gitignore)
│   ├── users.dat
│   ├── posts.dat
│   ├── friends.dat
│   ├── messages.dat
│   └── notifications.dat
│
├── assets/
│   ├── icons/                      ← UI icons (SVG or PNG)
│   └── images/                     ← user-uploaded post images
│
└── .gitignore                      ← include: /data/ /build/ *.user *.pro.user
```

**`CMakeLists.txt` starter:**

```cmake
cmake_minimum_required(VERSION 3.16)
project(Connectify VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

qt_add_executable(Connectify ${SOURCES} ${HEADERS} src/resources/resources.qrc)
target_link_libraries(Connectify PRIVATE Qt6::Widgets)
```

---

## 💾 Data Storage Format

Each `.dat` file uses **pipe-delimited plain text** for easy `std::fstream` parsing.

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
4002|1001|REQUEST|Farzam sent a friend request|1|1714904000
```

**Parsing example (C++ with fstream):**

```cpp
ifstream file("data/users.dat");
string line;
getline(file, line); // skip header row
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

- **Qt 6.x** (Community Edition — free): https://www.qt.io/download-open-source
  - During install select: Qt 6.x → Desktop (MinGW 64-bit) + Qt Creator
- **CMake 3.16+** (bundled with Qt installer)
- **Compiler:** MinGW (Windows) or GCC/Clang (Linux/macOS)

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/your-team/connectify.git
cd connectify

# Option A — Qt Creator (recommended for beginners)
#   File → Open File or Project → select CMakeLists.txt
#   Click the Run button (Ctrl+R)

# Option B — Command line
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/mingw_64
cmake --build . --parallel

# Run
./Connectify          # Linux/macOS
Connectify.exe        # Windows
```

> The `/data` directory is auto-created on first launch.  
> Default admin: `admin@connectify.com` / `admin123`

### Loading the QSS stylesheet in `main.cpp`

```cpp
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(styleFile.readAll());
    }

    MainWindow window;
    window.show();
    return app.exec();
}
```

---

## 👥 Team

**Team 05 — Section BSE-2B**  
**Course:** Object Oriented Programming (OOP) — Spring 2025  
**Submitted To:** Mam Hina Iqbal & Syed Saad Ali  
**Institution:** NUCES — National University of Computer and Emerging Sciences

| Role           | Name             | Roll No  | Primary Responsibilities                  |
| -------------- | ---------------- | -------- | ----------------------------------------- |
| 👑 Team Leader | Ali Abdullah     | 25L-3022 | Architecture, AuthManager, Qt MainWindow  |
| 👨‍💻 Member      | Farzam Zeeshan   | 25L-3049 | NewsFeed, Post classes, FeedPage          |
| 👨‍💻 Member      | Mustafa Amir     | 25L-3107 | FriendGraph, MessageManager, MessagesPage |
| 👨‍💻 Member      | M. Arslan        | 25L-3080 | FileManager, Data Persistence             |
| 👨‍💻 Member      | Saad Amin        | 25L-3045 | QSS Styling, SearchPage, NotifsPage       |
| 👨‍💻 Member      | Muhammad Khubaib | 25L-3026 | NotificationManager, AdminPage, Testing   |

---

## 📊 Progress Tracker

| Phase   | Description                        | Week       | Status         |
| ------- | ---------------------------------- | ---------- | -------------- |
| Phase 0 | Qt Installation & Setup            | Pre-Week 1 | 🔲 Not Started |
| Phase 1 | Foundation — Classes & Project     | 1-2        | 🔲 Not Started |
| Phase 2 | Core — Auth, File I/O, NewsFeed    | 3-4        | 🔲 Not Started |
| Phase 3 | Social — Friends, Messages, Search | 5-6        | 🔲 Not Started |
| Phase 4 | Qt GUI — All Pages + Styling       | 7-8        | 🔲 Not Started |
| Phase 5 | Testing, Docs & Submission         | 9          | 🔲 Not Started |

> Update `🔲 Not Started` → `🔄 In Progress` → `✅ Done` as you go!

---

<div align="center">

```
Built with ❤️ and way too many signal-slot connections
```

**NUCES — National University of Computer and Emerging Sciences**
_BS Software Engineering · 2nd Semester · Spring 2025_

</div>
