#pragma once
#include "Person.h"
#include "Account.h"
#include "Card.h"
#include "Transaction.h"
#include <vector>

namespace atmverse {

class Customer : public Person {
private:
    Account account_;
    Card card_;
    std::vector<Transaction> transactions_;
    std::string accountType_; // "Savings" or "Current"
    int failedPinAttempts_;

public:
    Customer() : failedPinAttempts_(0) {}
    Customer(const std::string& id, const std::string& name,
             const std::string& phone, const std::string& email,
             const Account& account, const Card& card);

    // Getters
    const Account& getAccount() const { return account_; }
    Account& getAccountRef() { return account_; }
    const Card& getCard() const { return card_; }
    Card& getCardRef() { return card_; }
    const std::vector<Transaction>& getTransactions() const { return transactions_; }
    int getFailedPinAttempts() const { return failedPinAttempts_; }

    // Operations
    void addTransaction(const Transaction& txn);
    std::vector<Transaction> getLastNTransactions(int n) const;
    std::vector<Transaction> searchTransactions(const std::string& query) const;
    void incrementFailedAttempts() { ++failedPinAttempts_; }
    void resetFailedAttempts() { failedPinAttempts_ = 0; }

    // Polymorphism
    std::string getRole() const override { return "Customer"; }
    json toJson() const override;
    void fromJson(const json& j) override;
};

} // namespace atmverse
