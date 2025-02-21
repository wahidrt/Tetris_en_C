#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "tetris.h"

const char tetriminos[7][4][5][5] = {
    // TYPE_I
    {
        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 1, 1, 1, 1},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {1, 1, 1, 1, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0}}},

    // TYPE_J
    {
        {{0, 0, 0, 0, 0},
         {0, 2, 0, 0, 0},
         {0, 2, 2, 2, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 2, 0, 0},
         {0, 0, 2, 0, 0},
         {0, 2, 2, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 2, 2, 2, 0},
         {0, 0, 0, 2, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 2, 2, 0},
         {0, 0, 2, 0, 0},
         {0, 0, 2, 0, 0},
         {0, 0, 0, 0, 0}},
    },

    // TYPE_L
    {
        {{0, 0, 0, 0, 0},
         {0, 0, 0, 3, 0},
         {0, 3, 3, 3, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 3, 3, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 3, 3, 3, 0},
         {0, 3, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 3, 3, 0},
         {0, 0, 0, 0, 0}},

    },

    // TYPE_O
    {
        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 4, 4, 0, 0},
         {0, 4, 4, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 4, 4, 0, 0},
         {0, 4, 4, 0, 0},
         {0, 0, 0, 0, 0}},

    },

    // TYPE_S
    {
        {{0, 0, 0, 0, 0},
         {0, 0, 5, 5, 0},
         {0, 5, 5, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 5, 0, 0, 0},
         {0, 5, 5, 0, 0},
         {0, 0, 5, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 5, 5, 0},
         {0, 5, 5, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 5, 0, 0},
         {0, 0, 5, 5, 0},
         {0, 0, 0, 5, 0},
         {0, 0, 0, 0, 0}},
    },

    // TYPE_T
    {
        {{0, 0, 0, 0, 0},
         {0, 0, 6, 0, 0},
         {0, 6, 6, 6, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 6, 0, 0},
         {0, 6, 6, 0, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 6, 6, 6, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 6, 6, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 0, 0, 0}},
    },

    // TYPE_Z
    {
        {{0, 0, 0, 0, 0},
         {0, 7, 7, 0, 0},
         {0, 0, 7, 7, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 7, 0, 0},
         {0, 7, 7, 0, 0},
         {0, 7, 0, 0, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 7, 7, 0, 0},
         {0, 0, 7, 7, 0},
         {0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 0},
         {0, 0, 0, 7, 0},
         {0, 0, 7, 7, 0},
         {0, 0, 7, 0, 0},
         {0, 0, 0, 0, 0}},
    }};

// --------------------------------------------------
// Crée et initialise une nouvelle instance de Tetris
// --------------------------------------------------
Tetris *tetris_new()
{
    Tetris *tet = malloc(sizeof(Tetris)); // Alloue la mémoire pour la structure Tetris
    if (!tet)                             // Vérifie si l'allocation a échoué
        return NULL;

    memset(tet->matrix, 0, sizeof(tet->matrix)); // Initialise la matrice principale à 0 (vide)
    memset(tet->buffer, 0, sizeof(tet->buffer)); // Initialise le buffer (pièce en mouvement) à 0

    // Définit une pièce aléatoire comme pièce actuelle et suivante
    tet->current_type = rand() % 7;
    tet->next_type = rand() % 7;

    return tet;
}
// --------------------------------------------------
// Libère l'instance de Tetris
// --------------------------------------------------
void tetris_del(Tetris *tet)
{
    if (tet)
    {
        free(tet);
    }
}
// --------------------------------------------------
// Remet un nouveau Tetrimino dans le buffer
// --------------------------------------------------
void tetris_reset(Tetris *tet)
{
    // Vider le buffer
    memset(tet->buffer, 0, sizeof(tet->buffer));

    // Passer au prochain Tetrimino
    tet->current_type = tet->next_type;
    tet->next_type = rand() % 7;

    // Position initiale
    tet->current_line = 0;
    tet->current_column = 3;
    tet->current_rotation = 0;

    // Recopier la forme (rotation 0) dans le buffer
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tet->buffer[i][j] = tetriminos[tet->current_type][0][i][j];
        }
    }
}
// --------------------------------------------------
// Vérifie si le Tetrimino peut aller à gauche
// --------------------------------------------------
int tetris_can_go_left(Tetris *tet)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tet->buffer[i][j])
            {
                int row = tet->current_line + i;
                int col = tet->current_column + j;
                // Si on est collé au mur gauche ou si la case à gauche est déjà occupée
                if (col == 0 || tet->matrix[row][col - 1] != 0)
                {
                    return 0; // Mouvement impossible
                }
            }
        }
    }
    return 1;
}

void tetris_move_left(Tetris *tet)
{
    if (tetris_can_go_left(tet))
    {
        tet->current_column--;
    }
}
// --------------------------------------------------
// Vérifie si le Tetrimino peut aller à droite
// --------------------------------------------------
int tetris_can_go_right(Tetris *tet)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tet->buffer[i][j])
            {
                int row = tet->current_line + i;
                int col = tet->current_column + j;
                // Si on est collé au mur droit ou la case à droite est occupée
                if (col == 9 || tet->matrix[row][col + 1] != 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void tetris_move_right(Tetris *tet)
{
    if (tetris_can_go_right(tet))
    {
        tet->current_column++;
    }
}

// ----------------------------------
// Vérifie si le Tetrimino peut descendre
// ----------------------------------
int tetris_can_go_down(Tetris *tet)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tet->buffer[i][j])
            {
                int row = tet->current_line + i;
                int col = tet->current_column + j;
                if (row == 19 || tet->matrix[row + 1][col] != 0)
                {
                    return 0; // On ne peut pas descendre
                }
            }
        }
    }
    return 1; // On peut descendre
}
// ----------------------------------
// Descente effective
// ----------------------------------
int tetris_move_down(Tetris *tet)
{
    // Peut-on descendre ?
    if (tetris_can_go_down(tet))
    {
        tet->current_line++;
        return 1; // Succès : on a bougé
    }
    else
    {
        // S'il ne peut pas descendre et qu'il est tout en haut
        // => Game Over (on renvoie -1)
        if (tet->current_line == 0)
        {
            // La pièce n'a jamais pu bouger => bloqué dès le spawn
            return -1;
        }

        // Sinon, c'est juste la fin de la descente
        tetris_shift_board(tet);
        tetris_reset(tet);

        return 0;
    }
}
// ----------------------------------
// Vérifie si un buffer est valide
// ----------------------------------
int tetris_is_valid_position(Tetris *tet, char new_buffer[5][5])
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (new_buffer[i][j])
            {
                int row = tet->current_line + i;
                int col = tet->current_column + j;
                // Vérifie si la position est hors des limites du tableau
                if (row < 0 || row >= 20 || col < 0 || col >= 10)
                {
                    return 0; // Hors limite
                }
                // Vérifie si la position est occupée
                if (tet->matrix[row][col])
                {
                    return 0; // Collision
                }
            }
        }
    }
    return 1;
}

// ---------------------------------------------
// Effectue la rotation du Tetrimino si possible
// ---------------------------------------------
void tetris_rotate(Tetris *tet)
{
    // On va juste vérifier si la rotation suivante est valide
    int new_rotation = (tet->current_rotation + 1) % 4;
    char new_buffer[5][5] = {0};
    // Construire la pièce dans new_buffer
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            new_buffer[j][4 - i] = tetriminos[tet->current_type][new_rotation][i][j];
        }
    }
    // Vérifier collisions
    if (tetris_is_valid_position(tet, new_buffer))
    {
        memcpy(tet->buffer, new_buffer, sizeof(new_buffer)); // Copie la rotation dans le buffer
        tet->current_rotation = new_rotation;                // Met à jour la rotation actuelle
    }
}
// ------------------------------------------------------------------
// Fixe le Tetrimino dans la matrix et gère la destruction des lignes
// ------------------------------------------------------------------
void tetris_shift_board(Tetris *tet)
{
    // ------------------------------------------------------------------
    //  1) Fixer la pièce dans matrix
    // ------------------------------------------------------------------
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tet->buffer[i][j])
            {
                int row = tet->current_line + i;
                int col = tet->current_column + j;
                tet->matrix[row][col] = tet->buffer[i][j];
            }
        }
    }

    // ------------------------------------------------------------------
    // 2) Vérifier combien de lignes pleines
    // ------------------------------------------------------------------
    int lines_cleared = 0;
    for (int i = 0; i < 20; i++)
    {
        int full = 1;
        for (int j = 0; j < 10; j++)
        {
            if (tet->matrix[i][j] == 0)
            {
                full = 0;
                break;
            }
        }
        if (full)
        {
            // Décaler les lignes au-dessus vers le bas
            for (int k = i; k > 0; k--)
            {
                memcpy(tet->matrix[k], tet->matrix[k - 1], sizeof(tet->matrix[k]));
            }
            // Vider la première
            memset(tet->matrix[0], 0, sizeof(tet->matrix[0]));

            lines_cleared++;
        }
    }

    // ------------------------------------------------------------------
    // 3) Calcul du score selon lines_cleared
    // ------------------------------------------------------------------
    if (lines_cleared > 0)
    {
        // On ajoute au cumul total de lignes
        tet->nbr_lines += lines_cleared;

        switch (lines_cleared)
        {
        case 1:
            tet->score += 100 * tet->level;
            break;
        case 2:
            tet->score += 300 * tet->level;
            break;
        case 3:
            tet->score += 500 * tet->level;
            break;
        case 4: // "Tetris"
            tet->score += 800 * tet->level;
            break;
        default:
            // si plus de 4 lignes d'un coup (bug ou autre), on traite comme un Tetris
            tet->score += 800 * tet->level;
            break;
        }

        // 4) Gestion du niveau : +1 toutes les 10 lignes (jusqu’à 15 max)
        while (tet->nbr_lines >= 10)
        {
            tet->nbr_lines -= 10;
            tet->level++;
            if (tet->level > 15)
            {
                tet->level = 15;
                break; // on n'augmente pas plus
            }
        }
    }
}

// ------------------------------------------------------------------
// Renvoie la vitesse de descente en fonction du niveau
// ------------------------------------------------------------------
float tetris_get_drop_speed(Tetris *tet)
{
    // On borne le niveau entre 1 et 15
    int l = tet->level;
    if (l < 1)
        l = 1;
    if (l > 15)
        l = 15;

    // On calcule la base
    float base = 0.8f - ((l - 1) * 0.007f);
    if (base < 0.0f)
        base = 0.0f; // éviter un nombre négatif

    // On applique la puissance (l-1)
    float speed = powf(base, (float)(l - 1));

    // Pour éviter des vitesses trop petites :
    return (speed < 0.007f) ? 0.007f : speed;
}
