#pragma once
#include <string>
#include <map>
#include <vector>
#include "json.hpp"
#include "atmverse/persistence/JSONManager.h"

namespace atmverse {

using json = nlohmann::json;

struct DispenseResult {
    bool success;
    std::map<int, int> notes; // denomination -> count
    std::string message;
};

class CashDispenser {
private:
    std::map<int, int> denominations_; // denomination -> count (ordered)
    double totalCash_;
    JSONManager* jsonManager_;

    void recalculateTotal();

public:
    explicit CashDispenser(JSONManager* jm);

    // Load/Save
    void loadFromJson();
    void saveToJson();

    // Operations
    DispenseResult dispense(double amount);
    bool canDispense(double amount) const;
    void refill(int denomination, int count);
    void refillAll(int count100, int count200, int count500, int count2000);

    // Getters
    double getTotalCash() const { return totalCash_; }
    const std::map<int, int>& getDenominations() const { return denominations_; }
    int getNoteCount(int denomination) const;
    int getTotalNotes() const;

    // Display helpers
    std::vector<std::string> renderCashStatus() const;
    std::vector<std::string> renderBarGraph() const;
};

} // namespace atmverse
