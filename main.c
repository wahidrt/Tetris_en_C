/*
 * main.c
 *
 * Ce fichier constitue le point d'entrée du programme Tetris.
 * Il initialise le générateur aléatoire, crée l'instance principale
 * du jeu, lance la boucle d'exécution puis libère les ressources.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_main.h>

#include "mino.h"
#include "game.h"

/**
 * Point d'entrée de l'application.
 *
 * @param argc Nombre d'arguments transmis au programme.
 * @param argv Tableau contenant les arguments de la ligne de commande.
 * @return EXIT_SUCCESS si le jeu se termine normalement,
 *         EXIT_FAILURE si son initialisation échoue.
 *
 * Les arguments ne sont pas utilisés par le jeu. La fonction crée
 * une fenêtre à la position (50, 50), exécute la boucle principale
 * avec game_run, puis détruit proprement l'instance avec game_del.
 */
int main(int argc, char *argv[])
{
    // Initialise la génération aléatoire utilisée pour choisir les Tetriminos.
    srand((unsigned)time(NULL));
    // Position initiale de la fenêtre sur l'écran.
    int pos_x = 50;
    int pos_y = 50;

    // Création de la structure Game et initialisation de SDL.
    Game *game = game_new(pos_x, pos_y);
    if (!game)
    {
        fprintf(stderr, "Erreur : Impossible d'initialiser le jeu\n");
        return EXIT_FAILURE;
    }

    // La fonction reste dans la boucle principale jusqu'à la fermeture du jeu.
    game_run(game);

    // Libération de la mémoire, des sons, des polices et des ressources SDL.
    game_del(game);

    return EXIT_SUCCESS;

    (void)argc;
    (void)argv;
}
