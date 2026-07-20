#pragma once
#include <string>
#include <vector>
#include "atmverse/models/Transaction.h"

namespace atmverse {

class Receipt {
public:
    static std::vector<std::string> generate(
        const std::string& bankName,
        const std::string& transactionType,
        const std::string& accountNumber,
        const std::string& transactionId,
        double amount,
        double balanceAfter,
        const std::string& dateTime,
        const std::string& additionalInfo = ""
    );

    static std::vector<std::string> generateTransferReceipt(
        const std::string& bankName,
        const std::string& fromAccount,
        const std::string& toAccount,
        const std::string& receiverName,
        const std::string& transactionId,
        double amount,
        double balanceAfter,
        const std::string& dateTime
    );
};

} // namespace atmverse
