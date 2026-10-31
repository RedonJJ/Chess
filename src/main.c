#include "echecs.h"

int main(void)
{

    // Crée le plateau de 64 cases (8x8).
    char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    // Place les pièces de départ et remplit les autres cases de CASE_VIDE.
    init_plateau(plateau);

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