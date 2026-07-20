#include "atmverse/core/Logger.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

std::string Logger::eventTypeToString(LogEventType type) const {
    switch (type) {
        case LogEventType::LOGIN: return "LOGIN";
        case LogEventType::LOGOUT: return "LOGOUT";
        case LogEventType::FAILED_LOGIN: return "FAILED_LOGIN";
        case LogEventType::PIN_CHANGE: return "PIN_CHANGE";
        case LogEventType::TRANSFER: return "TRANSFER";
        case LogEventType::DEPOSIT: return "DEPOSIT";
        case LogEventType::WITHDRAW: return "WITHDRAW";
        case LogEventType::CARD_LOCK: return "CARD_LOCK";
        case LogEventType::CARD_UNLOCK: return "CARD_UNLOCK";
        case LogEventType::CARD_FREEZE: return "CARD_FREEZE";
        case LogEventType::ADMIN_LOGIN: return "ADMIN_LOGIN";
        case LogEventType::CASH_REFILL: return "CASH_REFILL";
        case LogEventType::ACCOUNT_CREATE: return "ACCOUNT_CREATE";
        case LogEventType::ACCOUNT_DELETE: return "ACCOUNT_DELETE";
        case LogEventType::SYSTEM_START: return "SYSTEM_START";
        case LogEventType::SYSTEM_STOP: return "SYSTEM_STOP";
        case LogEventType::BACKUP: return "BACKUP";
        case LogEventType::RESTORE: return "RESTORE";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogEventType type, const std::string& userId,
                 const std::string& details) {
    json entry = {
        {"timestamp", util::getCurrentDateTime()},
        {"type", eventTypeToString(type)},
        {"userId", userId},
        {"details", details}
    };
    jsonManager_->appendLog(entry);
}

std::vector<json> Logger::getRecentLogs(int count) const {
    json logs = jsonManager_->loadLogs();
    std::vector<json> result;
    if (!logs.is_array()) return result;

    int start = std::max(0, static_cast<int>(logs.size()) - count);
    for (int i = static_cast<int>(logs.size()) - 1; i >= start; --i) {
        result.push_back(logs[i]);
    }
    return result;
}

std::vector<json> Logger::getLogsByType(const std::string& type) const {
    json logs = jsonManager_->loadLogs();
    std::vector<json> result;
    if (!logs.is_array()) return result;
    for (const auto& log : logs) {
        if (log.value("type", "") == type) {
            result.push_back(log);
        }
    }
    return result;
}

std::vector<json> Logger::getLogsByUser(const std::string& userId) const {
    json logs = jsonManager_->loadLogs();
    std::vector<json> result;
    if (!logs.is_array()) return result;
    for (const auto& log : logs) {
        if (log.value("userId", "") == userId) {
            result.push_back(log);
        }
    }
    return result;
}

std::vector<json> Logger::getLogsByDate(const std::string& date) const {
    json logs = jsonManager_->loadLogs();
    std::vector<json> result;
    if (!logs.is_array()) return result;
    for (const auto& log : logs) {
        std::string ts = log.value("timestamp", "");
        if (ts.substr(0, 10) == date) {
            result.push_back(log);
        }
    }
    return result;
}

} // namespace atmverse
