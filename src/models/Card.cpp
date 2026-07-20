#include "atmverse/models/Card.h"
#include "atmverse/utils/Utility.h"
#include <sstream>
#include <iomanip>

namespace atmverse {

Card::Card(const std::string& cardNum, const std::string& cvv,
           const std::string& expiry, const std::string& pinHash,
           const std::string& pinSalt, const std::string& accNum,
           const std::string& holderName, CardType type)
    : cardNumber_(cardNum), cvv_(cvv), expiryDate_(expiry),
      pinHash_(pinHash), pinSalt_(pinSalt),
      status_(CardStatus::ACTIVE), cardType_(type),
      dailyLimit_(50000), dailyUsed_(0),
      lastResetDate_(util::getCurrentDate()),
      accountNumber_(accNum), holderName_(holderName) {}

double Card::getDailyRemaining() const {
    return dailyLimit_ - dailyUsed_;
}

std::string Card::getStatusString() const {
    switch (status_) {
        case CardStatus::ACTIVE: return "Active";
        case CardStatus::FROZEN: return "Frozen";
        case CardStatus::LOCKED: return "Locked";
        case CardStatus::EXPIRED: return "Expired";
        case CardStatus::REPLACED: return "Replaced";
        default: return "Unknown";
    }
}

std::string Card::getCardTypeString() const {
    switch (cardType_) {
        case CardType::VISA: return "VISA";
        case CardType::MASTERCARD: return "MasterCard";
        default: return "Unknown";
    }
}

bool Card::validatePin(const std::string& pin) const {
    return util::verifyPin(pin, pinHash_, pinSalt_);
}

void Card::changePin(const std::string& newPinHash, const std::string& newSalt) {
    pinHash_ = newPinHash;
    pinSalt_ = newSalt;
}

void Card::freeze() { status_ = CardStatus::FROZEN; }
void Card::unfreeze() { status_ = CardStatus::ACTIVE; }
void Card::lock() { status_ = CardStatus::LOCKED; }
void Card::unlock() { status_ = CardStatus::ACTIVE; }

bool Card::recordUsage(double amount) {
    // Reset daily usage if new day
    std::string today = util::getCurrentDate();
    if (today != lastResetDate_) {
        dailyUsed_ = 0;
        lastResetDate_ = today;
    }
    if (dailyUsed_ + amount > dailyLimit_) return false;
    dailyUsed_ += amount;
    return true;
}

void Card::resetDailyUsage() {
    dailyUsed_ = 0;
    lastResetDate_ = util::getCurrentDate();
}

std::vector<std::string> Card::renderCard() const {
    std::string formattedNum = util::formatCardNumber(cardNumber_);
    std::string typeStr = getCardTypeString();
    std::string name = util::toUpper(holderName_);
    if (name.length() > 24) name = name.substr(0, 24);

    // Neon-style ATM card using Unicode
    std::vector<std::string> lines;
    lines.push_back("\033[38;5;27m\xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[1;38;5;39mATMVerse BANK\033[0m" + std::string(24 - 13, ' ') + "\033[1;33m" + typeStr + "\033[0m  \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m                                      \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[38;5;220m\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\033[0m                                   \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[38;5;220m\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\033[0m                                   \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m                                      \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[1;97m" + util::padRight(formattedNum, 22) + "\033[0m              \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m                                      \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[38;5;250mEXP " + expiryDate_ + "\033[0m                          \033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x91\033[0m  \033[1;97m" + util::padRight(name, 36) + "\033[0m\033[38;5;27m\xe2\x95\x91\033[0m");
    lines.push_back("\033[38;5;27m\xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d\033[0m");
    return lines;
}

CardStatus Card::statusFromString(const std::string& s) {
    if (s == "Frozen") return CardStatus::FROZEN;
    if (s == "Locked") return CardStatus::LOCKED;
    if (s == "Expired") return CardStatus::EXPIRED;
    if (s == "Replaced") return CardStatus::REPLACED;
    return CardStatus::ACTIVE;
}

CardType Card::typeFromString(const std::string& s) {
    if (s == "MasterCard") return CardType::MASTERCARD;
    return CardType::VISA;
}

json Card::toJson() const {
    return {
        {"cardNumber", cardNumber_},
        {"cvv", cvv_},
        {"expiryDate", expiryDate_},
        {"pinHash", pinHash_},
        {"pinSalt", pinSalt_},
        {"status", getStatusString()},
        {"cardType", getCardTypeString()},
        {"dailyLimit", dailyLimit_},
        {"dailyUsed", dailyUsed_},
        {"lastResetDate", lastResetDate_},
        {"accountNumber", accountNumber_},
        {"holderName", holderName_}
    };
}

void Card::fromJson(const json& j) {
    cardNumber_ = j.value("cardNumber", "");
    cvv_ = j.value("cvv", "");
    expiryDate_ = j.value("expiryDate", "");
    pinHash_ = j.value("pinHash", "");
    pinSalt_ = j.value("pinSalt", "");
    status_ = statusFromString(j.value("status", "Active"));
    cardType_ = typeFromString(j.value("cardType", "VISA"));
    dailyLimit_ = j.value("dailyLimit", 50000.0);
    dailyUsed_ = j.value("dailyUsed", 0.0);
    lastResetDate_ = j.value("lastResetDate", "");
    accountNumber_ = j.value("accountNumber", "");
    holderName_ = j.value("holderName", "");
}

} // namespace atmverse
