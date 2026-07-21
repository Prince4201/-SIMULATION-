#pragma once
#include <string>
#include <vector>
#include "atmverse/models/Customer.h"
#include "atmverse/models/Admin.h"
#include "atmverse/core/BankSystem.h"

namespace atmverse {

enum class ScreenState {
    WELCOME,
    INSERT_CARD,
    PIN_ENTRY,
    MAIN_MENU,
    WITHDRAW,
    DEPOSIT,
    BALANCE,
    TRANSFER,
    MINI_STATEMENT,
    TRANSACTION_HISTORY,
    CARD_SETTINGS,
    PROFILE,
    ADMIN_LOGIN,
    ADMIN_DASHBOARD,
    ADMIN_REPORTS,
    ADMIN_CASH,
    ADMIN_APPROVE_ACCOUNTS,
    ADMIN_USER_SEARCH,
    ADMIN_LOGS,
    APPLY_ACCOUNT,
    CHECK_STATUS,
    LOGOUT,
    EXIT
};

class ScreenManager {
private:
    BankSystem* bankSystem_;

    void drawBaseScreen(const std::string& title, const std::string& status = "");

public:
    explicit ScreenManager(BankSystem* bankSystem) : bankSystem_(bankSystem) {}

    // Renders the specific screen. Returns the next state or input if applicable.
    void renderWelcome();
    std::string renderInsertCard(); // Returns card number
    std::string renderPinEntry(int attemptsRemaining); // Returns PIN
    int renderMainMenu();
    double renderWithdraw();
    double renderDeposit();
    void renderBalance(const Customer& customer);
    std::pair<std::string, double> renderTransfer();
    void renderMiniStatement(const Customer& customer);
    void renderTransactionHistory(const Customer& customer);
    int renderCardSettings(const Customer& customer);
    void renderProfile(const Customer& customer);

    // Admin Screens
    std::pair<std::string, std::string> renderAdminLogin();
    int renderAdminDashboard();
    void renderAdminReports();
    void renderAdminCash();
    void renderAdminApproveAccounts();
    void renderAdminUserSearch();
    void renderAdminLogs();

    // Account Creation
    void renderApplyAccount();
    void renderCheckStatus();
};

} // namespace atmverse
