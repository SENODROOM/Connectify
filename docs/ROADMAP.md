# ROADMAP.md — Future Features Plan

Current status: **v1.0 — Core complete**

---

## v1.1 — Polish & Stability

These are small improvements that need no new OOP design.

| Feature | File(s) to change | Effort |
|---|---|---|
| Show commenter names below each post | `PostCard.cpp` | Small |
| Profile picture display in UI | `ProfilePage.cpp` | Small |
| Timestamp formatted as "2 hours ago" | `PostCard.cpp`, `NotifsPage.cpp` | Small |
| Post character limit (280 chars) | `FeedPage.cpp` | Tiny |
| Empty state illustrations | All pages | Small |
| Unread notification badge on nav button | `FeedPage.cpp` | Small |
| Admin can see ban reason / add notes | `Admin.h`, `AdminPage.cpp` | Medium |

---

## v1.2 — Social Features

Require new model fields or new manager logic.

### Comment display in UI

Currently comments are stored but not shown in the UI.

- Add a comment section below each `PostCard`
- Iterate `post->getComments().head()` to display each `Comment` node
- Add a comment input box with submit button
- Call `me->commentOn(post, text)` on submit
- Trigger `NotificationManager::notify()` for the post owner

**Files:** `PostCard.h/.cpp`, `FeedPage.cpp`

---

### Image Post creation

Currently only text posts can be created via the UI. Image posts exist in the model but the composer only creates `TextPost`.

- Add a "📎 Attach Image" button to the post composer in `FeedPage`
- Open a `QFileDialog` to pick an image file
- Create `ImagePost` instead of `TextPost` when an image is attached
- Display the image in `PostCard` using `QLabel` + `QPixmap`

**Files:** `FeedPage.cpp`, `PostCard.cpp`

---

### Follow Requests UI

`FriendGraph` fully handles requests in the backend but there is no UI to accept/reject them.

- Add a "Requests" tab to `NotifsPage` or a new `RequestsPage`
- Call `FriendGraph::instance().getPendingFor(me->getID(), count)`
- Show each request with Accept / Reject buttons
- On accept: call `FriendGraph::instance().acceptRequest(id, users)`

**Files:** `NotifsPage.cpp` or new `RequestsPage.h/.cpp`, `MainWindow.h/.cpp`

---

### User Search → Message

From the search results, allow clicking a user to open a direct message thread with them — not just view their profile.

**Files:** `SearchPage.cpp`, `MainWindow.cpp` (add `showMessagesWithUser(int id)` slot)

---

## v1.3 — Networking (LAN Multiplayer)

Allow users on the **same WiFi network** to exchange real-time messages using Qt's `QTcpSocket` and `QTcpServer`. No internet required.

### Design

```
User A (Host)                     User B (Client)
QTcpServer (port 5000)            QTcpSocket
      │                                 │
      │◄────── TCP connect ─────────────┤
      │◄────── "MSG:Hello!" ────────────┤
      │──────── "MSG:Hi!" ─────────────►│
```

### New Classes Needed

| Class | Type | Responsibility |
|---|---|---|
| `NetworkServer` | QObject | Wraps QTcpServer, accepts clients, broadcasts messages |
| `NetworkClient` | QObject | Wraps QTcpSocket, connects to server by IP |
| `NetworkMessage` | Plain struct | `{ senderName, content, timestamp }` serialized as `"NAME|MSG|TS\n"` |
| `LanChatPage` | QWidget page | UI: Host / Join buttons, IP input, chat panel |

### Protocol

Messages sent over TCP as plain text lines:
```
CONNECT|Ali Hassan\n        ← client announces their name
MSG|Ali Hassan|Hello!\n     ← chat message
DISCONNECT|Ali Hassan\n     ← client leaving
```

### Steps to Implement

1. Add `Qt6::Network` to `CMakeLists.txt`:
   ```cmake
   find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
   target_link_libraries(Connectify PRIVATE Qt6::Widgets Qt6::Network)
   ```
2. Create `NetworkServer.h/.cpp` in `src/managers/`
3. Create `NetworkClient.h/.cpp` in `src/managers/`
4. Create `LanChatPage.h/.cpp` in `src/ui/pages/`
5. Add "LAN Chat" button to `FeedPage` navbar
6. Register `LanChatPage` in `MainWindow`

**Files:** `CMakeLists.txt`, 4 new files, `FeedPage.cpp`, `MainWindow.h/.cpp`

---

## v1.4 — Data Improvements

| Feature | Description | Files |
|---|---|---|
| Password change | Add `changePassword(old, new)` to `AuthManager` | `AuthManager.h/.cpp`, new settings UI |
| Edit post | Allow editing text content after posting | `Post.h`, `FileManager.cpp`, `PostCard.cpp` |
| Delete own post | User can delete their own posts from `ProfilePage` | `ProfilePage.cpp` |
| Post sorting options | Sort feed by oldest / most liked | `NewsFeed.h/.cpp`, `FeedPage.cpp` |
| Block user | Prevent blocked users from seeing your content | `User.h` (new `FollowArray blocked_`), `NewsFeed.cpp` |

---

## v2.0 — Major Extension Ideas

These would be significant projects on their own.

| Idea | What it needs |
|---|---|
| **Groups / Communities** | New `Group` class, `GroupList`, group posts, group membership |
| **Hashtag system** | Parse `#` in post content, `HashtagManager` with inverted index using linked lists |
| **Stories (24h expiry)** | `StoryPost` subclass, `expiresAt_` field, story carousel widget |
| **Poll posts** | `PollPost` subclass with `OptionArray`, vote tracking per user |
| **Media gallery** | Image grid on `ProfilePage` showing all `ImagePost` thumbnails |
| **Dark mode** | Second `.qss` file, toggle button, save preference to a config file |

---

## How to Pick Up a Task

1. Read the relevant section above
2. Read `CONTRIBUTING.md` for the step-by-step process
3. Check `CLASSES.md` for the classes you will modify
4. Check `DATA_FORMAT.md` if you are adding persistent fields
5. Create your feature branch: `git checkout -b feature/comment-ui`
6. Implement, test with `run.bat`, submit
