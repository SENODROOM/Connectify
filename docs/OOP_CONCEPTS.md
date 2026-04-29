# OOP_CONCEPTS.md — OOP Concepts Used in Connectify

This document maps every OOP concept to the exact files and code where it appears.  
Use this as a reference when explaining the project.

---

## 1. Classes and Objects

Every entity in the system is a class. Objects are created with `new` and destroyed with `delete`.

```cpp
// Creating a User object on the heap
User* u = new User(1001, "Ali", "ali@example.com", hashedPass);

// Creating a TextPost object
Post* p = new TextPost(2001, 1001, "Hello world!");

// Object is destroyed when its owner (PostList) is destroyed
delete p;
```

**Where:** Every file in `src/models/`

---

## 2. Encapsulation

All data members are `private` or `protected`. External code can only interact through public methods.

```cpp
class User : public Person {
public:
    std::string getName()  const { return name_; }  // controlled read
    void setName(const std::string& n) { name_ = n; } // controlled write

private:
    std::string name_;     // cannot be accessed directly
    bool        banned_;   // hidden implementation detail
};
```

No code outside `User` can set `banned_` directly — it must call `setBanned()`.  
No code outside `Post` can modify `likes_` — it must call `like()` or `unlike()`.

**Where:** All model classes — `Person`, `User`, `Admin`, `Post`, `TextPost`, `ImagePost`

---

## 3. Inheritance

### Single Inheritance — Person hierarchy

```
Person  (abstract)
├── User     — regular social media account
└── Admin    — privileged administrative account
```

`User` and `Admin` inherit `id_`, `name_`, `email_`, `hashedPassword_` from `Person`.  
They also inherit `login()` and `getProfile()` and override where needed.

```cpp
// Person.h
class Person {
public:
    virtual std::string getRole() const = 0;   // must override
    virtual std::string getProfile() const;     // can override
    bool login(const std::string& email, const std::string& password) const;
protected:
    int         id_;
    std::string name_;
    // ...
};

// User.h
class User : public Person {
public:
    std::string getRole() const override { return "USER"; }
    std::string getProfile() const override; // adds follower stats
};

// Admin.h
class Admin : public Person {
public:
    std::string getRole() const override { return "ADMIN"; }
};
```

### Single Inheritance — Post hierarchy

```
Post  (abstract)
├── TextPost  — text-only content
└── ImagePost — image with caption
```

```cpp
class Post {
public:
    virtual void        display() const = 0;
    virtual std::string getType()  const = 0;
    // shared: likes, comments, timestamp, ownerID
};

class TextPost : public Post {
public:
    void        display() const override;
    std::string getType() const override { return "TEXT"; }
private:
    std::string content_;
};

class ImagePost : public Post {
public:
    void        display() const override;
    std::string getType() const override { return "IMAGE"; }
private:
    std::string imagePath_;
    std::string caption_;
};
```

**Where:** `src/models/Person.h`, `User.h`, `Admin.h`, `Post.h`, `TextPost.h`, `ImagePost.h`

---

## 4. Polymorphism

### Runtime Polymorphism (Virtual Functions)

A `Post*` pointer can point to either a `TextPost` or an `ImagePost`. The correct `display()` is called at runtime based on the actual type.

```cpp
Post* p = new TextPost(1, 1, "Hello");
p->display();   // calls TextPost::display() at runtime — not Post::display()

Post* q = new ImagePost(2, 1, "photo.png", "Nice view");
q->display();   // calls ImagePost::display() at runtime

// Same call, different behavior — this is polymorphism
```

### Used in FileManager for type detection

```cpp
Post* p = /* from PostList */;
if (p->getType() == "TEXT") {
    auto* tp = dynamic_cast<TextPost*>(p);   // downcast
    content = tp->getContent();
} else {
    auto* ip = dynamic_cast<ImagePost*>(p);  // downcast
    content = ip->getCaption();
}
```

### Used in FeedPage for rendering

```cpp
for (int i = 0; i < snap.size(); ++i) {
    Post* p = snap[i];
    auto* card = new PostCard(p, me, this);  // PostCard handles both types
    feedLayout_->addWidget(card);
}
```

**Where:** `FileManager.cpp`, `FeedPage.cpp`, `PostCard.cpp`, `SearchEngine.cpp`

---

## 5. Abstract Classes

A class with at least one **pure virtual function** (`= 0`) cannot be instantiated.

```cpp
class Person {
public:
    virtual std::string getRole() const = 0;   // pure virtual
    // Person p;  ← compile error — cannot instantiate abstract class
};

class Post {
public:
    virtual void        display() const = 0;   // pure virtual
    virtual std::string getType()  const = 0;  // pure virtual
};
```

This forces every subclass to provide an implementation. If `User` forgot to implement `getRole()`, it would also become abstract and could not be instantiated.

**Where:** `Person.h`, `Post.h`

---

## 6. Pointers and Dynamic Memory

The entire data storage system uses raw pointer-based custom data structures.

### Singly-Linked List (PostList)

```cpp
struct PostNode {
    Post*     post;    // pointer to heap-allocated Post
    PostNode* next;    // pointer to next node
    explicit PostNode(Post* p) : post(p), next(nullptr) {}
};

class PostList {
    PostNode* head_;   // pointer to first node
    // ...
};
```

Traversal:
```cpp
PostNode* node = user->getPosts().head();
while (node) {
    node->post->display();
    node = node->next;   // advance pointer
}
```

### Doubly-Linked List (MessageList)

```cpp
struct MsgNode {
    MsgNode* prev;   // points backward
    MsgNode* next;   // points forward
};
```

### Raw Dynamic Arrays

```cpp
class FollowArray {
    int* data_;      // raw int array on heap
    int  size_;
    int  capacity_;

    void grow() {
        int* newData = new int[capacity_ * 2];    // allocate larger
        for (int i = 0; i < size_; ++i)
            newData[i] = data_[i];                // manual copy
        delete[] data_;                           // free old
        data_ = newData;
    }
};
```

**Where:** `PostList.h`, `Comment.h`, `Message.h`, `Notification.h`, `FollowArray.h`, `LikeList.h`, `UserTable.h`

---

## 7. Destructors and Memory Management

Every class that owns heap memory has a destructor that frees it.

```cpp
// PostList owns its PostNode chain and the Post objects
class PostList {
public:
    ~PostList() {
        PostNode* cur = head_;
        while (cur) {
            PostNode* next = cur->next;
            delete cur->post;   // free the Post object
            delete cur;         // free the node
            cur = next;
        }
    }
};
```

Ownership chain — one destructor triggers the next:

```
~AuthManager()
  └─ delete user         → ~User()
                              └─ ~PostList()
                                    └─ delete Post  → ~Post()
                                                         └─ ~CommentList()
                                                         └─ ~LikeList() → delete[] data_
```

**Where:** `PostList.h`, `Comment.h`, `Message.h`, `Notification.h`, `LikeList.h`, `FollowArray.h`, `AuthManager.cpp`

---

## 8. Singleton Pattern

Managers are singletons — one global instance, private constructor, public `instance()`.

```cpp
class AuthManager {
public:
    static AuthManager& instance() {
        static AuthManager am;   // created once, lives forever
        return am;
    }

private:
    AuthManager();                                    // private — no outside construction
    AuthManager(const AuthManager&)            = delete;  // no copying
    AuthManager& operator=(const AuthManager&) = delete;  // no assignment
};

// Usage from anywhere:
User* u = AuthManager::instance().login(email, password);
```

**Where:** `AuthManager`, `FileManager`, `FriendGraph`, `MessageManager`, `NotificationManager`, `Session`

---

## 9. Composition

Classes are built from other classes as members (not inheritance).

```cpp
class User : public Person {
private:
    FollowArray following_;   // User HAS-A FollowArray
    FollowArray followers_;   // User HAS-A FollowArray
    PostList    posts_;       // User HAS-A PostList
};

class Post {
private:
    LikeList    likes_;       // Post HAS-A LikeList
    CommentList comments_;    // Post HAS-A CommentList
};
```

When a `User` is destroyed, its `PostList`, `FollowArray` members are also destroyed automatically.

**Where:** `User.h`, `Post.h`

---

## 10. `dynamic_cast` — Safe Downcasting

When you have a `Post*` and need to access subclass-specific methods, `dynamic_cast` safely converts it:

```cpp
Post* p = /* some post */;

// Try to cast to TextPost
TextPost* tp = dynamic_cast<TextPost*>(p);
if (tp) {
    // p really is a TextPost — safe to use TextPost methods
    std::string content = tp->getContent();
}

// Try to cast to ImagePost
ImagePost* ip = dynamic_cast<ImagePost*>(p);
if (ip) {
    std::string path = ip->getImagePath();
}
```

If the cast fails (wrong type), `dynamic_cast` returns `nullptr` instead of crashing.

**Where:** `FileManager.cpp`, `SearchEngine.cpp`, `PostCard.cpp`, `AdminPage.cpp`

---

## 11. `const` Correctness

Methods that do not modify the object are marked `const`. This allows them to be called on `const` references.

```cpp
class User {
public:
    std::string getName()       const { return name_; }    // read-only
    bool        isFollowing(int id) const;                 // read-only
    void        follow(int id);                            // modifies — no const
};

// Can pass User as const& without worrying about modification
void printUser(const User& u) {
    std::cout << u.getName();      // ✅ fine — getName() is const
    // u.follow(123);              // ❌ compile error — follow() is not const
}
```

**Where:** All model headers — every getter is `const`

---

## Summary Table

| Concept | Demonstrated In |
|---|---|
| Classes & Objects | All model files |
| Encapsulation | `Person`, `User`, `Post` private members |
| Inheritance | `Person→User/Admin`, `Post→TextPost/ImagePost` |
| Polymorphism | `Post::display()`, `Post::getType()` virtual calls |
| Abstract Classes | `Person` (getRole=0), `Post` (display=0, getType=0) |
| Raw Pointers | `PostList`, `MessageList`, `FollowArray`, `UserTable` |
| Linked Lists | `PostList` (singly), `MessageList` (doubly), `CommentList` (singly), `NotifList` (singly), `RequestList` (singly) |
| Destructors | `PostList`, `CommentList`, `LikeList`, `FollowArray`, `AuthManager` |
| Singleton | `AuthManager`, `FileManager`, `Session`, `FriendGraph`, `MessageManager`, `NotificationManager` |
| Composition | `User` has `PostList` + `FollowArray`, `Post` has `LikeList` + `CommentList` |
| dynamic_cast | `FileManager`, `SearchEngine`, `PostCard`, `AdminPage` |
| const correctness | All getters in all model classes |
