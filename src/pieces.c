#include "echecs.h"

// Retourne 1 si les deux pièces sont du même camp,
// majuscule == blancs, minuscule == noirs, sinon 0.
static int meme_couleur(char a, char b)
{
    if ((isupper(a) && isupper(b)) || (islower(a) && islower(b)))
        return 1;
    return 0;
}

// Vérifie si le coup respecte les règles.
// Retourne 1 si le coup est autorisé, sinon 0.
int mouvement_valide(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup)
{
    // Refuse le coup si la case de départ est une CASE_VIDE.
    if (plateau[coup.y_start][coup.x_start] == CASE_VIDE)
    {
        printf("Il n'y a aucune piece sur cette case.\n");
        return 0;
    }
    // Refuse le coup si la case d'arrivée contient une pièce du même camp.
    if (meme_couleur(plateau[coup.y_start][coup.x_start], plateau[coup.y_end][coup.x_end]))
    {
        printf("Tu ne peux pas prendre tes propres pieces.\n");
        return 0;
    }
    return 1;
}