# USER_GUIDE.md — Connectify User Manual

---

## Getting Started

### Creating an Account

1. Launch Connectify (double-click `run.bat` or `Connectify.exe`)
2. Click **"Create an account"** on the login screen
3. Enter your full name, email address, and a password (minimum 6 characters)
4. Confirm your password and click **Sign Up**
5. You will be taken directly to your feed

### Logging In

1. Enter your registered email and password
2. Click **Log In**
3. If your credentials are correct you will land on your feed

> **Admin login:** Use `admin@connectify.com` / `admin123` to access the admin panel.

### Logging Out

Click the **Logout** button in the top-right corner of any page.  
All your data is saved automatically before logout.

---

## The Feed

The feed shows posts from everyone you follow, sorted newest first.

### Creating a Post

1. Type your message in the text box at the top of the feed
2. Press **Enter** or click **Post**
3. Your post appears on your followers' feeds immediately

### Liking a Post

Click the 🤍 button below any post to like it.  
Click ❤️ again to unlike it.  
The like count updates instantly.

### Viewing Comments

The 💬 button shows the comment count. Comment display is coming in v1.1.

---

## Profile

Click **Profile** in the navigation bar to view your own profile.

Your profile shows:
- Your name and email
- Post count · Follower count · Following count
- All your posts listed below

### Viewing Another User's Profile

Double-click a user in the Search results to open their profile.

### Following / Unfollowing

On any profile that is not your own, you will see a **Follow** or **Unfollow** button.  
- Click **Follow** to start following that user — their posts will appear in your feed
- Click **Unfollow** to stop following them

---

## Search

Click **Search** in the navigation bar.

### Searching for Users

1. Type a name or email into the search box
2. Click **Search** or press Enter
3. Results appear under the **Users** tab
4. Double-click any user to view their profile

### Searching for Posts

1. Type a keyword into the search box
2. Click **Search**
3. Matching posts appear under the **Posts** tab

Search is case-insensitive. A partial match anywhere in the name, email, or post content will be returned.

---

## Messages

Click **Messages** in the navigation bar.

### Starting a Conversation

You can message any user whose profile you visit.  
*(Full "new conversation" button is coming in v1.2 — for now, conversations appear once the other user messages you first.)*

### Sending a Message

1. Click a conversation in the left panel to open it
2. Type your message in the box at the bottom
3. Press **Enter** or click **Send**

Messages appear as blue bubbles (yours) or grey bubbles (theirs).  
All messages are saved and will be there when you reopen the app.

---

## Notifications

Click **Notifications** in the navigation bar.

Notifications are created when:
- Someone likes your post → 👍 LIKE notification
- Someone follows you → 👥 FOLLOW notification

Unread notifications appear in **bold**.

### Marking All as Read

Click **Mark all read** in the top-right of the Notifications page.

---

## Admin Panel

Log in with `admin@connectify.com` / `admin123` to access the admin panel.

### Managing Users

The **Users** tab shows all registered accounts with their ID, name, and email.

- Click **Ban** to suspend a user — they will not be able to log in
- Click **Unban** to restore their access
- A confirmation dialog appears before any action is taken

### Managing Posts

The **Posts** tab shows all posts from all users with a content preview.

- Click **Delete** to permanently remove a post
- A confirmation dialog appears before deletion
- Deleted posts cannot be recovered

### Admin Logout

Click **Logout** in the top-right of the admin panel to return to the login screen.

---

## Data & Privacy

All your data is stored locally on this computer in the `data\` folder.  
No data is sent to any server or internet service.

**To delete your data:**  
Ask the admin to delete your account, or delete the relevant `.dat` files in `data\`.

**Your password:**  
Passwords are never stored in plain text. They are hashed using the djb2 algorithm before being saved.

---

## Keyboard Shortcuts

| Key | Action |
|---|---|
| `Enter` (in post box) | Submit post |
| `Enter` (in message box) | Send message |
| `Enter` (in search box) | Run search |
| `Enter` (on login/signup form) | Submit form |

---

## Frequently Asked Questions

**Q: I forgot my password. What do I do?**  
A: There is no password recovery in v1.0. Ask the admin to delete your account and re-register.

**Q: Can I edit a post after publishing?**  
A: Not in v1.0. Post editing is planned for v1.4.

**Q: Can I delete my own posts?**  
A: Not in v1.0 from the UI. The admin can delete any post. Self-delete is planned for v1.4.

**Q: Why is my feed empty?**  
A: You need to follow at least one user who has created posts. Go to Search, find users, visit their profile, and click Follow.

**Q: Can I use Connectify with friends on the same WiFi?**  
A: LAN networking is planned for v1.3. Currently all data is local to this one machine.

**Q: How do I reset everything?**  
A: Delete all `.dat` files in the `data\` folder next to `Connectify.exe`.
