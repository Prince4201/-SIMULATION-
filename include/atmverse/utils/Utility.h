#pragma once
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <regex>

namespace atmverse {
namespace util {

// ─── ID Generators ───────────────────────────────────────────
std::string generateAccountNumber();
std::string generateCardNumber();
std::string generateCVV();
std::string generateExpiryDate();
std::string generateTransactionId();
std::string generateAdminId();

// ─── PIN Hashing ─────────────────────────────────────────────
std::string hashPin(const std::string& pin, const std::string& salt);
std::string generateSalt();
bool verifyPin(const std::string& pin, const std::string& hash, const std::string& salt);

// ─── Date/Time ───────────────────────────────────────────────
std::string getCurrentDateTime();
std::string getCurrentDate();
std::string getCurrentTime();
std::string getTimestamp();
long long getEpochSeconds();

// ─── Formatters ──────────────────────────────────────────────
std::string formatCurrency(double amount);
std::string formatCardNumber(const std::string& cardNum);
std::string maskCardNumber(const std::string& cardNum);
std::string padRight(const std::string& str, int width, char fill = ' ');
std::string padLeft(const std::string& str, int width, char fill = ' ');
std::string centerString(const std::string& str, int width, char fill = ' ');
std::string truncate(const std::string& str, int maxLen);

// ─── Validators ──────────────────────────────────────────────
bool isNumeric(const std::string& str);
bool isValidPin(const std::string& pin);
bool isValidEmail(const std::string& email);
bool isValidPhone(const std::string& phone);
bool isValidAmount(double amount);
bool isValidName(const std::string& name);

// ─── Random ──────────────────────────────────────────────────
int randomInt(int min, int max);
std::string randomDigits(int count);

// ─── String Utils ────────────────────────────────────────────
std::string toUpper(const std::string& str);
std::string toLower(const std::string& str);
std::string trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delim);

} // namespace util
} // namespace atmverse
