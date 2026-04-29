# CLASSES.md — Complete Class Reference

---

## Model Layer

---

### `Person` — Abstract Base Class
**File:** `src/models/Person.h / Person.cpp`

The root of the user hierarchy. Cannot be instantiated directly.

| Member | Type | Description |
|---|---|---|
| `id_` | `int` | Unique user ID |
| `name_` | `string` | Display name |
| `email_` | `string` | Login email |
| `hashedPassword_` | `string` | djb2 hex hash |

| Method | Returns | Description |
|---|---|---|
| `getRole()` | `string` | **Pure virtual** — "USER" or "ADMIN" |
| `getProfile()` | `string` | Formatted profile string |
| `login(email, password)` | `bool` | Compares email + hash |
| `getID()` | `int` | — |
| `getName()` | `string` | — |
| `getEmail()` | `string` | — |
| `getHashedPassword()` | `string` | — |
| `setName(n)` | `void` | — |
| `setEmail(e)` | `void` | — |

---

### `User : Person`
**File:** `src/models/User.h / User.cpp`

A registered social media user. Owns a `PostList`, `FollowArray` for following/followers.

| Member | Type | Description |
|---|---|---|
| `following_` | `FollowArray` | IDs of users this user follows |
| `followers_` | `FollowArray` | IDs of users who follow this user |
| `posts_` | `PostList` | This user's posts (owned) |
| `banned_` | `bool` | Account status |
| `profilePicPath_` | `string` | Path to profile picture |

| Method | Returns | Description |
|---|---|---|
| `getRole()` | `string` | Returns `"USER"` |
| `getProfile()` | `string` | Name + stats string |
| `follow(userID)` | `void` | Add to following list |
| `unfollow(userID)` | `void` | Remove from following list |
| `isFollowing(id)` | `bool` | Check follow status |
| `addFollower(userID)` | `void` | Called on the followed user |
| `removeFollower(id)` | `void` | — |
| `addPost(Post*)` | `void` | Append to PostList |
| `removePost(pid)` | `bool` | Remove + delete post by ID |
| `findPost(pid)` | `Post*` | Find post by ID, nullptr if not found |
| `likePost(Post*)` | `void` | Calls post->like(id_) |
| `unlikePost(Post*)` | `void` | Calls post->unlike(id_) |
| `commentOn(Post*, text)` | `void` | Adds comment to post |
| `isBanned()` | `bool` | — |
| `setBanned(bool)` | `void` | — |
| `getFollowing()` | `const FollowArray&` | — |
| `getFollowers()` | `const FollowArray&` | — |
| `getPosts()` | `const PostList&` | — |

---

### `Admin : Person`
**File:** `src/models/Admin.h / Admin.cpp`

Special privileged account. One per application (hardcoded in `AuthManager`).

| Method | Returns | Description |
|---|---|---|
| `getRole()` | `string` | Returns `"ADMIN"` |
| `banUser(User*)` | `void` | Sets user->banned = true |
| `unbanUser(User*)` | `void` | Sets user->banned = false |
| `deletePost(postID, UserTable&)` | `bool` | Finds and removes post from its owner |

---

### `Post` — Abstract Base Class
**File:** `src/models/Post.h / Post.cpp`

Base for all post types. Owns a `LikeList` and `CommentList`.

| Member | Type | Description |
|---|---|---|
| `postID_` | `int` | Unique post ID |
| `ownerID_` | `int` | ID of the User who created it |
| `timestamp_` | `time_t` | Unix timestamp of creation |
| `likes_` | `LikeList` | Set of user IDs who liked |
| `comments_` | `CommentList` | Linked list of comments |

| Method | Returns | Description |
|---|---|---|
| `display()` | `void` | **Pure virtual** — print to console |
| `getType()` | `string` | **Pure virtual** — "TEXT" or "IMAGE" |
| `like(userID)` | `bool` | Add like, returns false if already liked |
| `unlike(userID)` | `bool` | Remove like |
| `isLikedBy(userID)` | `bool` | Check like status |
| `getLikes()` | `int` | Like count |
| `addComment(uid, username, text, ts)` | `void` | Append comment to list |
| `getPostID()` | `int` | — |
| `getOwnerID()` | `int` | — |
| `getTimestamp()` | `time_t` | — |
| `getComments()` | `const CommentList&` | — |

---

### `TextPost : Post`
**File:** `src/models/TextPost.h / TextPost.cpp`

| Member | Type | Description |
|---|---|---|
| `content_` | `string` | The post body text |

| Method | Returns | Description |
|---|---|---|
| `display()` | `void` | Prints post to console |
| `getType()` | `string` | Returns `"TEXT"` |
| `getContent()` | `string` | — |

---

### `ImagePost : Post`
**File:** `src/models/ImagePost.h / ImagePost.cpp`

| Member | Type | Description |
|---|---|---|
| `imagePath_` | `string` | File path to the image |
| `caption_` | `string` | Caption text |

| Method | Returns | Description |
|---|---|---|
| `display()` | `void` | Prints image path + caption |
| `getType()` | `string` | Returns `"IMAGE"` |
| `getImagePath()` | `string` | — |
| `getCaption()` | `string` | — |

---

## Custom Data Structures

---

### `CommentList`
**File:** `src/models/Comment.h`

Singly-linked list of `Comment` nodes. Owned by `Post`.

```
head → [Comment] → [Comment] → [Comment] → nullptr
```

| Method | Description |
|---|---|
| `add(userID, username, text, ts)` | Append new comment node to tail |
| `clear()` | Delete all nodes |
| `head()` | Return head pointer for traversal |
| `size()` | Node count |

**Comment node fields:** `userID`, `username`, `text`, `timestamp`, `next*`

---

### `PostList`
**File:** `src/models/PostList.h`

Singly-linked list of `Post*`. Owned by `User`. **Owns** the Post objects.

```
head → [PostNode(Post*)] → [PostNode(Post*)] → nullptr
```

| Method | Description |
|---|---|
| `append(Post*)` | Add to tail |
| `remove(postID)` | Find, unlink, delete Post and node |
| `find(postID)` | Linear search, returns `Post*` or nullptr |
| `clear()` | Delete all nodes and their Post objects |
| `head()` | Return `PostNode*` for manual traversal |
| `size()` | Node count |

---

### `MessageList`
**File:** `src/models/Message.h`

**Doubly**-linked list of `MsgNode`. Owned by `MessageManager`.

```
head ⇄ [MsgNode] ⇄ [MsgNode] ⇄ [MsgNode] ← tail
```

Each `MsgNode` has `prev*` and `next*` for bidirectional traversal.

| Method | Description |
|---|---|
| `append(msgID, senderID, receiverID, content, ts)` | Add to tail |
| `clear()` | Delete all nodes |
| `head()` | Front of list |
| `tail()` | Back of list |
| `size()` | Node count |

---

### `NotifList`
**File:** `src/models/Notification.h`

Singly-linked list of `NotifNode*`. Owned by `NotificationManager`.

| Method | Description |
|---|---|
| `append(NotifNode*)` | Add to tail |
| `clear()` | Delete all nodes |
| `head()` | Head pointer |
| `size()` | Count |

**NotifNode fields:** `notifID`, `ownerID`, `type` (enum), `message`, `isRead`, `timestamp`, `next*`

**NotifType enum:** `LIKE`, `COMMENT`, `REQUEST`, `FOLLOW`, `SYSTEM`

---

### `RequestList`
**File:** `src/managers/FriendGraph.h`

Singly-linked list of `RequestNode*`. Owned by `FriendGraph`.

**RequestNode fields:** `requestID`, `fromID`, `toID`, `status` ("PENDING"/"ACCEPTED"/"REJECTED"), `next*`

---

### `LikeList`
**File:** `src/models/LikeList.h`

Dynamic `int*` array. Grows by doubling. Owned by `Post`.  
Stores user IDs of everyone who liked a post. Prevents duplicate likes.

| Method | Returns | Description |
|---|---|---|
| `add(userID)` | `bool` | Add ID, false if already present |
| `remove(userID)` | `bool` | Remove ID |
| `has(userID)` | `bool` | Membership check |
| `count()` | `int` | Number of likes |

---

### `FollowArray`
**File:** `src/models/FollowArray.h`

Dynamic `int*` array. Owned by `User` (two instances: `following_` and `followers_`).

| Method | Returns | Description |
|---|---|---|
| `add(id)` | `bool` | Add if not present |
| `remove(id)` | `bool` | Remove |
| `has(id)` | `bool` | Check membership |
| `operator[](i)` | `int` | Index access |
| `size()` | `int` | Element count |

---

### `UserTable`
**File:** `src/models/UserTable.h`

Dynamic `User**` array. Does **not** own its User objects — `AuthManager` does.

| Method | Returns | Description |
|---|---|---|
| `add(User*)` | `void` | Append |
| `remove(userID)` | `bool` | Remove entry (does not delete User) |
| `findByID(id)` | `User*` | Linear search |
| `findByEmail(email)` | `User*` | Linear search |
| `operator[](i)` | `User*` | Index access |
| `size()` | `int` | Count |
| `clear()` | `void` | Reset size to 0 (does not delete Users) |

---

### `FeedSnapshot`
**File:** `src/managers/NewsFeed.h`

Temporary `Post**` array. Built fresh on each feed render. Does **not** own posts.  
Sorted by `sortNewestFirst()` using insertion sort.

---

### `Array<T>`
**File:** `src/models/Array.h`

Generic resizable `T*` array template. Can hold any pointer type.

| Method | Description |
|---|---|
| `push(item)` | Append |
| `remove(item)` | Remove first match |
| `removeAt(index)` | Remove by index |
| `contains(item)` | Linear search |
| `operator[](i)` | Index access |
| `size()` / `empty()` / `clear()` | — |

---

## Manager Layer

---

### `FileManager` — Singleton
**File:** `src/managers/FileManager.h / FileManager.cpp`

All disk I/O. Never instantiated directly — use `FileManager::instance()`.

| Method | Description |
|---|---|
| `saveAllUsers(UserTable&)` | Write users.dat |
| `loadAllUsers(UserTable&)` | Read users.dat, create User objects |
| `saveAllPosts(UserTable&)` | Write posts.dat |
| `loadAllPosts(UserTable&)` | Read posts.dat, attach to owners |
| `saveFriends(UserTable&)` | Write friends.dat |
| `loadFriends(UserTable&)` | Read friends.dat, wire follow arrays |
| `saveMessage(...)` | Append one message to messages.dat |
| `loadAllMessages(MessageList&)` | Populate MessageList from file |
| `appendNotification(NotifNode*)` | Append one notification |
| `saveAllNotifications(NotifList&)` | Rewrite entire notifications.dat |
| `loadAllNotifications(NotifList&)` | Populate NotifList from file |
| `hashPassword(string)` | static — djb2 hex hash |
| `ensureDataDir()` | Creates data/ directory if missing |

---

### `AuthManager` — Singleton
**File:** `src/managers/AuthManager.h / AuthManager.cpp`

Owns all `User*` objects and the single `Admin*`.

| Method | Returns | Description |
|---|---|---|
| `login(email, password)` | `User*` | Hash password, search, return user or nullptr |
| `isAdminLogin(email, password)` | `bool` | Check against hardcoded admin |
| `signup(name, email, password)` | `User*` | Validate, create user, save |
| `deleteAccount(userID)` | `bool` | Remove and delete user |
| `loadAll()` | `void` | Load users + posts + friends from disk |
| `saveAll()` | `void` | Save users + posts + friends to disk |
| `findUser(id)` | `User*` | Look up by ID |
| `findByEmail(email)` | `User*` | Look up by email |
| `getUsers()` | `UserTable&` | Reference to all users |
| `getAdmin()` | `Admin*` | The admin object |
| `nextUserID()` | `int` | Increment and return last ID |

---

### `NewsFeed`
**File:** `src/managers/NewsFeed.h / NewsFeed.cpp`

Stateless — no singleton needed.

| Method | Returns | Description |
|---|---|---|
| `generate(user, allUsers, snapshot)` | `void` | Fill FeedSnapshot with followed users' posts, sorted newest first |
| `nextPostID(allUsers)` | `int` | static — scan all posts, return max+1 |

---

### `FriendGraph` — Singleton
**File:** `src/managers/FriendGraph.h / FriendGraph.cpp`

Manages follow requests lifecycle.

| Method | Returns | Description |
|---|---|---|
| `sendRequest(fromID, toID)` | `void` | Create PENDING request, reject duplicates |
| `acceptRequest(requestID, UserTable&)` | `void` | Mark ACCEPTED, wire follow arrays |
| `rejectRequest(requestID)` | `void` | Mark REJECTED |
| `getPendingFor(userID, count)` | `RequestNode**` | Heap array — caller must `delete[]` |
| `loadRequests()` | `void` | Read from friend_requests.dat |
| `saveRequests()` | `void` | Write to friend_requests.dat |

---

### `MessageManager` — Singleton
**File:** `src/managers/MessageManager.h / MessageManager.cpp`

| Method | Returns | Description |
|---|---|---|
| `sendMessage(senderID, receiverID, content)` | `void` | Append to list + file |
| `getConversation(userA, userB, count)` | `MsgNode**` | Heap array, sorted by timestamp — caller must `delete[]` |
| `getInboxPartners(userID, count)` | `int*` | Unique partner IDs — caller must `delete[]` |
| `loadAll()` | `void` | Populate MessageList from file |

---

### `NotificationManager` — Singleton
**File:** `src/managers/NotificationManager.h / NotificationManager.cpp`

| Method | Returns | Description |
|---|---|---|
| `notify(ownerID, type, message)` | `void` | Create node, append to list + file |
| `markAllRead(userID)` | `void` | Set all isRead=true, rewrite file |
| `getAll(userID, count)` | `NotifNode**` | All notifications — caller must `delete[]` |
| `getUnread(userID, count)` | `NotifNode**` | Unread only — caller must `delete[]` |
| `loadAll()` | `void` | Populate NotifList from file |

---

### `SearchEngine`
**File:** `src/managers/SearchEngine.h / SearchEngine.cpp`

Stateless. All methods are `static`.

| Method | Returns | Description |
|---|---|---|
| `searchUsers(keyword, allUsers, count)` | `User**` | Case-insensitive name/email match — caller must `delete[]` |
| `searchPosts(keyword, allUsers, count)` | `Post**` | Case-insensitive content/caption match — caller must `delete[]` |

Both use manual substring search (no `std::string::find`).

---

## UI Layer

---

### `Session` — Singleton
**File:** `src/ui/Session.h / Session.cpp`

Holds the logged-in user for the duration of the app session.

| Method | Description |
|---|---|
| `set(User*)` | Set current user, clear admin flag |
| `setAdmin(bool)` | Set admin mode |
| `clear()` | Logout — nulls user pointer |
| `current()` | Returns `User*` or nullptr |
| `isAdmin()` | Returns bool |
| `isLoggedIn()` | True if user or admin |

---

### `MainWindow : QMainWindow`
**File:** `src/ui/MainWindow.h / MainWindow.cpp`

Root window. Holds a `QStackedWidget` with all 8 pages.  
Wires all navigation signals in `setupPages()`.  
Calls `AuthManager::loadAll()` on construction and `saveAll()` on destruction.

---

### Pages

| Class | File | Purpose |
|---|---|---|
| `LoginPage` | `ui/pages/LoginPage` | Email + password login form |
| `SignupPage` | `ui/pages/SignupPage` | Registration form with validation |
| `FeedPage` | `ui/pages/FeedPage` | News feed + post composer |
| `ProfilePage` | `ui/pages/ProfilePage` | User profile + follow button + posts |
| `MessagesPage` | `ui/pages/MessagesPage` | Inbox list + chat panel with bubbles |
| `SearchPage` | `ui/pages/SearchPage` | Keyword search for users and posts |
| `NotifsPage` | `ui/pages/NotifsPage` | Notification list + mark all read |
| `AdminPage` | `ui/pages/AdminPage` | User table + post table with ban/delete |

---

### Widgets

| Class | File | Purpose |
|---|---|---|
| `PostCard` | `ui/widgets/PostCard` | Single post display with like button |
| `ChatBubble` | `ui/widgets/ChatBubble` | Right-aligned (mine) or left-aligned (theirs) message bubble |
