#ifndef TETRIS_H
#define TETRIS_H

// ------------------------------------------------
// Enum pour les types de Tetriminos
// ------------------------------------------------
typedef enum
{
    TYPE_I,
    TYPE_J,
    TYPE_L,
    TYPE_O,
    TYPE_S,
    TYPE_T,
    TYPE_Z
} Type;

// -------------------------------------
// Structure représentant l'état du jeu Tetris
// -------------------------------------
typedef struct
{
    char matrix[20][10];
    char buffer[5][5];

    Type current_type;    // Type du Tetrimino actuel
    int current_line;     // Ligne actuelle du Tetrimino
    int current_column;   // Colonne actuelle du Tetrimino
    int current_rotation; // Rotation actuelle du Tetrimino

    Type next_type; // Type du prochain Tetrimino

    int level;            // Niveau du jeu
    int score;            // Score du joueur
    int nbr_lines;        // Nombre de lignes détruites
    float drop_speed[15]; // Vitesse de descente par niveau
} Tetris;

// ------------------------------------------------
// Déclarations des fonctions de gestion du Tetris
// ------------------------------------------------

extern const char tetriminos[7][4][5][5];
Tetris *tetris_new();
void tetris_del(Tetris *tet);
void tetris_reset(Tetris *tet);
int tetris_can_go_left(Tetris *tet);
int tetris_can_go_right(Tetris *tet);
int tetris_can_go_down(Tetris *tet);
int tetris_move_down(Tetris *tet);
void tetris_shift_board(Tetris *tet);
float tetris_get_drop_speed(Tetris *tet);
void tetris_rotate(Tetris *tet);
void tetris_move_left(Tetris *tet);
void tetris_move_right(Tetris *tet);
int tetris_is_valid_position(Tetris *tet, char buffer[5][5]);
#endif // TETRIS_H
