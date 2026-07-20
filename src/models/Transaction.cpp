#include "atmverse/models/Transaction.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

Transaction::Transaction(const std::string& id, TransactionType type, double amount,
                         double balBefore, double balAfter,
                         const std::string& from, const std::string& to,
                         const std::string& desc)
    : transactionId_(id), type_(type), status_(TransactionStatus::SUCCESS),
      amount_(amount), balanceBefore_(balBefore), balanceAfter_(balAfter),
      fromAccount_(from), toAccount_(to), description_(desc),
      dateTime_(util::getCurrentDateTime()),
      date_(util::getCurrentDate()),
      time_(util::getCurrentTime()) {}

std::string Transaction::getTypeString() const {
    switch (type_) {
        case TransactionType::WITHDRAW: return "WITHDRAW";
        case TransactionType::DEPOSIT: return "DEPOSIT";
        case TransactionType::TRANSFER_OUT: return "TRANSFER_OUT";
        case TransactionType::TRANSFER_IN: return "TRANSFER_IN";
        case TransactionType::REFUND: return "REFUND";
        default: return "UNKNOWN";
    }
}

std::string Transaction::getStatusString() const {
    switch (status_) {
        case TransactionStatus::SUCCESS: return "SUCCESS";
        case TransactionStatus::FAILED: return "FAILED";
        case TransactionStatus::PENDING: return "PENDING";
        default: return "UNKNOWN";
    }
}

bool Transaction::compareByDateDesc(const Transaction& a, const Transaction& b) {
    return a.dateTime_ > b.dateTime_;
}

bool Transaction::compareByDateAsc(const Transaction& a, const Transaction& b) {
    return a.dateTime_ < b.dateTime_;
}

bool Transaction::compareByAmountDesc(const Transaction& a, const Transaction& b) {
    return a.amount_ > b.amount_;
}

TransactionType Transaction::typeFromString(const std::string& s) {
    if (s == "WITHDRAW") return TransactionType::WITHDRAW;
    if (s == "DEPOSIT") return TransactionType::DEPOSIT;
    if (s == "TRANSFER_OUT") return TransactionType::TRANSFER_OUT;
    if (s == "TRANSFER_IN") return TransactionType::TRANSFER_IN;
    if (s == "REFUND") return TransactionType::REFUND;
    return TransactionType::WITHDRAW;
}

TransactionStatus Transaction::statusFromString(const std::string& s) {
    if (s == "FAILED") return TransactionStatus::FAILED;
    if (s == "PENDING") return TransactionStatus::PENDING;
    return TransactionStatus::SUCCESS;
}

json Transaction::toJson() const {
    return {
        {"transactionId", transactionId_},
        {"type", getTypeString()},
        {"status", getStatusString()},
        {"amount", amount_},
        {"balanceBefore", balanceBefore_},
        {"balanceAfter", balanceAfter_},
        {"fromAccount", fromAccount_},
        {"toAccount", toAccount_},
        {"description", description_},
        {"dateTime", dateTime_},
        {"date", date_},
        {"time", time_}
    };
}

void Transaction::fromJson(const json& j) {
    transactionId_ = j.value("transactionId", "");
    type_ = typeFromString(j.value("type", "WITHDRAW"));
    status_ = statusFromString(j.value("status", "SUCCESS"));
    amount_ = j.value("amount", 0.0);
    balanceBefore_ = j.value("balanceBefore", 0.0);
    balanceAfter_ = j.value("balanceAfter", 0.0);
    fromAccount_ = j.value("fromAccount", "");
    toAccount_ = j.value("toAccount", "");
    description_ = j.value("description", "");
    dateTime_ = j.value("dateTime", "");
    date_ = j.value("date", "");
    time_ = j.value("time", "");
}

} // namespace atmverse
