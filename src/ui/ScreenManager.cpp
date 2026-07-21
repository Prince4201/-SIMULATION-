#include "atmverse/ui/ScreenManager.h"
#include "atmverse/ui/TerminalUI.h"
#include "atmverse/ui/Themes.h"
#include "atmverse/ui/Animations.h"
#include "atmverse/utils/Utility.h"
#include <iostream>

namespace atmverse {

void ScreenManager::drawBaseScreen(const std::string& title, const std::string& status) {
    TerminalUI::clearScreen();
    TerminalUI::drawATMFrame();
    TerminalUI::drawATMHeader(bankSystem_->getBankName(), util::getCurrentDateTime());
    TerminalUI::drawATMFooter(status);

    if (!title.empty()) {
        TerminalUI::drawCenteredColored(6, title, theme::NEON_BLUE, theme::ATM_WIDTH);
        TerminalUI::drawLine(7, 5, theme::ATM_WIDTH - 10, theme::DARK_GRAY);
    }
}

void ScreenManager::renderWelcome() {
    TerminalUI::clearScreen();
    TerminalUI::drawATMFrame();
    TerminalUI::drawATMHeader(bankSystem_->getBankName(), util::getCurrentDateTime());
    TerminalUI::drawATMFooter("Insert Card | [A] Admin | [C] Create Account");

    Animations::welcomeGlow();

    TerminalUI::drawCenteredColored(16, "Please Insert Your Card", theme::WHITE, theme::ATM_WIDTH);
    TerminalUI::drawCenteredColored(18, theme::BLINK + theme::CYAN + "[ INSERT CARD ]" + theme::RESET, theme::CYAN, theme::ATM_WIDTH);
}

std::string ScreenManager::renderInsertCard() {
    drawBaseScreen("", "Enter 16-digit Card Number");
    TerminalUI::drawCenteredColored(12, "VIRTUAL CARD INSERTION", theme::CYAN, theme::ATM_WIDTH);

    TerminalUI::drawBox(14, 14, 32, 5, theme::DARK_GRAY, true);
    std::string cardNum = TerminalUI::getInput("Card Number: ", 16, 16);

    return cardNum;
}

std::string ScreenManager::renderPinEntry(int attemptsRemaining) {
    drawBaseScreen("SECURE LOGIN", "Use keypad to enter PIN");

    TerminalUI::drawKeypad(5, 10);

    TerminalUI::drawBox(30, 12, 22, 5, theme::DARK_GRAY, true);
    TerminalUI::moveCursor(13, 32);
    std::cout << theme::GRAY << "Attempts left: " << theme::WHITE << attemptsRemaining << theme::RESET;

    return TerminalUI::getMaskedInput("Enter PIN: ", 4, 32, 15);
}

int ScreenManager::renderMainMenu() {
    drawBaseScreen("MAIN MENU", "Select a transaction");

    std::vector<std::string> options = {
        "1. Withdraw Cash", "4. Transfer Funds",
        "2. Deposit Cash",  "5. Mini Statement",
        "3. Check Balance", "6. More Options",
        "0. Logout"
    };

    // Draw grid
    TerminalUI::drawBox(7, 10, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(9, 11, theme::WHITE + options[0]);

    TerminalUI::drawBox(33, 10, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(35, 11, theme::WHITE + options[1]);

    TerminalUI::drawBox(7, 14, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(9, 15, theme::WHITE + options[2]);

    TerminalUI::drawBox(33, 14, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(35, 15, theme::WHITE + options[3]);

    TerminalUI::drawBox(7, 18, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(9, 19, theme::WHITE + options[4]);

    TerminalUI::drawBox(33, 18, 20, 3, theme::NEON_BLUE);
    TerminalUI::drawText(35, 19, theme::WHITE + options[5]);

    TerminalUI::drawBox(20, 22, 20, 3, theme::RED);
    TerminalUI::drawText(26, 23, theme::WHITE + options[6]);

    return TerminalUI::getMenuChoice(0, 6, 20, 26);
}

double ScreenManager::renderWithdraw() {
    drawBaseScreen("CASH WITHDRAWAL", "Select amount or enter custom");

    std::vector<std::string> options = {
        "1. ₹ 500", "4. ₹ 5,000",
        "2. ₹ 1,000", "5. ₹ 10,000",
        "3. ₹ 2,000", "6. Custom Amount",
        "0. Cancel"
    };

    TerminalUI::drawBox(7, 10, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(9, 11, theme::WHITE + options[0]);
    TerminalUI::drawBox(33, 10, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(35, 11, theme::WHITE + options[1]);
    TerminalUI::drawBox(7, 14, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(9, 15, theme::WHITE + options[2]);
    TerminalUI::drawBox(33, 14, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(35, 15, theme::WHITE + options[3]);
    TerminalUI::drawBox(7, 18, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(9, 19, theme::WHITE + options[4]);
    TerminalUI::drawBox(33, 18, 20, 3, theme::NEON_BLUE); TerminalUI::drawText(35, 19, theme::WHITE + options[5]);
    TerminalUI::drawBox(20, 22, 20, 3, theme::DARK_GRAY); TerminalUI::drawText(26, 23, theme::GRAY + options[6]);

    int choice = TerminalUI::getMenuChoice(0, 6, 20, 26);

    switch (choice) {
        case 1: return 500;
        case 2: return 1000;
        case 3: return 2000;
        case 4: return 5000;
        case 5: return 10000;
        case 6: {
            TerminalUI::drawBox(14, 20, 32, 5, theme::DARK_GRAY, true);
            std::string amt = TerminalUI::getInput("Enter Amount (₹): ", 16, 22);
            if (util::isNumeric(amt)) {
                return std::stod(amt);
            }
            return -1;
        }
        case 0: return 0;
        default: return 0;
    }
}

double ScreenManager::renderDeposit() {
    drawBaseScreen("CASH DEPOSIT", "Enter total amount to deposit");
    TerminalUI::drawCenteredColored(10, "Accepting denominations: ₹100, ₹200, ₹500, ₹2000", theme::GRAY, theme::ATM_WIDTH);
    TerminalUI::drawBox(14, 13, 32, 5, theme::DARK_GRAY, true);
    std::string amt = TerminalUI::getInput("Amount (₹): ", 16, 15);

    if (util::isNumeric(amt)) {
        double val = std::stod(amt);
        if (static_cast<int>(val) % 100 == 0) return val;
        TerminalUI::showError("Amount must be multiple of 100");
        return -1;
    }
    return 0;
}

void ScreenManager::renderBalance(const Customer& customer) {
    drawBaseScreen("ACCOUNT BALANCE", "Press any key to return");

    TerminalUI::drawBox(10, 10, 40, 10, theme::NEON_BLUE, true);
    TerminalUI::moveCursor(12, 12);
    std::cout << theme::GRAY << "Account Type: " << theme::WHITE << customer.getAccount().getTypeString();
    TerminalUI::moveCursor(14, 12);
    std::cout << theme::GRAY << "Account No  : " << theme::WHITE << "XXXX" << customer.getAccount().getAccountNumber().substr(8);
    TerminalUI::moveCursor(16, 12);
    std::cout << theme::GRAY << "Available   : " << theme::NEON_GREEN << util::formatCurrency(customer.getAccount().getAvailableBalance());
    TerminalUI::moveCursor(18, 12);
    std::cout << theme::GRAY << "Daily Limit : " << theme::WHITE << util::formatCurrency(customer.getCard().getDailyRemaining());

    TerminalUI::waitForKey();
}

std::pair<std::string, double> ScreenManager::renderTransfer() {
    drawBaseScreen("FUND TRANSFER", "Enter beneficiary details");
    TerminalUI::drawBox(10, 10, 40, 10, theme::DARK_GRAY, true);
    std::string acc = TerminalUI::getInput("Receiver A/C: ", 12, 12);
    if (acc.empty()) return {"", 0};

    std::string amtStr = TerminalUI::getInput("Amount (₹): ", 12, 16);
    double amt = 0;
    if (util::isNumeric(amtStr)) amt = std::stod(amtStr);

    return {acc, amt};
}

void ScreenManager::renderMiniStatement(const Customer& customer) {
    drawBaseScreen("MINI STATEMENT", "Press any key to return");

    auto txns = customer.getLastNTransactions(7); // Show last 7

    std::vector<std::string> headers = {"Date", "Type", "Amount (₹)"};
    std::vector<int> widths = {10, 15, 12};
    std::vector<std::vector<std::string>> rows;

    for (const auto& t : txns) {
        std::string sign = (t.getType() == TransactionType::WITHDRAW || t.getType() == TransactionType::TRANSFER_OUT) ? "-" : "+";
        std::string col = (sign == "-") ? theme::RED : theme::NEON_GREEN;
        rows.push_back({
            t.getDate().substr(5),
            util::truncate(t.getTypeString(), 15),
            col + sign + std::to_string(static_cast<int>(t.getAmount())) + theme::RESET
        });
    }

    if (rows.empty()) {
        TerminalUI::drawCenteredColored(14, "No recent transactions.", theme::GRAY, theme::ATM_WIDTH);
    } else {
        TerminalUI::drawTable(5, 10, headers, rows, widths);
    }

    TerminalUI::waitForKey();
}

void ScreenManager::renderTransactionHistory(const Customer& customer) {
    drawBaseScreen("TRANSACTION HISTORY", "Press any key to return");
    // Simplified for terminal, just shows more transactions than mini statement
    auto txns = customer.getLastNTransactions(12);

    TerminalUI::moveCursor(9, 3);
    std::cout << theme::NEON_BLUE << "Date       | Type            | Amount    | Balance" << theme::RESET;
    TerminalUI::drawLine(10, 3, 54, theme::DARK_GRAY);

    int y = 11;
    for (const auto& t : txns) {
        TerminalUI::moveCursor(y++, 3);
        std::string sign = (t.getType() == TransactionType::WITHDRAW || t.getType() == TransactionType::TRANSFER_OUT) ? "-" : "+";
        std::string col = (sign == "-") ? theme::RED : theme::NEON_GREEN;
        std::cout << theme::LIGHT_GRAY << t.getDate() << " | "
                  << util::padRight(util::truncate(t.getTypeString(), 15), 15) << " | "
                  << col << util::padRight(sign + std::to_string(static_cast<int>(t.getAmount())), 9) << theme::LIGHT_GRAY << " | "
                  << util::formatCurrency(t.getBalanceAfter()) << theme::RESET;
    }

    TerminalUI::waitForKey();
}

int ScreenManager::renderCardSettings(const Customer& customer) {
    drawBaseScreen("CARD SETTINGS", "Select option");

    std::vector<std::string> options = {
        "1. View Card Details",
        "2. Change PIN",
        "3. Block/Unblock Card",
        "0. Back"
    };

    TerminalUI::drawBox(14, 10, 32, 12, theme::DARK_GRAY, true);
    int y = 12;
    for (const auto& opt : options) {
        TerminalUI::drawText(16, y, theme::WHITE + opt);
        y += 2;
    }

    return TerminalUI::getMenuChoice(0, 3, 16, 20);
}

void ScreenManager::renderProfile(const Customer& customer) {
    drawBaseScreen("USER PROFILE", "Press any key to return");

    TerminalUI::drawBox(10, 10, 40, 12, theme::NEON_BLUE, true);
    TerminalUI::moveCursor(12, 12);
    std::cout << theme::GRAY << "Name    : " << theme::WHITE << customer.getName();
    TerminalUI::moveCursor(14, 12);
    std::cout << theme::GRAY << "Phone   : " << theme::WHITE << customer.getPhone();
    TerminalUI::moveCursor(16, 12);
    std::cout << theme::GRAY << "Email   : " << theme::WHITE << customer.getEmail();
    TerminalUI::moveCursor(18, 12);
    std::cout << theme::GRAY << "Joined  : " << theme::WHITE << customer.getCreatedAt().substr(0, 10);
    TerminalUI::moveCursor(20, 12);
    std::cout << theme::GRAY << "Status  : " << (customer.getAccount().isActive() ? theme::NEON_GREEN + "Active" : theme::RED + "Inactive");

    TerminalUI::waitForKey();
}

std::pair<std::string, std::string> ScreenManager::renderAdminLogin() {
    drawBaseScreen("ADMINISTRATOR LOGIN", "Enter credentials");

    TerminalUI::drawBox(14, 12, 32, 7, theme::DARK_GRAY, true);
    std::string id = TerminalUI::getInput("Admin ID: ", 16, 14);
    std::string pwd = TerminalUI::getMaskedInput("Password: ", 20, 16, 16);

    return {id, pwd};
}

int ScreenManager::renderAdminDashboard() {
    drawBaseScreen("ADMIN DASHBOARD", "Select action");

    TerminalUI::drawBox(4, 9, 24, 3, theme::NEON_BLUE); TerminalUI::drawText(6, 10, theme::WHITE + "1. Reports");
    TerminalUI::drawBox(32, 9, 24, 3, theme::NEON_BLUE); TerminalUI::drawText(34, 10, theme::WHITE + "2. Cash Management");
    TerminalUI::drawBox(4, 13, 24, 3, theme::NEON_BLUE); TerminalUI::drawText(6, 14, theme::WHITE + "3. User Search");
    TerminalUI::drawBox(32, 13, 24, 3, theme::NEON_BLUE); TerminalUI::drawText(34, 14, theme::WHITE + "4. System Logs");
    TerminalUI::drawBox(4, 17, 24, 3, theme::NEON_BLUE); TerminalUI::drawText(6, 18, theme::WHITE + "5. Approve Accounts");
    TerminalUI::drawBox(32, 17, 24, 3, theme::RED); TerminalUI::drawText(34, 18, theme::WHITE + "0. Logout");

    // Show pending count badge
    auto pending = bankSystem_->getPendingRequests();
    if (!pending.empty()) {
        TerminalUI::moveCursor(18, 26);
        std::cout << theme::RED << theme::BOLD << " (" << pending.size() << ")" << theme::RESET;
    }

    return TerminalUI::getMenuChoice(0, 5, 20, 22);
}

void ScreenManager::renderAdminReports() {
    drawBaseScreen("SYSTEM REPORTS", "Press any key to return");
    auto report = bankSystem_->getReportGenerator().generateDailyReport(
        bankSystem_->getCustomers(), bankSystem_->getCashDispenser().getTotalCash());
    auto lines = bankSystem_->getReportGenerator().renderReport(report);
    TerminalUI::printLines(lines, 9);
    TerminalUI::waitForKey();
}

void ScreenManager::renderAdminCash() {
    drawBaseScreen("ATM CASH MANAGEMENT", "Press any key to return");
    auto lines = bankSystem_->getCashDispenser().renderBarGraph();
    TerminalUI::printLines(lines, 9);
    TerminalUI::waitForKey();
}

void ScreenManager::renderApplyAccount() {
    drawBaseScreen("OPEN NEW ACCOUNT", "Fill in your details");

    TerminalUI::drawBox(8, 9, 44, 16, theme::NEON_BLUE, true);

    TerminalUI::moveCursor(10, 10);
    std::cout << theme::CYAN << theme::BOLD << "  Account Application Form" << theme::RESET;
    TerminalUI::drawLine(10, 11, 40, theme::DARK_GRAY);

    std::string name  = TerminalUI::getInput("Full Name : ", 10, 13);
    std::string phone = TerminalUI::getInput("Phone     : ", 10, 15);
    std::string email = TerminalUI::getInput("Email     : ", 10, 17);

    // Account type selection
    TerminalUI::moveCursor(19, 10);
    std::cout << theme::CYAN << "Account Type:" << theme::RESET;
    TerminalUI::drawBox(10, 20, 18, 3, theme::NEON_GREEN);
    TerminalUI::drawText(12, 21, theme::WHITE + "1. Savings");
    TerminalUI::drawBox(30, 20, 18, 3, theme::NEON_GREEN);
    TerminalUI::drawText(32, 21, theme::WHITE + "2. Current");

    int typeChoice = TerminalUI::getMenuChoice(1, 2, 20, 23);
    std::string accountType = (typeChoice == 2) ? "Current" : "Savings";

    // PIN setup
    drawBaseScreen("SET YOUR PIN", "Choose a 4-digit PIN");
    TerminalUI::drawBox(14, 12, 32, 7, theme::NEON_BLUE, true);
    std::string pin1 = TerminalUI::getMaskedInput("Set PIN    : ", 4, 16, 14);
    std::string pin2 = TerminalUI::getMaskedInput("Confirm PIN: ", 4, 16, 16);

    if (pin1 != pin2 || pin1.length() != 4) {
        TerminalUI::showError("PINs do not match or invalid length!");
        return;
    }

    if (name.empty() || phone.empty() || email.empty()) {
        TerminalUI::showError("All fields are required!");
        return;
    }

    Animations::processingDots("Submitting Application");

    if (bankSystem_->submitAccountRequest(name, phone, email, pin1, accountType)) {
        drawBaseScreen("APPLICATION SUBMITTED", "");
        TerminalUI::drawBox(8, 10, 44, 10, theme::NEON_GREEN, true);
        TerminalUI::moveCursor(12, 12);
        std::cout << theme::NEON_GREEN << theme::BOLD << "  Application Submitted!" << theme::RESET;
        TerminalUI::moveCursor(14, 12);
        std::cout << theme::WHITE << "  Your request is PENDING" << theme::RESET;
        TerminalUI::moveCursor(15, 12);
        std::cout << theme::WHITE << "  admin approval." << theme::RESET;
        TerminalUI::moveCursor(17, 12);
        std::cout << theme::GRAY << "  You will receive your" << theme::RESET;
        TerminalUI::moveCursor(18, 12);
        std::cout << theme::GRAY << "  Card Number upon approval." << theme::RESET;
        TerminalUI::waitForKey();
    } else {
        TerminalUI::showError("Failed to submit application!");
    }
}

void ScreenManager::renderAdminApproveAccounts() {
    auto pending = bankSystem_->getPendingRequests();

    drawBaseScreen("APPROVE ACCOUNTS", std::to_string(pending.size()) + " pending request(s)");

    if (pending.empty()) {
        TerminalUI::drawCenteredColored(14, "No pending account requests", theme::GRAY, theme::ATM_WIDTH);
        TerminalUI::waitForKey();
        return;
    }

    int y = 9;
    int idx = 0;
    for (const auto& req : pending) {
        if (y > 22) break; // prevent overflow
        std::string color = (idx % 2 == 0) ? theme::NEON_BLUE : theme::CYAN;
        TerminalUI::drawBox(4, y, 52, 4, color, true);
        TerminalUI::moveCursor(y + 1, 6);
        std::cout << theme::WHITE << theme::BOLD << req.getName()
                  << theme::RESET << theme::GRAY << "  |  " << req.getAccountType()
                  << "  |  " << req.getTimestamp() << theme::RESET;
        TerminalUI::moveCursor(y + 2, 6);
        std::cout << theme::DARK_GRAY << "ID: " << req.getRequestId()
                  << "  Phone: " << req.getPhone() << theme::RESET;
        y += 5;
        ++idx;
    }

    TerminalUI::drawLine(4, y, 52, theme::DARK_GRAY);
    TerminalUI::moveCursor(y + 1, 6);
    std::cout << theme::CYAN << "Enter Request ID to approve (or 'R' + ID to reject):" << theme::RESET;
    std::string input = TerminalUI::getInput("> ", 6, y + 2);
    input = util::toUpper(input);

    if (input.empty()) return;

    if (input.length() > 2 && (input.substr(0, 2) == "R " || input.substr(0, 2) == "r ")) {
        std::string reqId = util::trim(input.substr(2));
        Animations::processingDots("Rejecting");
        if (bankSystem_->rejectRequest(reqId)) {
            TerminalUI::showError("Request " + reqId + " REJECTED");
        } else {
            TerminalUI::showError("Request ID not found!");
        }
    } else {
        Animations::processingDots("Approving Account");
        if (bankSystem_->approveRequest(input)) {
            // Find the newly created customer to show card number
            auto& customers = bankSystem_->getCustomersRef();
            if (!customers.empty()) {
                auto& newCust = customers.back();
                drawBaseScreen("ACCOUNT APPROVED", "");
                TerminalUI::drawBox(6, 10, 48, 12, theme::NEON_GREEN, true);
                TerminalUI::moveCursor(12, 8);
                std::cout << theme::NEON_GREEN << theme::BOLD << "  Account Created Successfully!" << theme::RESET;
                TerminalUI::moveCursor(14, 8);
                std::cout << theme::GRAY << "  Name    : " << theme::WHITE << newCust.getName();
                TerminalUI::moveCursor(15, 8);
                std::cout << theme::GRAY << "  Account : " << theme::CYAN << newCust.getAccount().getAccountNumber();
                TerminalUI::moveCursor(16, 8);
                std::cout << theme::GRAY << "  Card    : " << theme::NEON_GREEN << newCust.getCard().getCardNumber();
                TerminalUI::moveCursor(17, 8);
                std::cout << theme::GRAY << "  Type    : " << theme::WHITE << newCust.getCard().getCardTypeString();
                TerminalUI::moveCursor(19, 8);
                std::cout << theme::GOLD << "  Share Card Number with the customer." << theme::RESET;
                TerminalUI::waitForKey();
            }
        } else {
            TerminalUI::showError("Request ID not found!");
        }
    }
}

void ScreenManager::renderAdminUserSearch() {
    drawBaseScreen("USER SEARCH");
    
    TerminalUI::moveCursor(8, 6);
    std::cout << theme::CYAN << "Search by Name, Phone, Account, or Card Number" << theme::RESET;
    
    std::string query = TerminalUI::getInput("> ", 6, 10);
    if (query.empty()) return;

    auto results = bankSystem_->searchCustomers(query);

    drawBaseScreen("SEARCH RESULTS", std::to_string(results.size()) + " found");

    if (results.empty()) {
        TerminalUI::drawCenteredColored(14, "No customers found matching: " + query, theme::GRAY, theme::ATM_WIDTH);
        TerminalUI::waitForKey();
        return;
    }

    int y = 8;
    for (size_t i = 0; i < results.size() && y < 24; ++i) {
        Customer* cust = results[i];
        std::string color = (i % 2 == 0) ? theme::NEON_BLUE : theme::CYAN;
        TerminalUI::drawBox(4, y, 52, 4, color, true);
        TerminalUI::moveCursor(y + 1, 6);
        std::cout << theme::WHITE << theme::BOLD << cust->getName() 
                  << theme::RESET << theme::GRAY << "  |  " << cust->getPhone();
        TerminalUI::moveCursor(y + 2, 6);
        std::cout << theme::DARK_GRAY << "Acc: " << cust->getAccount().getAccountNumber()
                  << "  |  Card: " << util::maskCardNumber(cust->getCard().getCardNumber());
        y += 5;
    }

    TerminalUI::waitForKey();
}

void ScreenManager::renderAdminLogs() {
    drawBaseScreen("SYSTEM LOGS");
    
    auto logs = bankSystem_->getLogger().getRecentLogs(10);
    
    if (logs.empty()) {
        TerminalUI::drawCenteredColored(14, "No system logs available", theme::GRAY, theme::ATM_WIDTH);
        TerminalUI::waitForKey();
        return;
    }

    int y = 6;
    for (size_t i = 0; i < logs.size() && y < 27; ++i) {
        const auto& log = logs[i];
        std::string type = log.value("type", "UNKNOWN");
        std::string timestamp = log.value("timestamp", "");
        std::string userId = log.value("userId", "");
        std::string details = log.value("details", "");

        std::string color = theme::CYAN;
        if (type == "FAILED_LOGIN" || type == "CARD_FREEZE") color = theme::RED;
        else if (type == "LOGIN" || type == "ACCOUNT_CREATE") color = theme::NEON_GREEN;

        TerminalUI::moveCursor(y++, 6);
        std::cout << color << "[" << timestamp << "] " << theme::WHITE << type;
        TerminalUI::moveCursor(y++, 8);
        std::cout << theme::GRAY << "User: " << userId << " | " << details << theme::RESET;
    }

    TerminalUI::waitForKey();
}

} // namespace atmverse
