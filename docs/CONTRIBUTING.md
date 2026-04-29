# CONTRIBUTING.md — How to Contribute to Connectify

Welcome. This guide covers everything you need to add features, fix bugs, or extend the project while keeping the codebase clean and consistent.

---

## Table of Contents

1. [Ground Rules](#1-ground-rules)
2. [Project Conventions](#2-project-conventions)
3. [How to Add a New Feature](#3-how-to-add-a-new-feature)
4. [How to Add a New Model Class](#4-how-to-add-a-new-model-class)
5. [How to Add a New Page](#5-how-to-add-a-new-page)
6. [How to Add a New Data Field](#6-how-to-add-a-new-data-field)
7. [How to Add a New Manager](#7-how-to-add-a-new-manager)
8. [Code Style](#8-code-style)
9. [Common Mistakes to Avoid](#9-common-mistakes-to-avoid)
10. [Rebuild and Test](#10-rebuild-and-test)

---

## 1. Ground Rules

These rules are **non-negotiable** for this project:

- ❌ No `std::vector`, `std::list`, `std::map`, `std::unordered_map`, or any STL container
- ❌ No `std::sort`, `std::find`, `std::copy` — implement manually
- ✅ Use raw pointer arrays, linked list nodes, and the existing custom structures (`PostList`, `FollowArray`, `UserTable`, etc.)
- ✅ Every new class must follow OOP principles — encapsulation, single responsibility
- ✅ Qt code belongs only in `src/ui/` — models and managers must be Qt-free
- ✅ New `.cpp` files are picked up automatically by `file(GLOB_RECURSE)` in CMakeLists.txt — no need to edit it

---

## 2. Project Conventions

### Naming

| Thing | Convention | Example |
|---|---|---|
| Classes | PascalCase | `TextPost`, `FriendGraph` |
| Methods | camelCase | `getProfile()`, `sendMessage()` |
| Private members | camelCase with trailing `_` | `name_`, `lastMsgID_` |
| Files | Match class name exactly | `TextPost.h`, `TextPost.cpp` |
| Qt object names (QSS) | camelCase string | `"postCard"`, `"primaryBtn"` |

### File Placement

| What | Where |
|---|---|
| Domain classes (no Qt) | `src/models/` |
| Business logic singletons | `src/managers/` |
| Qt pages | `src/ui/pages/` |
| Qt reusable widgets | `src/ui/widgets/` |
| QSS stylesheet changes | `src/resources/style.qss` |

### Every class needs both a `.h` and a `.cpp`

Even if the `.cpp` only has a constructor. This keeps build times predictable and avoids header-only linking issues.

---

## 3. How to Add a New Feature

Use this checklist for any new feature:

```
[ ] Design the model change — what new data does this need?
[ ] Add/modify model classes in src/models/
[ ] Add persistence — update FileManager save/load if new data needs storing
[ ] Add business logic — new manager or new methods on existing manager
[ ] Add UI — new page or extend an existing page
[ ] Wire navigation — connect signals in MainWindow::setupPages() if new page
[ ] Test — build with run.bat, manually test the feature
[ ] Update DATA_FORMAT.md if you changed any .dat file format
[ ] Update CLASSES.md if you added a new class
```

---

## 4. How to Add a New Model Class

**Example: adding a `Story` post type**

### Step 1 — Create the header

```cpp
// src/models/StoryPost.h
#pragma once
#include "Post.h"

class StoryPost : public Post {
public:
    StoryPost(int postID, int ownerID,
              const std::string& mediaPath,
              time_t expiresAt,
              time_t timestamp = time(nullptr));

    void        display() const override;
    std::string getType() const override { return "STORY"; }

    std::string getMediaPath() const { return mediaPath_; }
    time_t      getExpiresAt() const { return expiresAt_; }
    bool        isExpired()    const { return time(nullptr) > expiresAt_; }

private:
    std::string mediaPath_;
    time_t      expiresAt_;
};
```

### Step 2 — Create the implementation

```cpp
// src/models/StoryPost.cpp
#include "StoryPost.h"
#include <iostream>

StoryPost::StoryPost(int postID, int ownerID,
                     const std::string& mediaPath,
                     time_t expiresAt, time_t timestamp)
    : Post(postID, ownerID, timestamp),
      mediaPath_(mediaPath), expiresAt_(expiresAt)
{}

void StoryPost::display() const {
    std::cout << "[Story #" << postID_ << " | Owner: " << ownerID_ << "]\n"
              << "Media: " << mediaPath_ << "\n";
}
```

### Step 3 — Handle in FileManager

In `FileManager::saveAllPosts()`, add a branch for `"STORY"`:
```cpp
} else if (p->getType() == "STORY") {
    auto* sp = dynamic_cast<StoryPost*>(p);
    if (sp) { content = sp->getMediaPath(); }
}
```

In `FileManager::loadAllPosts()`, add:
```cpp
else if (type == "STORY")
    post = new StoryPost(pid, oid, imagePath, ...);
```

### Step 4 — Handle in UI (PostCard or a new widget)

Use `dynamic_cast<StoryPost*>(p)` to detect and render differently.

---

## 5. How to Add a New Page

**Example: adding a `SettingsPage`**

### Step 1 — Create the page files

```cpp
// src/ui/pages/SettingsPage.h
#pragma once
#include <QWidget>

class SettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit SettingsPage(QWidget* parent = nullptr);
    void refresh();
signals:
    void goBack();
private:
    void setupUI();
};
```

```cpp
// src/ui/pages/SettingsPage.cpp
#include "SettingsPage.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

SettingsPage::SettingsPage(QWidget* parent) : QWidget(parent) { setupUI(); }

void SettingsPage::setupUI() {
    auto* root = new QVBoxLayout(this);
    auto* backBtn = new QPushButton("← Back"); backBtn->setObjectName("linkBtn");
    root->addWidget(backBtn);
    root->addWidget(new QLabel("Settings coming soon."));
    connect(backBtn, &QPushButton::clicked, this, &SettingsPage::goBack);
}

void SettingsPage::refresh() {}
```

### Step 2 — Register in MainWindow.h

```cpp
// Add the forward declaration at the top
class SettingsPage;

// Add the member variable
SettingsPage* settingsPage_;
```

### Step 3 — Wire in MainWindow.cpp

```cpp
// In setupPages():
#include "pages/SettingsPage.h"

settingsPage_ = new SettingsPage(this);
stack_->addWidget(settingsPage_);
connect(settingsPage_, &SettingsPage::goBack, this, &MainWindow::showFeed);

// Trigger from FeedPage (add a Settings nav button there)
connect(feedPage_, &FeedPage::goToSettings, this, &MainWindow::showSettings);
```

```cpp
// Add the slot declaration in MainWindow.h:
void showSettings();

// Add the implementation in MainWindow.cpp:
void MainWindow::showSettings() {
    settingsPage_->refresh();
    stack_->setCurrentWidget(settingsPage_);
}
```

---

## 6. How to Add a New Data Field

**Example: adding a `bio` field to User**

### Step 1 — Add to the model

```cpp
// In User.h — add private member:
std::string bio_;

// Add getter/setter:
std::string getBio() const { return bio_; }
void        setBio(const std::string& b) { bio_ = b; }
```

### Step 2 — Update FileManager

In `saveAllUsers()`:
```cpp
f << u->getID()    << '|'
  << ...
  << sanitize(u->getProfilePic()) << '|'
  << sanitize(u->getBio())        << '\n';  // ← add this
```

Update the header line too:
```cpp
f << "userID|name|email|hashedPassword|isBanned|profilePicPath|bio\n";
```

In `loadAllUsers()`:
```cpp
std::string bio;
std::getline(ss, bio, '|');   // ← add after profilePicPath
u->setBio(bio);
```

### Step 3 — Update DATA_FORMAT.md

Add the new field to the `users.dat` table in `DATA_FORMAT.md`.

### Step 4 — Delete the old users.dat

The existing file has the old column count. Delete it so it regenerates:
```powershell
Remove-Item C:\Programming\Connectify\build\data\users.dat
```

---

## 7. How to Add a New Manager

**Example: adding a `HashtagManager`**

```cpp
// src/managers/HashtagManager.h
#pragma once
#include "../models/Post.h"
#include "../models/UserTable.h"

class HashtagManager {
public:
    static HashtagManager& instance();

    // Returns raw Post** array — caller must delete[]
    Post** searchByHashtag(const char* tag,
                           const UserTable& allUsers,
                           int& count) const;
private:
    HashtagManager() = default;
    HashtagManager(const HashtagManager&)            = delete;
    HashtagManager& operator=(const HashtagManager&) = delete;
};
```

Key rules for new managers:
- Always a **singleton** with private constructor and deleted copy
- No Qt includes unless strictly needed
- Methods that return arrays always use `T** arr` + `int& count` pattern
- Always document that caller must `delete[]` the returned array

---

## 8. Code Style

### Header guards

Use `#pragma once` — no old-style `#ifndef` guards.

### Initializer lists

Always initialize members in constructor initializer list, not in body:
```cpp
// ✅ Good
TextPost::TextPost(int pid, int oid, const std::string& c, time_t ts)
    : Post(pid, oid, ts), content_(c)
{}

// ❌ Bad
TextPost::TextPost(...) {
    content_ = c;  // assignment, not initialization
}
```

### Raw pointer returns

Always document ownership clearly:
```cpp
// Returns heap array of matching User* — caller must delete[] (NOT the users)
User** searchUsers(const std::string& keyword, const UserTable& allUsers, int& count);
```

### Qt UI setup

All UI construction goes in a private `setupUI()` method called from the constructor. Keep `refresh()` methods separate from setup.

### Avoid magic numbers

```cpp
// ❌ Bad
int id = ++lastID_;  // what does 4000 mean?

// ✅ Good
static constexpr int NOTIF_ID_START = 4000;
int lastID_ = NOTIF_ID_START;
```

---

## 9. Common Mistakes to Avoid

### ❌ Returning a pointer to a local variable

```cpp
// WRONG — local goes out of scope
Post* getLatest() {
    TextPost p(1, 1, "hello");
    return &p;  // dangling pointer!
}

// CORRECT — heap allocated, caller owns it
Post* getLatest() {
    return new TextPost(1, 1, "hello");
}
```

### ❌ Forgetting to delete[] caller-owned arrays

```cpp
int count = 0;
User** results = SearchEngine::searchUsers("ali", users, count);
// ... use results ...
delete[] results;  // ← must do this — NOT delete results
```

### ❌ Adding Qt includes to model/manager files

```cpp
// WRONG — in src/models/User.h
#include <QString>   // ❌ Qt in model layer

// CORRECT — convert at the UI layer
QString name = QString::fromStdString(user->getName());
```

### ❌ Modifying the UserTable while iterating it

```cpp
// WRONG
for (int i = 0; i < users.size(); ++i) {
    if (condition) users.remove(users[i]->getID());  // invalidates iteration
}

// CORRECT — collect IDs first, then remove
int toRemove[64]; int count = 0;
for (int i = 0; i < users.size(); ++i)
    if (condition) toRemove[count++] = users[i]->getID();
for (int i = 0; i < count; ++i)
    users.remove(toRemove[i]);
```

### ❌ Double-deleting posts

`PostList` owns its posts and deletes them in `~PostList()`. Never `delete` a post you got from `findPost()` or from iterating the list — only call `user->removePost(id)` which handles both.

---

## 10. Rebuild and Test

After any change:

```powershell
# Windows — double-click or run:
C:\Programming\Connectify\run.bat
```

If you added a new `.cpp` file, CMake's `GLOB_RECURSE` picks it up automatically — no need to edit `CMakeLists.txt`. However you must do a clean cmake configure once:

```powershell
cd C:\Programming\Connectify\build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
```

After that, `run.bat` is sufficient for all subsequent builds.
