# ATMVerse 🏧

**ATMVerse** is a production-quality, immersive ATM and Banking Simulation System built entirely in C++17. It features a stunning ANSI terminal UI, advanced object-oriented architecture, and robust JSON-based data persistence.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)

## Features 🚀

- **Immersive Terminal UI**: Stunning neon-blue aesthetic with ANSI escape codes and Unicode box-drawing.
- **Realistic Animations**: Simulates card insertion, cash counting, dispensing, and receipt printing.
- **Robust Authentication**: SHA-256 style PIN hashing with a 3-attempt lockout mechanism.
- **Smart Cash Dispenser**: Greedy algorithm for optimal note distribution (₹100, ₹200, ₹500, ₹2000).
- **JSON Persistence**: Uses `nlohmann/json` to store accounts, cards, transactions, and logs locally (no DB needed).
- **Comprehensive Logging**: Tracks every system event for auditing.

## Screenshots 📸

*(Placeholder for Screenshots)*
- `docs/screenshots/welcome.png`
- `docs/screenshots/pin.png`
- `docs/screenshots/withdraw.png`
- `docs/screenshots/receipt.png`

## Getting Started 🛠️

### Prerequisites
- A modern terminal supporting ANSI colors (Windows Terminal, VS Code Terminal, macOS Terminal, or modern Linux terminals).
- CMake 3.15+
- C++17 compatible compiler (GCC, Clang, MSVC)

### Build Instructions

1. **Clone the repository** (or download the source):
   ```bash
   git clone https://github.com/yourusername/ATMVerse.git
   cd ATMVerse
   ```

2. **Build with CMake**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the Application**:
   ```bash
   # Windows
   .\ATMVerse.exe
   
   # Linux/macOS
   ./ATMVerse
   ```

## Default Credentials 🔐

On the first run, the system will auto-generate seed data in the `data/` folder.

**Customer (Test Account):**
- **Card Number**: `4726 8234 7734 9921` (Format: `4726823477349921`)
- **PIN**: `1234`

**Admin Login (Press 'A' on Welcome Screen):**
- **Admin ID**: `admin001`
- **Password**: `1234`

## Architecture 🏛️

ATMVerse follows a strict 5-layer architecture:
1. **Presentation Layer**: `TerminalUI`, `Animations`, `ScreenManager`
2. **Controller Layer**: `ATMController` (State Machine)
3. **Business Logic Layer**: `BankSystem`, `AuthManager`, `CashDispenser`
4. **Model Layer**: `Customer`, `Admin`, `Card`, `Account`
5. **Persistence Layer**: `JSONManager`

See [Architecture Docs](docs/ARCHITECTURE.md) for detailed diagrams.

## License 📜
This project is licensed under the MIT License - see the LICENSE file for details.
