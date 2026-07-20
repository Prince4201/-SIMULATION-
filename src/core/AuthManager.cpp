#include "atmverse/core/AuthManager.h"

namespace atmverse {

bool AuthManager::authenticateCustomer(Customer& customer, const std::string& pin) {
    if (isCardLocked(customer)) return false;

    if (customer.getCard().validatePin(pin)) {
        customer.resetFailedAttempts();
        return true;
    }

    customer.incrementFailedAttempts();
    if (customer.getFailedPinAttempts() >= maxAttempts_) {
        lockCard(customer);
    }
    return false;
}

bool AuthManager::isCardLocked(const Customer& customer) const {
    return customer.getCard().getStatus() == CardStatus::LOCKED;
}

bool AuthManager::isCardUsable(const Customer& customer) const {
    return customer.getCard().isUsable() && customer.getAccount().isActive();
}

void AuthManager::lockCard(Customer& customer) {
    customer.getCardRef().lock();
}

void AuthManager::unlockCard(Customer& customer) {
    customer.getCardRef().unlock();
    customer.resetFailedAttempts();
}

bool AuthManager::authenticateAdmin(const Admin& admin, const std::string& password) {
    return admin.validatePassword(password);
}

int AuthManager::getRemainingAttempts(const Customer& customer) const {
    return maxAttempts_ - customer.getFailedPinAttempts();
}

} // namespace atmverse
