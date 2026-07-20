#include "atmverse/models/Customer.h"
#include "atmverse/utils/Utility.h"
#include <algorithm>

namespace atmverse {

Customer::Customer(const std::string& id, const std::string& name,
                   const std::string& phone, const std::string& email,
                   const Account& account, const Card& card)
    : Person(id, name, phone, email), account_(account), card_(card),
      failedPinAttempts_(0) {
    accountType_ = account.getTypeString();
}

void Customer::addTransaction(const Transaction& txn) {
    transactions_.push_back(txn);
}

std::vector<Transaction> Customer::getLastNTransactions(int n) const {
    std::vector<Transaction> result;
    int start = std::max(0, static_cast<int>(transactions_.size()) - n);
    for (int i = static_cast<int>(transactions_.size()) - 1; i >= start; --i) {
        result.push_back(transactions_[i]);
    }
    return result;
}

std::vector<Transaction> Customer::searchTransactions(const std::string& query) const {
    std::vector<Transaction> results;
    std::string lowerQuery = util::toLower(query);
    for (const auto& txn : transactions_) {
        if (util::toLower(txn.getTypeString()).find(lowerQuery) != std::string::npos ||
            util::toLower(txn.getDescription()).find(lowerQuery) != std::string::npos ||
            txn.getDate().find(query) != std::string::npos ||
            std::to_string(static_cast<int>(txn.getAmount())).find(query) != std::string::npos) {
            results.push_back(txn);
        }
    }
    return results;
}

json Customer::toJson() const {
    json j = Person::toJson();
    j["account"] = account_.toJson();
    j["card"] = card_.toJson();
    j["failedPinAttempts"] = failedPinAttempts_;
    j["accountType"] = accountType_;

    json txns = json::array();
    for (const auto& txn : transactions_) {
        txns.push_back(txn.toJson());
    }
    j["transactions"] = txns;
    return j;
}

void Customer::fromJson(const json& j) {
    Person::fromJson(j);
    if (j.contains("account")) account_.fromJson(j["account"]);
    if (j.contains("card")) card_.fromJson(j["card"]);
    failedPinAttempts_ = j.value("failedPinAttempts", 0);
    accountType_ = j.value("accountType", "Savings");

    transactions_.clear();
    if (j.contains("transactions") && j["transactions"].is_array()) {
        for (const auto& t : j["transactions"]) {
            Transaction txn;
            txn.fromJson(t);
            transactions_.push_back(txn);
        }
    }
}

} // namespace atmverse
