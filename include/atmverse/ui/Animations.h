#pragma once
#include <string>
#include <vector>
#include <map>

namespace atmverse {

class Animations {
public:
    // ─── ATM Card Animations ─────────────────────────────────
    static void cardInsertAnimation();
    static void cardReadingAnimation();
    static void cardEjectAnimation();

    // ─── Cash Animations ─────────────────────────────────────
    static void cashCountAnimation(const std::map<int, int>& notes);
    static void cashDispenseAnimation(double amount);
    static void depositAnimation(double amount);

    // ─── Receipt ─────────────────────────────────────────────
    static void receiptPrintAnimation(const std::vector<std::string>& receipt);

    // ─── Loading & Processing ────────────────────────────────
    static void loadingSpinner(const std::string& message, int durationMs = 2000);
    static void processingDots(const std::string& message, int durationMs = 1500);
    static void progressBarAnimation(const std::string& label, int durationMs = 2000);

    // ─── Welcome ─────────────────────────────────────────────
    static void bootAnimation();
    static void welcomeGlow();

    // ─── Typewriter ──────────────────────────────────────────
    static void typewriter(const std::string& text, int x, int y, int delayMs = 30);

    // ─── Status ──────────────────────────────────────────────
    static void successAnimation(const std::string& message);
    static void errorAnimation(const std::string& message);
    static void transferAnimation();
};

} // namespace atmverse
