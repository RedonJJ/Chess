#include "echecs.h"

int main(void)
{
    // Crée le plateau de 64 cases (8x8).
    char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    // Place les pièces de départ et remplit les autres cases de CASE_VIDE.
    init_plateau(plateau);

    t_coup coup;

    int joueur = BLANC;

    printf("Pour jouer, tapez la case de depart puis la case d'arrivee separees d'un espace (ex: e2 e4).\n");
    while (1)
    {
        afficher_plateau(plateau);
        if (joueur == BLANC)
            printf("Au tour des blancs.\n");
        else
        {
            printf("Au tour des noirs.\n");
        }

        if (lire_coup(&coup) == 1 && mouvement_valide(plateau, coup, joueur) == 1)
        {
            deplacer_piece(plateau, coup);
            if (joueur == BLANC)
                joueur = NOIR;
            else
                joueur = BLANC;
        }
    }

    return 0;
}