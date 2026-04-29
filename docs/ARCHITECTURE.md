# ARCHITECTURE.md — Connectify System Architecture

## Overview

Connectify is structured in three strict layers. No layer may depend on a layer above it.

```
┌─────────────────────────────────────────────────┐
│                  UI Layer (Qt6)                  │
│   MainWindow · 8 Pages · 2 Widgets · Session    │
├─────────────────────────────────────────────────┤
│              Manager Layer (Business Logic)      │
│  AuthManager · FileManager · NewsFeed ·         │
│  FriendGraph · MessageManager ·                 │
│  NotificationManager · SearchEngine             │
├─────────────────────────────────────────────────┤
│                 Model Layer (Pure C++)           │
│  Person · User · Admin · Post · TextPost ·      │
│  ImagePost · Custom Data Structures             │
└─────────────────────────────────────────────────┘
```

**Rule:** Models know nothing about managers. Managers know nothing about Qt. UI knows about both.

---

## Layer 1 — Model Layer

All domain classes. Zero Qt dependency. Zero STL containers.

### Inheritance Hierarchy

```
Person  (abstract)
├── User
└── Admin

Post  (abstract)
├── TextPost
└── ImagePost
```

### Custom Data Structures

Every collection in this project is a hand-written pointer-based structure.

```
CommentList          — singly-linked list       — Post comments
PostList             — singly-linked list       — User's posts (owns Post* nodes)
RequestList          — singly-linked list       — Friend requests
NotifList            — singly-linked list       — Notifications
MessageList          — doubly-linked list       — Messages (prev + next pointers)
LikeList             — raw int* dynamic array   — User IDs who liked a post
FollowArray          — raw int* dynamic array   — Follower / following ID lists
UserTable            — raw User** dynamic array — All registered users
FeedSnapshot         — raw Post** dynamic array — Rendered news feed (no ownership)
Array<T>             — generic raw T* array     — Reusable template
```

### Ownership Rules

| Structure | Owns its elements? |
|---|---|
| `PostList` | ✅ Yes — destructor deletes all `Post*` |
| `CommentList` | ✅ Yes — destructor deletes all `Comment*` |
| `NotifList` | ✅ Yes — destructor deletes all `NotifNode*` |
| `MessageList` | ✅ Yes — destructor deletes all `MsgNode*` |
| `RequestList` | ✅ Yes — destructor deletes all `RequestNode*` |
| `UserTable` | ❌ No — `AuthManager` owns User objects |
| `FeedSnapshot` | ❌ No — posts belong to their User |
| `LikeList` | ✅ Yes — owns `int[]` array |
| `FollowArray` | ✅ Yes — owns `int[]` array |

---

## Layer 2 — Manager Layer

Business logic. All singletons. No Qt includes except where signalling is needed.

### Manager Responsibilities

```
AuthManager
  ├── Owns all User* objects (UserTable)
  ├── Owns the single Admin* object
  ├── Login / signup / ban / delete account
  └── Delegates load/save to FileManager

FileManager
  ├── All fstream read/write
  ├── Pipe-delimited flat file format
  ├── Password hashing (djb2)
  └── ensureDataDir() creates data/ on first run

NewsFeed
  ├── Generates FeedSnapshot for a user
  ├── Collects posts from followed users
  └── Insertion-sorts by timestamp descending

FriendGraph
  ├── Owns RequestList
  ├── send / accept / reject requests
  └── Mutates User follow/follower arrays on accept

MessageManager
  ├── Owns MessageList
  ├── Sends messages (appends to file immediately)
  └── Returns raw MsgNode** arrays to callers

NotificationManager
  ├── Owns NotifList
  ├── Creates NotifNode and appends to file
  └── markAllRead() rewrites entire file

SearchEngine
  ├── Stateless — all static methods
  ├── Manual substring search (no std::string::find)
  └── Returns raw pointer arrays — caller must delete[]
```

### Singleton Access Pattern

Every manager is accessed via `instance()`:

```cpp
AuthManager::instance().login(email, password);
FileManager::instance().saveAllUsers(users);
NotificationManager::instance().notify(ownerID, NotifType::LIKE, msg);
```

---

## Layer 3 — UI Layer

Qt6 Widgets. Reads from managers, renders to screen, emits signals for navigation.

### Page Navigation — QStackedWidget

`MainWindow` holds a single `QStackedWidget`. Only one page is visible at a time.  
Navigation is done entirely through Qt signals and slots — pages never call each other directly.

```
MainWindow (QStackedWidget)
│
├── LoginPage    ──loginSuccess──►  showFeed()
│                ──adminLogin───►  showAdmin()
│                ──goToSignup───►  showSignup()
│
├── SignupPage   ──signupSuccess─►  showFeed()
│                ──goToLogin────►  showLogin()
│
├── FeedPage     ──goToProfile──►  showProfile(id)
│                ──goToMessages─►  showMessages()
│                ──goToSearch───►  showSearch()
│                ──goToNotifs───►  showNotifs()
│                ──logout───────►  showLogin()
│
├── ProfilePage  ──goBack───────►  showFeed()
├── MessagesPage ──goBack───────►  showFeed()
├── SearchPage   ──goBack───────►  showFeed()
│                ──openProfile──►  showProfile(id)
├── NotifsPage   ──goBack───────►  showFeed()
└── AdminPage    ──logout───────►  showLogin()
```

### Session Singleton

`Session` holds a raw `User*` pointer to the currently logged-in user.  
It is set by `LoginPage` / `SignupPage` and cleared on logout.  
All pages read from it to know who is logged in.

```cpp
User* me = Session::instance().current();
bool  isAdmin = Session::instance().isAdmin();
```

---

## Data Flow — Creating a Post

```
User types text → clicks "Post"
        │
        ▼
FeedPage::onCreatePost()
        │
        ├── NewsFeed::nextPostID(allUsers)   // find highest ID + 1
        ├── new TextPost(pid, ownerID, text)  // create Post object
        ├── me->addPost(post)                 // append to User's PostList
        └── FileManager::saveAllPosts(users)  // write to posts.dat
        │
        ▼
FeedPage::buildFeed()                        // re-render feed
```

## Data Flow — Login

```
User enters email + password → clicks "Log In"
        │
        ▼
LoginPage::onLogin()
        │
        ├── AuthManager::isAdminLogin()  → yes → Session::setAdmin(true) → showAdmin()
        └── AuthManager::login()
                │
                ├── FileManager::hashPassword(password)   // djb2 hash
                ├── UserTable linear search by email+hash
                ├── Check isBanned()
                └── returns User*
                        │
                        ▼
               Session::instance().set(user)
                        │
                        ▼
               emit loginSuccess() → MainWindow::showFeed()
```

---

## File Storage Architecture

All data lives in `data/` as pipe-delimited plain text. The directory is created automatically.

```
data/
├── users.dat              userID|name|email|hashedPassword|isBanned|profilePicPath
├── posts.dat              postID|ownerID|type|content|imagePath|timestamp|likes
├── friends.dat            fromID|toID|status
├── friend_requests.dat    requestID|fromID|toID|status
├── messages.dat           msgID|senderID|receiverID|content|timestamp
└── notifications.dat      notifID|ownerID|type|message|isRead|timestamp
```

`|` characters inside content fields are replaced with `;` on save and restored on load.

---

## Memory Management Summary

```
AuthManager destructor
  └── deletes every User* in UserTable
        └── ~User() → ~PostList() → deletes every Post* in chain
                          └── ~Post() → ~CommentList() → deletes every Comment*
                                     → ~LikeList()    → deletes int[]

FriendGraph destructor
  └── ~RequestList() → deletes every RequestNode*

MessageManager destructor
  └── ~MessageList() → deletes every MsgNode*

NotificationManager destructor
  └── ~NotifList() → deletes every NotifNode*
```

All heap-allocated raw pointer arrays returned to callers  
(from `SearchEngine`, `MessageManager`, `NotificationManager`, `FriendGraph`)  
must be `delete[]`-d by the caller. This is documented on each method.
