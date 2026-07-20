# ATMVerse Architecture

This project is built using C++17 with a strict **Object-Oriented Design (OOP)** following a 5-layer architecture.

## 1. Presentation Layer (`ui/`)
Responsible for all user interactions, drawing the ATM interface, and playing animations.
- **`TerminalUI`**: Low-level ANSI escape code wrappers, cursor manipulation, input masking (`_getch` / `termios`).
- **`ScreenManager`**: High-level screen compositor (draws the Main Menu, Withdraw Screen, etc.).
- **`Animations`**: Simulates hardware actions (card insertion, cash dispensing) using timed console writes.
- **`Themes`**: ANSI color constants and Unicode box-drawing characters.

## 2. Controller Layer (`core/`)
Manages the application state and orchestrates the flow.
- **`ATMController`**: A State Machine (`ScreenState`) that loops continually, handles user input from `ScreenManager`, and delegates business logic to the `BankSystem`.

## 3. Business Logic Layer (`core/`)
Handles all banking rules.
- **`BankSystem`**: The central orchestrator. Maintains `unordered_map` indices for $O(1)$ customer lookups. Handles ACID-like transaction flows (e.g., deducting balance before dispensing cash, rolling back if dispensing fails).
- **`AuthManager`**: Validates PINs using a Salt + XOR hashing algorithm. Manages the 3-attempt lockout policy and session timeouts.
- **`CashDispenser`**: Implements a Greedy Algorithm to optimally dispense cash using available denominations (₹2000, ₹500, ₹200, ₹100).
- **`ReportGenerator`**: Aggregates data and uses `priority_queue` to find the most active users.
- **`Logger`**: Records every system action for auditing.

## 4. Model Layer (`models/`)
Data representations using encapsulation, inheritance, and polymorphism.
- **`Person` (Abstract Base Class)**: Contains common attributes (`id`, `name`) and virtual serialization methods.
- **`Customer`**: Inherits `Person`. Composed of `Account` and `Card`. Contains a `vector<Transaction>`.
- **`Admin`**: Inherits `Person`. Contains admin-specific authentication logic.
- **`Account`, `Card`, `Transaction`**: Core banking entities with appropriate getters/setters and JSON serialization logic.

## 5. Persistence Layer (`persistence/`)
- **`JSONManager`**: Built on top of `nlohmann/json`. Handles thread-safe file I/O to the `data/` directory. Automatically seeds data if files are missing.

---

## Data Structures Used
| Data Structure | Purpose | Layer |
| :--- | :--- | :--- |
| `std::unordered_map` | Fast $O(1)$ lookups by Card/Account numbers | `BankSystem` |
| `std::map` | Ordered storage of cash denominations | `CashDispenser` |
| `std::vector` | General lists (transactions, customers, logs) | Models |
| `std::stack` | UI Screen navigation (Back button functionality) | `ATMController` |
| `std::priority_queue` | Finding the "Most Active User" efficiently | `ReportGenerator` |
| `std::set` | Enforcing globally unique account/card numbers | `BankSystem` |
