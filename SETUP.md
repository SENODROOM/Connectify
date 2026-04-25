# 🚀 Connectify — Complete Setup Guide (100% Free)

> This guide sets up Connectify using **only free and open-source tools** — no Qt account, no paid license needed. Follow each step in order from a fresh machine.

---

## 📋 Table of Contents

1. [System Requirements](#1-system-requirements)
2. [Install a C++ Compiler](#2-install-a-c-compiler)
3. [Install CMake](#3-install-cmake)
4. [Install Qt 6 (Free — No Account Needed)](#4-install-qt-6-free--no-account-needed)
5. [Get the Project Files](#5-get-the-project-files)
6. [Build the Project](#6-build-the-project)
7. [Run the App](#7-run-the-app)
8. [First Login](#8-first-login)
9. [Troubleshooting](#9-troubleshooting)

---

## 1. System Requirements

| Component | Minimum |
|-----------|---------|
| OS | Windows 10/11 · Ubuntu 20.04+ · macOS 12+ |
| RAM | 4 GB |
| Disk | 3 GB free |
| C++ Standard | C++17 |

---

## 2. Install a C++ Compiler

### 🪟 Windows — MSYS2 + MinGW (Free)

**MSYS2** is the easiest free way to get a modern GCC on Windows.

1. Go to **https://www.msys2.org** and download `msys2-x86_64-*.exe`
2. Run the installer — leave the default path `C:\msys64` as-is
3. When it finishes, it opens the **MSYS2 UCRT64** terminal automatically
4. Inside that terminal, run:
   ```bash
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake
   ```
   Press `Y` when asked to confirm
5. Close the MSYS2 terminal
6. Add MinGW to Windows PATH:
   - Press **Win + R**, type `sysdm.cpl`, press Enter
   - Go to **Advanced → Environment Variables**
   - Under **System Variables**, select `Path` → click **Edit**
   - Click **New** → add: `C:\msys64\ucrt64\bin`
   - Click OK on all windows
7. Open a new **Command Prompt** (not MSYS2) and verify:
   ```
   g++ --version
   cmake --version
   ```
   Both should print version numbers.

---

### 🐧 Ubuntu / Debian Linux

```bash
sudo apt update
sudo apt install -y build-essential g++ cmake ninja-build
```

Verify:
```bash
g++ --version
cmake --version
```

---

### 🍎 macOS

```bash
xcode-select --install
```

A popup appears — click **Install** and wait. Then verify:
```bash
g++ --version
```

Install CMake via Homebrew (free):
```bash
# Install Homebrew first if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Then install cmake
brew install cmake
cmake --version
```

---

## 3. Install CMake

> **Windows users:** CMake was already installed in Step 2 via MSYS2/pacman — skip this.
> **macOS users:** CMake was installed via Homebrew in Step 2 — skip this.
> **Linux users:** CMake was installed via apt in Step 2 — skip this.

If for any reason CMake is still missing:

```bash
# Linux
sudo apt install -y cmake

# macOS
brew install cmake
```

---

## 4. Install Qt 6 (Free — No Account Needed)

> ✅ Qt 6 is open-source and completely free under the **LGPL v3 license** — which is fine for a student/course project. The methods below require **no Qt account and no credit card**.

---

### 🪟 Windows — via MSYS2 (Recommended, Fully Free)

Open **MSYS2 UCRT64** terminal (from Start Menu → MSYS2 UCRT64) and run:

```bash
pacman -S mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools
```

Press `Y` to confirm. This installs Qt 6 entirely for free.

Qt will be installed to: `C:\msys64\ucrt64`

---

### 🐧 Ubuntu 22.04 / 24.04 — via apt (Free)

```bash
sudo apt update
sudo apt install -y qt6-base-dev qt6-tools-dev libqt6widgets6 cmake ninja-build
```

That's it — no account, no installer.

---

### 🐧 Ubuntu 20.04 (Qt 6 not in default repos)

Ubuntu 20.04's default repos only have Qt 5. Add the Qt PPA first:

```bash
sudo add-apt-repository ppa:thopiekar/qt6
sudo apt update
sudo apt install -y qt6-base-dev
```

---

### 🍎 macOS — via Homebrew (Free)

```bash
brew install qt@6
```

Then add Qt to your PATH permanently:

```bash
# For Apple Silicon (M1/M2/M3)
echo 'export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"' >> ~/.zshrc
echo 'export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"' >> ~/.zshrc
source ~/.zshrc

# For Intel Mac
echo 'export PATH="/usr/local/opt/qt@6/bin:$PATH"' >> ~/.zshrc
echo 'export CMAKE_PREFIX_PATH="/usr/local/opt/qt@6"' >> ~/.zshrc
source ~/.zshrc
```

Verify Qt is found:
```bash
qmake6 --version
# or
qmake --version
```

---

### Verify Qt Installation (All Platforms)

Run this to confirm CMake can see Qt:

```bash
# Windows (Command Prompt)
where qmake

# Linux / macOS
which qmake6 || which qmake
```

---

## 5. Get the Project Files

### Option A — Extract from ZIP

You received `Connectify.zip`. Extract it:

#### 🪟 Windows
Right-click `Connectify.zip` → **Extract All…** → choose `C:\Projects\Connectify` → click **Extract**

#### 🐧 Linux / 🍎 macOS
```bash
cd ~/Desktop
unzip Connectify.zip -d Connectify
cd Connectify
```

### Option B — From Git

```bash
git clone https://github.com/your-username/Connectify.git
cd Connectify
```

Your folder should look like this after extraction:

```
Connectify/
├── CMakeLists.txt
├── SETUP.md
├── data/
│   ├── users.dat
│   ├── posts.dat
│   ├── friends.dat
│   ├── friend_requests.dat
│   ├── messages.dat
│   └── notifications.dat
└── src/
    ├── main.cpp
    ├── models/
    ├── managers/
    ├── ui/
    │   ├── pages/
    │   └── widgets/
    └── resources/
```

If `data/` is missing, create it:
```bash
mkdir data
```

---

## 6. Build the Project

We build **out-of-source** — build files go into a separate `build/` folder.

---

### 🪟 Windows (Command Prompt — not MSYS2 terminal)

```cmd
cd C:\Projects\Connectify

mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64"

mingw32-make -j4
```

---

### 🐧 Linux (Terminal)

```bash
cd ~/Desktop/Connectify

mkdir build && cd build

cmake ..

make -j$(nproc)
```

> If CMake still cannot find Qt, provide the path explicitly:
> ```bash
> cmake .. -DCMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt6"
> ```

---

### 🍎 macOS (Terminal)

```bash
cd ~/Desktop/Connectify

mkdir build && cd build

cmake ..

make -j$(sysctl -n hw.logicalcpu)
```

> If CMake cannot find Qt, use:
> ```bash
> # Apple Silicon
> cmake .. -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"
>
> # Intel Mac
> cmake .. -DCMAKE_PREFIX_PATH="/usr/local/opt/qt@6"
> ```

---

### ✅ Successful Build Output

```
[ 10%] Building CXX object CMakeFiles/Connectify.dir/src/main.cpp.o
[ 20%] Building CXX object ...
...
[100%] Linking CXX executable Connectify
[100%] Built target Connectify
```

The line **Built target Connectify** means you're ready to run.

---

## 7. Run the App

### 🪟 Windows

```cmd
cd C:\Projects\Connectify\build
Connectify.exe
```

> **Getting "missing DLL" errors?**
> Run this once from inside your `build/` folder:
> ```cmd
> C:\msys64\ucrt64\bin\windeployqt6.exe Connectify.exe
> ```
> Then run `Connectify.exe` again.

---

### 🐧 Linux

```bash
cd ~/Desktop/Connectify/build
./Connectify
```

> If you get a **"cannot connect to display"** error on a headless machine, make sure you are running a desktop session (not SSH without X forwarding).

---

### 🍎 macOS

```bash
cd ~/Desktop/Connectify/build
./Connectify
```

> **macOS blocking the app?** Go to **System Settings → Privacy & Security** → scroll down → click **Open Anyway**.

---

## 8. First Login

### 🔑 Built-in Admin Account

| Field    | Value                  |
|----------|------------------------|
| Email    | `admin@connectify.com` |
| Password | `admin123`             |

The Admin panel lets you view all users, ban/unban accounts, and delete posts.

### 👤 Create a Regular User Account

1. On the login screen click **"Create an account"**
2. Fill in your name, email, and a password (minimum 6 characters)
3. Click **Sign Up** — you'll land on your feed immediately

### 🗂️ Where Data Is Saved

All data is stored in plain text files inside the `data/` folder next to your executable:

```
data/
├── users.dat              ← registered accounts
├── posts.dat              ← all posts
├── friends.dat            ← follow graph
├── friend_requests.dat    ← pending requests
├── messages.dat           ← direct messages
└── notifications.dat      ← notifications
```

To **reset all data**, delete the files inside `data/` and restart the app — they will be recreated automatically.

---

## 9. Troubleshooting

### ❌ `cmake` or `g++` not found

Close your terminal and open a **new** one — PATH changes only take effect in new windows. Then retry.

---

### ❌ `Could not find Qt6` during cmake

| OS | Fix |
|----|-----|
| Windows | Run cmake with `-DCMAKE_PREFIX_PATH="C:\msys64\ucrt64"` |
| Linux | Run `sudo apt install qt6-base-dev` then retry cmake |
| macOS | Run `brew install qt@6` then add it to PATH (see Step 4) |

---

### ❌ Missing DLL errors on Windows at runtime

```cmd
C:\msys64\ucrt64\bin\windeployqt6.exe Connectify.exe
```

Run that command from inside your `build/` folder, then launch the app again.

---

### ❌ App crashes instantly on startup

The `data/` directory is missing. Create it next to the executable:

```bash
# Linux / macOS
mkdir -p build/data

# Windows
mkdir build\data
```

Then run the app again.

---

### ❌ Build error: `undefined reference to vtable`

Make sure your `CMakeLists.txt` uses `file(GLOB_RECURSE SOURCES "src/*.cpp")` — this is already present in the provided file. If you added new `.cpp` files manually, delete the `build/` folder and rebuild.

---

### ❌ `mingw32-make` not found on Windows

Make sure `C:\msys64\ucrt64\bin` is in your PATH (Step 2). Alternatively use the universal cmake build command:
```cmd
cmake --build .
```
from inside the `build/` folder — this works without needing `make` directly on your PATH.

---

### 🔄 Nuclear Option — Clean Rebuild

If something is badly broken, wipe the build folder and start fresh:

```bash
# Linux / macOS
rm -rf build/
mkdir build && cd build
cmake ..
make -j$(nproc)
```

```cmd
:: Windows
rmdir /s /q build
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64"
mingw32-make -j4
```

---

## 🎉 Quick Reference Card

| Task | Command / Action |
|------|-----------------|
| Install Qt (Windows) | `pacman -S mingw-w64-ucrt-x86_64-qt6-base` in MSYS2 |
| Install Qt (Linux) | `sudo apt install qt6-base-dev` |
| Install Qt (macOS) | `brew install qt@6` |
| Build the project | `mkdir build && cd build && cmake .. && make -j4` |
| Run the app | `./Connectify` (Linux/Mac) · `Connectify.exe` (Windows) |
| Admin login | `admin@connectify.com` / `admin123` |
| Reset all data | Delete files inside `data/` folder |
| Clean rebuild | Delete `build/` folder, repeat Step 6 |

---

*Connectify — OOP Course Project · C++17 · Qt 6 (LGPL, Free) · CMake · MSYS2*
