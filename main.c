#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_main.h>

#include "mino.h"
#include "game.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    int pos_x = 50;
    int pos_y = 50;

    Game *game = game_new(pos_x, pos_y);
    if (!game)
    {
        fprintf(stderr, "Erreur : Impossible d'initialiser le jeu\n");
        return EXIT_FAILURE;
    }

    game_run(game);
    game_del(game);

    return EXIT_SUCCESS;

    (void)argc;
    (void)argv;
}
