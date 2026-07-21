#include "atmverse/core/ATMController.h"
#include "atmverse/ui/TerminalUI.h"
#include "atmverse/ui/Animations.h"
#include "atmverse/ui/Themes.h"
#include "atmverse/core/Receipt.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

ATMController::ATMController(BankSystem* bankSystem)
    : bankSystem_(bankSystem), screenManager_(bankSystem),
      currentState_(ScreenState::WELCOME),
      currentCustomer_(nullptr), currentAdmin_(nullptr),
      isRunning_(true), sessionStartTime_(0) {}

void ATMController::changeState(ScreenState newState) {
    if (currentState_ != ScreenState::WELCOME && currentState_ != ScreenState::LOGOUT &&
        currentState_ != ScreenState::ADMIN_LOGIN) {
        history_.push(currentState_);
    }
    currentState_ = newState;
}

void ATMController::goBack() {
    if (!history_.empty()) {
        currentState_ = history_.top();
        history_.pop();
    } else {
        currentState_ = ScreenState::MAIN_MENU;
    }
}

void ATMController::resetSession() {
    currentCustomer_ = nullptr;
    currentAdmin_ = nullptr;
    while (!history_.empty()) history_.pop();
    currentState_ = ScreenState::WELCOME;
    sessionStartTime_ = 0;
}

bool ATMController::checkTimeout() {
    if (currentCustomer_ == nullptr) return false;
    long long now = util::getEpochSeconds();
    if (now - sessionStartTime_ > bankSystem_->getAuthManager().getSessionTimeout()) {
        TerminalUI::showWarning("Session Timed Out!");
        resetSession();
        return true;
    }
    sessionStartTime_ = now; // reset timer on activity
    return false;
}

void ATMController::run() {
    TerminalUI::enableANSI();
    Animations::bootAnimation();

    while (isRunning_) {
        if (checkTimeout()) continue;

        switch (currentState_) {
            case ScreenState::WELCOME:
                handleWelcome();
                break;
            case ScreenState::INSERT_CARD:
                handleInsertCard();
                break;
            case ScreenState::PIN_ENTRY:
                handlePinEntry();
                break;
            case ScreenState::MAIN_MENU:
                handleMainMenu();
                break;
            case ScreenState::WITHDRAW:
                handleWithdraw();
                break;
            case ScreenState::DEPOSIT:
                handleDeposit();
                break;
            case ScreenState::BALANCE:
                screenManager_.renderBalance(*currentCustomer_);
                changeState(ScreenState::MAIN_MENU);
                break;
            case ScreenState::TRANSFER:
                handleTransfer();
                break;
            case ScreenState::MINI_STATEMENT:
                screenManager_.renderMiniStatement(*currentCustomer_);
                changeState(ScreenState::MAIN_MENU);
                break;
            case ScreenState::TRANSACTION_HISTORY:
                screenManager_.renderTransactionHistory(*currentCustomer_);
                changeState(ScreenState::MAIN_MENU);
                break;
            case ScreenState::CARD_SETTINGS:
                handleCardSettings();
                break;
            case ScreenState::PROFILE:
                screenManager_.renderProfile(*currentCustomer_);
                changeState(ScreenState::MAIN_MENU);
                break;
            case ScreenState::ADMIN_LOGIN:
                handleAdminLogin();
                break;
            case ScreenState::ADMIN_DASHBOARD:
                handleAdminDashboard();
                break;
            case ScreenState::APPLY_ACCOUNT:
                handleApplyAccount();
                break;
            case ScreenState::ADMIN_APPROVE_ACCOUNTS:
                handleAdminApproveAccounts();
                break;
            case ScreenState::ADMIN_USER_SEARCH:
                handleAdminUserSearch();
                break;
            case ScreenState::ADMIN_LOGS:
                handleAdminLogs();
                break;
            case ScreenState::CHECK_STATUS:
                handleCheckStatus();
                break;
            case ScreenState::LOGOUT:
                Animations::cardEjectAnimation();
                TerminalUI::showSuccess("Logged out successfully");
                bankSystem_->getLogger().log(LogEventType::LOGOUT,
                    currentCustomer_ ? currentCustomer_->getId() : "ADMIN", "Logout");
                resetSession();
                break;
            case ScreenState::EXIT:
                isRunning_ = false;
                break;
            default:
                resetSession();
                break;
        }
    }

    TerminalUI::clearScreen();
    TerminalUI::drawCentered(10, "Shutting down ATMVerse...");
    TerminalUI::sleep_ms(1000);
    TerminalUI::clearScreen();
}

void ATMController::handleWelcome() {
    screenManager_.renderWelcome();
    char ch = TerminalUI::getKeyPress();
    if (ch == 'a' || ch == 'A') {
        changeState(ScreenState::ADMIN_LOGIN);
    } else if (ch == '2' || ch == 'c' || ch == 'C') {
        changeState(ScreenState::APPLY_ACCOUNT);
    } else if (ch == '3' || ch == 's' || ch == 'S') {
        changeState(ScreenState::CHECK_STATUS);
    } else if (ch == 'q' || ch == 'Q' || ch == '0') {
        changeState(ScreenState::EXIT);
    } else {
        changeState(ScreenState::INSERT_CARD);
    }
}

void ATMController::handleInsertCard() {
    Animations::cardInsertAnimation();
    std::string cardNum = screenManager_.renderInsertCard();

    if (cardNum.empty()) {
        changeState(ScreenState::WELCOME);
        return;
    }

    Animations::cardReadingAnimation();
    currentCustomer_ = bankSystem_->findByCardNumber(cardNum);

    if (currentCustomer_) {
        if (!bankSystem_->getAuthManager().isCardUsable(*currentCustomer_)) {
            TerminalUI::showError("Card is Blocked or Inactive");
            Animations::cardEjectAnimation();
            resetSession();
        } else {
            sessionStartTime_ = util::getEpochSeconds();
            changeState(ScreenState::PIN_ENTRY);
        }
    } else {
        TerminalUI::showError("Invalid Card Number");
        Animations::cardEjectAnimation();
        resetSession();
    }
}

void ATMController::handlePinEntry() {
    int attempts = bankSystem_->getAuthManager().getRemainingAttempts(*currentCustomer_);
    std::string pin = screenManager_.renderPinEntry(attempts);

    Animations::processingDots("Verifying");

    if (bankSystem_->getAuthManager().authenticateCustomer(*currentCustomer_, pin)) {
        sessionStartTime_ = util::getEpochSeconds();
        bankSystem_->getLogger().log(LogEventType::LOGIN, currentCustomer_->getId(), "Customer Login");
        changeState(ScreenState::MAIN_MENU);
    } else {
        if (bankSystem_->getAuthManager().isCardLocked(*currentCustomer_)) {
            TerminalUI::showError("Card Blocked due to multiple failed attempts!");
            bankSystem_->getLogger().log(LogEventType::CARD_LOCK, currentCustomer_->getId(), "Card locked due to PIN failures");
            Animations::cardEjectAnimation();
            resetSession();
        } else {
            TerminalUI::showError("Invalid PIN");
        }
    }
}

void ATMController::handleMainMenu() {
    int choice = screenManager_.renderMainMenu();
    switch (choice) {
        case 1: changeState(ScreenState::WITHDRAW); break;
        case 2: changeState(ScreenState::DEPOSIT); break;
        case 3: changeState(ScreenState::BALANCE); break;
        case 4: changeState(ScreenState::TRANSFER); break;
        case 5: changeState(ScreenState::MINI_STATEMENT); break;
        case 6: changeState(ScreenState::CARD_SETTINGS); break;
        case 0: changeState(ScreenState::LOGOUT); break;
        default: break;
    }
}

void ATMController::handleWithdraw() {
    double amount = screenManager_.renderWithdraw();
    if (amount <= 0) {
        goBack();
        return;
    }

    Animations::processingDots("Processing Transaction");

    if (bankSystem_->withdraw(*currentCustomer_, amount)) {
        Animations::cashCountAnimation(std::map<int,int>()); // simplify generic count
        Animations::cashDispenseAnimation(amount);
        TerminalUI::showSuccess("Please collect your cash");

        if (TerminalUI::getConfirmation("Print Receipt?", 20, 26)) {
            auto txns = currentCustomer_->getLastNTransactions(1);
            if (!txns.empty()) {
                auto receipt = Receipt::generate(
                    bankSystem_->getBankName(), "CASH WITHDRAWAL",
                    currentCustomer_->getAccount().getAccountNumber(),
                    txns[0].getId(), amount, txns[0].getBalanceAfter(),
                    txns[0].getDateTime()
                );
                Animations::receiptPrintAnimation(receipt);
            }
        }
        changeState(ScreenState::MAIN_MENU);
    } else {
        TerminalUI::showError("Transaction Failed! Check limits/balance.");
    }
}

void ATMController::handleDeposit() {
    double amount = screenManager_.renderDeposit();
    if (amount <= 0) {
        goBack();
        return;
    }

    Animations::depositAnimation(amount);
    Animations::processingDots("Verifying Notes");

    if (bankSystem_->deposit(*currentCustomer_, amount)) {
        TerminalUI::showSuccess("Cash deposited successfully");
        if (TerminalUI::getConfirmation("Print Receipt?", 20, 26)) {
            auto txns = currentCustomer_->getLastNTransactions(1);
            if (!txns.empty()) {
                auto receipt = Receipt::generate(
                    bankSystem_->getBankName(), "CASH DEPOSIT",
                    currentCustomer_->getAccount().getAccountNumber(),
                    txns[0].getId(), amount, txns[0].getBalanceAfter(),
                    txns[0].getDateTime()
                );
                Animations::receiptPrintAnimation(receipt);
            }
        }
        changeState(ScreenState::MAIN_MENU);
    } else {
        TerminalUI::showError("Deposit Failed!");
    }
}

void ATMController::handleTransfer() {
    auto transferData = screenManager_.renderTransfer();
    std::string acc = transferData.first;
    double amt = transferData.second;
    if (acc.empty() || amt <= 0) {
        goBack();
        return;
    }

    Customer* receiver = bankSystem_->findByAccountNumber(acc);
    if (!receiver) {
        TerminalUI::showError("Invalid Receiver Account!");
        return;
    }

    if (TerminalUI::getConfirmation("Transfer " + util::formatCurrency(amt) + " to " + receiver->getName() + "?", 10, 22)) {
        Animations::transferAnimation();
        if (bankSystem_->transfer(*currentCustomer_, acc, amt)) {
            TerminalUI::showSuccess("Transfer Successful!");
            changeState(ScreenState::MAIN_MENU);
        } else {
            TerminalUI::showError("Transfer Failed! Check balance.");
        }
    } else {
        goBack();
    }
}

void ATMController::handleCardSettings() {
    int choice = screenManager_.renderCardSettings(*currentCustomer_);
    switch (choice) {
        case 1:
            changeState(ScreenState::PROFILE);
            break;
        case 2: {
            TerminalUI::drawBox(10, 20, 40, 5, theme::DARK_GRAY, true);
            std::string oldP = TerminalUI::getMaskedInput("Old PIN: ", 4, 12, 21);
            std::string newP = TerminalUI::getMaskedInput("New PIN: ", 4, 12, 22);
            if (bankSystem_->changePin(*currentCustomer_, oldP, newP)) {
                TerminalUI::showSuccess("PIN Changed Successfully!");
            } else {
                TerminalUI::showError("PIN Change Failed!");
            }
            break;
        }
        case 3:
            TerminalUI::showInfo("Feature restricted. Visit branch.");
            break;
        case 0:
            goBack();
            break;
    }
}

void ATMController::handleAdminLogin() {
    auto loginData = screenManager_.renderAdminLogin();
    std::string id = loginData.first;
    std::string pwd = loginData.second;

    Animations::processingDots("Authenticating");

    bool auth = false;
    for (auto& admin : bankSystem_->getAdminsRef()) {
        if ((admin.getId() == id || admin.getAdminId() == id) && bankSystem_->getAuthManager().authenticateAdmin(admin, pwd)) {
            currentAdmin_ = &admin;
            auth = true;
            break;
        }
    }

    if (auth) {
        sessionStartTime_ = util::getEpochSeconds();
        bankSystem_->getLogger().log(LogEventType::ADMIN_LOGIN, currentAdmin_->getId(), "Admin Login");
        TerminalUI::showSuccess("Welcome, " + currentAdmin_->getName());
        changeState(ScreenState::ADMIN_DASHBOARD);
    } else {
        TerminalUI::showError("Invalid Admin Credentials");
        resetSession();
    }
}

void ATMController::handleAdminDashboard() {
    int choice = screenManager_.renderAdminDashboard();
    switch (choice) {
        case 1:
            screenManager_.renderAdminReports();
            break;
        case 2:
            screenManager_.renderAdminCash();
            break;
        case 3:
            changeState(ScreenState::ADMIN_USER_SEARCH);
            break;
        case 4:
            changeState(ScreenState::ADMIN_LOGS);
            break;
        case 5:
            changeState(ScreenState::ADMIN_APPROVE_ACCOUNTS);
            break;
        case 0:
            changeState(ScreenState::LOGOUT);
            break;
    }
}

void ATMController::handleApplyAccount() {
    screenManager_.renderApplyAccount();
    changeState(ScreenState::WELCOME);
}

void ATMController::handleAdminApproveAccounts() {
    screenManager_.renderAdminApproveAccounts();
    changeState(ScreenState::ADMIN_DASHBOARD);
}

void ATMController::handleAdminUserSearch() {
    screenManager_.renderAdminUserSearch();
    changeState(ScreenState::ADMIN_DASHBOARD);
}

void ATMController::handleAdminLogs() {
    screenManager_.renderAdminLogs();
    changeState(ScreenState::ADMIN_DASHBOARD);
}

void ATMController::handleCheckStatus() {
    screenManager_.renderCheckStatus();
    changeState(ScreenState::WELCOME);
}

} // namespace atmverse
