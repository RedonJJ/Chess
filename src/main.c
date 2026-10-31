#include "echecs.h"

int main(void)
{

    // Tableau représentant le plateau d'échec.
    char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU] = {
        {'t', 'c', 'f', 'd', 'r', 'f', 'c', 't'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'T', 'C', 'F', 'D', 'R', 'F', 'C', 'T'}};

    t_coup coup;

    while (1)
    {
        afficher_plateau(plateau);
        if (lire_coup(&coup) == 1)
        {
            deplacer_piece(plateau, coup);
        }
        else
        {
            printf("Coup invalide.\n");
        }
    }

    return 0;
}