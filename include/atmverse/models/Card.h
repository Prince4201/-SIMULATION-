#pragma once
#include <string>
#include "json.hpp"

namespace atmverse {

using json = nlohmann::json;

enum class CardStatus { ACTIVE, FROZEN, LOCKED, EXPIRED, REPLACED };
enum class CardType { VISA, MASTERCARD };

class Card {
private:
    std::string cardNumber_;
    std::string cvv_;
    std::string expiryDate_;
    std::string pinHash_;
    std::string pinSalt_;
    CardStatus status_;
    CardType cardType_;
    double dailyLimit_;
    double dailyUsed_;
    std::string lastResetDate_;
    std::string accountNumber_; // Link to account
    std::string holderName_;

public:
    Card() : status_(CardStatus::ACTIVE), cardType_(CardType::VISA),
             dailyLimit_(50000), dailyUsed_(0) {}

    Card(const std::string& cardNum, const std::string& cvv,
         const std::string& expiry, const std::string& pinHash,
         const std::string& pinSalt, const std::string& accNum,
         const std::string& holderName, CardType type = CardType::VISA);

    // Getters
    const std::string& getCardNumber() const { return cardNumber_; }
    const std::string& getCVV() const { return cvv_; }
    const std::string& getExpiryDate() const { return expiryDate_; }
    const std::string& getPinHash() const { return pinHash_; }
    const std::string& getPinSalt() const { return pinSalt_; }
    CardStatus getStatus() const { return status_; }
    CardType getCardType() const { return cardType_; }
    double getDailyLimit() const { return dailyLimit_; }
    double getDailyUsed() const { return dailyUsed_; }
    double getDailyRemaining() const;
    const std::string& getAccountNumber() const { return accountNumber_; }
    const std::string& getHolderName() const { return holderName_; }

    std::string getStatusString() const;
    std::string getCardTypeString() const;

    // Setters
    void setDailyLimit(double limit) { dailyLimit_ = limit; }
    void setHolderName(const std::string& name) { holderName_ = name; }

    // Operations
    bool validatePin(const std::string& pin) const;
    void changePin(const std::string& newPinHash, const std::string& newSalt);
    void freeze();
    void unfreeze();
    void lock();
    void unlock();
    bool recordUsage(double amount);
    void resetDailyUsage();
    bool isUsable() const { return status_ == CardStatus::ACTIVE; }

    // ASCII card display
    std::vector<std::string> renderCard() const;

    // Serialization
    json toJson() const;
    void fromJson(const json& j);

    // Static helpers
    static CardStatus statusFromString(const std::string& s);
    static CardType typeFromString(const std::string& s);
};

} // namespace atmverse
