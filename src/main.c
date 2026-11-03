#include "echecs.h"

// Affiche un message correspondant à la raison du refus du coup.
static void afficher_erreur(t_resultat resultat)
{
    switch (resultat)
    {
    case ERREUR_CASE_VIDE:
        printf("Il n'y a aucune piece sur cette case.\n");
        break;
    case ERREUR_PIECE_ADVERSE:
        printf("Ce n'est pas ta piece.\n");
        break;
    case ERREUR_CHEMIN_BLOQUE:
        printf("Une piece bloque le chemin.\n");
        break;
    case ERREUR_PRISE_MEME_CAMP:
        printf("Tu ne peux pas prendre tes propres pieces.\n");
        break;
    case ERREUR_CAVALIER:
        printf("Le cavalier se deplace en L.\n");
        break;
    case ERREUR_ROI:
        printf("Le roi se deplace d'une seule case.\n");
        break;
    case ERREUR_TOUR:
        printf("La tour se deplace en ligne droite.\n");
        break;
    case ERREUR_FOU:
        printf("Le fou se deplace en diagonale.\n");
        break;
    case ERREUR_DAME:
        printf("La dame se deplace en diagonale ou en ligne droite.\n");
        break;
    case ERREUR_PION:
        printf("Le pion avance d'une case (jusqu'a deux au premier coup) et prend en diagonale.\n");
        break;
    case COUP_VALIDE:
        break;
    }
}

int main(void)
{
    // Crée le plateau de 64 cases (8x8).
    char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    // Place les pièces de départ et remplit les autres cases de CASE_VIDE.
    init_plateau(plateau);

    t_coup coup;

    int joueur = BLANC;

    t_case en_passant;

    en_passant.x = -1;
    en_passant.y = -1;

    printf("Pour jouer, tapez la case de depart puis la case d'arrivee separees d'un espace (ex: e2 e4).\n");
    while (1)
    {
        afficher_plateau(plateau);
        if (joueur == BLANC)
            printf("Au tour des blancs.\n");
        else
            printf("Au tour des noirs.\n");
        // Lit le coup du joueur (lire_coup affiche déjà un message si le format est faux).
        if (lire_coup(&coup) == 1)
        {
            // Vérifie si le coup respecte les règles établies.
            t_resultat resultat = mouvement_valide(plateau, coup, joueur, en_passant);
            if (resultat == COUP_VALIDE)
            {
                deplacer_piece(plateau, coup);
                // Passe la main à l'autre joueur.
                if (joueur == BLANC)
                    joueur = NOIR;
                else
                    joueur = BLANC;
            }
            else
                // Explique au joueur pourquoi son coup est refusé.
                afficher_erreur(resultat);
        }
    }
    return 0;
}