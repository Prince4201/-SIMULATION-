#pragma once
#include <string>
#include <vector>
#include <functional>

namespace atmverse {

class TerminalUI {
public:
    // ─── Screen Control ──────────────────────────────────────
    static void clearScreen();
    static void moveCursor(int row, int col);
    static void hideCursor();
    static void showCursor();
    static void enableANSI();  // Windows-specific

    // ─── Drawing Primitives ──────────────────────────────────
    static void drawBox(int x, int y, int width, int height,
                        const std::string& color = "", bool rounded = true);
    static void drawDoubleBox(int x, int y, int width, int height,
                              const std::string& color = "");
    static void drawLine(int x, int y, int width, const std::string& color = "");
    static void drawText(int x, int y, const std::string& text);
    static void drawCentered(int y, const std::string& text, int width = 60);
    static void drawCenteredColored(int y, const std::string& text,
                                     const std::string& color, int width = 60);

    // ─── ATM Frame ───────────────────────────────────────────
    static void drawATMFrame();
    static void drawATMHeader(const std::string& bankName, const std::string& dateTime);
    static void drawATMFooter(const std::string& status = "");
    static void drawStatusBar(const std::string& message, const std::string& color = "");

    // ─── Input ───────────────────────────────────────────────
    static std::string getInput(const std::string& prompt, int x = -1, int y = -1);
    static std::string getMaskedInput(const std::string& prompt, int maxLen = 4,
                                       int x = -1, int y = -1);
    static int getMenuChoice(int min, int max, int x = -1, int y = -1);
    static char getKeyPress();
    static bool getConfirmation(const std::string& prompt, int x = -1, int y = -1);

    // ─── Tables ──────────────────────────────────────────────
    static void drawTable(int x, int y, const std::vector<std::string>& headers,
                          const std::vector<std::vector<std::string>>& rows,
                          const std::vector<int>& colWidths);

    // ─── Progress & Loading ──────────────────────────────────
    static void drawProgressBar(int x, int y, int width, double progress,
                                 const std::string& color = "");

    // ─── Utility ─────────────────────────────────────────────
    static void sleep_ms(int ms);
    static int getTerminalWidth();
    static int getTerminalHeight();
    static void printLines(const std::vector<std::string>& lines, int startY = -1);

    // ─── ATM Keypad ──────────────────────────────────────────
    static void drawKeypad(int x, int y);

    // ─── Message Boxes ───────────────────────────────────────
    static void showSuccess(const std::string& message);
    static void showError(const std::string& message);
    static void showWarning(const std::string& message);
    static void showInfo(const std::string& message);
    static void waitForKey(const std::string& message = "Press any key to continue...");
};

} // namespace atmverse
