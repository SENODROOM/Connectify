<div align="center">

```
 ██████╗ ██████╗ ███╗   ██╗███╗   ██╗███████╗ ██████╗████████╗██╗███████╗██╗   ██╗
██╔════╝██╔═══██╗████╗  ██║████╗  ██║██╔════╝██╔════╝╚══██╔══╝██║██╔════╝╚██╗ ██╔╝
██║     ██║   ██║██╔██╗ ██║██╔██╗ ██║█████╗  ██║        ██║   ██║█████╗   ╚████╔╝
██║     ██║   ██║██║╚██╗██║██║╚██╗██║██╔══╝  ██║        ██║   ██║██╔══╝    ╚██╔╝
╚██████╗╚██████╔╝██║ ╚████║██║ ╚████║███████╗╚██████╗   ██║   ██║██║        ██║
 ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═══╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝╚═╝        ╚═╝
```

### _A C++ Social Media Management System_

---

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus&logoColor=white)
![WinForms](https://img.shields.io/badge/WinForms-GUI-blueviolet?style=for-the-badge&logo=windows&logoColor=white)
![OOP](https://img.shields.io/badge/OOP-Core%20Principles-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-In%20Development-brightgreen?style=for-the-badge)
![Course](https://img.shields.io/badge/Course-Object%20Oriented%20Programming-red?style=for-the-badge)

</div>

---

## 📖 About

**Connectify** is a C++ WinForms application that simulates a modern digital social networking platform. Built on rock-solid OOP principles, it handles complex user relationships, content flow, and private communication — all wrapped in an interactive GUI.

> _"Bridging theoretical OOP concepts with practical software engineering."_

---

## ✨ Features

| Feature                     | Description                                                  |
| --------------------------- | ------------------------------------------------------------ |
| 🔐 **User Management**      | Secure Login, Signup & Account Deletion for Admins and Users |
| 🛡️ **Admin Command Center** | View all users, delete posts, and ban accounts               |
| 📰 **Dynamic News Feed**    | Real-time posts from followed users using C++ pointers       |
| 🖼️ **Multimedia Posts**     | Text & image-based posts powered by Inheritance              |
| 🤝 **Social Graph**         | Send, accept, or reject friend requests & manage followers   |
| ❤️ **Engagement Module**    | Real-time Likes & Comments on posts                          |
| 💬 **Private Messaging**    | Secure one-to-one chat between connected users               |
| 🔍 **Search & Discovery**   | Search users or posts by keyword or username                 |
| 💾 **Persistent Storage**   | Full system state saved via `fstream` file handling          |
| 🔔 **Notification Engine**  | Real-time alerts for likes, comments & friend requests       |

---

## 🧠 OOP Concepts Applied

```
┌─────────────────────┬──────────────────────────────────────────────────────────┐
│   OOP Concept       │   How It's Used in Connectify                           │
├─────────────────────┼──────────────────────────────────────────────────────────┤
│ 🔒 Encapsulation    │ Private data members for passwords & personal info;      │
│                     │ secure access via public Getter/Setter methods           │
├─────────────────────┼──────────────────────────────────────────────────────────┤
│ 🧬 Inheritance      │ Base Person class → derived Admin & User classes         │
│                     │ for permission-level management                          │
├─────────────────────┼──────────────────────────────────────────────────────────┤
│ 🎭 Abstraction      │ Abstract Post class hides complex formatting logic       │
│                     │ from the main News Feed interface                        │
├─────────────────────┼──────────────────────────────────────────────────────────┤
│ 🔄 Polymorphism     │ Overridden display() in TextPost & ImagePost classes     │
│                     │ to render different content types in the UI              │
├─────────────────────┼──────────────────────────────────────────────────────────┤
│ 💾 File Handling    │ fstream serialization of user objects & post data        │
│                     │ into .txt / .dat files for persistent storage            │
└─────────────────────┴──────────────────────────────────────────────────────────┘
```

---

## 🏗️ Architecture Overview

```
                        ┌─────────────┐
                        │   Person    │  ← Base Class
                        └──────┬──────┘
                    ┌──────────┴──────────┐
                    │                     │
             ┌──────▼──────┐       ┌──────▼──────┐
             │    Admin    │       │    User     │
             │  (RBAC: Hi) │       │ (RBAC: Std) │
             └─────────────┘       └──────┬──────┘
                                          │
                    ┌─────────────────────┼─────────────────────┐
                    │                     │                     │
             ┌──────▼──────┐      ┌───────▼──────┐    ┌────────▼─────┐
             │  TextPost   │      │  ImagePost   │    │  Message     │
             │  (display())│      │  (display()) │    │  (Private)   │
             └─────────────┘      └──────────────┘    └──────────────┘
```

---

## 👥 Project Team

**Team 05 — BSE-2B | OOP | BS Software Engineering (2nd Semester, Spring)**

| Role           | Name             | Roll No  |
| -------------- | ---------------- | -------- |
| 👑 Team Leader | Ali Abdullah     | 25L-3022 |
| 👨‍💻 Member      | Farzam Zeeshan   | 25L-3049 |
| 👨‍💻 Member      | Mustafa Amir     | 25L-3107 |
| 👨‍💻 Member      | M. Arslan        | 25L-3080 |
| 👨‍💻 Member      | Saad Amin        | 25L-3045 |
| 👨‍💻 Member      | Muhammad Khubaib | 25L-3026 |

---

## 🎓 Course Info

- **Course:** Object Oriented Programming (OOP)
- **Section:** BSE-2B
- **Submitted To:** Mam Hina Iqbal & Syed Saad Ali

---

## 🚀 Expected Outcomes

- ✅ **Robust System Architecture** — Clean Admin/User separation via Inheritance & RBAC
- ✅ **Data Integrity & Persistence** — Reliable fstream file-handling across system restarts
- ✅ **Advanced Interaction Logic** — Friendship graphs & polymorphic news feed generation
- ✅ **Professional Documentation** — Clean, modular code with a comprehensive GitHub repository

---

<div align="center">

_Made with ❤️ and a lot of `cout` statements_

**NUCES — National University of Computer and Emerging Sciences**

</div>
