#pragma once
#include <string>
#include <vector>
#include "json.hpp"

namespace atmverse {

using json = nlohmann::json;

enum class TransactionType { WITHDRAW, DEPOSIT, TRANSFER_OUT, TRANSFER_IN, REFUND };
enum class TransactionStatus { SUCCESS, FAILED, PENDING };

class Transaction {
private:
    std::string transactionId_;
    TransactionType type_;
    TransactionStatus status_;
    double amount_;
    double balanceBefore_;
    double balanceAfter_;
    std::string fromAccount_;
    std::string toAccount_;
    std::string description_;
    std::string dateTime_;
    std::string date_;
    std::string time_;

public:
    Transaction() : type_(TransactionType::WITHDRAW),
                    status_(TransactionStatus::PENDING),
                    amount_(0), balanceBefore_(0), balanceAfter_(0) {}

    Transaction(const std::string& id, TransactionType type, double amount,
                double balBefore, double balAfter,
                const std::string& from, const std::string& to,
                const std::string& desc);

    // Getters
    const std::string& getId() const { return transactionId_; }
    TransactionType getType() const { return type_; }
    TransactionStatus getStatus() const { return status_; }
    double getAmount() const { return amount_; }
    double getBalanceBefore() const { return balanceBefore_; }
    double getBalanceAfter() const { return balanceAfter_; }
    const std::string& getFromAccount() const { return fromAccount_; }
    const std::string& getToAccount() const { return toAccount_; }
    const std::string& getDescription() const { return description_; }
    const std::string& getDateTime() const { return dateTime_; }
    const std::string& getDate() const { return date_; }
    const std::string& getTime() const { return time_; }

    std::string getTypeString() const;
    std::string getStatusString() const;

    // Setters
    void setStatus(TransactionStatus s) { status_ = s; }

    // Comparators for sorting
    static bool compareByDateDesc(const Transaction& a, const Transaction& b);
    static bool compareByDateAsc(const Transaction& a, const Transaction& b);
    static bool compareByAmountDesc(const Transaction& a, const Transaction& b);

    // Serialization
    json toJson() const;
    void fromJson(const json& j);

    static TransactionType typeFromString(const std::string& s);
    static TransactionStatus statusFromString(const std::string& s);
};

} // namespace atmverse
