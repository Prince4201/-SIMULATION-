#pragma once
#include "atmverse/core/BankSystem.h"
#include "atmverse/ui/ScreenManager.h"
#include <stack>

namespace atmverse {

class ATMController {
private:
    BankSystem* bankSystem_;
    ScreenManager screenManager_;
    ScreenState currentState_;
    std::stack<ScreenState> history_;

    Customer* currentCustomer_;
    Admin* currentAdmin_;

    bool isRunning_;
    long long sessionStartTime_;

    // Helpers
    void changeState(ScreenState newState);
    void goBack();
    void resetSession();
    bool checkTimeout();

    // State Handlers
    void handleWelcome();
    void handleInsertCard();
    void handlePinEntry();
    void handleMainMenu();
    void handleWithdraw();
    void handleDeposit();
    void handleTransfer();
    void handleCardSettings();
    // Admin handlers
    void handleAdminLogin();
    void handleAdminDashboard();
    void handleAdminApproveAccounts();
    void handleAdminUserSearch();
    void handleAdminLogs();
    void handleApplyAccount();
    void handleCheckStatus();

public:
    explicit ATMController(BankSystem* bankSystem);
    void run();
};

} // namespace atmverse
