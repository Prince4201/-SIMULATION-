#include "atmverse/utils/Utility.h"
#include <numeric>
#include <functional>

namespace atmverse {
namespace util {

// ─── Random Engine ───────────────────────────────────────────
static std::mt19937& getEngine() {
    static std::mt19937 engine(
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())
    );
    return engine;
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getEngine());
}

std::string randomDigits(int count) {
    std::string result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        result += static_cast<char>('0' + randomInt(0, 9));
    }
    return result;
}

// ─── ID Generators ───────────────────────────────────────────
std::string generateAccountNumber() {
    // Format: 1000 + 8 random digits = 12-digit account number
    return "1000" + randomDigits(8);
}

std::string generateCardNumber() {
    // VISA-style: starts with 4, 16 digits total
    // 4XXX XXXX XXXX XXXX
    std::string card = "4";
    card += randomDigits(15);
    // Luhn checksum fix for last digit
    int sum = 0;
    for (int i = 0; i < 15; ++i) {
        int d = card[i] - '0';
        if (i % 2 == 0) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
    }
    int check = (10 - (sum % 10)) % 10;
    card[15] = static_cast<char>('0' + check);
    return card;
}

std::string generateCVV() {
    return randomDigits(3);
}

std::string generateExpiryDate() {
    int month = randomInt(1, 12);
    int year = randomInt(28, 35); // 2028 to 2035
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << month << "/" << year;
    return oss.str();
}

std::string generateTransactionId() {
    return "TXN" + randomDigits(10);
}

std::string generateAdminId() {
    return "ADM" + randomDigits(5);
}

// ─── PIN Hashing (XOR + Salt) ────────────────────────────────
std::string generateSalt() {
    return randomDigits(8);
}

std::string hashPin(const std::string& pin, const std::string& salt) {
    // Simple but effective hash: XOR-rotate with salt
    std::string combined = pin + salt;
    unsigned long long hash = 5381;
    for (char c : combined) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // djb2
    }
    // Secondary pass with salt reversal
    std::string revSalt(salt.rbegin(), salt.rend());
    combined = revSalt + pin;
    unsigned long long hash2 = 0x100000001B3ULL;
    for (char c : combined) {
        hash2 ^= static_cast<unsigned char>(c);
        hash2 *= 0x100000001B3ULL; // FNV-1a style
    }
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(16) << hash
        << std::setw(16) << hash2;
    return oss.str();
}

bool verifyPin(const std::string& pin, const std::string& hash, const std::string& salt) {
    return hashPin(pin, salt) == hash;
}

// ─── Date/Time ───────────────────────────────────────────────
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#ifdef _WIN32
    struct tm* tm_ptr = localtime(&t); if (tm_ptr) tm_buf = *tm_ptr;
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#ifdef _WIN32
    struct tm* tm_ptr = localtime(&t); if (tm_ptr) tm_buf = *tm_ptr;
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d");
    return oss.str();
}

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#ifdef _WIN32
    struct tm* tm_ptr = localtime(&t);
    if (tm_ptr) tm_buf = *tm_ptr;
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    return oss.str();
}

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#ifdef _WIN32
    struct tm* tm_ptr = localtime(&t); if (tm_ptr) tm_buf = *tm_ptr;
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%d%H%M%S")
        << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

long long getEpochSeconds() {
    return static_cast<long long>(
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
    );
}

// ─── Formatters ──────────────────────────────────────────────
std::string formatCurrency(double amount) {
    // Indian format: ₹1,23,456.00
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    std::string numStr = oss.str();

    // Split at decimal
    auto dotPos = numStr.find('.');
    std::string intPart = numStr.substr(0, dotPos);
    std::string decPart = numStr.substr(dotPos);

    bool negative = false;
    if (!intPart.empty() && intPart[0] == '-') {
        negative = true;
        intPart = intPart.substr(1);
    }

    // Indian comma system: last 3 digits, then groups of 2
    std::string formatted;
    int len = static_cast<int>(intPart.length());
    if (len <= 3) {
        formatted = intPart;
    } else {
        formatted = intPart.substr(len - 3);
        int remaining = len - 3;
        int pos = 0;
        std::string prefix = intPart.substr(0, remaining);
        // Insert commas every 2 digits from right
        std::string commaPart;
        for (int i = static_cast<int>(prefix.length()) - 1; i >= 0; --i) {
            commaPart = prefix[i] + commaPart;
            pos++;
            if (pos % 2 == 0 && i > 0) {
                commaPart = "," + commaPart;
            }
        }
        formatted = commaPart + "," + formatted;
    }

    return std::string(negative ? "-" : "") + "\xe2\x82\xb9" + formatted + decPart;
}

std::string formatCardNumber(const std::string& cardNum) {
    if (cardNum.length() != 16) return cardNum;
    return cardNum.substr(0, 4) + " " + cardNum.substr(4, 4) + " "
         + cardNum.substr(8, 4) + " " + cardNum.substr(12, 4);
}

std::string maskCardNumber(const std::string& cardNum) {
    if (cardNum.length() < 8) return cardNum;
    return "XXXX XXXX XXXX " + cardNum.substr(12, 4);
}

std::string padRight(const std::string& str, int width, char fill) {
    if (static_cast<int>(str.length()) >= width) return str.substr(0, width);
    return str + std::string(width - str.length(), fill);
}

std::string padLeft(const std::string& str, int width, char fill) {
    if (static_cast<int>(str.length()) >= width) return str.substr(0, width);
    return std::string(width - str.length(), fill) + str;
}

std::string centerString(const std::string& str, int width, char fill) {
    if (static_cast<int>(str.length()) >= width) return str.substr(0, width);
    int total = width - static_cast<int>(str.length());
    int left = total / 2;
    int right = total - left;
    return std::string(left, fill) + str + std::string(right, fill);
}

std::string truncate(const std::string& str, int maxLen) {
    if (static_cast<int>(str.length()) <= maxLen) return str;
    if (maxLen <= 3) return str.substr(0, maxLen);
    return str.substr(0, maxLen - 3) + "...";
}

// ─── Validators ──────────────────────────────────────────────
bool isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isValidPin(const std::string& pin) {
    return pin.length() == 4 && isNumeric(pin);
}

bool isValidEmail(const std::string& email) {
    // Simple email validation
    auto at = email.find('@');
    auto dot = email.rfind('.');
    return at != std::string::npos && dot != std::string::npos
        && at > 0 && dot > at + 1 && dot < email.length() - 1;
}

bool isValidPhone(const std::string& phone) {
    std::string digits;
    for (char c : phone) {
        if (std::isdigit(c)) digits += c;
    }
    return digits.length() == 10;
}

bool isValidAmount(double amount) {
    return amount > 0 && amount <= 200000; // Max single transaction
}

bool isValidName(const std::string& name) {
    return name.length() >= 2 && name.length() <= 50 &&
           std::all_of(name.begin(), name.end(), [](char c) {
               return std::isalpha(c) || c == ' ' || c == '.';
           });
}

// ─── String Utils ────────────────────────────────────────────
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace util
} // namespace atmverse
