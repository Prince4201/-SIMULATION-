#include "atmverse/ui/TerminalUI.h"
#include "atmverse/ui/Themes.h"
#include "atmverse/utils/Utility.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

namespace atmverse {

// ─── Screen Control ──────────────────────────────────────────
void TerminalUI::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void TerminalUI::moveCursor(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H" << std::flush;
}

void TerminalUI::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void TerminalUI::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void TerminalUI::enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);

    // Set UTF-8 code page
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

// ─── Drawing Primitives ──────────────────────────────────────
void TerminalUI::drawBox(int x, int y, int width, int height,
                          const std::string& color, bool rounded)
{
    std::string tl = rounded ? theme::RBOX_TL : theme::BOX_TL;
    std::string tr = rounded ? theme::RBOX_TR : theme::BOX_TR;
    std::string bl = rounded ? theme::RBOX_BL : theme::BOX_BL;
    std::string br = rounded ? theme::RBOX_BR : theme::BOX_BR;
    std::string h = theme::BOX_H;
    std::string v = theme::BOX_V;

    std::string c = color.empty() ? theme::DEEP_BLUE : color;
    std::string hline;
    for (int i = 0; i < width - 2; ++i) hline += theme::BOX_H;

    moveCursor(y, x);
    std::cout << c << tl << hline << tr << theme::RESET;

    for (int row = 1; row < height - 1; ++row) {
        moveCursor(y + row, x);
        std::cout << c << v << theme::RESET
                  << std::string((size_t)(width - 2), ' ')
                  << c << v << theme::RESET;
    }

    moveCursor(y + height - 1, x);
    std::cout << c << bl << hline << br << theme::RESET;
}

void TerminalUI::drawDoubleBox(int x, int y, int width, int height,
                                const std::string& color)
{
    std::string c = color.empty() ? theme::NEON_BLUE : color;
    std::string hline;
    for (int i = 0; i < width - 2; ++i) hline += theme::DBOX_H;

    moveCursor(y, x);
    std::cout << c << theme::DBOX_TL << hline << theme::DBOX_TR << theme::RESET;

    for (int row = 1; row < height - 1; ++row) {
        moveCursor(y + row, x);
        std::cout << c << theme::DBOX_V << theme::RESET
                  << std::string((size_t)(width - 2), ' ')
                  << c << theme::DBOX_V << theme::RESET;
    }

    moveCursor(y + height - 1, x);
    std::cout << c << theme::DBOX_BL << hline << theme::DBOX_BR << theme::RESET;
}

void TerminalUI::drawLine(int x, int y, int width, const std::string& color) {
    std::string c = color.empty() ? theme::DARK_GRAY : color;
    moveCursor(y, x);
    std::cout << c;
    for (int i = 0; i < width; ++i) std::cout << theme::BOX_H;
    std::cout << theme::RESET;
}

void TerminalUI::drawText(int x, int y, const std::string& text) {
    moveCursor(y, x);
    std::cout << text << theme::RESET;
}

void TerminalUI::drawCentered(int y, const std::string& text, int width) {
    // Calculate visible text length (exclude ANSI codes)
    int visLen = 0;
    bool inEscape = false;
    for (char c : text) {
        if (c == '\033') inEscape = true;
        else if (inEscape && c == 'm') inEscape = false;
        else if (!inEscape) visLen++;
    }

    int pad = std::max(0, (width - visLen) / 2);
    moveCursor(y, pad + 1);
    std::cout << text << theme::RESET;
}

void TerminalUI::drawCenteredColored(int y, const std::string& text,
                                       const std::string& color, int width) {
    int pad = std::max(0, (width - static_cast<int>(text.length())) / 2);
    moveCursor(y, pad + 1);
    std::cout << color << text << theme::RESET;
}

// ─── ATM Frame ───────────────────────────────────────────────
void TerminalUI::drawATMFrame() {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;

    // Outer ATM body (double line)
    drawDoubleBox(1, 1, w, h, theme::DEEP_BLUE);

    // Inner screen area (rounded)
    drawBox(3, 3, w - 4, h - 8, theme::NEON_BLUE, true);
}

void TerminalUI::drawATMHeader(const std::string& bankName, const std::string& dateTime) {
    int w = theme::ATM_WIDTH;
    // Bank name centered in header
    drawCentered(2, theme::BOLD + theme::NEON_BLUE + "  " + theme::DBOX_H + theme::DBOX_H +
                    " " + bankName + " " + theme::DBOX_H + theme::DBOX_H, w);

    // Date/time on the right inside frame
    moveCursor(4, 5);
    std::cout << theme::GOLD << theme::STAR << " " << theme::WHITE << bankName << theme::RESET;

    moveCursor(4, w - static_cast<int>(dateTime.length()) - 2);
    std::cout << theme::GRAY << dateTime << theme::RESET;

    // Separator
    drawLine(4, 5, w - 6, theme::DARK_GRAY);
}

void TerminalUI::drawATMFooter(const std::string& status) {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;

    // Card slot visual
    moveCursor(h - 4, 5);
    std::cout << theme::DARK_GRAY << theme::BOX_TL;
    for (int i = 0; i < 20; ++i) std::cout << theme::BOX_H;
    std::cout << theme::BOX_TR << theme::GRAY << " Card Slot" << theme::RESET;

    moveCursor(h - 3, 5);
    std::cout << theme::DARK_GRAY << theme::BOX_BL;
    for (int i = 0; i < 20; ++i) std::cout << theme::BOX_H;
    std::cout << theme::BOX_BR << theme::RESET;

    // Cash dispenser
    moveCursor(h - 4, 35);
    std::cout << theme::DARK_GRAY << theme::BOX_TL;
    for (int i = 0; i < 15; ++i) std::cout << theme::BOX_H;
    std::cout << theme::BOX_TR << theme::GRAY << " Cash" << theme::RESET;

    moveCursor(h - 3, 35);
    std::cout << theme::DARK_GRAY << theme::BOX_BL;
    for (int i = 0; i < 15; ++i) std::cout << theme::BOX_H;
    std::cout << theme::BOX_BR << theme::RESET;

    // Status bar at bottom
    if (!status.empty()) {
        moveCursor(h - 1, 3);
        std::cout << theme::DIM << theme::GRAY << " " << status << theme::RESET;
    }
}

void TerminalUI::drawStatusBar(const std::string& message, const std::string& color) {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    moveCursor(h, 2);
    std::cout << (color.empty() ? theme::GRAY : color) << " " << message
              << std::string((size_t)std::max(0, w - static_cast<int>(message.length()) - 4), ' ')
              << theme::RESET;
}

// ─── Input ───────────────────────────────────────────────────
std::string TerminalUI::getInput(const std::string& prompt, int x, int y) {
    if (x > 0 && y > 0) {
        moveCursor(y, x);
    }
    std::cout << theme::CYAN << prompt << theme::WHITE;
    showCursor();
    std::string input;
    std::getline(std::cin, input);
    hideCursor();
    return util::trim(input);
}

std::string TerminalUI::getMaskedInput(const std::string& prompt, int maxLen,
                                        int x, int y)
{
    if (x > 0 && y > 0) {
        moveCursor(y, x);
    }
    std::cout << theme::CYAN << prompt << theme::NEON_GREEN;
    showCursor();

    std::string input;
#ifdef _WIN32
    char ch;
    while (true) {
        ch = static_cast<char>(_getch());
        if (ch == '\r' || ch == '\n') break;
        if (ch == '\b' || ch == 127) {
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        } else if (static_cast<int>(input.length()) < maxLen && std::isdigit(ch)) {
            input += ch;
            std::cout << theme::NEON_GREEN << theme::PIN_DOT << theme::RESET;
        }
    }
#else
    // POSIX terminal raw mode
    struct termios oldT, newT;
    tcgetattr(STDIN_FILENO, &oldT);
    newT = oldT;
    newT.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newT);

    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == '\n' || ch == '\r') break;
        if (ch == 127 || ch == '\b') {
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (static_cast<int>(input.length()) < maxLen && std::isdigit(ch)) {
            input += ch;
            std::cout << theme::NEON_GREEN << theme::PIN_DOT << theme::RESET << std::flush;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
#endif

    std::cout << theme::RESET;
    hideCursor();
    return input;
}

int TerminalUI::getMenuChoice(int min, int max, int x, int y) {
    while (true) {
        std::string input = getInput(theme::ARROW_R + " Choice: ", x, y);
        if (util::isNumeric(input)) {
            int choice = std::stoi(input);
            if (choice >= min && choice <= max) return choice;
        }
        if (x > 0 && y > 0) {
            moveCursor(y, x);
            std::cout << std::string((size_t)30, ' ');
        }
    }
}

char TerminalUI::getKeyPress() {
#ifdef _WIN32
    return static_cast<char>(_getch());
#else
    struct termios oldT, newT;
    tcgetattr(STDIN_FILENO, &oldT);
    newT = oldT;
    newT.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newT);
    char ch;
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
    return ch;
#endif
}

bool TerminalUI::getConfirmation(const std::string& prompt, int x, int y) {
    std::string input = getInput(prompt + " (y/n): ", x, y);
    return !input.empty() && (input[0] == 'y' || input[0] == 'Y');
}

// ─── Tables ──────────────────────────────────────────────────
void TerminalUI::drawTable(int x, int y,
                            const std::vector<std::string>& headers,
                            const std::vector<std::vector<std::string>>& rows,
                            const std::vector<int>& colWidths)
{
    int totalW = 1;
    for (int w : colWidths) totalW += w + 3;

    // Header
    moveCursor(y, x);
    std::cout << theme::NEON_BLUE;
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << " " << util::padRight(headers[i], colWidths[i]) << " " << theme::DARK_GRAY << theme::BOX_V << theme::NEON_BLUE;
    }
    std::cout << theme::RESET;

    // Separator
    moveCursor(y + 1, x);
    std::cout << theme::DARK_GRAY;
    for (size_t i = 0; i < colWidths.size(); ++i) {
        for (int j = 0; j < colWidths[i] + 2; ++j) std::cout << theme::BOX_H;
        if (i < colWidths.size() - 1) std::cout << theme::BOX_CR;
    }
    std::cout << theme::RESET;

    // Rows
    for (size_t r = 0; r < rows.size(); ++r) {
        moveCursor(y + 2 + static_cast<int>(r), x);
        for (size_t c = 0; c < rows[r].size() && c < colWidths.size(); ++c) {
            std::cout << theme::LIGHT_GRAY << " " << util::padRight(rows[r][c], colWidths[c])
                      << " " << theme::DARK_GRAY << theme::BOX_V << theme::RESET;
        }
    }
}

// ─── Progress Bar ────────────────────────────────────────────
void TerminalUI::drawProgressBar(int x, int y, int width, double progress,
                                   const std::string& color)
{
    int filled = static_cast<int>(progress * width);
    moveCursor(y, x);
    std::string c = color.empty() ? theme::NEON_BLUE : color;
    std::cout << theme::DARK_GRAY << "[" << c;
    for (int i = 0; i < width; ++i) {
        std::cout << (i < filled ? theme::BLOCK_FULL : theme::BLOCK_LIGHT);
    }
    std::cout << theme::DARK_GRAY << "] " << theme::WHITE
              << static_cast<int>(progress * 100) << "%" << theme::RESET;
}

// ─── Utility ─────────────────────────────────────────────────
void TerminalUI::sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

int TerminalUI::getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_col;
#endif
}

int TerminalUI::getTerminalHeight() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_row;
#endif
}

void TerminalUI::printLines(const std::vector<std::string>& lines, int startY) {
    for (size_t i = 0; i < lines.size(); ++i) {
        if (startY > 0) {
            moveCursor(startY + static_cast<int>(i), 5);
        }
        std::cout << lines[i] << std::endl;
    }
}

// ─── Keypad ──────────────────────────────────────────────────
void TerminalUI::drawKeypad(int x, int y) {
    std::string border = theme::DARK_GRAY;
    std::string key = theme::NEON_BLUE;
    std::string r = theme::RESET;

    moveCursor(y, x);
    std::cout << border << theme::RBOX_TL;
    for (int i = 0; i < 19; ++i) std::cout << theme::BOX_H;
    std::cout << theme::RBOX_TR << r;

    // Row 1: 1 2 3
    moveCursor(y + 1, x);
    std::cout << border << theme::BOX_V << r << key << "  [1]  [2]  [3]  " << r << border << theme::BOX_V << r;

    // Row 2: 4 5 6
    moveCursor(y + 2, x);
    std::cout << border << theme::BOX_V << r << key << "  [4]  [5]  [6]  " << r << border << theme::BOX_V << r;

    // Row 3: 7 8 9
    moveCursor(y + 3, x);
    std::cout << border << theme::BOX_V << r << key << "  [7]  [8]  [9]  " << r << border << theme::BOX_V << r;

    // Row 4: * 0 #
    moveCursor(y + 4, x);
    std::cout << border << theme::BOX_V << r << theme::SOFT_RED << "  [*]" << r
              << key << "  [0]  " << r << theme::NEON_GREEN << "[#]  " << r
              << border << theme::BOX_V << r;

    moveCursor(y + 5, x);
    std::cout << border << theme::RBOX_BL;
    for (int i = 0; i < 19; ++i) std::cout << theme::BOX_H;
    std::cout << theme::RBOX_BR << r;
}

// ─── Message Boxes ───────────────────────────────────────────
void TerminalUI::showSuccess(const std::string& message) {
    int w = theme::ATM_WIDTH;
    int y = 16;
    drawBox(8, y, w - 14, 3, theme::NEON_GREEN, true);
    moveCursor(y + 1, 10);
    std::cout << theme::NEON_GREEN << " " << theme::CHECK << " " << theme::WHITE << message << theme::RESET;
    sleep_ms(1500);
}

void TerminalUI::showError(const std::string& message) {
    int w = theme::ATM_WIDTH;
    int y = 16;
    drawBox(8, y, w - 14, 3, theme::RED, true);
    moveCursor(y + 1, 10);
    std::cout << theme::RED << " " << theme::CROSS << " " << theme::WHITE << message << theme::RESET;
    sleep_ms(1500);
}

void TerminalUI::showWarning(const std::string& message) {
    int w = theme::ATM_WIDTH;
    int y = 16;
    drawBox(8, y, w - 14, 3, theme::AMBER, true);
    moveCursor(y + 1, 10);
    std::cout << theme::AMBER << " ! " << theme::WHITE << message << theme::RESET;
    sleep_ms(1500);
}

void TerminalUI::showInfo(const std::string& message) {
    int w = theme::ATM_WIDTH;
    int y = 16;
    drawBox(8, y, w - 14, 3, theme::LIGHT_BLUE, true);
    moveCursor(y + 1, 10);
    std::cout << theme::LIGHT_BLUE << " i " << theme::WHITE << message << theme::RESET;
    sleep_ms(1000);
}

void TerminalUI::waitForKey(const std::string& message) {
    int h = theme::ATM_HEIGHT;
    moveCursor(h - 5, 5);
    std::cout << theme::DIM << theme::GRAY << message << theme::RESET;
    showCursor();
    getKeyPress();
    hideCursor();
}

} // namespace atmverse
