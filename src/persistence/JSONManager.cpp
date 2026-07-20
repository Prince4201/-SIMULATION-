#include "atmverse/persistence/JSONManager.h"
#include "atmverse/utils/Utility.h"
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(a) _mkdir(a)
#else
#define MKDIR(a) mkdir(a, 0777)
#endif

inline bool dirExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFDIR) != 0;
}

inline bool fileExists(const std::string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0);
}

namespace atmverse {

JSONManager::JSONManager(const std::string& dataDir) : dataDir_(dataDir) {}

std::string JSONManager::getPath(const std::string& filename) const {
    return dataDir_ + "/" + filename;
}

json JSONManager::readFile(const std::string& filename) const {
    std::string path = getPath(filename);
    std::ifstream file(path);
    if (!file.is_open()) {
        return json::array();
    }
    try {
        json j;
        file >> j;
        return j;
    } catch (...) {
        return json::array();
    }
}

bool JSONManager::writeFile(const std::string& filename, const json& data) const {
    
    std::string path = getPath(filename);
    std::ofstream file(path);
    if (!file.is_open()) return false;
    try {
        file << data.dump(4);
        return true;
    } catch (...) {
        return false;
    }
}

void JSONManager::initialize() {
    if (!dirExists(dataDir_)) {
        MKDIR(dataDir_.c_str());
    }

    // Create empty files if they don't exist
    std::vector<std::string> files = {
        "accounts.json", "admins.json", "atm_cash.json",
        "logs.json", "settings.json", "daily_reports.json"
    };
    for (const auto& f : files) {
        std::string path = getPath(f);
        if (!fileExists(path)) {
            std::ofstream out(path);
            if (f == "atm_cash.json") {
                json cash = {
                    {"denominations", {
                        {"100", 500}, {"200", 300}, {"500", 200}, {"2000", 100}
                    }},
                    {"totalCash", 350000},
                    {"lastRefill", util::getCurrentDateTime()}
                };
                out << cash.dump(4);
            } else if (f == "settings.json") {
                json settings = {
                    {"bankName", "ATMVerse Bank"},
                    {"sessionTimeout", 120},
                    {"maxPinAttempts", 3},
                    {"defaultDailyLimit", 50000},
                    {"minWithdraw", 100},
                    {"maxWithdraw", 25000},
                    {"version", "1.0.0"}
                };
                out << settings.dump(4);
            } else {
                out << "[]";
            }
        }
    }
}

bool JSONManager::isInitialized() const {
    return dirExists(dataDir_);
}

// ─── Customer Operations ─────────────────────────────────────
std::vector<Customer> JSONManager::loadCustomers() const {
    json j = readFile("accounts.json");
    std::vector<Customer> customers;
    if (j.is_array()) {
        for (const auto& item : j) {
            Customer c;
            c.fromJson(item);
            customers.push_back(c);
        }
    }
    return customers;
}

bool JSONManager::saveCustomers(const std::vector<Customer>& customers) const {
    json j = json::array();
    for (const auto& c : customers) {
        j.push_back(c.toJson());
    }
    return writeFile("accounts.json", j);
}

// ─── Admin Operations ────────────────────────────────────────
std::vector<Admin> JSONManager::loadAdmins() const {
    json j = readFile("admins.json");
    std::vector<Admin> admins;
    if (j.is_array()) {
        for (const auto& item : j) {
            Admin a;
            a.fromJson(item);
            admins.push_back(a);
        }
    }
    return admins;
}

bool JSONManager::saveAdmins(const std::vector<Admin>& admins) const {
    json j = json::array();
    for (const auto& a : admins) {
        j.push_back(a.toJson());
    }
    return writeFile("admins.json", j);
}

// ─── ATM Cash ────────────────────────────────────────────────
json JSONManager::loadATMCash() const {
    return readFile("atm_cash.json");
}

bool JSONManager::saveATMCash(const json& cash) const {
    return writeFile("atm_cash.json", cash);
}

// ─── Logs ────────────────────────────────────────────────────
json JSONManager::loadLogs() const {
    return readFile("logs.json");
}

bool JSONManager::appendLog(const json& logEntry) const {
    json logs = loadLogs();
    if (!logs.is_array()) logs = json::array();
    logs.push_back(logEntry);
    return writeFile("logs.json", logs);
}

// ─── Settings ────────────────────────────────────────────────
json JSONManager::loadSettings() const {
    json j = readFile("settings.json");
    if (j.is_array() && j.empty()) {
        // Return default settings
        return {
            {"bankName", "ATMVerse Bank"},
            {"sessionTimeout", 120},
            {"maxPinAttempts", 3},
            {"defaultDailyLimit", 50000},
            {"minWithdraw", 100},
            {"maxWithdraw", 25000},
            {"version", "1.0.0"}
        };
    }
    return j;
}

bool JSONManager::saveSettings(const json& settings) const {
    return writeFile("settings.json", settings);
}

// ─── Reports ─────────────────────────────────────────────────
json JSONManager::loadReports() const {
    return readFile("daily_reports.json");
}

bool JSONManager::appendReport(const json& report) const {
    json reports = loadReports();
    if (!reports.is_array()) reports = json::array();
    reports.push_back(report);
    return writeFile("daily_reports.json", reports);
}

// ─── Backup/Restore ──────────────────────────────────────────
bool JSONManager::backupAll(const std::string& backupDir) const {
    if (!dirExists(backupDir)) MKDIR(backupDir.c_str());
#ifdef _WIN32
    std::string cmd = "copy " + dataDir_ + "\\*.json " + backupDir + "\\ > NUL";
#else
    std::string cmd = "cp " + dataDir_ + "/*.json " + backupDir + "/";
#endif
    return (system(cmd.c_str()) == 0);
}

bool JSONManager::restoreAll(const std::string& backupDir) const {
    if (!dirExists(backupDir)) return false;
#ifdef _WIN32
    std::string cmd = "copy " + backupDir + "\\*.json " + dataDir_ + "\\ > NUL";
#else
    std::string cmd = "cp " + backupDir + "/*.json " + dataDir_ + "/";
#endif
    return (system(cmd.c_str()) == 0);
}

// ─── Seed Default Data ───────────────────────────────────────
void JSONManager::seedDefaultData() {
    // Seed default admin if none exists
    auto admins = loadAdmins();
    if (admins.empty()) {
        std::string salt = util::generateSalt();
        std::string hash = util::hashPin("1234", salt);
        Admin defaultAdmin("admin001", "System Administrator",
                          "9999999999", "admin@atmverse.com",
                          "ADM00001", hash, salt, "SuperAdmin");
        admins.push_back(defaultAdmin);
        saveAdmins(admins);
    }

    // Seed a sample customer for testing
    auto customers = loadCustomers();
    if (customers.empty()) {
        std::string accNum = "100000000001";
        std::string cardNum = "4726823477349921";
        std::string cvv = "847";
        std::string expiry = "08/30";
        std::string salt = util::generateSalt();
        std::string pinHash = util::hashPin("1234", salt);

        Account acc(accNum, 50000.0, AccountType::SAVINGS);
        Card card(cardNum, cvv, expiry, pinHash, salt, accNum, "Prince Jaiswal", CardType::VISA);

        Customer customer("CUS00001", "Prince Jaiswal",
                         "9876543210", "prince@email.com", acc, card);
        customers.push_back(customer);
        saveCustomers(customers);
    }
}

} // namespace atmverse
