# DATA_FORMAT.md — File Storage Format

All persistent data is stored in the `data/` directory as plain pipe-delimited text files.  
This directory is created automatically on first run.

---

## General Rules

- Fields are separated by `|`
- The first line of every file is a header row
- Pipe characters inside field values are replaced with `;` on save
- Timestamps are stored as Unix epoch integers (`time_t`)
- Boolean flags are stored as `1` (true) or `0` (false)
- Empty string fields are stored as empty (two consecutive pipes `||`)

---

## users.dat

Stores all registered user accounts.

```
userID|name|email|hashedPassword|isBanned|profilePicPath
```

| Field | Type | Example |
|---|---|---|
| `userID` | int | `1001` |
| `name` | string | `Ali Hassan` |
| `email` | string | `ali@example.com` |
| `hashedPassword` | string (hex) | `8f2a91bc...` |
| `isBanned` | 0 or 1 | `0` |
| `profilePicPath` | string | `` (empty if none) |

**Example:**
```
userID|name|email|hashedPassword|isBanned|profilePicPath
1001|Ali Hassan|ali@example.com|8f2a91bc44d7e301|0|
1002|Sara Khan|sara@example.com|3c7d8a22f1b09e44|0|assets/sara.png
1003|Bad Actor|bad@example.com|1d2e3f4a5b6c7d8e|1|
```

---

## posts.dat

Stores all posts from all users.

```
postID|ownerID|type|content|imagePath|timestamp|likes
```

| Field | Type | Example |
|---|---|---|
| `postID` | int | `2001` |
| `ownerID` | int | `1001` |
| `type` | `TEXT` or `IMAGE` | `TEXT` |
| `content` | string | `Hello world!` |
| `imagePath` | string | `` (empty for TEXT posts) |
| `timestamp` | Unix epoch | `1714300800` |
| `likes` | int | `5` |

> **Note:** `likes` count is saved for reference but the actual like tracking (which user liked) is rebuilt from file relationships. The canonical like count comes from `LikeList` in memory.

**Example:**
```
postID|ownerID|type|content|imagePath|timestamp|likes
2001|1001|TEXT|Hello everyone!||1714300800|3
2002|1002|TEXT|Loving this app||1714301200|1
2003|1001|IMAGE|Check this out|assets/photo.png|1714302000|0
```

---

## friends.dat

Stores accepted follow relationships.

```
fromID|toID|status
```

| Field | Type | Description |
|---|---|---|
| `fromID` | int | The follower's user ID |
| `toID` | int | The followed user's ID |
| `status` | string | Always `ACCEPTED` in this file |

**Example:**
```
fromID|toID|status
1001|1002|ACCEPTED
1002|1001|ACCEPTED
1003|1001|ACCEPTED
```

> This file is rewritten completely every time `saveFriends()` is called. It is derived from each User's `FollowArray`.

---

## friend_requests.dat

Stores all friend/follow requests including pending and rejected ones.

```
requestID|fromID|toID|status
```

| Field | Type | Values |
|---|---|---|
| `requestID` | int | e.g. `5001` |
| `fromID` | int | Sender's user ID |
| `toID` | int | Recipient's user ID |
| `status` | string | `PENDING`, `ACCEPTED`, `REJECTED` |

**Example:**
```
requestID|fromID|toID|status
5001|1001|1003|ACCEPTED
5002|1002|1003|PENDING
5003|1004|1001|REJECTED
```

---

## messages.dat

Stores all direct messages between users. New messages are **appended** (not rewritten).

```
msgID|senderID|receiverID|content|timestamp
```

| Field | Type | Example |
|---|---|---|
| `msgID` | int | `3001` |
| `senderID` | int | `1001` |
| `receiverID` | int | `1002` |
| `content` | string | `Hey how are you?` |
| `timestamp` | Unix epoch | `1714305600` |

**Example:**
```
msgID|senderID|receiverID|content|timestamp
3001|1001|1002|Hey how are you?|1714305600
3002|1002|1001|I am good thanks!|1714305660
3003|1001|1002|Great to hear!|1714305720
```

---

## notifications.dat

Stores all notifications for all users.  
New notifications are **appended**. The entire file is rewritten only when `markAllRead()` is called.

```
notifID|ownerID|type|message|isRead|timestamp
```

| Field | Type | Values |
|---|---|---|
| `notifID` | int | e.g. `4001` |
| `ownerID` | int | Who receives this notification |
| `type` | string | `LIKE`, `COMMENT`, `REQUEST`, `FOLLOW`, `SYSTEM` |
| `message` | string | Human-readable description |
| `isRead` | 0 or 1 | `0` = unread |
| `timestamp` | Unix epoch | — |

**Example:**
```
notifID|ownerID|type|message|isRead|timestamp
4001|1002|LIKE|Ali Hassan liked your post.|0|1714306000
4002|1001|FOLLOW|Sara Khan started following you.|0|1714306100
4003|1002|COMMENT|Ali Hassan commented on your post.|1|1714306200
```

---

## ID Ranges

ID ranges are pre-assigned by convention to avoid collisions:

| Entity | ID Range | Starting value |
|---|---|---|
| Users | 1001+ | `lastUserID_ = 1000` |
| Posts | 2001+ | `nextPostID()` scans all posts |
| Messages | 3001+ | `lastMsgID_ = 3000` |
| Notifications | 4001+ | `lastID_ = 4000` |
| Friend Requests | 5001+ | `lastRequestID_ = 5000` |
| Admin | 1 | Hardcoded |

---

## Resetting Data

To reset specific data, delete the relevant `.dat` file. It will be recreated with just the header on next save.

```powershell
# Reset everything
Remove-Item C:\Programming\Connectify\build\data\*.dat

# Reset only messages
Remove-Item C:\Programming\Connectify\build\data\messages.dat

# Reset only notifications
Remove-Item C:\Programming\Connectify\build\data\notifications.dat
```

> **Warning:** Deleting `users.dat` removes all accounts. Deleting `friends.dat` removes all follow relationships. The app handles missing files gracefully — it simply starts empty.
