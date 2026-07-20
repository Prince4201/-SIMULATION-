#include "atmverse/core/Receipt.h"
#include "atmverse/utils/Utility.h"
#include <sstream>
#include <iomanip>

namespace atmverse {

std::vector<std::string> Receipt::generate(
    const std::string& bankName,
    const std::string& transactionType,
    const std::string& accountNumber,
    const std::string& transactionId,
    double amount,
    double balanceAfter,
    const std::string& dateTime,
    const std::string& additionalInfo)
{
    std::vector<std::string> lines;
    int w = 40;

    lines.push_back("\033[38;5;250m  " + std::string(w, '\xe2\x94\x80') + "\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("\033[1;36m" + bankName + "\033[0m", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("TRANSACTION RECEIPT", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x9c" + std::string(w - 2, '\xe2\x94\x80') + "\xe2\x94\xa4\033[0m");

    auto addRow = [&](const std::string& label, const std::string& value) {
        std::string row = "  " + util::padRight(label, 16) + value;
        lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m \033[38;5;245m" +
                        util::padRight(label, 15) + "\033[1;97m" +
                        util::padRight(value, w - 19) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    };

    addRow("Date/Time:", dateTime);
    addRow("Type:", transactionType);
    addRow("Account:", "XXXX" + accountNumber.substr(accountNumber.length() - 4));
    addRow("Txn ID:", transactionId);
    addRow("Amount:", util::formatCurrency(amount));
    addRow("Balance:", util::formatCurrency(balanceAfter));

    if (!additionalInfo.empty()) {
        addRow("Info:", additionalInfo);
    }

    lines.push_back("\033[38;5;250m  \xe2\x94\x9c" + std::string(w - 2, '\xe2\x94\x80') + "\xe2\x94\xa4\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("\033[38;5;245mThank you for banking with us\033[0m", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  " + std::string(w, '\xe2\x94\x80') + "\033[0m");

    return lines;
}

std::vector<std::string> Receipt::generateTransferReceipt(
    const std::string& bankName,
    const std::string& fromAccount,
    const std::string& toAccount,
    const std::string& receiverName,
    const std::string& transactionId,
    double amount,
    double balanceAfter,
    const std::string& dateTime)
{
    std::vector<std::string> lines;
    int w = 40;

    lines.push_back("\033[38;5;250m  " + std::string(w, '\xe2\x94\x80') + "\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("\033[1;36m" + bankName + "\033[0m", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("TRANSFER RECEIPT", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x9c" + std::string(w - 2, '\xe2\x94\x80') + "\xe2\x94\xa4\033[0m");

    auto addRow = [&](const std::string& label, const std::string& value) {
        lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m \033[38;5;245m" +
                        util::padRight(label, 15) + "\033[1;97m" +
                        util::padRight(value, w - 19) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    };

    addRow("Date/Time:", dateTime);
    addRow("Txn ID:", transactionId);
    addRow("From:", "XXXX" + fromAccount.substr(fromAccount.length() - 4));
    addRow("To:", "XXXX" + toAccount.substr(toAccount.length() - 4));
    addRow("Receiver:", receiverName);
    addRow("Amount:", util::formatCurrency(amount));
    addRow("Balance:", util::formatCurrency(balanceAfter));

    lines.push_back("\033[38;5;250m  \xe2\x94\x9c" + std::string(w - 2, '\xe2\x94\x80') + "\xe2\x94\xa4\033[0m");
    lines.push_back("\033[38;5;250m  \xe2\x94\x82\033[0m" + util::centerString("\033[38;5;245mThank you for banking with us\033[0m", w - 2) + "\033[38;5;250m\xe2\x94\x82\033[0m");
    lines.push_back("\033[38;5;250m  " + std::string(w, '\xe2\x94\x80') + "\033[0m");

    return lines;
}

} // namespace atmverse
