#pragma once
#include <string>

namespace atmverse {
namespace theme {

// ─── ANSI Color Codes ────────────────────────────────────────
// Reset
const std::string RESET       = "\033[0m";

// Styles
const std::string BOLD        = "\033[1m";
const std::string DIM         = "\033[2m";
const std::string ITALIC      = "\033[3m";
const std::string UNDERLINE   = "\033[4m";
const std::string BLINK       = "\033[5m";
const std::string REVERSE     = "\033[7m";

// ─── Neon Blue ATM Theme ─────────────────────────────────────
// Primary colors (256-color mode)
const std::string NEON_BLUE    = "\033[38;5;39m";
const std::string DEEP_BLUE    = "\033[38;5;27m";
const std::string LIGHT_BLUE   = "\033[38;5;117m";
const std::string CYAN         = "\033[38;5;51m";
const std::string TEAL         = "\033[38;5;44m";

// Accent colors
const std::string NEON_GREEN   = "\033[38;5;46m";
const std::string GREEN        = "\033[38;5;34m";
const std::string SOFT_GREEN   = "\033[38;5;114m";
const std::string GOLD         = "\033[38;5;220m";
const std::string AMBER        = "\033[38;5;214m";
const std::string RED          = "\033[38;5;196m";
const std::string SOFT_RED     = "\033[38;5;203m";
const std::string ORANGE       = "\033[38;5;208m";
const std::string MAGENTA      = "\033[38;5;199m";
const std::string PURPLE       = "\033[38;5;141m";

// Neutral colors
const std::string WHITE        = "\033[1;97m";
const std::string LIGHT_GRAY   = "\033[38;5;250m";
const std::string GRAY         = "\033[38;5;245m";
const std::string DARK_GRAY    = "\033[38;5;240m";
const std::string DARKER_GRAY  = "\033[38;5;236m";

// Background colors
const std::string BG_DARK      = "\033[48;5;234m";
const std::string BG_DARKER    = "\033[48;5;232m";
const std::string BG_BLUE      = "\033[48;5;17m";
const std::string BG_DEEP_BLUE = "\033[48;5;18m";
const std::string BG_GREEN     = "\033[48;5;22m";
const std::string BG_RED       = "\033[48;5;52m";

// ─── Unicode Box Drawing ─────────────────────────────────────
// Single line
const std::string BOX_H  = "\xe2\x94\x80"; // ─
const std::string BOX_V  = "\xe2\x94\x82"; // │
const std::string BOX_TL = "\xe2\x94\x8c"; // ┌
const std::string BOX_TR = "\xe2\x94\x90"; // ┐
const std::string BOX_BL = "\xe2\x94\x94"; // └
const std::string BOX_BR = "\xe2\x94\x98"; // ┘
const std::string BOX_LT = "\xe2\x94\x9c"; // ├
const std::string BOX_RT = "\xe2\x94\xa4"; // ┤
const std::string BOX_TT = "\xe2\x94\xac"; // ┬
const std::string BOX_BT = "\xe2\x94\xb4"; // ┴
const std::string BOX_CR = "\xe2\x94\xbc"; // ┼

// Double line
const std::string DBOX_H  = "\xe2\x95\x90"; // ═
const std::string DBOX_V  = "\xe2\x95\x91"; // ║
const std::string DBOX_TL = "\xe2\x95\x94"; // ╔
const std::string DBOX_TR = "\xe2\x95\x97"; // ╗
const std::string DBOX_BL = "\xe2\x95\x9a"; // ╚
const std::string DBOX_BR = "\xe2\x95\x9d"; // ╝
const std::string DBOX_LT = "\xe2\x95\xa0"; // ╠
const std::string DBOX_RT = "\xe2\x95\xa3"; // ╣

// Rounded corners
const std::string RBOX_TL = "\xe2\x95\xad"; // ╭
const std::string RBOX_TR = "\xe2\x95\xae"; // ╮
const std::string RBOX_BL = "\xe2\x95\xb0"; // ╰
const std::string RBOX_BR = "\xe2\x95\xaf"; // ╯

// ─── Special Characters ──────────────────────────────────────
const std::string BLOCK_FULL  = "\xe2\x96\x88"; // █
const std::string BLOCK_LIGHT = "\xe2\x96\x91"; // ░
const std::string BLOCK_MED   = "\xe2\x96\x92"; // ▒
const std::string BLOCK_DARK  = "\xe2\x96\x93"; // ▓
const std::string BULLET      = "\xe2\x97\x8f"; // ●
const std::string CIRCLE      = "\xe2\x97\x8b"; // ○
const std::string DIAMOND     = "\xe2\x97\x86"; // ◆
const std::string ARROW_R     = "\xe2\x96\xb6"; // ▶
const std::string ARROW_L     = "\xe2\x97\x80"; // ◀
const std::string CHECK       = "\xe2\x9c\x93"; // ✓
const std::string CROSS       = "\xe2\x9c\x97"; // ✗
const std::string STAR        = "\xe2\x98\x85"; // ★
const std::string RUPEE       = "\xe2\x82\xb9"; // ₹
const std::string PIN_DOT     = "\xe2\x97\x8f"; // ● (for PIN masking)

// ─── ATM Dimensions ──────────────────────────────────────────
const int ATM_WIDTH = 60;
const int ATM_HEIGHT = 30;
const int INNER_WIDTH = ATM_WIDTH - 4;

} // namespace theme
} // namespace atmverse
