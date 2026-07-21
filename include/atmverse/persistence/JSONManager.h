#pragma once
#include <string>
#include <fstream>

#include "json.hpp"
#include "atmverse/models/Customer.h"
#include "atmverse/models/Admin.h"
#include "atmverse/models/AccountRequest.h"

namespace atmverse {

using json = nlohmann::json;

class JSONManager {
private:
    std::string dataDir_;


    // Internal I/O
    json readFile(const std::string& filename) const;
    bool writeFile(const std::string& filename, const json& data) const;
    std::string getPath(const std::string& filename) const;

public:
    explicit JSONManager(const std::string& dataDir = "data");

    // Initialize data directory and seed files
    void initialize();
    bool isInitialized() const;

    // Customer operations
    std::vector<Customer> loadCustomers() const;
    bool saveCustomers(const std::vector<Customer>& customers) const;

    // Admin operations
    std::vector<Admin> loadAdmins() const;
    bool saveAdmins(const std::vector<Admin>& admins) const;

    // Account Requests
    std::vector<AccountRequest> loadAccountRequests() const;
    bool saveAccountRequests(const std::vector<AccountRequest>& requests) const;

    // ATM Cash
    json loadATMCash() const;
    bool saveATMCash(const json& cash) const;

    // Logs
    json loadLogs() const;
    bool appendLog(const json& logEntry) const;

    // Settings
    json loadSettings() const;
    bool saveSettings(const json& settings) const;

    // Reports
    json loadReports() const;
    bool appendReport(const json& report) const;

    // Backup/Restore
    bool backupAll(const std::string& backupDir) const;
    bool restoreAll(const std::string& backupDir) const;

    // Seed data
    void seedDefaultData();
};

} // namespace atmverse
