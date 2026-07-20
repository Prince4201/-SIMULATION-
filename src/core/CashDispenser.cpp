#include "atmverse/core/CashDispenser.h"
#include "atmverse/utils/Utility.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace atmverse {

CashDispenser::CashDispenser(JSONManager* jm)
    : totalCash_(0), jsonManager_(jm) {
    denominations_[100] = 0;
    denominations_[200] = 0;
    denominations_[500] = 0;
    denominations_[2000] = 0;
}

void CashDispenser::recalculateTotal() {
    totalCash_ = 0;
    for (const auto& pair : denominations_) {
        int denom = pair.first;
        int count = pair.second;
        totalCash_ += denom * count;
    }
}

void CashDispenser::loadFromJson() {
    json cash = jsonManager_->loadATMCash();
    if (cash.contains("denominations")) {
        auto& d = cash["denominations"];
        denominations_[100] = d.value("100", 0);
        denominations_[200] = d.value("200", 0);
        denominations_[500] = d.value("500", 0);
        denominations_[2000] = d.value("2000", 0);
    }
    recalculateTotal();
}

void CashDispenser::saveToJson() {
    recalculateTotal();
    json cash = {
        {"denominations", {
            {"100", denominations_[100]},
            {"200", denominations_[200]},
            {"500", denominations_[500]},
            {"2000", denominations_[2000]}
        }},
        {"totalCash", totalCash_},
        {"lastRefill", util::getCurrentDateTime()}
    };
    jsonManager_->saveATMCash(cash);
}

DispenseResult CashDispenser::dispense(double amount) {
    DispenseResult result;
    result.success = false;

    int intAmount = static_cast<int>(amount);
    if (intAmount % 100 != 0) {
        result.message = "Amount must be a multiple of 100";
        return result;
    }

    if (intAmount > static_cast<int>(totalCash_)) {
        result.message = "Insufficient cash in ATM";
        return result;
    }

    // Greedy algorithm: use largest denomination first
    std::map<int, int> dispensed;
    int remaining = intAmount;

    // Iterate denominations in descending order
    std::vector<int> denoms = {2000, 500, 200, 100};
    for (int d : denoms) {
        if (remaining <= 0) break;
        int needed = remaining / d;
        int available = denominations_[d];
        int used = std::min(needed, available);
        if (used > 0) {
            dispensed[d] = used;
            remaining -= d * used;
        }
    }

    if (remaining > 0) {
        result.message = "Cannot dispense exact amount with available denominations";
        return result;
    }

    // Deduct from ATM
    for (const auto& pair : dispensed) { auto denom = pair.first; auto count = pair.second;
        denominations_[denom] -= count;
    }
    recalculateTotal();
    saveToJson();

    result.success = true;
    result.notes = dispensed;
    result.message = "Cash dispensed successfully";
    return result;
}

bool CashDispenser::canDispense(double amount) const {
    int intAmount = static_cast<int>(amount);
    if (intAmount % 100 != 0) return false;
    if (intAmount > static_cast<int>(totalCash_)) return false;

    int remaining = intAmount;
    std::vector<int> denoms = {2000, 500, 200, 100};
    for (int d : denoms) {
        if (remaining <= 0) break;
        int needed = remaining / d;
        auto it = denominations_.find(d);
        int available = (it != denominations_.end()) ? it->second : 0;
        int used = std::min(needed, available);
        remaining -= d * used;
    }
    return remaining == 0;
}

void CashDispenser::refill(int denomination, int count) {
    denominations_[denomination] += count;
    recalculateTotal();
    saveToJson();
}

void CashDispenser::refillAll(int c100, int c200, int c500, int c2000) {
    denominations_[100] += c100;
    denominations_[200] += c200;
    denominations_[500] += c500;
    denominations_[2000] += c2000;
    recalculateTotal();
    saveToJson();
}

int CashDispenser::getNoteCount(int denomination) const {
    auto it = denominations_.find(denomination);
    return (it != denominations_.end()) ? it->second : 0;
}

int CashDispenser::getTotalNotes() const {
    int total = 0;
    for (const auto& pair : denominations_) { auto _ = pair.first; auto count = pair.second;
        total += count;
    }
    return total;
}

std::vector<std::string> CashDispenser::renderCashStatus() const {
    std::vector<std::string> lines;
    lines.push_back("\033[1;36m  ATM Cash Status\033[0m");
    lines.push_back("  \033[38;5;240m" + std::string(36, '\xe2\x94\x80') + "\033[0m");

    std::vector<int> denoms = {2000, 500, 200, 100};
    for (int d : denoms) {
        auto it = denominations_.find(d);
        int count = (it != denominations_.end()) ? it->second : 0;
        double value = d * count;
        std::ostringstream oss;
        oss << "  \033[38;5;250m\xe2\x82\xb9" << std::setw(5) << d << " \033[38;5;245mx "
            << std::setw(4) << count << " \033[38;5;240m= \033[1;97m"
            << util::formatCurrency(value) << "\033[0m";
        lines.push_back(oss.str());
    }
    lines.push_back("  \033[38;5;240m" + std::string(36, '\xe2\x94\x80') + "\033[0m");
    lines.push_back("  \033[1;32mTotal: " + util::formatCurrency(totalCash_) + "\033[0m");
    return lines;
}

std::vector<std::string> CashDispenser::renderBarGraph() const {
    std::vector<std::string> lines;
    lines.push_back("\033[1;36m  ATM Cash Graph\033[0m");
    lines.push_back("");

    int maxCount = 0;
    for (const auto& pair : denominations_) { auto _ = pair.first; auto count = pair.second;
        maxCount = std::max(maxCount, count);
    }
    if (maxCount == 0) maxCount = 1;

    std::vector<int> denoms = {100, 200, 500, 2000};
    for (int d : denoms) {
        auto it = denominations_.find(d);
        int count = (it != denominations_.end()) ? it->second : 0;
        int barLen = static_cast<int>((static_cast<double>(count) / maxCount) * 20);

        std::string bar(barLen, '\xe2\x96\x88');
        std::string empty(20 - barLen, '\xe2\x96\x91');

        std::ostringstream oss;
        oss << "  \033[38;5;250m\xe2\x82\xb9" << std::setw(5) << d << " \033[38;5;39m"
            << bar << "\033[38;5;240m" << empty
            << " \033[97m" << count << "\033[0m";
        lines.push_back(oss.str());
    }
    return lines;
}

} // namespace atmverse
