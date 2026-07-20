cmake_minimum_required(VERSION 3.15)
project(ATMVerse VERSION 1.0.0 LANGUAGES CXX)

# Enforce C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Define the executable
add_executable(ATMVerse
    src/main.cpp
    src/utils/Utility.cpp
    src/models/Person.cpp
    src/models/Customer.cpp
    src/models/Admin.cpp
    src/models/Account.cpp
    src/models/Card.cpp
    src/models/Transaction.cpp
    src/persistence/JSONManager.cpp
    src/core/Logger.cpp
    src/core/CashDispenser.cpp
    src/core/AuthManager.cpp
    src/core/Receipt.cpp
    src/core/ReportGenerator.cpp
    src/core/BankSystem.cpp
    src/ui/Themes.cpp
    src/ui/TerminalUI.cpp
    src/ui/Animations.cpp
    src/ui/ScreenManager.cpp
    src/core/ATMController.cpp
)

# Include directories
target_include_directories(ATMVerse PRIVATE include)
target_include_directories(ATMVerse PRIVATE extern)
