#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "atmverse/persistence/JSONManager.h"

namespace atmverse {

using json = nlohmann::json;

enum class LogEventType {
    LOGIN, LOGOUT, FAILED_LOGIN, PIN_CHANGE, TRANSFER,
    DEPOSIT, WITHDRAW, CARD_LOCK, CARD_UNLOCK, CARD_FREEZE,
    ADMIN_LOGIN, CASH_REFILL, ACCOUNT_CREATE, ACCOUNT_DELETE,
    SYSTEM_START, SYSTEM_STOP, BACKUP, RESTORE
};

class Logger {
private:
    JSONManager* jsonManager_;

public:
    explicit Logger(JSONManager* jm) : jsonManager_(jm) {}

    void log(LogEventType type, const std::string& userId,
             const std::string& details);

    std::string eventTypeToString(LogEventType type) const;
    std::vector<json> getRecentLogs(int count = 50) const;
    std::vector<json> getLogsByType(const std::string& type) const;
    std::vector<json> getLogsByUser(const std::string& userId) const;
    std::vector<json> getLogsByDate(const std::string& date) const;
};

} // namespace atmverse
