# Contributing to ATMVerse

We welcome contributions to ATMVerse! This document outlines the process for contributing.

## Getting Started
1. Fork the repository.
2. Clone your fork locally.
3. Build the project using CMake to ensure your environment is set up correctly.

## Code Style
- **C++ Standard**: C++17
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `BankSystem`)
  - Methods/Functions: `camelCase` (e.g., `calculateBalance`)
  - Variables: `camelCase` (e.g., `accountNumber`)
  - Private Members: `camelCase_` with trailing underscore (e.g., `balance_`)
  - Constants/Enums: `UPPER_SNAKE_CASE` (e.g., `MAX_ATTEMPTS`)
- **Indentation**: 4 spaces, no tabs.
- **Braces**: Opening brace on the same line as the statement.

## Project Structure
Please adhere to the existing 5-layer architecture (`ui`, `core`, `models`, `persistence`, `utils`). Do not mix UI rendering (e.g., `std::cout`) into the business logic (`core`) or `models`.

## Submitting a Pull Request
1. Create a feature branch: `git checkout -b feature/my-new-feature`
2. Commit your changes: `git commit -m 'Add some feature'`
3. Push to the branch: `git push origin feature/my-new-feature`
4. Submit a Pull Request targeting the `main` branch.

Please ensure your code compiles without warnings and does not break existing functionality.
