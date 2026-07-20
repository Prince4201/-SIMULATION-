#include "atmverse/core/ReportGenerator.h"
#include "atmverse/utils/Utility.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <queue>

namespace atmverse {

ReportData ReportGenerator::generateDailyReport(
    const std::vector<Customer>& customers, double atmCash) const
{
    ReportData report;
    report.totalUsers = static_cast<int>(customers.size());
    report.todayTransactions = 0;
    report.totalWithdrawals = 0;
    report.totalDeposits = 0;
    report.totalTransfers = 0;
    report.highestBalance = 0;
    report.mostActiveCount = 0;
    report.atmCashRemaining = atmCash;
    report.generatedAt = util::getCurrentDateTime();

    std::string today = util::getCurrentDate();

    // Use priority_queue to find most active user
    using UserActivity = std::pair<int, std::string>; // (count, name)
    std::priority_queue<UserActivity> activityQueue;

    for (const auto& customer : customers) {
        // Track highest balance
        if (customer.getAccount().getBalance() > report.highestBalance) {
            report.highestBalance = customer.getAccount().getBalance();
            report.highestBalanceUser = customer.getName();
        }

        int todayCount = 0;
        for (const auto& txn : customer.getTransactions()) {
            if (txn.getDate() == today) {
                report.todayTransactions++;
                todayCount++;
            }
            switch (txn.getType()) {
                case TransactionType::WITHDRAW:
                    report.totalWithdrawals += txn.getAmount(); break;
                case TransactionType::DEPOSIT:
                    report.totalDeposits += txn.getAmount(); break;
                case TransactionType::TRANSFER_OUT:
                    report.totalTransfers += txn.getAmount(); break;
                default: break;
            }
        }
        activityQueue.push({todayCount, customer.getName()});
    }

    if (!activityQueue.empty()) {
        auto top = activityQueue.top();
        report.mostActiveUser = top.second;
        report.mostActiveCount = top.first;
    }

    return report;
}

void ReportGenerator::saveReport(const ReportData& report) const {
    json j = {
        {"totalUsers", report.totalUsers},
        {"todayTransactions", report.todayTransactions},
        {"totalWithdrawals", report.totalWithdrawals},
        {"totalDeposits", report.totalDeposits},
        {"totalTransfers", report.totalTransfers},
        {"mostActiveUser", report.mostActiveUser},
        {"mostActiveCount", report.mostActiveCount},
        {"highestBalanceUser", report.highestBalanceUser},
        {"highestBalance", report.highestBalance},
        {"atmCashRemaining", report.atmCashRemaining},
        {"generatedAt", report.generatedAt}
    };
    jsonManager_->appendReport(j);
}

std::vector<std::string> ReportGenerator::renderReport(const ReportData& report) const {
    std::vector<std::string> lines;
    lines.push_back("\033[1;36m  \xe2\x94\x8c" + std::string(44, '\xe2\x94\x80') + "\xe2\x94\x90\033[0m");
    lines.push_back("\033[1;36m  \xe2\x94\x82\033[0m" + util::centerString("\033[1;33m DAILY REPORT \033[0m", 44) + "\033[1;36m\xe2\x94\x82\033[0m");
    lines.push_back("\033[1;36m  \xe2\x94\x82\033[0m" + util::centerString(report.generatedAt, 44) + "\033[1;36m\xe2\x94\x82\033[0m");
    lines.push_back("\033[1;36m  \xe2\x94\x9c" + std::string(44, '\xe2\x94\x80') + "\xe2\x94\xa4\033[0m");

    auto row = [&](const std::string& label, const std::string& value) {
        lines.push_back("\033[1;36m  \xe2\x94\x82\033[0m \033[38;5;245m" +
                        util::padRight(label, 26) + "\033[1;97m" +
                        util::padRight(value, 17) + "\033[1;36m\xe2\x94\x82\033[0m");
    };

    row("Total Users", std::to_string(report.totalUsers));
    row("Today's Transactions", std::to_string(report.todayTransactions));
    row("Total Withdrawals", util::formatCurrency(report.totalWithdrawals));
    row("Total Deposits", util::formatCurrency(report.totalDeposits));
    row("Total Transfers", util::formatCurrency(report.totalTransfers));
    row("Most Active User", util::truncate(report.mostActiveUser, 15));
    row("Highest Balance", util::formatCurrency(report.highestBalance));
    row("ATM Cash Left", util::formatCurrency(report.atmCashRemaining));

    lines.push_back("\033[1;36m  \xe2\x94\x94" + std::string(44, '\xe2\x94\x80') + "\xe2\x94\x98\033[0m");
    return lines;
}

std::vector<std::string> ReportGenerator::renderUserTable(
    const std::vector<Customer>& customers) const
{
    std::vector<std::string> lines;
    lines.push_back("\033[1;36m  " + util::padRight("Name", 18) +
                    util::padRight("Account", 14) +
                    util::padRight("Balance", 14) +
                    util::padRight("Status", 10) + "\033[0m");
    lines.push_back("  \033[38;5;240m" + std::string(56, '\xe2\x94\x80') + "\033[0m");

    for (const auto& c : customers) {
        lines.push_back("  \033[97m" + util::padRight(util::truncate(c.getName(), 16), 18) +
                        "\033[38;5;245m" + util::padRight(c.getAccount().getAccountNumber(), 14) +
                        "\033[1;32m" + util::padRight(util::formatCurrency(c.getAccount().getBalance()), 14) +
                        (c.getAccount().isActive() ? "\033[32mActive" : "\033[31mInactive") +
                        "\033[0m");
    }
    return lines;
}

} // namespace atmverse
