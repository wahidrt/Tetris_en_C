#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "tetris.h"

typedef enum
{
    STATE_MENU,     // Écran d'accueil
    STATE_PLAY,     // Partie en cours
    STATE_GAMEOVER, // Partie terminée
    STATE_PAUSE     // Écran de Pause
} GameState;

// ------------------------------------------------
// Structure représentant l'état global du jeu
// ------------------------------------------------
typedef struct
{
    SDL_Window *win;
    SDL_Renderer *ren;
    TTF_Font *fontSmall;
    TTF_Font *fontLarge;

    Tetris *tet;

    int tet_offset_x;
    int tet_offset_y;

    int mino_size;

    Uint64 freq;
    Uint64 count;

    Mix_Music *music;          // Musique de fond
    Mix_Chunk *soundLand;      // Son joué lorsque le Tetrimino atterrit
    Mix_Chunk *soundGameOver;  // Son joué lors du Game Over
    Mix_Music *soundPause;     // Musique pour l'état Pause
    Mix_Chunk *soundLevel;     // Son joué lorsqu'on passe un niveau
    Mix_Chunk *soundLineClear; // Son joué lorsque des lignes sont détruites

    GameState state;

} Game;

// ------------------------------------------------
// Fonctions principales de gestion du jeu
// ------------------------------------------------
Game *game_new(int x, int y);

void game_del(Game *g);

void game_run(Game *g);

void game_update(Game *g);

#endif // GAME_H
