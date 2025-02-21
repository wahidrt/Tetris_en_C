#ifndef MINO_H
#define MINO_H

#include "game.h"
#include "tetris.h"

// ------------------------------------------------
// Définition des couleurs pour chaque Tetrimino
// ------------------------------------------------
typedef struct
{
    Uint8 r, g, b; // Composantes rouge, vert, bleu
} Color;

static const Color TetriminoColors[] = {
    {64, 224, 208},     // TYPE_I (turquoise)
    {0x00, 0x00, 0xFF}, // TYPE_J (bleu)
    {0xFF, 0xA5, 0x00}, // TYPE_L (orange)
    {0xFF, 0xEF, 0x00}, // TYPE_O (jaune canari)
    {0x7C, 0xFC, 0x00}, // TYPE_S (vert prairie)
    {0x80, 0x00, 0x80}, // TYPE_T (violet)
    {0xFF, 0x00, 0x00}  // TYPE_Z (rouge)
};

void mino_display(Game *g, Type t, int l, int c);

#endif // MINO_H
