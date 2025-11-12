# DiskSnapshot

Using VHDX for disk snapshot and MFC for UI

## Overview

DiskSnapshot is a Windows application for creating disk snapshots using the VHDX format. It provides a graphical user interface (GUI) built with Microsoft Foundation Classes (MFC), making it easy to manage and snapshot disks directly from your desktop.

<<<<<<< HEAD
## Develop Process
- **Front End (UI):** Mostly Done (Button mapping remain undone)
- **Back End:** Just Basic Idea

## Note
- This program use legacy UI and diskpart so that it can run even in WinRE or WinPE
- However migrate from legacy part to VHDX snapshots might have following problem:
- - 1.Performance loss (Although no obvious performance loss show in my test(M.2 SSD in USB disk box))
- - 2.Base Knowledge (Build a base VHDX or migrate still require a lot of skills)
- - 3.Protential Unstable (It hasn't been fully tested) (eg. Diff VHDX chain is easy to break)

=======
>>>>>>> refs/remotes/origin/main
## Features

- **VHDX-Based Snapshots:** Capture disk states using Microsoft's VHDX virtual disk format.
- **Intuitive UI:** Easily manage disks and snapshots through a Windows desktop application powered by MFC.
- **High Performance:** Written in C++ and C for speed and reliability on Windows systems.

## Requirements

- Windows 7 or newer
- Visual Studio (recommended for building)
- Administrative privileges (for disk access)
- [MFC](https://learn.microsoft.com/en-us/cpp/mfc/mfc-desktop-applications) libraries

## Getting Started

### Build Instructions

1. Open the project in Visual Studio.
2. Make sure you have MFC support installed.
3. Build the solution (`Release` or `Debug` mode).
4. Run the generated executable with administrator privileges.

### Usage

1. Launch DiskSnapshot.
2. Select the disk you want to snapshot.
3. Click the **Snapshot** button.
4. Manage your snapshots via the application's UI.

## Repository Structure

- `src/` : C++ and C source code for disk snapshot logic.
- `ui/`  : MFC-based UI components.

## Technologies

- **C++** (primary) and **C** (minor) for system and performance code
- **MFC** for the graphical interface
- **VHDX** for snapshot format

## Contributing

Pull requests and issues are welcome! Please check the existing issues before submitting new ones.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Author

[YuanPeixi](https://github.com/YuanPeixi)
