#pragma once
#include <string>
#include "json.hpp"

namespace atmverse {

using json = nlohmann::json;

enum class AccountType { SAVINGS, CURRENT };
enum class AccountStatus { ACTIVE, FROZEN, CLOSED };

class Account {
private:
    std::string accountNumber_;
    double balance_;
    double availableBalance_;
    AccountType type_;
    AccountStatus status_;
    std::string createdAt_;

public:
    Account() : balance_(0), availableBalance_(0),
                type_(AccountType::SAVINGS), status_(AccountStatus::ACTIVE) {}

    Account(const std::string& accNum, double balance, AccountType type);

    // Getters
    const std::string& getAccountNumber() const { return accountNumber_; }
    double getBalance() const { return balance_; }
    double getAvailableBalance() const { return availableBalance_; }
    AccountType getType() const { return type_; }
    AccountStatus getStatus() const { return status_; }
    const std::string& getCreatedAt() const { return createdAt_; }

    std::string getTypeString() const;
    std::string getStatusString() const;

    // Operations
    bool credit(double amount);
    bool debit(double amount);
    void freeze();
    void unfreeze();
    void close();
    bool isActive() const { return status_ == AccountStatus::ACTIVE; }

    // Serialization
    json toJson() const;
    void fromJson(const json& j);

    // Static helpers
    static AccountType typeFromString(const std::string& s);
    static AccountStatus statusFromString(const std::string& s);
};

} // namespace atmverse
