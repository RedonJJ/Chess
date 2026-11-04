#include "echecs.h"

// Retourne la case où se trouve le roi du joueur.
t_case trouver_roi(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur)
{

    char roi;
    // Lettre du roi cherché, majuscule pour les blancs, minuscule pour les noirs.
    if (joueur == BLANC)
        roi = 'R';
    else
        roi = 'r';

    // Parcourt toutes les cases du plateau.
    for (int i = 0; i < TAILLE_PLATEAU; i++)
    {
        for (int j = 0; j < TAILLE_PLATEAU; j++)
        {
            // Case trouvée : on renvoie sa position.
            if (plateau[i][j] == roi)
            {
                t_case position;
                position.x = j;
                position.y = i;
                return position;
            }
        }
    }

    // Roi introuvable (ne devrait jamais arriver).
    t_case aucune;
    aucune.x = -1;
    aucune.y = -1;
    return aucune;
}

// Retourne 1 si la case cible est attaquée par une pièce du joueur attaquant, sinon 0.
int case_attaque(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_case cible, int attaquant)
{

    // Aucune prise en passant possible.
    t_case pas_en_passant;
    pas_en_passant.x = -1;
    pas_en_passant.y = -1;

    // Parcourt toutes les cases du plateau.
    for (int i = 0; i < TAILLE_PLATEAU; i++)
    {
        for (int j = 0; j < TAILLE_PLATEAU; j++)
        {
            // Coup imaginaire : de la case visitée vers la case cible.
            t_coup coup;
            coup.x_start = j;
            coup.y_start = i;
            coup.x_end = cible.x;
            coup.y_end = cible.y;

            // Si la pièce de cette case peut aller sur la cible, la cible est attaquée.
            if (mouvement_valide(plateau, coup, attaquant, pas_en_passant) == COUP_VALIDE)
                return 1;
        }
    }
    // Aucune pièce de l'attaquant ne peut atteindre la case.
    return 0;
}

// Retourne 1 si le roi du joueur est en échec sinon 0.
int est_en_echec(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur)
{
    // Position du roi du joueur.
    t_case roi = trouver_roi(plateau, joueur);

    // Le roi est en échec si sa case est attaquée par l'adversaire.
    if (joueur == BLANC)
        return case_attaque(plateau, roi, NOIR);
    return case_attaque(plateau, roi, BLANC);
}

// Retourne 1 si le coup laisse le roi en échec.
// Le coup est essayé sur une copie du plateau, le vrai plateau n'est pas modifié.
int laisse_roi_en_echec(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur)
{
    // Copie le plateau case par case.
    char copie[TAILLE_PLATEAU][TAILLE_PLATEAU];
    for (int i = 0; i < TAILLE_PLATEAU; i++)
    {
        for (int j = 0; j < TAILLE_PLATEAU; j++)
        {
            copie[i][j] = plateau[i][j];
        }
    }
    // Joue le coup sur la copie.
    deplacer_piece(copie, coup);

    // Le roi du joueur est-il en échec après ce coup ?
    return est_en_echec(copie, joueur);
}

// Retourne 1 si le joueur a au moins un coup autorisé, sinon 0.
// Sert à détecter l'échec et mat et le pat.
int a_un_coup_possible(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur, t_case en_passant)
{
    // Essaie chaque case de départ vers chaque case d'arrivée.
    for (int y_depart = 0; y_depart < TAILLE_PLATEAU; y_depart++)
    {
        for (int x_depart = 0; x_depart < TAILLE_PLATEAU; x_depart++)
        {
            for (int y_arriver = 0; y_arriver < TAILLE_PLATEAU; y_arriver++)
            {
                for (int x_arriver = 0; x_arriver < TAILLE_PLATEAU; x_arriver++)
                {
                    // Coup à essayer.
                    t_coup coup;
                    coup.x_start = x_depart;
                    coup.y_start = y_depart;
                    coup.x_end = x_arriver;
                    coup.y_end = y_arriver;

                    // Le coup respecte les règles et ne laisse pas le roi en échec.
                    if (mouvement_valide(plateau, coup, joueur, en_passant) == COUP_VALIDE && !laisse_roi_en_echec(plateau, coup, joueur))
                        return 1;
                }
            }
        }
    }
    // Aucun coup autorisé.
    return 0;
}