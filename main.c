/* Fiver for Game Boy (akin to Wordle).
 * A weekend project by stacksmashing.
 * Contact: twitter.com/ghidraninja
 *          code@stacksmashing.net
 */
#include <gb/gb.h>
#include <gb/drawing.h>
#include <sms/hardware.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <rand.h>
#include <time.h>

#include "decode.h"

void set_box_color_for_letter(uint8_t e);
void set_letter_color_for_letter(uint8_t e);

#define FLASH_DELAY ((uint32_t)250 * CLOCKS_PER_SEC / 1000)

const char *kb[3] = {
"Q W E R T Y U I O P",
" A S D F G H J K L",
"  Z X C V B N M"};

uint8_t kb_coords[3] = {
    10,
    9,
    7
};

uint8_t kb_offsets[3] = {
    0,
    1,
    2
};

uint8_t seeded = 0;
uint8_t kb_x = 0;
uint8_t kb_y = 0;
uint8_t guess_nr;
uint8_t hard = 0;
// current word
char word[6];
// current guess
char guess[6];
// list of all accepted guesses
char guesses[6][5];
// list of evaluation bitmaps for all the letters
// the best value is what counts
#define RIGHT_LETTER_WRONG_PLACE  4
#define RIGHT_LETTER_RIGHT_PLACE  2
#define WRONG_LETTER              1
uint8_t guessed[26];
// evaluation of current word: 
// 0 = wrong letter
// RIGHT_LETTER_RIGHT_PLACE = right letter right place
// RIGHT_LETTER_WRONG_PLACE + count = right letter wrong place, with count
uint8_t evals[6][5]; 

static void waitpaduprepeat() {
    static uint16_t delta = (uint32_t)350 * CLOCKS_PER_SEC / 1000;
    uint8_t j;
    uint16_t start = sys_time;
    while ((j = joypad()) && (uint16_t)(sys_time-start) < delta) ;
    if (j)
        delta = (uint32_t)100 * CLOCKS_PER_SEC / 1000;
    else
        delta = (uint32_t)350 * CLOCKS_PER_SEC / 1000;
}

uint8_t contains(char *str, char c) {
    uint8_t count = 0;
    while(*str) {
        if(*str == c) {
            count++;
        } 
        str++;
    }
    return count;
}

void evaluate_letters(char* guess, uint8_t* eval) {
    for (uint8_t i=0;i<5;i++) {
        char c = guess[i];
        if (word[i] == c) { 
            eval[i] = RIGHT_LETTER_RIGHT_PLACE;
            guessed[c-'A'] = RIGHT_LETTER_RIGHT_PLACE;
        }
        else {
            eval[i] = 0;
            guessed[c-'A'] |= WRONG_LETTER;
        }
    }
    for (uint8_t i=0;i<5;i++) {
        if (!eval[i]) {
            char c = guess[i];
            uint8_t count = contains(word, c);
            if (count) {
                uint8_t already = 0;
                for (uint8_t j=0;j<5;j++) {
                    if (eval[j] && guess[j] == c)
                        already++;
                }
                if (already < count) {
                    uint8_t cg = contains(guess, c);
                    eval[i] = RIGHT_LETTER_WRONG_PLACE + (cg < count ? cg : count);
                    guessed[c-'A'] |= RIGHT_LETTER_WRONG_PLACE;
                }
            }            
        }
    }
}

uint8_t validate_hard(char* prevGuess, uint8_t* prevEval, char* curGuess, uint8_t* curEval) {
    for (uint8_t i=0; i<5; i++) {
        uint8_t e = prevEval[i];
        if (e == RIGHT_LETTER_RIGHT_PLACE) {
            if (curEval[i] != RIGHT_LETTER_RIGHT_PLACE)
                return 0;
        }
        else if (e >= RIGHT_LETTER_WRONG_PLACE) {
            if (contains(curGuess, prevGuess[i]) < e-RIGHT_LETTER_WRONG_PLACE)
                return 0;
        }
    }
    return 1;
}

void draw_word_rect(uint8_t x, uint8_t y, char* guess, uint8_t* eval) {
    uint8_t gx = x/8;
    uint8_t gy = y/8;
    x -= 3;
    y -= 4;
    for(uint8_t i=0; i < 5; i++) {
        if(guess) {
            char letter = guess[i];
            uint8_t e = eval[i];
            set_box_color_for_letter(e);
            box(x, y, x+14, y+14, M_FILL);
            set_letter_color_for_letter(e);
            gotogxy(gx, gy);
            wrtchr(letter);
            gx += 2;
        } else {
            color(BLACK, WHITE, M_NOFILL);
            box(x, y, x+14, y+14, M_NOFILL);
        }
        
        x += 16;
    }
}




void set_box_color_for_letter(uint8_t e) {
    if(e == RIGHT_LETTER_RIGHT_PLACE) {
        color(BLACK, BLACK, M_FILL);
    } else if(e >= RIGHT_LETTER_WRONG_PLACE) {
        color(BLACK, DKGREY, M_FILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
}

void set_letter_color_for_letter(uint8_t e) {
    if(e == RIGHT_LETTER_RIGHT_PLACE) {
        color(WHITE, BLACK, M_NOFILL);
    } else if(e >= RIGHT_LETTER_WRONG_PLACE) {
        color(WHITE, DKGREY, M_NOFILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
}


void set_color_for_letter(char letter) {
    if(letter == ' ') {
        color(BLACK, WHITE, M_NOFILL);
    }
    else {
        uint8_t g = guessed[letter-'A'];
        if (g & RIGHT_LETTER_RIGHT_PLACE) {
            color(DKGREY, WHITE, M_NOFILL);
        } else if(g & RIGHT_LETTER_WRONG_PLACE) {
            color(DKGREY, WHITE, M_NOFILL);
        } else if(g & WRONG_LETTER) {
            color(LTGREY, WHITE, M_NOFILL);
        } else {
            color(BLACK, WHITE, M_NOFILL);
        }
    }
}

int kb_vert_offset = 15;

void erase_keyboard() {
    color(WHITE, WHITE, SOLID);
    box(0, kb_vert_offset*8-1, 160, 144, M_FILL);
}

void draw_keyboard(uint8_t x, uint8_t y) {
    for(uint8_t i=0; i < 3; i++) {
        gotogxy(x, y + i);
        const char* k = kb[i];
        char letter;
        while ((letter = *k++)) {
            set_color_for_letter(letter);
            wrtchr(letter);
        }
    }
}

void highlight_key() {
    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8);
    uint8_t y = (kb_vert_offset + kb_y) * 8;
    color(BLACK, WHITE, M_NOFILL);
    box(x, y-1, x+8, y+7, M_NOFILL);
}

void dehighlight_key() {
    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8);
    uint8_t y = (kb_vert_offset + kb_y) * 8;
    color(WHITE, WHITE, M_NOFILL);
    box(x, y-1, x+8, y+7, M_NOFILL);

    uint8_t gx = (kb_x * 2) + (kb_offsets[kb_y]);
    uint8_t gy = (kb_vert_offset + kb_y);
    gotogxy(gx, gy);
    char letter = kb[kb_y][kb_offsets[kb_y]+ (kb_x*2)];
    set_color_for_letter(letter);
    wrtchr(letter);
}

char getletter() {
    return kb[kb_y][kb_offsets[kb_y] + (kb_x*2)];
}

void show_answer() {
    erase_keyboard();
    const uint8_t line = 2 + (6 * 2);
    uint8_t x = 5;
    for(uint8_t i=0; i < 5; i++) {
        color(BLACK, WHITE, M_NOFILL);
        gotogxy(x, line);
        wrtchr(word[i]);

        x += 2;
    }
    waitpad(J_START | J_A);
    reset();
}

void render_guess(uint8_t final) {
    // first box is at 5, 2
    uint8_t line = 2 + (guess_nr * 2);
    uint8_t x = 5;
    for(uint8_t i=0; i < 5; i++) {
        color(BLACK, final ? LTGREY : WHITE, M_NOFILL);
        gotogxy(x, line);
        if(guess[i] != 0) {
            wrtchr(guess[i]);
        } else {
            wrtchr(' ');
        }

        x += 2;
    }
}

void draw_hard() {
    color(WHITE, DKGREY, M_FILL);
    gotogxy(19,17);
    wrtchr(hard ? 'H' : 'E');
}

void draw_board() {
    for(uint8_t i=0; i < 6; i++) {
        char *g = NULL;
        uint8_t *e = NULL;
        if(i < guess_nr) {
            g = guesses[i];
            e = evals[i];
        }
        draw_word_rect(40, 16+(i*16), g, e);
    }
}


void show_win() {
    erase_keyboard();
    gotogxy(5, 14);
    color(BLACK, WHITE, M_NOFILL);
    gprint("You won!!!");
    gotogxy(3, 16);
    wrtchr('0'+guess_nr);
    gprint("/6 - Congrats!");
    waitpad(J_START | J_A);
    reset();
}


void run_fiver(void)
{
    strcpy(word, "EMPTY");
    uint8_t has_random = 0;
    
    guess_nr = 0;
    memset(guess, 0, sizeof(guess));
    memset(guessed, 0, sizeof(guessed));
    memset(guesses, 0, sizeof(guesses));
    memset(evals, 0, sizeof(evals));

    for(uint8_t i=0; i < 6; i++) {
        draw_word_rect(40, 16+(i*16), NULL, NULL);
    }

    gotogxy(2, 0);
    gprint("Game Boy  FIVER");
    draw_keyboard(0, kb_vert_offset);
    draw_hard();
    
    while (joypad());
    
    color(LTGREY, WHITE, M_NOFILL);
    highlight_key();
    while(1) {
        uint8_t j = joypad();

        switch(j) {
            case J_RIGHT:
                dehighlight_key();
                kb_x += 1;
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = 0;
                }
                highlight_key();
                waitpaduprepeat();
                break;
            case J_LEFT:
                dehighlight_key();
                if (kb_x == 0) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                else {
                    kb_x -= 1;
                }
                highlight_key();
                waitpaduprepeat();
                break;
            case J_UP:
                dehighlight_key();
                if (kb_y == 0) {
                    kb_y = 2;
                }
                else {
                    kb_y -= 1;
                }
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                highlight_key();
                waitpaduprepeat();
                break;
            case J_DOWN:
                dehighlight_key();
                kb_y += 1;
                if(kb_y > 2) {
                    kb_y = 0;
                }
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                highlight_key();
                waitpaduprepeat();
                break;
            case J_SELECT: 
                hard = !hard;
                draw_hard();
                waitpaduprepeat();
                break;
            case J_START: {
                if(strlen(guess) != 5) break;
                render_guess(1);
                uint8_t valid = 1;
                uint16_t start = sys_time;
                evaluate_letters(guess, evals[guess_nr]);
                if(hard) {
                    uint8_t i;
                    for (i=0; i<guess_nr; i++)
                        if (!validate_hard(guesses[i], evals[i], guess, evals[guess_nr])) {
                            valid = 0;
                            break;
                        }
                }
                valid = valid && filterWord(guess);
                while ((uint16_t)(sys_time-start) < FLASH_DELAY) ;
                
                if (!valid) {
                    guess[4] = 0;
                    render_guess(0);
                    break;
                }
                strcpy(guesses[guess_nr], guess);
                guess_nr += 1;
                draw_board();
                draw_keyboard(0, kb_vert_offset);
                highlight_key();
                if(strcmp(word, guess) == 0) {
                    show_win();
                    return;
                    break;
                }
                if(guess_nr == 6) {
                    show_answer();
                    return;
                    break;
                }
                // empty guess
                memset(guess, 0, 5);
                // TODO
                break;
            }
            case J_A:
                if(strlen(guess) == 5) break;
                guess[strlen(guess)] = getletter();
                render_guess(0);
                if((has_random == 0)) {
                    if (! seeded) {
                        initrand((uint16_t)LY_REG | ((uint16_t)DIV_REG << 8));
                        seeded = 1;
                    }
                    
                    uint16_t r;
                    do {
                        r = randw() & (NUM_ANSWERS_ROUNDED_UP_POW2-1);
                    }
                    while(r >= NUM_ANSWERS);
                    getSpecialWord(r, word);
                    has_random = 1;
                }
                waitpaduprepeat();
                break;
            case J_B:
                if(strlen(guess) == 0) break;
                guess[strlen(guess)-1] = 0;
                render_guess(0);
                waitpaduprepeat();
                break;
            default:
            break;
        }

        wait_vbl_done();
    }
}

void main() {
    while(1) {
        run_fiver();    
    }
}
