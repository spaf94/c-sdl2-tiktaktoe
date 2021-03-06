/******************************************************************************/

#include "tictactoe.h"

/******************************************************************************/

#define WINDOW_TITLE        "Tic-Tac-Toe"
#define WINDOW_WIDTH        640
#define WINDOW_HEIGHT       480
#define CRAZY_TTF_FILE      "../c-sdl2-tictactoe/fonts/crazy.ttf"
#define MOUSE_DOUBLE_CLICK  2

/******************************************************************************/

/**
* @brief Initialize SDL data
* @return game context
*/
static
void _tictactoe_sdl_data_init( tictactoe_t *ttt )
{
    // SDL compiled version
    SDL_version compiled = { 0 };
    SDL_VERSION( &compiled );
    SDL_Log( "SDL compiled version %d.%d.%d \n",
             compiled.major, compiled.minor, compiled.patch );
    // SDL linked version
    SDL_version linked = { 0 };
    SDL_GetVersion( &linked );
    SDL_Log( "SDL linked version %d.%d.%d \n",
             linked.major, linked.minor, linked.patch );

    // Create game window
    ttt->window = SDL_CreateWindow( WINDOW_TITLE,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT,
                                    0 );
    // Create game renderer
    uint32_t flags = (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ttt->renderer = SDL_CreateRenderer( ttt->window, -1, flags );
    // Load font
    ttt->font = TTF_OpenFont( CRAZY_TTF_FILE, 24 );
    if ( ttt->font == NULL )
        SDL_Log( "OPEN FONT FAILED! \n" );
}

/******************************************************************************/

/**
* @brief On board closing
* @param quit   true, when is quit
* @param cb_arg callback argument
*/
static
void _tictactoe_board_closing_cb( bool quit, void *cb_arg )
{
    tictactoe_t *ttt = cb_arg;
    if ( quit )
    {
        // Force menu redraw
        menu_drawn_set( ttt->menu, false );
        ttt->play_mode = PLAY_MODE_NONE;
    }

    // Restart board
    board_restart( ttt->board );
}

/******************************************************************************/

/**
* @brief Create game context
* @return game context
*/
tictactoe_t *tictactoe_new()
{
    tictactoe_t *ttt = calloc( 1, sizeof (*ttt) );
    ttt->running = true;
    ttt->play_mode = PLAY_MODE_NONE;
    // Init SDL data
    _tictactoe_sdl_data_init( ttt );
    // Init menu
    ttt->menu = menu_new(
                ttt->renderer, ttt->font, WINDOW_HEIGHT, WINDOW_WIDTH );
    ttt->board = board_new(
                ttt->renderer,
                ttt->font,
                _tictactoe_board_closing_cb,
                ttt );

    return ttt;
}

/******************************************************************************/

/**
* @brief Release game resources
* @param ttt    game context
*/
void tictactoe_free( tictactoe_t *ttt )
{
    if ( ttt != NULL )
    {
        if ( ttt->menu != NULL )
            menu_free( ttt->menu );
        if ( ttt->board != NULL )
            board_free( ttt->board );

        // Release SDL resources
        if ( ttt->renderer != NULL )
            SDL_DestroyRenderer( ttt->renderer );
        if ( ttt->font != NULL )
            TTF_CloseFont( ttt->font );
        if ( ttt->window != NULL )
            SDL_DestroyWindow( ttt->window );

        free( ttt );
    }
}

/******************************************************************************/

/**
* @brief Render the game
* @param ttt    game context
*/
void tictactoe_render( tictactoe_t *ttt )
{
    const bool menu = (ttt->play_mode == PLAY_MODE_NONE);
    if ( menu )
        menu_render( ttt->menu );
    else
        board_render( ttt->board );

    SDL_RenderPresent( ttt->renderer );
}

/******************************************************************************/

/**
 * @brief Handling ENTER when are in menu mode
 * @param ttt   Game context
 */
static
void _tictactoe_play_mode_none_handle( tictactoe_t *ttt )
{
    const menu_option_t op = menu_option_get( ttt->menu );
    switch ( op )
    {
    case MENU_OPTION_NONE:
        ttt->play_mode = PLAY_MODE_NONE;
        break;
    case MENU_OPTION_1VS1:
        ttt->play_mode = PLAY_MODE_1VS1;
        break;
    case MENU_OPTION_1VSCOM:
        ttt->play_mode = PLAY_MODE_1VSCOM;
        break;
    case MENU_OPTION_QUIT:
        ttt->running = false;
        break;
    }
}

/******************************************************************************/

/**
* @brief Handling ENTER keydown event
* @param ttt    game context
*/
static
void _tictactoe_events_keydown_ENTER_handle( tictactoe_t *ttt )
{
    switch ( ttt->play_mode )
    {
    case PLAY_MODE_NONE:
        _tictactoe_play_mode_none_handle( ttt );
        break;
    case PLAY_MODE_1VS1:
        board_move_set( ttt->board );
        break;
    case PLAY_MODE_1VSCOM:
        break;
    }
}

/******************************************************************************/

/**
* @brief Handling UP keydown event
* @param ttt    game context
*/
static
void _tictactoe_events_keydown_UP_handle( tictactoe_t *ttt )
{
    const bool menu = (ttt->play_mode == PLAY_MODE_NONE);
    if ( menu )
    {
        menu_option_change( ttt->menu, false );
    }
    else
    {
        board_player_move( ttt->board, BOARD_DIRECTION_UP );
    }
}

/******************************************************************************/

/**
* @brief Handling DOWN keydown event
* @param ttt    game context
*/
static
void _tictactoe_events_keydown_DOWN_handle( tictactoe_t *ttt )
{
    const bool menu = (ttt->play_mode == PLAY_MODE_NONE);
    if ( menu )
    {
        menu_option_change( ttt->menu, true );
    }
    else
    {
        board_player_move( ttt->board, BOARD_DIRECTION_DOWN );
    }
}

/******************************************************************************/

/**
* @brief Handling RIGHT keydown event
* @param ttt    game context
*/
static
void _tictactoe_events_keydown_RIGHT_handle( tictactoe_t *ttt )
{
    const bool play = (ttt->play_mode != PLAY_MODE_NONE);
    SDL_Log( "_tictactoe_events_keydown_RIGHT_handle [PLAY:%d]\n", play );
    if ( play )
        board_player_move( ttt->board, BOARD_DIRECTION_RIGHT );
}

/******************************************************************************/

/**
* @brief Handling LEFT keydown event
* @param ttt    game context
*/
static
void _tictactoe_events_keydown_LEFT_handle( tictactoe_t *ttt )
{
    const bool play = (ttt->play_mode != PLAY_MODE_NONE);
    if ( play )
        board_player_move( ttt->board, BOARD_DIRECTION_LEFT );
}

/******************************************************************************/

/**
* @brief Handling keydown events
* @param ttt    game context
* @param key_ev keyboard event
*/
static
void _tictactoe_events_keydown_handle( tictactoe_t *ttt, SDL_KeyboardEvent *key_ev )
{
    switch ( key_ev->keysym.sym )
    {
    case SDLK_ESCAPE:
        ttt->running = false;
        break;
    case SDLK_DOWN:
        _tictactoe_events_keydown_DOWN_handle( ttt );
        break;
    case SDLK_UP:
        _tictactoe_events_keydown_UP_handle( ttt );
        break;
    case SDLK_RIGHT:
        _tictactoe_events_keydown_RIGHT_handle( ttt );
        break;
    case SDLK_LEFT:
        _tictactoe_events_keydown_LEFT_handle( ttt );
        break;
    case SDLK_RETURN:
        _tictactoe_events_keydown_ENTER_handle( ttt );
        break;
    default:
        break;
    }
}

/******************************************************************************/

/**
* @brief Handling mouse button down events
* @param ttt    game context
* @param key_ev keyboard event
*/
void _tictactoe_events_mousebuttondown_handle(
        tictactoe_t *ttt, SDL_MouseButtonEvent *mouse_button_ev )
{
    const bool down = (mouse_button_ev->type == SDL_MOUSEBUTTONDOWN);
    const bool pressed = (mouse_button_ev->state == SDL_PRESSED);

    if ( down && pressed )
    {
        const bool menu = (ttt->play_mode == PLAY_MODE_NONE);
        if ( menu )
        {
            const bool ok = menu_option_choose(
                        ttt->menu, mouse_button_ev->x, mouse_button_ev->y );

            if ( ok && mouse_button_ev->clicks == MOUSE_DOUBLE_CLICK )
                _tictactoe_events_keydown_ENTER_handle( ttt );
        }
        else
        {
            const bool ok =board_player_xy_move(
                        ttt->board, mouse_button_ev->x, mouse_button_ev->y );

            if ( ok && mouse_button_ev->clicks == MOUSE_DOUBLE_CLICK )
                board_move_set( ttt->board );
        }
    }
}

/******************************************************************************/

/**
* @brief Handle game events
* @param ttt    game context
*/
void tictactoe_events_handle( tictactoe_t *ttt )
{
    SDL_Event event = { 0 };

    while ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
        case SDL_KEYDOWN:
            _tictactoe_events_keydown_handle( ttt, &event.key );
            break;
        case SDL_MOUSEBUTTONDOWN:
            _tictactoe_events_mousebuttondown_handle( ttt, &event.button );
            break;
        case SDL_WINDOWEVENT_CLOSE:
        case SDL_QUIT:
            ttt->running = false;
            break;
        }
    }
}

/******************************************************************************/

