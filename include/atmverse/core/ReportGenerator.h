#pragma once
#include <string>
#include <vector>
#include <map>
#include "json.hpp"
#include "atmverse/models/Customer.h"
#include "atmverse/persistence/JSONManager.h"

namespace atmverse {

using json = nlohmann::json;

struct ReportData {
    int totalUsers;
    int todayTransactions;
    double totalWithdrawals;
    double totalDeposits;
    double totalTransfers;
    std::string mostActiveUser;
    int mostActiveCount;
    std::string highestBalanceUser;
    double highestBalance;
    double atmCashRemaining;
    std::string generatedAt;
};

class ReportGenerator {
private:
    JSONManager* jsonManager_;

public:
    explicit ReportGenerator(JSONManager* jm) : jsonManager_(jm) {}

    ReportData generateDailyReport(const std::vector<Customer>& customers,
                                    double atmCash) const;
    void saveReport(const ReportData& report) const;
    std::vector<std::string> renderReport(const ReportData& report) const;
    std::vector<std::string> renderUserTable(const std::vector<Customer>& customers) const;
};

} // namespace atmverse
