#include "atmverse/models/AccountRequest.h"

namespace atmverse {

AccountRequest::AccountRequest(const std::string& reqId, const std::string& name,
                               const std::string& phone, const std::string& email,
                               const std::string& pinHash, const std::string& pinSalt,
                               const std::string& accountType, RequestStatus status,
                               const std::string& timestamp)
    : requestId_(reqId), name_(name), phone_(phone), email_(email),
      pinHash_(pinHash), pinSalt_(pinSalt), accountType_(accountType),
      status_(status), timestamp_(timestamp) {}

json AccountRequest::toJson() const {
    json j;
    j["requestId"] = requestId_;
    j["name"] = name_;
    j["phone"] = phone_;
    j["email"] = email_;
    j["pinHash"] = pinHash_;
    j["pinSalt"] = pinSalt_;
    j["accountType"] = accountType_;
    j["timestamp"] = timestamp_;
    
    switch (status_) {
        case RequestStatus::PENDING: j["status"] = "PENDING"; break;
        case RequestStatus::APPROVED: j["status"] = "APPROVED"; break;
        case RequestStatus::REJECTED: j["status"] = "REJECTED"; break;
    }
    return j;
}

void AccountRequest::fromJson(const json& j) {
    requestId_ = j.value("requestId", "");
    name_ = j.value("name", "");
    phone_ = j.value("phone", "");
    email_ = j.value("email", "");
    pinHash_ = j.value("pinHash", "");
    pinSalt_ = j.value("pinSalt", "");
    accountType_ = j.value("accountType", "Savings");
    timestamp_ = j.value("timestamp", "");

    std::string statStr = j.value("status", "PENDING");
    if (statStr == "APPROVED") status_ = RequestStatus::APPROVED;
    else if (statStr == "REJECTED") status_ = RequestStatus::REJECTED;
    else status_ = RequestStatus::PENDING;
}

} // namespace atmverse
