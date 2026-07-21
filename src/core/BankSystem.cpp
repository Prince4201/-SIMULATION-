#include "atmverse/core/BankSystem.h"
#include "atmverse/utils/Utility.h"
#include <algorithm>

namespace atmverse {

BankSystem::BankSystem()
    : jsonManager_("data"),
      cashDispenser_(&jsonManager_),
      logger_(&jsonManager_),
      authManager_(3, 120),
      reportGenerator_(&jsonManager_) {}

void BankSystem::initialize() {
    jsonManager_.initialize();
    jsonManager_.seedDefaultData();
    loadAll();
    cashDispenser_.loadFromJson();
    settings_ = jsonManager_.loadSettings();

    // Configure auth from settings
    int maxAttempts = settings_.value("maxPinAttempts", 3);
    int timeout = settings_.value("sessionTimeout", 120);
    authManager_ = AuthManager(maxAttempts, timeout);

    logger_.log(LogEventType::SYSTEM_START, "SYSTEM", "ATMVerse started");
}

void BankSystem::loadAll() {
    customers_ = jsonManager_.loadCustomers();
    admins_ = jsonManager_.loadAdmins();
    accountRequests_ = jsonManager_.loadAccountRequests();
    buildIndices();
}

void BankSystem::saveAll() {
    jsonManager_.saveCustomers(customers_);
    jsonManager_.saveAdmins(admins_);
    jsonManager_.saveAccountRequests(accountRequests_);
    cashDispenser_.saveToJson();
}

void BankSystem::buildIndices() {
    cardIndex_.clear();
    accountIndex_.clear();
    usedCardNumbers_.clear();
    usedAccountNumbers_.clear();

    for (int i = 0; i < static_cast<int>(customers_.size()); ++i) {
        const auto& c = customers_[i];
        cardIndex_[c.getCard().getCardNumber()] = i;
        accountIndex_[c.getAccount().getAccountNumber()] = i;
        usedCardNumbers_.insert(c.getCard().getCardNumber());
        usedAccountNumbers_.insert(c.getAccount().getAccountNumber());
    }
}

Customer* BankSystem::findByCardNumber(const std::string& cardNum) {
    auto it = cardIndex_.find(cardNum);
    if (it != cardIndex_.end() && it->second < static_cast<int>(customers_.size())) {
        return &customers_[it->second];
    }
    return nullptr;
}

Customer* BankSystem::findByAccountNumber(const std::string& accNum) {
    auto it = accountIndex_.find(accNum);
    if (it != accountIndex_.end() && it->second < static_cast<int>(customers_.size())) {
        return &customers_[it->second];
    }
    return nullptr;
}

Customer* BankSystem::findByName(const std::string& name) {
    std::string lowerName = util::toLower(name);
    for (auto& c : customers_) {
        if (util::toLower(c.getName()) == lowerName) return &c;
    }
    return nullptr;
}

std::vector<Customer*> BankSystem::searchCustomers(const std::string& query) {
    std::vector<Customer*> results;
    std::string lq = util::toLower(query);
    for (auto& c : customers_) {
        if (util::toLower(c.getName()).find(lq) != std::string::npos ||
            c.getAccount().getAccountNumber().find(query) != std::string::npos ||
            c.getCard().getCardNumber().find(query) != std::string::npos) {
            results.push_back(&c);
        }
    }
    return results;
}

Customer BankSystem::createCustomer(const std::string& name, const std::string& phone,
                                     const std::string& email, const std::string& pin,
                                     AccountType type, double initialBalance)
{
    // Generate unique identifiers
    std::string accNum;
    do {
        accNum = util::generateAccountNumber();
    } while (usedAccountNumbers_.count(accNum));

    std::string cardNum;
    do {
        cardNum = util::generateCardNumber();
    } while (usedCardNumbers_.count(cardNum));

    std::string cvv = util::generateCVV();
    std::string expiry = util::generateExpiryDate();
    std::string salt = util::generateSalt();
    std::string pinHash = util::hashPin(pin, salt);
    std::string custId = "CUS" + util::randomDigits(5);

    CardType cardType = (util::randomInt(0, 1) == 0) ? CardType::VISA : CardType::MASTERCARD;

    Account account(accNum, initialBalance, type);
    Card card(cardNum, cvv, expiry, pinHash, salt, accNum, name, cardType);
    Customer customer(custId, name, phone, email, account, card);

    customers_.push_back(customer);
    buildIndices();
    saveAll();

    logger_.log(LogEventType::ACCOUNT_CREATE, custId,
               "Account created: " + accNum + " Card: " + cardNum);

    return customer;
}

bool BankSystem::deleteCustomer(const std::string& accountNumber) {
    auto it = std::find_if(customers_.begin(), customers_.end(),
        [&](const Customer& c) {
            return c.getAccount().getAccountNumber() == accountNumber;
        });

    if (it == customers_.end()) return false;

    logger_.log(LogEventType::ACCOUNT_DELETE, it->getId(),
               "Account deleted: " + accountNumber);

    customers_.erase(it);
    buildIndices();
    saveAll();
    return true;
}

bool BankSystem::withdraw(Customer& customer, double amount) {
    if (!customer.getAccount().isActive()) return false;
    if (!customer.getCard().isUsable()) return false;

    // Check daily limit
    if (!customer.getCardRef().recordUsage(amount)) return false;

    // Check ATM cash
    if (!cashDispenser_.canDispense(amount)) return false;

    double balBefore = customer.getAccount().getBalance();
    if (!customer.getAccountRef().debit(amount)) {
        customer.getCardRef().recordUsage(-amount); // Rollback
        return false;
    }

    auto result = cashDispenser_.dispense(amount);
    if (!result.success) {
        customer.getAccountRef().credit(amount); // Rollback
        return false;
    }

    Transaction txn(util::generateTransactionId(), TransactionType::WITHDRAW,
                    amount, balBefore, customer.getAccount().getBalance(),
                    customer.getAccount().getAccountNumber(), "",
                    "ATM Withdrawal");
    customer.addTransaction(txn);
    saveAll();

    logger_.log(LogEventType::WITHDRAW, customer.getId(),
               "Withdrawal: " + util::formatCurrency(amount));

    return true;
}

bool BankSystem::deposit(Customer& customer, double amount) {
    if (!customer.getAccount().isActive()) return false;

    double balBefore = customer.getAccount().getBalance();
    customer.getAccountRef().credit(amount);

    // Add cash to ATM
    int intAmount = static_cast<int>(amount);
    // Distribute into denominations (simulation)
    int n2000 = intAmount / 2000; intAmount %= 2000;
    int n500 = intAmount / 500; intAmount %= 500;
    int n200 = intAmount / 200; intAmount %= 200;
    int n100 = intAmount / 100;
    cashDispenser_.refillAll(n100, n200, n500, n2000);

    Transaction txn(util::generateTransactionId(), TransactionType::DEPOSIT,
                    amount, balBefore, customer.getAccount().getBalance(),
                    "", customer.getAccount().getAccountNumber(),
                    "ATM Deposit");
    customer.addTransaction(txn);
    saveAll();

    logger_.log(LogEventType::DEPOSIT, customer.getId(),
               "Deposit: " + util::formatCurrency(amount));

    return true;
}

bool BankSystem::transfer(Customer& sender, const std::string& receiverAccount,
                           double amount)
{
    Customer* receiver = findByAccountNumber(receiverAccount);
    if (!receiver) return false;
    if (!sender.getAccount().isActive() || !receiver->getAccount().isActive()) return false;

    double senderBalBefore = sender.getAccount().getBalance();
    if (!sender.getAccountRef().debit(amount)) return false;

    double receiverBalBefore = receiver->getAccount().getBalance();
    receiver->getAccountRef().credit(amount);

    std::string txnId = util::generateTransactionId();

    // Sender transaction
    Transaction txnOut(txnId, TransactionType::TRANSFER_OUT,
                       amount, senderBalBefore, sender.getAccount().getBalance(),
                       sender.getAccount().getAccountNumber(), receiverAccount,
                       "Transfer to " + receiver->getName());
    sender.addTransaction(txnOut);

    // Receiver transaction
    Transaction txnIn(txnId + "R", TransactionType::TRANSFER_IN,
                      amount, receiverBalBefore, receiver->getAccount().getBalance(),
                      sender.getAccount().getAccountNumber(), receiverAccount,
                      "Transfer from " + sender.getName());
    receiver->addTransaction(txnIn);

    saveAll();

    logger_.log(LogEventType::TRANSFER, sender.getId(),
               "Transfer: " + util::formatCurrency(amount) + " to " + receiverAccount);

    return true;
}

bool BankSystem::changePin(Customer& customer, const std::string& oldPin,
                            const std::string& newPin)
{
    if (!customer.getCard().validatePin(oldPin)) return false;
    if (!util::isValidPin(newPin)) return false;

    std::string salt = util::generateSalt();
    std::string hash = util::hashPin(newPin, salt);
    customer.getCardRef().changePin(hash, salt);
    saveAll();

    logger_.log(LogEventType::PIN_CHANGE, customer.getId(), "PIN changed successfully");
    return true;
}

void BankSystem::freezeCard(Customer& customer) {
    customer.getCardRef().freeze();
    saveAll();
    logger_.log(LogEventType::CARD_FREEZE, customer.getId(), "Card frozen");
}

void BankSystem::unfreezeCard(Customer& customer) {
    customer.getCardRef().unfreeze();
    saveAll();
    logger_.log(LogEventType::CARD_UNLOCK, customer.getId(), "Card unfrozen");
}

void BankSystem::lockCard(Customer& customer) {
    customer.getCardRef().lock();
    saveAll();
    logger_.log(LogEventType::CARD_LOCK, customer.getId(), "Card locked");
}

void BankSystem::unlockCard(Customer& customer) {
    customer.getCardRef().unlock();
    customer.resetFailedAttempts();
    saveAll();
    logger_.log(LogEventType::CARD_UNLOCK, customer.getId(), "Card unlocked");
}

bool BankSystem::setDailyLimit(Customer& customer, double limit) {
    if (limit < 1000 || limit > 200000) return false;
    customer.getCardRef().setDailyLimit(limit);
    saveAll();
    return true;
}

std::string BankSystem::getBankName() const {
    return settings_.value("bankName", "ATMVerse Bank");
}

// ─── Account Request Management ──────────────────────────────
bool BankSystem::submitAccountRequest(const std::string& name, const std::string& phone,
                                       const std::string& email, const std::string& pin,
                                       const std::string& accountType)
{
    std::string reqId = "REQ" + util::randomDigits(6);
    std::string salt = util::generateSalt();
    std::string pinHash = util::hashPin(pin, salt);
    std::string timestamp = util::getCurrentDateTime();

    AccountRequest req(reqId, name, phone, email, pinHash, salt,
                       accountType, RequestStatus::PENDING, timestamp);
    accountRequests_.push_back(req);
    jsonManager_.saveAccountRequests(accountRequests_);
    logger_.log(LogEventType::SYSTEM_START, reqId, "Account request submitted: " + name);
    return true;
}

std::vector<AccountRequest> BankSystem::getPendingRequests() const {
    std::vector<AccountRequest> pending;
    for (const auto& req : accountRequests_) {
        if (req.getStatus() == RequestStatus::PENDING) {
            pending.push_back(req);
        }
    }
    return pending;
}

bool BankSystem::approveRequest(const std::string& requestId) {
    for (auto& req : accountRequests_) {
        if (req.getRequestId() == requestId && req.getStatus() == RequestStatus::PENDING) {
            req.setStatus(RequestStatus::APPROVED);

            // Determine AccountType enum
            AccountType accType = AccountType::SAVINGS;
            if (req.getAccountType() == "Current") {
                accType = AccountType::CURRENT;
            }

            // Generate unique identifiers
            std::string accNum;
            do {
                accNum = util::generateAccountNumber();
            } while (usedAccountNumbers_.count(accNum));

            std::string cardNum;
            do {
                cardNum = util::generateCardNumber();
            } while (usedCardNumbers_.count(cardNum));

            std::string cvv = util::generateCVV();
            std::string expiry = util::generateExpiryDate();
            std::string custId = "CUS" + util::randomDigits(5);
            CardType cardType = (util::randomInt(0, 1) == 0) ? CardType::VISA : CardType::MASTERCARD;

            Account account(accNum, 0.0, accType);
            Card card(cardNum, cvv, expiry, req.getPinHash(), req.getPinSalt(),
                      accNum, req.getName(), cardType);
            Customer customer(custId, req.getName(), req.getPhone(), req.getEmail(),
                              account, card);

            customers_.push_back(customer);
            
            // Set the assigned identifiers in the request
            req.setAssignedAccountNumber(accNum);
            req.setAssignedCardNumber(cardNum);

            buildIndices();
            saveAll();

            logger_.log(LogEventType::ACCOUNT_CREATE, custId,
                       "Account approved: " + accNum + " Card: " + cardNum);
            return true;
        }
    }
    return false;
}

bool BankSystem::rejectRequest(const std::string& requestId) {
    for (auto& req : accountRequests_) {
        if (req.getRequestId() == requestId && req.getStatus() == RequestStatus::PENDING) {
            req.setStatus(RequestStatus::REJECTED);
            jsonManager_.saveAccountRequests(accountRequests_);
            logger_.log(LogEventType::SYSTEM_START, requestId, "Account request rejected");
            return true;
        }
    }
    return false;
}

AccountRequest* BankSystem::checkRequestStatus(const std::string& query) {
    for (auto& req : accountRequests_) {
        if (req.getRequestId() == query || req.getPhone() == query) {
            return &req;
        }
    }
    return nullptr;
}

} // namespace atmverse

