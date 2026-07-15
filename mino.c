/*
 * mino.c
 *
 * Ce module assure l'affichage d'un mino, c'est-à-dire d'un bloc
 * élémentaire composant un Tetrimino. Le dessin utilise plusieurs
 * nuances d'une même couleur afin de produire un léger effet de relief.
 */

#include <SDL2/SDL.h>

#include "mino.h"
#include "game.h"

/**
 * Dessine un mino dans la fenêtre SDL.
 *
 * @param g Instance du jeu contenant le renderer, la taille des blocs
 *          et les décalages du plateau.
 * @param t Type du Tetrimino, utilisé pour sélectionner sa couleur.
 * @param l Indice de ligne du bloc dans le plateau.
 * @param c Indice de colonne du bloc dans le plateau.
 *
 * La position en pixels est calculée à partir de la ligne, de la
 * colonne, de la taille d'un mino et des décalages du plateau. Le bloc
 * est ensuite dessiné avec une couleur de base, une bordure claire en
 * haut et à gauche, puis une bordure sombre en bas et à droite.
 */
void mino_display(Game *g, Type t, int l, int c)
{
    // Une valeur de type invalide ne doit pas indexer le tableau des couleurs.
    if (t < TYPE_I || t > TYPE_Z)
    {
        return; // Vérification du type
    }
    // -------------------------------------
    // Taille du mino
    // -------------------------------------
    int size = g->mino_size;

    // -------------------------------------
    // Coordonnées de la position à afficher
    // -------------------------------------
    int x = g->tet_offset_x + c * size;
    int y = g->tet_offset_y + l * size;

    // -------------------------------------
    // Couleurs claire et sombre pour le relief
    // -------------------------------------
    // La couleur claire et la couleur sombre sont bornées entre 0 et 255.
    Color base = TetriminoColors[t];
    Color light = {base.r + 50 > 255 ? 255 : base.r + 50,
                   base.g + 50 > 255 ? 255 : base.g + 50,
                   base.b + 50 > 255 ? 255 : base.b + 50};
    Color dark = {base.r - 50 < 0 ? 0 : base.r - 50,
                  base.g - 50 < 0 ? 0 : base.g - 50,
                  base.b - 50 < 0 ? 0 : base.b - 50};

    SDL_Rect rect;

    // -------------------------------------
    // Couleur de base (plein du mino)
    // -------------------------------------
    SDL_SetRenderDrawColor(g->ren, base.r, base.g, base.b, 0xFF);
    rect = (SDL_Rect){x, y, (3 * size) / 4, (3 * size) / 4};
    SDL_RenderFillRect(g->ren, &rect);

    // -------------------------------------
    // Ligne supérieure (claire)
    // -------------------------------------
    SDL_SetRenderDrawColor(g->ren, light.r, light.g, light.b, 0xFF);
    rect = (SDL_Rect){x, y, (3 * size) / 4, size / 4};
    SDL_RenderFillRect(g->ren, &rect);

    // -------------------------------------
    // Colonne gauche (claire)
    // -------------------------------------
    rect = (SDL_Rect){x, y, size / 4, (3 * size) / 4};
    SDL_RenderFillRect(g->ren, &rect);

    // -------------------------------------
    // Ligne inférieure (sombre)
    // -------------------------------------
    SDL_SetRenderDrawColor(g->ren, dark.r, dark.g, dark.b, 0xFF);
    rect = (SDL_Rect){x, y + (3 * size) / 4, size - 2, size / 4 - 1};
    SDL_RenderFillRect(g->ren, &rect);

    // -------------------------------------
    // Colonne droite (sombre)
    // -------------------------------------
    rect = (SDL_Rect){x + (3 * size) / 4, y, size / 4 - 1, size - 2};
    SDL_RenderFillRect(g->ren, &rect);
}
