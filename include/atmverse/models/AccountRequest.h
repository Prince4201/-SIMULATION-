#pragma once
#include <string>
#include <json.hpp>

using json = nlohmann::json;

namespace atmverse {

enum class RequestStatus {
    PENDING,
    APPROVED,
    REJECTED
};

class AccountRequest {
private:
    std::string requestId_;
    std::string name_;
    std::string phone_;
    std::string email_;
    std::string pinHash_;
    std::string pinSalt_;
    std::string accountType_; // "Savings" or "Current"
    RequestStatus status_;
    std::string timestamp_;

public:
    AccountRequest() = default;
    AccountRequest(const std::string& reqId, const std::string& name,
                   const std::string& phone, const std::string& email,
                   const std::string& pinHash, const std::string& pinSalt,
                   const std::string& accountType, RequestStatus status,
                   const std::string& timestamp);

    // Getters
    const std::string& getRequestId() const { return requestId_; }
    const std::string& getName() const { return name_; }
    const std::string& getPhone() const { return phone_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getPinHash() const { return pinHash_; }
    const std::string& getPinSalt() const { return pinSalt_; }
    const std::string& getAccountType() const { return accountType_; }
    RequestStatus getStatus() const { return status_; }
    const std::string& getTimestamp() const { return timestamp_; }

    // Setters
    void setStatus(RequestStatus s) { status_ = s; }

    // Serialization
    json toJson() const;
    void fromJson(const json& j);
};

} // namespace atmverse
