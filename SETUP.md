# 🛠️ Fix Guide — Windows Build Errors (Connectify)

You hit two errors:

1. `The C++ compiler is not able to compile a simple test program` — broken compiler
2. `mingw32-make is not recognized` — make tool not on PATH

Follow every step below **in order**. Do not skip any step.

---

## Step 1 — Fix the Broken Compiler

Your MSYS2 installation is missing essential packages. We will install everything fresh.

### 1a — Open the correct terminal

Press **Win** → search for **"MSYS2 UCRT64"** → click it.

> ⚠️ Do NOT use regular Command Prompt or PowerShell for this step. It must be the MSYS2 UCRT64 terminal (blue icon).

### 1b — Install the full toolchain

Paste this entire command into the MSYS2 UCRT64 terminal and press Enter:

```bash
pacman -Syu --noconfirm
```

It will update the package database. The terminal **may close itself** — that is normal. Reopen **MSYS2 UCRT64** and run:

```bash
pacman -Su --noconfirm
```

Then install the compiler and build tools:

```bash
pacman -S --needed --noconfirm \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-make \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-qt6-base \
  mingw-w64-ucrt-x86_64-qt6-tools
```

Wait for it to finish (may take 5–15 minutes on first run).

### 1c — Verify the compiler works

Still inside the MSYS2 UCRT64 terminal, run:

```bash
g++ --version
```

You should see something like:

```
g++ (Rev1, Built by MSYS2 project) 13.x.x
```

If you see a version number — the compiler is working. ✅

---

## Step 2 — Fix PATH (so PowerShell can find the tools)

You must add the MSYS2 tools to your Windows PATH so that PowerShell and Command Prompt can use them.

### 2a — Open Environment Variables

1. Press **Win + R**, type `sysdm.cpl`, press **Enter**
2. Click the **Advanced** tab
3. Click **Environment Variables**

### 2b — Edit the System PATH

1. Under **System variables** (bottom box), find the row called **Path**
2. Click on it once to select it
3. Click **Edit**
4. Click **New** and add these two entries — one at a time:

```
C:\msys64\ucrt64\bin
```

```
C:\msys64\usr\bin
```

5. Click **OK** → **OK** → **OK** on all windows

### 2c — Close ALL PowerShell windows

PATH changes only take effect in **newly opened** terminals. Close every PowerShell and Command Prompt window you have open right now.

### 2d — Reopen PowerShell and verify

Open a **new** PowerShell window and run each line:

```powershell
g++ --version
cmake --version
mingw32-make --version
```

All three should print version numbers. If any still fails, double-check that you saved the PATH correctly in Step 2b and that you opened a **new** terminal.

---

## Step 3 — Clean Old Build Files

The failed cmake run left broken cache files behind. You must delete them before trying again.

Open PowerShell and run:

```powershell
cd C:\Programming\Connectify
Remove-Item -Recurse -Force build
mkdir build
cd build
```

---

## Step 4 — Build the Project

Now run cmake. Use the **explicit compiler path** flags to make sure it picks up the correct GCC:

```powershell
cmake .. `
  -G "MinGW Makefiles" `
  -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" `
  -DCMAKE_C_COMPILER="C:/msys64/ucrt64/bin/gcc.exe" `
  -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
```

> The backtick `` ` `` at the end of each line is PowerShell's line-continuation character.
> If you prefer one line, paste this instead:

```powershell
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_C_COMPILER="C:/msys64/ucrt64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
```

You should see output ending with:

```
-- Build files have been written to: C:/Programming/Connectify/build
```

Then compile:

```powershell
mingw32-make -j4
```

A successful build ends with:

```
[100%] Linking CXX executable Connectify.exe
[100%] Built target Connectify
```

---

## Step 5 — Run the App

```powershell
.\Connectify.exe
```

### If you get "missing DLL" errors on launch

Run this once from inside your `build\` folder:

```powershell
C:\msys64\ucrt64\bin\windeployqt6.exe .\Connectify.exe
```

Then run `.\Connectify.exe` again.

---

## Quick Reference — All Commands in Order

Open a **new** PowerShell after fixing PATH, then:

```powershell
# Clean
cd C:\Programming\Connectify
Remove-Item -Recurse -Force build
mkdir build
cd build

# Configure
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_C_COMPILER="C:/msys64/ucrt64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"

# Build
mingw32-make -j4

# Run
.\Connectify.exe
```

---

## Troubleshooting

### ❌ `g++` still not found after fixing PATH

Open **MSYS2 UCRT64** terminal and run:

```bash
which g++
```

It should say `/ucrt64/bin/g++`. If it doesn't, re-run the pacman install command from Step 1b.

### ❌ cmake says `Could not find Qt6`

Qt was not installed by pacman. Open MSYS2 UCRT64 and run:

```bash
pacman -S mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools
```

### ❌ `mingw32-make` still not found in PowerShell

Check that `C:\msys64\ucrt64\bin` is exactly in your PATH — no typos, no extra spaces. Then close and reopen PowerShell completely.

### ❌ cmake still picks the wrong compiler

Delete the build folder entirely and rerun cmake with the explicit `-DCMAKE_C_COMPILER` and `-DCMAKE_CXX_COMPILER` flags from Step 4. CMake caches compiler detection — a full clean is required.

### ❌ App opens then immediately crashes

The `data\` folder is missing next to the executable. Create it:

```powershell
mkdir C:\Programming\Connectify\build\data
```

Then run the app again.

---

_Connectify — Windows Build Fix Guide_
