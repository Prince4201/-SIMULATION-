#pragma once
#include <string>
#include "atmverse/models/Customer.h"
#include "atmverse/models/Admin.h"

namespace atmverse {

class AuthManager {
private:
    int maxAttempts_;
    int sessionTimeout_; // seconds

public:
    AuthManager(int maxAttempts = 3, int timeout = 120)
        : maxAttempts_(maxAttempts), sessionTimeout_(timeout) {}

    // Customer auth
    bool authenticateCustomer(Customer& customer, const std::string& pin);
    bool isCardLocked(const Customer& customer) const;
    bool isCardUsable(const Customer& customer) const;
    void lockCard(Customer& customer);
    void unlockCard(Customer& customer);

    // Admin auth
    bool authenticateAdmin(const Admin& admin, const std::string& password);

    // Getters
    int getMaxAttempts() const { return maxAttempts_; }
    int getSessionTimeout() const { return sessionTimeout_; }
    int getRemainingAttempts(const Customer& customer) const;
};

} // namespace atmverse
