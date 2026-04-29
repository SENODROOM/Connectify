# SETUP.md — Complete Windows Setup Guide (100% Free)

> Everything from zero — compiler, Qt, build, and run. All tools are free and open-source.

---

## Prerequisites Checklist

Before starting, confirm you have nothing installed — or skip to whichever step you need.

- [ ] MSYS2 installed
- [ ] GCC compiler working
- [ ] CMake working  
- [ ] Qt 6 installed
- [ ] Project files extracted
- [ ] First build successful

---

## Step 1 — Install MSYS2

MSYS2 is a free package manager for Windows that gives you GCC, CMake, and Qt all in one place.

1. Go to **https://www.msys2.org**
2. Download `msys2-x86_64-*.exe`
3. Run the installer — keep the default path `C:\msys64`
4. When installation finishes, the **MSYS2 UCRT64** terminal opens automatically

---

## Step 2 — Install the Full Toolchain

Inside the **MSYS2 UCRT64** terminal (blue icon in Start Menu), run:

```bash
pacman -Syu --noconfirm
```

The terminal may close itself — that is normal. Reopen **MSYS2 UCRT64** and run:

```bash
pacman -Su --noconfirm
```

Then install everything needed:

```bash
pacman -S --needed --noconfirm \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-make \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-qt6-base \
  mingw-w64-ucrt-x86_64-qt6-tools
```

This installs GCC 15, CMake, Make, Ninja, and Qt 6. Wait for it to finish (5–15 minutes).

---

## Step 3 — Add to Windows PATH

So that PowerShell can find these tools:

1. Press **Win + R**, type `sysdm.cpl`, press Enter
2. Click **Advanced** → **Environment Variables**
3. Under **System Variables**, click on **Path** → **Edit**
4. Click **New** and add each of these — one at a time:

```
C:\msys64\ucrt64\bin
C:\msys64\usr\bin
```

5. Click **OK** on all windows

### Verify — open a NEW PowerShell window and run:

```powershell
g++ --version
cmake --version
mingw32-make --version
```

All three must print version numbers. If any fails, close PowerShell and open a new one.

Expected output:
```
g++.exe (Rev14, Built by MSYS2 project) 15.2.0
cmake version 4.2.x
GNU Make 4.4.1
```

---

## Step 4 — Get the Project Files

### Option A — From ZIP

1. Right-click `Connectify.zip` → **Extract All**
2. Extract to `C:\Programming\Connectify`

### Option B — From Git

```powershell
cd C:\Programming
git clone https://github.com/your-team/Connectify.git
```

Your folder must look like this:

```
C:\Programming\Connectify\
├── CMakeLists.txt
├── run.bat
├── data\
└── src\
    ├── main.cpp
    ├── models\
    ├── managers\
    ├── ui\
    └── resources\
```

---

## Step 5 — First Build

Open PowerShell and run:

```powershell
cd C:\Programming\Connectify
mkdir build
cd build

cmake .. -G "MinGW Makefiles" `
  -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" `
  -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"

mingw32-make -j4
```

A successful build ends with:
```
[100%] Linking CXX executable Connectify.exe
[100%] Built target Connectify
```

---

## Step 6 — Run the App

```powershell
.\Connectify.exe
```

If you see **"missing DLL"** errors, run this once:
```powershell
C:\msys64\ucrt64\bin\windeployqt6.exe .\Connectify.exe
```
Then run `.\Connectify.exe` again.

---

## Step 7 — Every Time After This

Just double-click `run.bat` in the project root, or from PowerShell:

```powershell
C:\Programming\Connectify\run.bat
```

That's it. No cmake needed again unless you add new `.cpp` files.

---

## First Login

| Role | Email | Password |
|---|---|---|
| Admin | `admin@connectify.com` | `admin123` |
| User | Register via Sign Up | your choice |

---

## Troubleshooting

### cmake says "CXX compiler broken"

Open MSYS2 UCRT64 terminal and reinstall the compiler:
```bash
pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-gcc
```

Then delete your `build\` folder and redo Step 5.

### `mingw32-make` not found in PowerShell

PATH was not saved correctly. Redo Step 3 and make sure you clicked OK on **all three** dialog windows. Then close and reopen PowerShell.

### `Could not find Qt6` during cmake

Qt was not installed. Open MSYS2 UCRT64 terminal:
```bash
pacman -S mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools
```

Then delete `build\` and redo Step 5.

### App crashes immediately

The `data\` folder is missing next to the executable. Create it:
```powershell
mkdir C:\Programming\Connectify\build\data
```

### Clean rebuild (nuclear option)

```powershell
cd C:\Programming\Connectify
Remove-Item -Recurse -Force build
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\msys64\ucrt64" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe"
mingw32-make -j4
```
