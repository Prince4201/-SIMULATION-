#include "atmverse/ui/Animations.h"
#include "atmverse/ui/TerminalUI.h"
#include "atmverse/ui/Themes.h"
#include "atmverse/utils/Utility.h"
#include <iostream>

namespace atmverse {

void Animations::cardInsertAnimation() {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    int slotX = 5;
    int slotY = h - 4;

    std::string cardArt = "\033[38;5;250m" + theme::BOX_TL + std::string((size_t)10, theme::BOX_H[0]) + theme::BOX_TR + "\033[0m";

    for (int i = 0; i < 5; ++i) {
        TerminalUI::moveCursor(slotY - 4 + i, slotX + 4);
        std::cout << cardArt << std::flush;
        TerminalUI::sleep_ms(200);
        TerminalUI::moveCursor(slotY - 4 + i, slotX + 4);
        std::cout << std::string((size_t)12, ' ') << std::flush;
    }
}

void Animations::cardReadingAnimation() {
    int w = theme::ATM_WIDTH;
    TerminalUI::drawCenteredColored(12, "READING CARD DATA...", theme::CYAN, w);
    TerminalUI::drawBox(w/2 - 12, 14, 24, 3, theme::DARK_GRAY, true);

    for (int i = 0; i <= 20; ++i) {
        TerminalUI::drawProgressBar(w/2 - 10, 15, 20, i / 20.0, theme::NEON_GREEN);
        TerminalUI::sleep_ms(100);
    }
    TerminalUI::sleep_ms(500);
}

void Animations::cardEjectAnimation() {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    int slotX = 5;
    int slotY = h - 4;

    std::string cardArt = "\033[38;5;250m" + theme::BOX_TL + std::string((size_t)10, theme::BOX_H[0]) + theme::BOX_TR + "\033[0m";
    std::string cardBody = "\033[38;5;250m" + theme::BOX_V + "  VISA    " + theme::BOX_V + "\033[0m";

    for (int i = 0; i < 4; ++i) {
        TerminalUI::moveCursor(slotY - i, slotX + 4);
        std::cout << cardArt << std::flush;
        if (i > 0) {
            TerminalUI::moveCursor(slotY - i + 1, slotX + 4);
            std::cout << cardBody << std::flush;
        }
        TerminalUI::sleep_ms(200);
    }
    TerminalUI::sleep_ms(1000);
    for (int i = 0; i < 4; ++i) {
        TerminalUI::moveCursor(slotY - 3 + i, slotX + 4);
        std::cout << std::string((size_t)12, ' ') << std::flush;
        if (i < 3) {
            TerminalUI::moveCursor(slotY - 2 + i, slotX + 4);
            std::cout << std::string((size_t)12, ' ') << std::flush;
        }
    }
}

void Animations::cashCountAnimation(const std::map<int, int>& notes) {
    int w = theme::ATM_WIDTH;
    int totalNotes = 0;
    for (auto& n : notes) totalNotes += n.second;

    TerminalUI::drawCenteredColored(15, "COUNTING CASH...", theme::AMBER, w);

    for (int i = 0; i < totalNotes; ++i) {
        TerminalUI::drawCenteredColored(17, "*click* *whir*", theme::GRAY, w);
        TerminalUI::sleep_ms(100);
        TerminalUI::drawCenteredColored(17, " *click* *whir* ", theme::GRAY, w);
        TerminalUI::sleep_ms(100);
    }
    TerminalUI::drawCenteredColored(17, std::string((size_t)20, ' '), theme::GRAY, w);
}

void Animations::cashDispenseAnimation(double amount) {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    int cashX = 35;
    int cashY = h - 4;

    std::string note = "\033[38;5;114m" + theme::DBOX_TL + std::string((size_t)10, theme::DBOX_H[0]) + theme::DBOX_TR + "\033[0m";
    std::string noteBody = "\033[38;5;114m" + theme::DBOX_V + "  \xe2\x82\xb9      " + theme::DBOX_V + "\033[0m";

    for (int i = 0; i < 3; ++i) {
        TerminalUI::moveCursor(cashY - i, cashX + 2);
        std::cout << note << std::flush;
        if (i > 0) {
            TerminalUI::moveCursor(cashY - i + 1, cashX + 2);
            std::cout << noteBody << std::flush;
        }
        TerminalUI::sleep_ms(300);
    }
    TerminalUI::sleep_ms(1000);
    for (int i = 0; i < 3; ++i) {
        TerminalUI::moveCursor(cashY - 2 + i, cashX + 2);
        std::cout << std::string((size_t)12, ' ') << std::flush;
        if (i < 2) {
            TerminalUI::moveCursor(cashY - 1 + i, cashX + 2);
            std::cout << std::string((size_t)12, ' ') << std::flush;
        }
    }
}

void Animations::depositAnimation(double amount) {
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    int cashX = 35;
    int cashY = h - 4;

    std::string note = "\033[38;5;114m" + theme::DBOX_TL + std::string((size_t)10, theme::DBOX_H[0]) + theme::DBOX_TR + "\033[0m";
    std::string noteBody = "\033[38;5;114m" + theme::DBOX_V + "  \xe2\x82\xb9      " + theme::DBOX_V + "\033[0m";

    for (int i = 2; i >= 0; --i) {
        TerminalUI::moveCursor(cashY - i, cashX + 2);
        std::cout << note << std::flush;
        if (i > 0) {
            TerminalUI::moveCursor(cashY - i + 1, cashX + 2);
            std::cout << noteBody << std::flush;
        }
        TerminalUI::sleep_ms(300);
        TerminalUI::moveCursor(cashY - i, cashX + 2);
        std::cout << std::string((size_t)12, ' ') << std::flush;
        if (i > 0) {
            TerminalUI::moveCursor(cashY - i + 1, cashX + 2);
            std::cout << std::string((size_t)12, ' ') << std::flush;
        }
    }
}

void Animations::receiptPrintAnimation(const std::vector<std::string>& receipt) {
    int w = theme::ATM_WIDTH;
    TerminalUI::clearScreen();
    TerminalUI::drawATMFrame();
    TerminalUI::drawATMHeader("ATMVerse", util::getCurrentDateTime());
    TerminalUI::drawATMFooter("Printing receipt...");

    int startY = 6;
    for (size_t i = 0; i < receipt.size(); ++i) {
        TerminalUI::moveCursor(startY + i, 5);
        std::cout << receipt[i] << std::flush;
        TerminalUI::sleep_ms(150);
    }
    TerminalUI::waitForKey("Take your receipt and press any key...");
}

void Animations::loadingSpinner(const std::string& message, int durationMs) {
    std::string frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int w = theme::ATM_WIDTH;
    int iterations = durationMs / 100;
    for (int i = 0; i < iterations; ++i) {
        TerminalUI::drawCenteredColored(15, theme::CYAN + frames[i % 10] + " " + theme::WHITE + message, theme::WHITE, w);
        TerminalUI::sleep_ms(100);
    }
    TerminalUI::drawCentered(15, std::string((size_t)(message.length() + 3), ' '), w);
}

void Animations::processingDots(const std::string& message, int durationMs) {
    int w = theme::ATM_WIDTH;
    int iterations = durationMs / 400;
    for (int i = 0; i < iterations; ++i) {
        TerminalUI::drawCenteredColored(15, message + ".", theme::CYAN, w);
        TerminalUI::sleep_ms(400);
        TerminalUI::drawCenteredColored(15, message + "..", theme::CYAN, w);
        TerminalUI::sleep_ms(400);
        TerminalUI::drawCenteredColored(15, message + "...", theme::CYAN, w);
        TerminalUI::sleep_ms(400);
    }
    TerminalUI::drawCentered(15, std::string((size_t)(message.length() + 4), ' '), w);
}

void Animations::progressBarAnimation(const std::string& label, int durationMs) {
    int w = theme::ATM_WIDTH;
    TerminalUI::drawCenteredColored(14, label, theme::WHITE, w);
    TerminalUI::drawBox(w/2 - 12, 16, 24, 3, theme::DARK_GRAY, true);
    int steps = 20;
    int delay = durationMs / steps;
    for (int i = 0; i <= steps; ++i) {
        TerminalUI::drawProgressBar(w/2 - 10, 17, 20, (double)i / steps, theme::CYAN);
        TerminalUI::sleep_ms(delay);
    }
    TerminalUI::sleep_ms(300);
}

void Animations::bootAnimation() {
    TerminalUI::clearScreen();
    int w = theme::ATM_WIDTH;
    int h = theme::ATM_HEIGHT;
    TerminalUI::drawDoubleBox(1, 1, w, h, theme::DARK_GRAY);
    TerminalUI::sleep_ms(500);

    TerminalUI::drawCenteredColored(h/2 - 2, "ATMVerse OS v1.0", theme::CYAN, w);
    TerminalUI::sleep_ms(800);

    TerminalUI::drawCenteredColored(h/2, "Initializing Hardware...", theme::GRAY, w);
    TerminalUI::sleep_ms(800);
    TerminalUI::drawCenteredColored(h/2, "Loading Secure Modules... ", theme::GRAY, w);
    TerminalUI::sleep_ms(800);
    TerminalUI::drawCenteredColored(h/2, "Connecting to Bank Server...", theme::GRAY, w);
    TerminalUI::sleep_ms(800);
    TerminalUI::drawCenteredColored(h/2, "System Ready.               ", theme::NEON_GREEN, w);
    TerminalUI::sleep_ms(1000);
}

void Animations::welcomeGlow() {
    int w = theme::ATM_WIDTH;
    TerminalUI::drawCenteredColored(12, "W E L C O M E", theme::DEEP_BLUE, w);
    TerminalUI::sleep_ms(150);
    TerminalUI::drawCenteredColored(12, "W E L C O M E", theme::NEON_BLUE, w);
    TerminalUI::sleep_ms(150);
    TerminalUI::drawCenteredColored(12, "W E L C O M E", theme::CYAN, w);
    TerminalUI::sleep_ms(150);
    TerminalUI::drawCenteredColored(12, "W E L C O M E", theme::WHITE, w);
}

void Animations::typewriter(const std::string& text, int x, int y, int delayMs) {
    TerminalUI::moveCursor(y, x);
    std::cout << theme::WHITE;
    for (char c : text) {
        std::cout << c << std::flush;
        TerminalUI::sleep_ms(delayMs);
    }
    std::cout << theme::RESET;
}

void Animations::successAnimation(const std::string& message) {
    int w = theme::ATM_WIDTH;
    TerminalUI::drawCenteredColored(15, theme::CHECK + " " + message, theme::NEON_GREEN, w);
    TerminalUI::sleep_ms(2000);
}

void Animations::errorAnimation(const std::string& message) {
    int w = theme::ATM_WIDTH;
    for (int i = 0; i < 3; ++i) {
        TerminalUI::drawCenteredColored(15, theme::CROSS + " " + message, theme::RED, w);
        TerminalUI::sleep_ms(300);
        TerminalUI::drawCenteredColored(15, std::string((size_t)(message.length() + 2), ' '), theme::RED, w);
        TerminalUI::sleep_ms(300);
    }
    TerminalUI::drawCenteredColored(15, theme::CROSS + " " + message, theme::RED, w);
    TerminalUI::sleep_ms(1000);
}

void Animations::transferAnimation() {
    int w = theme::ATM_WIDTH;
    TerminalUI::drawCenteredColored(15, "Transferring Funds", theme::CYAN, w);
    for (int i = 0; i < 5; ++i) {
        TerminalUI::drawCenteredColored(17, ">>>", theme::NEON_GREEN, w);
        TerminalUI::sleep_ms(200);
        TerminalUI::drawCenteredColored(17, "   ", theme::NEON_GREEN, w);
        TerminalUI::sleep_ms(200);
    }
}

} // namespace atmverse
