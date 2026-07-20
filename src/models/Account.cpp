#include "atmverse/models/Account.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

Account::Account(const std::string& accNum, double balance, AccountType type)
    : accountNumber_(accNum), balance_(balance), availableBalance_(balance),
      type_(type), status_(AccountStatus::ACTIVE),
      createdAt_(util::getCurrentDateTime()) {}

std::string Account::getTypeString() const {
    switch (type_) {
        case AccountType::SAVINGS: return "Savings";
        case AccountType::CURRENT: return "Current";
        default: return "Unknown";
    }
}

std::string Account::getStatusString() const {
    switch (status_) {
        case AccountStatus::ACTIVE: return "Active";
        case AccountStatus::FROZEN: return "Frozen";
        case AccountStatus::CLOSED: return "Closed";
        default: return "Unknown";
    }
}

bool Account::credit(double amount) {
    if (amount <= 0 || status_ != AccountStatus::ACTIVE) return false;
    balance_ += amount;
    availableBalance_ += amount;
    return true;
}

bool Account::debit(double amount) {
    if (amount <= 0 || status_ != AccountStatus::ACTIVE) return false;
    if (availableBalance_ < amount) return false;
    balance_ -= amount;
    availableBalance_ -= amount;
    return true;
}

void Account::freeze() { status_ = AccountStatus::FROZEN; }
void Account::unfreeze() { status_ = AccountStatus::ACTIVE; }
void Account::close() { status_ = AccountStatus::CLOSED; }

AccountType Account::typeFromString(const std::string& s) {
    if (s == "Current") return AccountType::CURRENT;
    return AccountType::SAVINGS;
}

AccountStatus Account::statusFromString(const std::string& s) {
    if (s == "Frozen") return AccountStatus::FROZEN;
    if (s == "Closed") return AccountStatus::CLOSED;
    return AccountStatus::ACTIVE;
}

json Account::toJson() const {
    return {
        {"accountNumber", accountNumber_},
        {"balance", balance_},
        {"availableBalance", availableBalance_},
        {"type", getTypeString()},
        {"status", getStatusString()},
        {"createdAt", createdAt_}
    };
}

void Account::fromJson(const json& j) {
    accountNumber_ = j.value("accountNumber", "");
    balance_ = j.value("balance", 0.0);
    availableBalance_ = j.value("availableBalance", 0.0);
    type_ = typeFromString(j.value("type", "Savings"));
    status_ = statusFromString(j.value("status", "Active"));
    createdAt_ = j.value("createdAt", "");
}

} // namespace atmverse
