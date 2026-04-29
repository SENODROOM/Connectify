# Connectify

A social media desktop application built in **C++17** with **Qt6**, demonstrating core Object-Oriented Programming principles through custom pointer-based data structures — no STL containers used.

---

## Features

| Feature | Description |
|---|---|
| 👤 Auth | Register, login, logout with hashed passwords |
| 📰 Feed | News feed showing posts from followed users |
| ✍️ Posts | Create and view text posts |
| ❤️ Likes | Like and unlike posts |
| 👥 Follow | Follow and unfollow other users |
| 🔍 Search | Search users and posts by keyword |
| 💬 Messages | Direct messaging between users |
| 🔔 Notifications | Like and follow notifications |
| 🛡️ Admin | Ban users, delete posts, view all accounts |
| 💾 Persistence | All data saved to plain text `.dat` files |

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| GUI Framework | Qt 6 (Widgets) |
| Build System | CMake 3.16+ |
| Compiler | GCC 13+ (MinGW on Windows) |
| Data Storage | Custom pipe-delimited flat files |

---

## OOP Concepts Demonstrated

- **Inheritance** — `Person → User / Admin`, `Post → TextPost / ImagePost`
- **Polymorphism** — virtual `display()` and `getRole()` on base classes
- **Encapsulation** — all data private, exposed through getters/setters
- **Abstract Classes** — `Person` and `Post` have pure virtual methods
- **Custom Data Structures** — singly/doubly linked lists, raw pointer arrays (no STL containers)
- **Singleton Pattern** — `AuthManager`, `FileManager`, `Session`, `NotificationManager`

---

## Project Structure

```
Connectify/
├── CMakeLists.txt
├── run.bat                        ← double-click to build & run (Windows)
├── data/                          ← auto-generated flat file database
│   ├── users.dat
│   ├── posts.dat
│   ├── friends.dat
│   ├── friend_requests.dat
│   ├── messages.dat
│   └── notifications.dat
└── src/
    ├── main.cpp
    ├── models/                    ← pure C++ domain classes
    │   ├── Array.h                ← generic dynamic pointer array
    │   ├── Person.h / .cpp        ← abstract base
    │   ├── User.h / .cpp
    │   ├── Admin.h / .cpp
    │   ├── UserTable.h            ← raw User** array
    │   ├── Post.h / .cpp          ← abstract base
    │   ├── TextPost.h / .cpp
    │   ├── ImagePost.h / .cpp
    │   ├── PostList.h             ← singly-linked list of Post*
    │   ├── Comment.h              ← singly-linked list of comments
    │   ├── LikeList.h             ← raw int* array of user IDs
    │   ├── FollowArray.h          ← raw int* array for follow graph
    │   ├── Message.h              ← doubly-linked list of messages
    │   └── Notification.h         ← singly-linked list of notifications
    ├── managers/                  ← business logic singletons
    │   ├── FileManager.h / .cpp   ← all file I/O
    │   ├── AuthManager.h / .cpp   ← login, signup, ban
    │   ├── NewsFeed.h / .cpp      ← feed generation + sorting
    │   ├── FriendGraph.h / .cpp   ← follow requests
    │   ├── MessageManager.h / .cpp
    │   ├── NotificationManager.h / .cpp
    │   └── SearchEngine.h / .cpp
    ├── ui/                        ← Qt6 UI layer
    │   ├── Session.h / .cpp       ← logged-in user singleton
    │   ├── MainWindow.h / .cpp    ← QStackedWidget router
    │   ├── pages/
    │   │   ├── LoginPage
    │   │   ├── SignupPage
    │   │   ├── FeedPage
    │   │   ├── ProfilePage
    │   │   ├── MessagesPage
    │   │   ├── SearchPage
    │   │   ├── NotifsPage
    │   │   └── AdminPage
    │   └── widgets/
    │       ├── PostCard           ← reusable post display widget
    │       └── ChatBubble         ← message bubble widget
    └── resources/
        ├── style.qss              ← global Qt stylesheet
        └── resources.qrc
```

---

## Quick Start

### Prerequisites

- MSYS2 with `mingw-w64-ucrt-x86_64-gcc`, `qt6-base`, `qt6-tools` installed
- CMake 3.16+
- `C:\msys64\ucrt64\bin` on your system PATH

### First-time build

```powershell
cd C:\Programming\Connectify
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
.\Connectify.exe
```

### Every time after that

```powershell
# From project root — just double-click run.bat or:
C:\Programming\Connectify\run.bat
```

---

## Default Credentials

| Role | Email | Password |
|---|---|---|
| Admin | `admin@connectify.com` | `admin123` |
| User | register via Sign Up screen | your choice |

---

## Resetting Data

Delete any `.dat` file inside `data\` — it will be recreated empty on next launch.  
To reset everything: `Remove-Item C:\Programming\Connectify\build\data\*.dat`

---

## License

OOP Course Project — BSE-2B
