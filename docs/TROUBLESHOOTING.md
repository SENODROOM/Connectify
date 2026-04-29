# TROUBLESHOOTING.md — Build & Runtime Error Reference

---

## Build Errors

---

### ❌ `The C++ compiler is not able to compile a simple test program`

**Cause:** GCC is installed but broken — missing runtime libraries.

**Fix:**
```bash
# In MSYS2 UCRT64 terminal
pacman -Syu --noconfirm
pacman -Su --noconfirm
pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-gcc
```

Then delete the `build\` folder and redo cmake.

---

### ❌ `mingw32-make is not recognized`

**Cause:** `C:\msys64\ucrt64\bin` is not on your Windows PATH.

**Fix:**
1. Win + R → `sysdm.cpl` → Advanced → Environment Variables
2. Edit System `Path` → Add `C:\msys64\ucrt64\bin` and `C:\msys64\usr\bin`
3. Click OK on all windows
4. Close and reopen PowerShell

---

### ❌ `Could not find Qt6`

**Cause:** Qt 6 not installed via MSYS2, or `CMAKE_PREFIX_PATH` not set.

**Fix — install Qt:**
```bash
# In MSYS2 UCRT64 terminal
pacman -S mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools
```

**Fix — set prefix path in cmake:**
```powershell
cmake .. -G "MinGW Makefiles" `
  -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" `
  -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
```

---

### ❌ `'Notification' has not been declared` in Notification.cpp

**Cause:** Old `Notification.cpp` from version 1 of the project is still present. The new version defines everything inside `Notification.h`.

**Fix — empty the file:**
```powershell
Set-Content -Path "C:\Programming\Connectify\src\models\Notification.cpp" `
  -Value "// Defined inline in Notification.h`n// This file is intentionally empty."
```

Then clean and rebuild:
```powershell
cd build
Remove-Item -Recurse -Force *
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
```

---

### ❌ `unable to deduce 'auto*' from lambda` in FeedPage.cpp

**Cause:** GCC 15 is stricter about `auto*` deduction from lambda types.

**Fix — replace the lambda button factory** in `FeedPage.cpp`:

Find and remove:
```cpp
auto* mkBtn = [](const QString& t, const QString& obj) {
    auto* b = new QPushButton(t); b->setObjectName(obj); return b;
};
auto* searchBtn   = mkBtn("Search",        "navBtn");
// ...
for (auto* b : {searchBtn, msgsBtn, notifsBtn, profileBtn}) navL->addWidget(b);
```

Replace with:
```cpp
auto* searchBtn  = new QPushButton("Search");        searchBtn->setObjectName("navBtn");
auto* msgsBtn    = new QPushButton("Messages");      msgsBtn->setObjectName("navBtn");
auto* notifsBtn  = new QPushButton("Notifications"); notifsBtn->setObjectName("navBtn");
auto* profileBtn = new QPushButton("Profile");       profileBtn->setObjectName("navBtn");
auto* logoutBtn  = new QPushButton("Logout");        logoutBtn->setObjectName("dangerBtn");
navL->addWidget(searchBtn);
navL->addWidget(msgsBtn);
navL->addWidget(notifsBtn);
navL->addWidget(profileBtn);
```

---

### ❌ `unable to deduce 'std::initializer_list<auto>'`

**Cause:** Same issue as above — the `for (auto* b : {...})` range loop cannot deduce a common type for separately declared `auto*` variables.

**Fix:** Remove the range loop and add each widget individually (see fix above).

---

### ❌ `undefined reference to vtable for ClassName`

**Cause:** A class has a virtual function declared but its `.cpp` file is missing or the class body is incomplete.

**Fix:**
1. Make sure `ClassName.cpp` exists and is not empty
2. Delete `build\` entirely and rebuild — CMake's GLOB_RECURSE needs a fresh run to pick up new files
```powershell
Remove-Item -Recurse -Force build
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
```

---

### ❌ `error: use of deleted function` on copy

**Cause:** Trying to copy a class that has `= delete` on its copy constructor (all linked list classes and managers).

**Fix:** Pass by pointer or reference, never by value:
```cpp
// ❌ Wrong — tries to copy PostList
PostList copy = user->getPosts();

// ✅ Correct — use const reference
const PostList& posts = user->getPosts();
```

---

### ❌ New `.cpp` file not being compiled

**Cause:** CMake uses `file(GLOB_RECURSE)` which caches the file list. Adding a new file doesn't trigger a re-glob automatically.

**Fix:** Re-run cmake once after adding new files:
```powershell
cd C:\Programming\Connectify\build
cmake ..
mingw32-make -j4
```

---

## Runtime Errors

---

### ❌ App window opens then immediately closes

**Cause 1:** `data\` directory missing next to the executable.
```powershell
mkdir C:\Programming\Connectify\build\data
```

**Cause 2:** Corrupted `.dat` file — a line with missing fields causes a `std::stoi` exception.
```powershell
# Reset all data
Remove-Item C:\Programming\Connectify\build\data\*.dat
```

---

### ❌ Missing DLL error on startup

**Cause:** Qt DLL files are not next to `Connectify.exe`.

**Fix — run once from `build\` folder:**
```powershell
C:\msys64\ucrt64\bin\windeployqt6.exe .\Connectify.exe
```

---

### ❌ Login fails even with correct password

**Cause:** `users.dat` was written by a different version of the app using a different hash or field format.

**Fix:** Delete `users.dat` and re-register:
```powershell
Remove-Item C:\Programming\Connectify\build\data\users.dat
```

---

### ❌ Feed is empty after following users

**Cause:** The followed user has no posts, or the follow relationship was not saved to `friends.dat`.

**Debug steps:**
1. Open `data\friends.dat` — confirm the follow entry exists: `fromID|toID|ACCEPTED`
2. Open `data\posts.dat` — confirm the followed user has posts with their `ownerID`
3. If either file is missing the data, the save did not trigger — check that `AuthManager::saveAll()` was called

---

### ❌ Messages not appearing after restart

**Cause:** `messages.dat` is missing or the header line is duplicated (happens if the file was partially written).

**Fix:**
```powershell
Remove-Item C:\Programming\Connectify\build\data\messages.dat
```

Messages will be gone but the app will work correctly going forward.

---

### ❌ Notifications showing after being marked read

**Cause:** `markAllRead()` rewrites `notifications.dat` but the in-memory `NotifList` was not reloaded. This is a known limitation — notifications are loaded once at startup.

**Workaround:** Restart the app after marking all read.

---

## Nuclear Clean Rebuild

If nothing else works, this always resets to a known good state:

```powershell
cd C:\Programming\Connectify

# Wipe build cache
Remove-Item -Recurse -Force build

# Wipe data (loses all user data)
Remove-Item -Recurse -Force build\data

# Rebuild from scratch
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
.\Connectify.exe
```
