// gfx_common.h

#ifndef _GFX_COMMON_H
#define _GFX_COMMON_H

// Sprite Data
#include "../res/letter_cursor_tiles.h"

// Map Data
//#include "../res/board_grid_tiles_gbcompress.h"
#include "../res/font_tiles_1bpp_gbcompress.h"
#include "../res/font_num_tiles_1bpp_gbcompressed.h"
#include "../res/board_letter_tiles_8x8_1bpp_gbcomp.h"

// Window / Dialog data
#include "../res/dialog_tiles_2bpp_gbcompressed.h"


#define PRINT_BKG 0
#define PRINT_WIN 1


extern const uint8_t tile_blank[];

void print_gotoxy(uint8_t x, uint8_t y, uint8_t target);
void print_str(char * txt);

void draw_letter_to_tileid(uint8_t letter, uint8_t index);



// Hardware correction factor for X position of Window when displayed on screen
#define WIN_X_OFFSET 7

#define SP_TILES_CURSOR_START 0u
#define SP_TILES_CURSOR_LEN   1u

#define SP_ID_CURSOR_START 0u
#define SP_ID_CURSOR_LEN   4u


// TODO: could move to keybaord.h
#define KEYBD_START_X     0u // Starting Tile Col of Keyboard
#define KEYBD_START_Y    14u // Starting Tile Row of Keyboard


// TODO: COUNT needs to be BOARD SIZE IN TILES (5 * 6 * 4)
#define BOARD_LETTERS_BYTES_PER_TILE  8u // 1bpp tiles = 8 bytes per 8x8 tile
#define BOARD_LETTERS_ALPHA           26u
#define BOARD_LETTERS_BLANK           1u
#define BOARD_LETTERS_FLIP            3u
#define BOARD_LETTERS_COUNT          (BOARD_LETTERS_ALPHA + BOARD_LETTERS_BLANK + BOARD_LETTERS_FLIP) // Last entry is blank space
#define BOARD_LETTERS_TILES_PER       4u // Last entry is blank space
// Empty space is last char in board font tile set
#define BOARD_LETTERS_SPACE_CHAR (BOARD_LETTERS_COUNT - (BOARD_LETTERS_BLANK + BOARD_LETTERS_FLIP))
#define BOARD_LETTERS_FLIP_1     (BOARD_LETTERS_COUNT - BOARD_LETTERS_FLIP)
#define BOARD_LETTERS_FLIP_2     (BOARD_LETTERS_FLIP_1 + 1)
#define BOARD_LETTERS_FLIP_3     (BOARD_LETTERS_FLIP_2 + 1)


#define FONT_LETTERS_COUNT           29u // 26 letters + 3 special chars: ".", "!", ":"
#define FONT_LETTERS_BYTES_PER_TILE   8u // 1bpp tiles = 8 bytes per 8x8 tile

extern uint8_t board_letters_decomp_buf[];
extern uint8_t font_letters_decomp_buf[];


#define BOARD_GRID_X  4u // Start x,y in Tiles
#define BOARD_GRID_Y  1u
#define BOARD_GRID_W  (WORD_LENGTH) // from common.h // 5u // Size in Tiles
#define BOARD_GRID_H  (MAX_GUESSES) // from common.h
#define BOARD_TILE_W  2u
#define BOARD_TILE_H  2u
#define BOARD_GRID_TILES_PER_LETTER 4u
#define BOARD_GRID_VRAM_BYTES_PER_LETTER (BOARD_LETTERS_BYTES_PER_TILE * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILES_PER_ROW    (BOARD_GRID_W * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILE_W (BOARD_GRID_W * BOARD_TILE_W)
#define BOARD_GRID_TILE_H (BOARD_GRID_H * BOARD_TILE_H)

#define BOARD_TILE_X_START (BOARD_GRID_X)
#define BOARD_TILE_Y_START (BOARD_GRID_Y)


// ==== VRAM tile assignment for Gameplay Screen ====
#define BG_TILES_BLANK_START 0u
#define BG_TILES_BLANK_LEN   1u

#define BG_TILES_KEYBD_START  (BG_TILES_BLANK_START + BG_TILES_BLANK_LEN)
#define BG_TILES_KEYBD_LEN    26u // 26 letters

#define BG_TILES_BOARD_LETTERS_START (BG_TILES_KEYBD_START + BG_TILES_KEYBD_LEN)
#define BG_TILES_BOARD_LETTERS_LEN   (BOARD_GRID_W * BOARD_GRID_H * BOARD_GRID_TILES_PER_LETTER)

#define BG_TILES_DIALOG_START (BG_TILES_BOARD_LETTERS_START + BG_TILES_BOARD_LETTERS_LEN)
#define BG_TILES_DIALOG_LEN   5u

#define BG_TILES_FONT_START   (BG_TILES_DIALOG_START + BG_TILES_DIALOG_LEN)
#define BG_TILES_FONT_LEN     29u // 26 letters + 3 special chars: ".", "!", ":"
#define BG_TILES_FONT_PERIOD  (BG_TILES_FONT_START + 26u)
#define BG_TILES_FONT_EXCLAIM (BG_TILES_FONT_START + 27u)
#define BG_TILES_FONT_COLON   (BG_TILES_FONT_START + 28u)

#define BG_TILES_FONT_NUM_START   (BG_TILES_FONT_START + BG_TILES_FONT_LEN)
#define BG_TILES_FONT_NUM_LEN     10u // 10 digits


#define DIALOG_TILE_L    4u
#define DIALOG_TILE_UL   0u
#define DIALOG_TILE_R    3u
#define DIALOG_TILE_UR   2u
#define DIALOG_TILE_TOP  1u



/*
// Board Letter box colors: Foreground, Background
// Standard color scheme
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY) // Filled in box text
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)     // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)      // Full contrast text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text
*/

/*
// Board Letter box colors: Foreground, Background
// Alt color scheme #1 (lighter board, less inversion)
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_WHITE)      // Full contrast text
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)  // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_WHITE)      // Full contrast text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text
*/

// Board Letter box colors: Foreground, Background
// Alt color scheme #2 (lighter board, less inversion)
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)      // Full contrast text#
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Inverted text
// #define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_DARK_GRAY, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)      // Full contrast text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
// #define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text

/*
// Board Letter box colors: Foreground, Background
// Alt color scheme #3 (lighter board, less inversion)
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Faded text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)      // Full contrast text
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
// #define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)      // Full contrast text
//#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text

*/


#endif