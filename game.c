#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "mino.h"
#include "tetris.h"
// -------------------------------------------------------------
// Fonction interne pour initialiser la SDL et créer une fenêtre
// -------------------------------------------------------------
static int game_sdl_init(Game *g, int x, int y)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur TTF_Init: %s\n", TTF_GetError());
        return 0;
    }

    g->win = SDL_CreateWindow("Tetris", x, y, 500, 600, SDL_WINDOW_SHOWN);
    if (!g->win)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        return 0;
    }

    g->ren = SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g->ren)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        return 0;
    }

    g->fontSmall = TTF_OpenFont("Tetris.ttf", 14);
    if (!g->fontSmall)
    {
        fprintf(stderr, "Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return 0;
    }
    g->fontLarge = TTF_OpenFont("Tetris.ttf", 64);
    if (!g->fontLarge)
    {
        fprintf(stderr, "Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return 0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        return 0;
    }

    return 1;
}
// ----------------------------------------
// Initialise une nouvelle instance du jeu
// ----------------------------------------
Game *game_new(int x, int y)
{
    Game *g = (Game *)malloc(sizeof(Game));
    if (!g)
    {
        fprintf(stderr, "Erreur d'allocation pour Game\n");
        return NULL;
    }

    g->win = NULL;
    g->ren = NULL;
    g->fontSmall = NULL;
    g->fontLarge = NULL;
    g->tet = NULL;

    if (!game_sdl_init(g, x, y))
    {
        game_del(g);
        return NULL;
    }

    g->tet = tetris_new();

    if (!g->tet)
    {
        fprintf(stderr, "Erreur lors de la création de Tetris\n");
        game_del(g);
        return NULL;
    }

    g->tet_offset_x = 0;
    g->tet_offset_y = 0;
    g->mino_size = 30;

    g->freq = SDL_GetPerformanceFrequency();
    g->count = SDL_GetPerformanceCounter();

    g->soundLineClear = Mix_LoadWAV("line.wav");
    if (!g->soundLineClear)
    {
        fprintf(stderr, "Erreur Mix_LoadWAV Bloc.wav: %s\n", Mix_GetError());
        game_del(g);
        return NULL;
    }

    g->soundLand = Mix_LoadWAV("Bloc.wav");
    if (!g->soundLand)
    {
        fprintf(stderr, "Erreur Mix_LoadWAV land.wav: %s\n", Mix_GetError());
        game_del(g);
        return NULL;
    }

    g->soundGameOver = Mix_LoadWAV("Game_Over.wav");
    if (!g->soundGameOver)
    {
        fprintf(stderr, "Erreur Mix_LoadWAV gameover.wav: %s\n", Mix_GetError());
        game_del(g);
        return NULL;
    }

    g->soundPause = Mix_LoadMUS("Pause.mp3");
    if (!g->soundPause)
    {
        fprintf(stderr, "Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        game_del(g);
        return NULL;
    }

    g->soundLevel = Mix_LoadWAV("Level.wav");
    if (!g->soundLevel)
    {
        fprintf(stderr, "Erreur Mix_LoadWAV gameover.wav: %s\n", Mix_GetError());
        game_del(g);
        return NULL;
    }

    g->music = Mix_LoadMUS("Tetris.mp3");
    if (!g->music)
    {
        fprintf(stderr, "Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        return 0; // ou return NULL;
    }

    Mix_PlayMusic(g->music, -1); // -1 pour une répétition infinie

    return g;
}
// ----------------------------------------
// Libère les ressources associées au jeu
// ----------------------------------------
void game_del(Game *g)
{
    if (g)
    {
        if (g->tet)
        {
            tetris_del(g->tet);
        }
        if (g->fontLarge)
        {
            TTF_CloseFont(g->fontLarge);
        }
        if (g->fontSmall)
        {
            TTF_CloseFont(g->fontSmall);
        }
        if (g->ren)
        {
            SDL_DestroyRenderer(g->ren);
        }
        if (g->soundLineClear)
        {
            Mix_FreeChunk(g->soundLineClear);
            g->soundLineClear = NULL;
        }
        if (g->win)
        {
            SDL_DestroyWindow(g->win);
        }
        if (g->music)
        {
            Mix_FreeMusic(g->music);
        }
        if (g->soundLand)
        {
            Mix_FreeChunk(g->soundLand);
        }
        if (g->soundGameOver)
        {
            Mix_FreeChunk(g->soundGameOver);
        }
        if (g->soundPause)
        {
            Mix_FreeMusic(g->soundPause);
        }
        if (g->soundLevel)
        {
            Mix_FreeChunk(g->soundLevel);
        }
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        free(g);
    }
}

void game_update(Game *g)
{
    SDL_SetRenderDrawColor(g->ren, 24, 24, 24, 255);
    SDL_RenderClear(g->ren);

    // ---------------------------------------------------
    // Selon l'état du jeu, on fait un affichage différent
    // ---------------------------------------------------
    switch (g->state)
    {

    // -------------------
    // 1) État MENU : on attend que le joueur appuie sur Espace/Entrée pour commencer la partie
    // -------------------
    case STATE_MENU:
    {
        // ----------------------------------
        // Affichage d'un titre
        // ----------------------------------
        SDL_Color colorTitle = {0, 255, 255, 255}; // Vert + Bleu = Cyan
        SDL_Surface *surfaceTitle = TTF_RenderText_Solid(g->fontLarge, "TETRIS", colorTitle);
        SDL_Texture *textureTitle = SDL_CreateTextureFromSurface(g->ren, surfaceTitle);

        SDL_Rect rectTitle;
        rectTitle.x = (500 - surfaceTitle->w) / 2; // Centré en largeur (fenêtre 500 px)
        rectTitle.y = 50;                          // Un peu descendu de 50 px
        rectTitle.w = surfaceTitle->w;
        rectTitle.h = surfaceTitle->h;

        SDL_RenderCopy(g->ren, textureTitle, NULL, &rectTitle);

        SDL_FreeSurface(surfaceTitle);
        SDL_DestroyTexture(textureTitle);

        // ----------------------------------
        // Affichage d'un message pour lancer le jeu
        // ----------------------------------
        SDL_Color colorMsg = {231, 240, 13, 255}; // Jaune
        SDL_Surface *surfaceMsg = TTF_RenderText_Solid(g->fontSmall,
                                                       "Appuyez sur [ESPACE] ou [ENTREE] pour jouer", colorMsg);
        SDL_Texture *textureMsg = SDL_CreateTextureFromSurface(g->ren, surfaceMsg);

        SDL_Rect rectMsg;
        rectMsg.x = (500 - surfaceMsg->w) / 2;
        rectMsg.y = 150;
        rectMsg.w = surfaceMsg->w;
        rectMsg.h = surfaceMsg->h;

        SDL_RenderCopy(g->ren, textureMsg, NULL, &rectMsg);

        SDL_FreeSurface(surfaceMsg);
        SDL_DestroyTexture(textureMsg);

        // ----------------------------------
        // Affichage des instructions
        // ----------------------------------
        // On se crée un petit tableau de chaînes pour chaque ligne
        const char *instructions[] = {
            "Instructions du jeu :",
            " - Fleche gauche : Deplacer la piece a gauche",
            " - Fleche droite : Deplacer la piece a droite",
            " - Fleche haut   : Faire pivoter la piece a gauche",
            " - Fleche bas    : Faire descendre la piece",
            " - P             : Mettre en pause / Reprendre",
            " - Espace/Entree : Lancer ou relancer la partie",
            " - ESC           : Quitter le jeu",
            NULL};

        int yPos = 300;                             // On commence à dessiner à 300 px du haut
        SDL_Color colorText = {200, 200, 200, 255}; // Gris clair

        for (int i = 0; instructions[i] != NULL; i++)
        {
            SDL_Surface *surfLine = TTF_RenderText_Solid(g->fontSmall, instructions[i], colorText);
            SDL_Texture *texLine = SDL_CreateTextureFromSurface(g->ren, surfLine);

            SDL_Rect rectLine;
            rectLine.x = 40;
            rectLine.y = yPos;
            rectLine.w = surfLine->w;
            rectLine.h = surfLine->h;

            SDL_RenderCopy(g->ren, texLine, NULL, &rectLine);

            SDL_FreeSurface(surfLine);
            SDL_DestroyTexture(texLine);

            yPos += 30; // Espace entre chaque ligne
        }
    }
    break;

    // ----------------------------------
    // 2) État PLAY : la partie est en cours
    // ----------------------------------
    case STATE_PLAY:
    {
        // Dessin du cadre du plateau
        SDL_Rect game_frame = {0, 0, 300, 600};
        SDL_SetRenderDrawColor(g->ren, 255, 0, 255, 255);
        SDL_RenderDrawRect(g->ren, &game_frame);

        // Dessin de l'arrière-plan des stats
        SDL_Rect stats_bg = {300, 0, 200, 600};
        SDL_SetRenderDrawColor(g->ren, 30, 30, 30, 255);
        SDL_RenderFillRect(g->ren, &stats_bg);

        // Affichage de la matrice fixée
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (g->tet->matrix[i][j] != 0)
                {
                    mino_display(g, g->tet->matrix[i][j] - 1, i, j);
                }
            }
        }

        // ----------------------------------
        // Affichage du Tetrimino en mouvement
        // ----------------------------------
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (g->tet->buffer[i][j] != 0)
                {
                    int row = g->tet->current_line + i;
                    int col = g->tet->current_column + j;
                    mino_display(g, g->tet->current_type, row, col);
                }
            }
        }

        // ----------------------------------
        // Affichage du Tetrimino suivant
        // ----------------------------------
        SDL_Rect preview_frame = {325, 200, 150, 150};
        SDL_SetRenderDrawColor(g->ren, 255, 0, 255, 255);
        SDL_RenderDrawRect(g->ren, &preview_frame);

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (tetriminos[g->tet->next_type][0][i][j])
                {
                    // Coordonnées du mini-bloc
                    SDL_Rect block = {
                        preview_frame.x + j * 30,
                        preview_frame.y + i * 30,
                        27, 27};

                    // Récupère la couleur de base du Tetrimino
                    Color base = TetriminoColors[g->tet->next_type];
                    Color light = {
                        base.r + 50 > 255 ? 255 : base.r + 50,
                        base.g + 50 > 255 ? 255 : base.g + 50,
                        base.b + 50 > 255 ? 255 : base.b + 50};
                    Color dark = {
                        base.r - 50 < 0 ? 0 : base.r - 50,
                        base.g - 50 < 0 ? 0 : base.g - 50,
                        base.b - 50 < 0 ? 0 : base.b - 50};

                    // Peins le bloc avec la couleur de base
                    SDL_SetRenderDrawColor(g->ren, base.r, base.g, base.b, 255);
                    SDL_RenderFillRect(g->ren, &block);

                    // Ajoute la bande claire (en haut)
                    SDL_SetRenderDrawColor(g->ren, light.r, light.g, light.b, 255);
                    SDL_Rect top_band = {block.x, block.y, (3 * block.w) / 4, block.h / 4};
                    SDL_RenderFillRect(g->ren, &top_band);

                    // Ajoute la bande claire (à gauche)
                    SDL_Rect left_band = {block.x, block.y, block.w / 4, (3 * block.h) / 4};
                    SDL_RenderFillRect(g->ren, &left_band);

                    // Ajoute la bande sombre (en bas)
                    SDL_SetRenderDrawColor(g->ren, dark.r, dark.g, dark.b, 255);
                    SDL_Rect bottom_band = {block.x, block.y + (3 * block.h) / 4, block.w, block.h / 4 + 1};
                    SDL_RenderFillRect(g->ren, &bottom_band);

                    // Ajoute la bande sombre (à droite)
                    SDL_Rect right_band = {block.x + (3 * block.w) / 4, block.y, block.w / 4 + 1, block.h};
                    SDL_RenderFillRect(g->ren, &right_band);
                }
            }
        }

        // -------------------
        // Affichage des stats (score, niveau, lignes)
        // -------------------
        SDL_Color color = {231, 240, 13, 255};
        char info[64];
        SDL_Surface *surface;
        SDL_Texture *texture;
        SDL_Rect rect;

        // Score
        snprintf(info, sizeof(info), "Score: %d", g->tet->score);
        surface = TTF_RenderText_Solid(g->fontSmall, info, color);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);
        rect = (SDL_Rect){325, 50, surface->w, surface->h};
        SDL_RenderCopy(g->ren, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Niveau
        snprintf(info, sizeof(info), "Level: %d", g->tet->level);
        surface = TTF_RenderText_Solid(g->fontSmall, info, color);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);
        rect = (SDL_Rect){325, 100, surface->w, surface->h};
        SDL_RenderCopy(g->ren, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Lignes détruites
        snprintf(info, sizeof(info), "Lines: %d", g->tet->nbr_lines);
        surface = TTF_RenderText_Solid(g->fontSmall, info, color);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);
        rect = (SDL_Rect){325, 150, surface->w, surface->h};
        SDL_RenderCopy(g->ren, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    break;

    // -------------------
    // 3) État GAMEOVER : on affiche un message final, et on attend que le joueur relance
    // -------------------
    case STATE_GAMEOVER:
    {
        // Couleur pour le message principal
        SDL_Color color1 = {255, 0, 0, 255}; // Rouge
        SDL_Surface *surface = TTF_RenderText_Solid(g->fontLarge, "GAME OVER", color1);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(g->ren, surface);

        SDL_Rect dstRect;
        dstRect.x = (500 - surface->w) / 2;
        dstRect.y = (600 - surface->h) / 4;
        dstRect.w = surface->w;
        dstRect.h = surface->h;

        SDL_RenderCopy(g->ren, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Petit message pour rejouer
        SDL_Color color2 = {255, 0, 255, 255}; // Magenta
        surface = TTF_RenderText_Solid(g->fontSmall, "Appuyez sur [ESPACE] pour rejouer", color2);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);

        dstRect.x = (500 - surface->w) / 2;
        dstRect.y = ((600 - surface->h) / 2) + 50;
        dstRect.w = surface->w;
        dstRect.h = surface->h;

        SDL_RenderCopy(g->ren, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Affichage des statistiques finales : Score, Niveau, Lignes détruites
        SDL_Color statColor = {255, 255, 0, 255};
        char info[64];

        // Affichage du score
        sprintf(info, "Score: %d", g->tet->score);
        surface = TTF_RenderText_Solid(g->fontSmall, info, statColor);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);

        dstRect.x = (500 - surface->w) / 2;
        dstRect.y = ((600 - surface->h) / 2) + 120;
        dstRect.w = surface->w;
        dstRect.h = surface->h;

        SDL_RenderCopy(g->ren, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Affichage du niveau atteint
        sprintf(info, "Niveau: %d", g->tet->level);
        surface = TTF_RenderText_Solid(g->fontSmall, info, statColor);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);

        dstRect.x = (500 - surface->w) / 2;
        dstRect.y = ((600 - surface->h) / 2) + 160;
        dstRect.w = surface->w;
        dstRect.h = surface->h;

        SDL_RenderCopy(g->ren, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Affichage du nombre de lignes détruites
        sprintf(info, "Lignes: %d", g->tet->nbr_lines);
        surface = TTF_RenderText_Solid(g->fontSmall, info, statColor);
        texture = SDL_CreateTextureFromSurface(g->ren, surface);

        dstRect.x = (500 - surface->w) / 2;
        dstRect.y = ((600 - surface->h) / 2) + 200;
        dstRect.w = surface->w;
        dstRect.h = surface->h;

        SDL_RenderCopy(g->ren, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    break;

    case STATE_PAUSE:
    {
        // ----------------------------------
        // On affiche un message "JEU EN PAUSE"
        // ----------------------------------
        SDL_Color colorPause = {231, 240, 13, 255};
        SDL_Surface *surfacePause = TTF_RenderText_Solid(g->fontLarge, "PAUSE", colorPause);
        SDL_Texture *texturePause = SDL_CreateTextureFromSurface(g->ren, surfacePause);

        SDL_Rect rectPause;
        rectPause.x = (500 - surfacePause->w) / 2;
        rectPause.y = 150;
        rectPause.w = surfacePause->w;
        rectPause.h = surfacePause->h;

        SDL_RenderCopy(g->ren, texturePause, NULL, &rectPause);

        SDL_FreeSurface(surfacePause);
        SDL_DestroyTexture(texturePause);

        // On ajoute ensuite quelques instructions de sortie de pause
        SDL_Color colorInstr = {255, 0, 255, 255};
        const char *pauseInstr[] = {
            "Appuyez sur P pour reprendre la partie",
            NULL};

        int yPos = 300;
        for (int i = 0; pauseInstr[i] != NULL; i++)
        {
            SDL_Surface *surfLine = TTF_RenderText_Solid(g->fontSmall, pauseInstr[i], colorInstr);
            SDL_Texture *texLine = SDL_CreateTextureFromSurface(g->ren, surfLine);

            SDL_Rect rectLine;
            rectLine.x = (500 - surfLine->w) / 2;
            rectLine.y = yPos;
            rectLine.w = surfLine->w;
            rectLine.h = surfLine->h;

            SDL_RenderCopy(g->ren, texLine, NULL, &rectLine);

            SDL_FreeSurface(surfLine);
            SDL_DestroyTexture(texLine);

            yPos += 30;
        }
    }
    break;
    }

    // Enfin, on met à jour l'écran
    SDL_RenderPresent(g->ren);
}

void game_run(Game *g)
{
    int running = 1;
    SDL_Event e;

    // État initial
    g->state = STATE_MENU;

    // Variables pour mesurer la descente automatique
    Uint64 last_time = SDL_GetPerformanceCounter();
    Uint64 current_time;
    float elapsed = 0.0f;

    while (running)
    {
        // -------------------
        // 1) Gestion des événements
        // -------------------
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                // Touche P pour mettre en pause / reprendre
                case SDLK_p:
                    if (g->state == STATE_PLAY)
                    {
                        g->state = STATE_PAUSE;
                        // Stop la musique de jeu
                        Mix_HaltMusic();
                        // Lance la musique de pause
                        Mix_PlayMusic(g->soundPause, -1);
                    }
                    else if (g->state == STATE_PAUSE)
                    {
                        g->state = STATE_PLAY;
                        Mix_HaltMusic();
                        // Re-lance la musique principale
                        Mix_PlayMusic(g->music, -1);
                    }
                    break;

                // Échap => quitter
                case SDLK_ESCAPE:
                    running = 0;
                    break;

                // Espace ou Entrée => lancer / relancer la partie
                case SDLK_SPACE:
                case SDLK_RETURN:
                    if (g->state == STATE_MENU)
                    {
                        Mix_PlayMusic(g->music, -1);
                        memset(g->tet->matrix, 0, sizeof(g->tet->matrix));
                        g->tet->score = 0;
                        g->tet->level = 1;
                        g->tet->nbr_lines = 0;
                        tetris_reset(g->tet);
                        g->state = STATE_PLAY;
                    }
                    else if (g->state == STATE_GAMEOVER)
                    {
                        memset(g->tet->matrix, 0, sizeof(g->tet->matrix));
                        g->tet->score = 0;
                        g->tet->level = 1;
                        g->tet->nbr_lines = 0;
                        tetris_reset(g->tet);
                        g->state = STATE_PLAY;
                        Mix_PlayMusic(g->music, -1);
                    }
                    break;

                // Flèche gauche
                case SDLK_LEFT:
                    if (g->state == STATE_PLAY)
                    {
                        if (tetris_can_go_left(g->tet))
                            tetris_move_left(g->tet);
                    }
                    break;

                // Flèche droite
                case SDLK_RIGHT:
                    if (g->state == STATE_PLAY)
                    {
                        if (tetris_can_go_right(g->tet))
                            tetris_move_right(g->tet);
                    }
                    break;

                // Flèche bas
                case SDLK_DOWN:
                    if (g->state == STATE_PLAY)
                    {
                        if (tetris_can_go_down(g->tet))
                            tetris_move_down(g->tet);
                    }
                    break;

                // Flèche haut => rotation
                case SDLK_UP:
                    if (g->state == STATE_PLAY)
                        tetris_rotate(g->tet);
                    break;

                default:
                    break;
                }
            }
        }

        // -------------------
        // 2) DESCENTE AUTOMATIQUE
        // -------------------
        // On ne fait descendre la pièce *QUE* si on est en mode PLAY
        if (g->state == STATE_PLAY)
        {
            current_time = SDL_GetPerformanceCounter();
            elapsed = (float)(current_time - last_time) / (float)g->freq;

            if (elapsed > tetris_get_drop_speed(g->tet))
            {
                int oldLines = g->tet->nbr_lines; // Nombre de lignes avant
                int oldLevel = g->tet->level;     // Niveau avant

                int ret = tetris_move_down(g->tet);
                if (ret == -1)
                {
                    Mix_PlayChannel(-1, g->soundGameOver, 0);
                    g->state = STATE_GAMEOVER;
                    Mix_HaltMusic();
                }
                else if (ret == 0)
                {
                    // => Le bloc vient de se poser
                    Mix_PlayChannel(-1, g->soundLand, 0);

                    // Vérifie les changements après tetris_shift_board
                    int newLines = g->tet->nbr_lines; // Lignes après
                    int newLevel = g->tet->level;     // Niveau après

                    if (newLines > oldLines)
                    {
                        Mix_PlayChannel(-1, g->soundLineClear, 0); // Jouer le son pour les lignes cassées
                    }

                    if (newLevel > oldLevel)
                    {
                        Mix_PlayChannel(-1, g->soundLevel, 0); // Jouer le son pour le "level up"
                    }
                }
                last_time = current_time;
            }
        }
        // ----------------------------------
        // 3) Mise à jour de l'affichage (dessine l'écran)
        // ----------------------------------
        game_update(g);
    }
}
