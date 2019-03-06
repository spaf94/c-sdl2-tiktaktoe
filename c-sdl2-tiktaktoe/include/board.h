#ifndef BOARD_H
#define BOARD_H

/******************************************************************************/

#include "internal.h"
#include "engine.h"

/******************************************************************************/

typedef enum
{
    BOARD_DIRECTION_UP,
    BOARD_DIRECTION_DOWN,
    BOARD_DIRECTION_RIGHT,
    BOARD_DIRECTION_LEFT
} board_direction_t;

typedef struct
{
    // SDL
    SDL_Renderer *renderer;
    TTF_Font *font;
    // Aux data
    int window_w;
    int window_h;
    // Board
    engine_t *engine;
    int timer;
    bool playing;
    bool blinking;
    bool playerX;
    // Board rectangles
    SDL_Rect rect_arr[GAME_BOARD_DIVS][GAME_BOARD_DIVS];
    engine_move_t play_data;
} board_t;

/******************************************************************************/

/**
* @brief Create a board context
* @param renderer   game renderer
* @param font       game font
* @param wh         game window heigth
* @param ww         game window width
* @return board context
*/
board_t *board_new( SDL_Renderer *renderer, TTF_Font *font, int wh, int ww );

/******************************************************************************/

/**
* @brief Release board context
* @param board   board context
*/
void board_free( board_t *board );

/******************************************************************************/

/**
* @brief Render the board
* @param board   board context
*/
void board_render( board_t *board );

/******************************************************************************/

void board_player_move( board_t *board, board_direction_t direction );

/******************************************************************************/

#endif // BOARD_H
