#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "atmverse/models/Customer.h"
#include "atmverse/models/Admin.h"
#include "atmverse/persistence/JSONManager.h"
#include "atmverse/core/CashDispenser.h"
#include "atmverse/core/Logger.h"
#include "atmverse/core/AuthManager.h"
#include "atmverse/core/ReportGenerator.h"

namespace atmverse {

class BankSystem {
private:
    JSONManager jsonManager_;
    CashDispenser cashDispenser_;
    Logger logger_;
    AuthManager authManager_;
    ReportGenerator reportGenerator_;

    // In-memory data stores
    std::vector<Customer> customers_;
    std::vector<Admin> admins_;
    std::unordered_map<std::string, int> cardIndex_;    // cardNum -> index
    std::unordered_map<std::string, int> accountIndex_;  // accNum -> index
    std::set<std::string> usedCardNumbers_;
    std::set<std::string> usedAccountNumbers_;

    json settings_;

    void buildIndices();

public:
    BankSystem();

    // Initialization
    void initialize();
    void loadAll();
    void saveAll();

    // Customer lookup (O(1))
    Customer* findByCardNumber(const std::string& cardNum);
    Customer* findByAccountNumber(const std::string& accNum);
    Customer* findByName(const std::string& name);
    std::vector<Customer*> searchCustomers(const std::string& query);

    // Account management
    Customer createCustomer(const std::string& name, const std::string& phone,
                           const std::string& email, const std::string& pin,
                           AccountType type = AccountType::SAVINGS,
                           double initialBalance = 0.0);
    bool deleteCustomer(const std::string& accountNumber);

    // Transactions
    bool withdraw(Customer& customer, double amount);
    bool deposit(Customer& customer, double amount);
    bool transfer(Customer& sender, const std::string& receiverAccount, double amount);

    // PIN management
    bool changePin(Customer& customer, const std::string& oldPin,
                   const std::string& newPin);

    // Card operations
    void freezeCard(Customer& customer);
    void unfreezeCard(Customer& customer);
    void lockCard(Customer& customer);
    void unlockCard(Customer& customer);
    bool setDailyLimit(Customer& customer, double limit);

    // Getters
    JSONManager& getJsonManager() { return jsonManager_; }
    CashDispenser& getCashDispenser() { return cashDispenser_; }
    Logger& getLogger() { return logger_; }
    AuthManager& getAuthManager() { return authManager_; }
    ReportGenerator& getReportGenerator() { return reportGenerator_; }
    const std::vector<Customer>& getCustomers() const { return customers_; }
    std::vector<Customer>& getCustomersRef() { return customers_; }
    const std::vector<Admin>& getAdmins() const { return admins_; }
    std::vector<Admin>& getAdminsRef() { return admins_; }
    const json& getSettings() const { return settings_; }
    std::string getBankName() const;
    int getTotalCustomers() const { return static_cast<int>(customers_.size()); }
};

} // namespace atmverse
