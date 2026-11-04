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