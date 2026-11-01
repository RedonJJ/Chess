#include "echecs.h"

// Retourne 1 si les deux pièces sont du même camp,
// majuscule == blancs, minuscule == noirs, sinon 0.
static int meme_couleur(char a, char b)
{
    if ((isupper(a) && isupper(b)) || (islower(a) && islower(b)))
        return 1;
    return 0;
}

// Retourne 1 si le déplacement forme un L (2 cases dans un sens, 1 dans l'autre) sinon 0,
// Le cavalier saute par-dessus les pièces donc le chemin n'est pas vérifié.
static int mouvement_cavalier(int distance_x, int distance_y)
{
    if ((distance_x == 1 && distance_y == 2) || (distance_y == 1 && distance_x == 2))
        return 1;
    return 0;
}

// Retourne 1 si le roi se déplace d'une seule case (dans n'importe quelle direction), sinon 0.
static int mouvement_roi(int distance_x, int distance_y)
{
    if (distance_x <= 1 && distance_y <= 1)
        return 1;
    return 0;
}

// Retourne 1 si la tour se déplace en ligne droite (même ligne ou même colonne) sinon 0.
static int mouvement_tour(int distance_x, int distance_y)
{
    if (distance_x == 0 || distance_y == 0)
        return 1;
    return 0;
}

// Retourne 1 si n est positif, -1 si n est négatif et 0 si n est nul.
static int signe(int n)
{
    if (n > 0)
        return 1;
    if (n < 0)
        return -1;
    return 0;
}

// Retourne 1 si toutes les cases entre le départ et l'arrivée sont vides sinon 0.
static int chemin_libre(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup)
{
    // Sens du déplacement sur chaque axe : +1, -1 ou 0.
    int pas_x = signe(coup.x_end - coup.x_start);
    int pas_y = signe(coup.y_end - coup.y_start);

    // Première case après le départ.
    int x = coup.x_start + pas_x;
    int y = coup.y_start + pas_y;

    // Avance case par case jusqu'à l'arrivée.
    while (x != coup.x_end || y != coup.y_end)
    {
        // Une pièce bloque le chemin.
        if (plateau[y][x] != CASE_VIDE)
            return 0;
        // Case suivante.
        x = x + pas_x;
        y = y + pas_y;
    }
    return 1;
}

// Vérifie si le coup respecte les règles.
// Retourne 1 si le coup est autorisé, sinon 0.
int mouvement_valide(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup)
{

    // Nombre de cases parcourues horizontalement (x) et verticalement (y),
    // abs() rend le résultat positif : -2 devient 2,
    // On veut savoir de combien de cases la pièce bouge, pas dans quel sens.
    int distance_x = abs(coup.x_end - coup.x_start);
    int distance_y = abs(coup.y_end - coup.y_start);

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

    // Type de la pièce en majuscule pour traiter pièces blanches et noires de la même façon.
    char type = toupper(plateau[coup.y_start][coup.x_start]);

    // Vérifie la règle de déplacement propre à chaque pièce.
    switch (type)
    {
    // Cavalier : refuse le coup s'il ne forme pas un L.
    case 'C':
        if (!mouvement_cavalier(distance_x, distance_y))
        {
            printf("Le cavalier se deplace en L.\n");
            return 0;
        }
        break;
    // Roi : refuse le coup s'il bouge de plus d'une case.
    case 'R':
        if (!mouvement_roi(distance_x, distance_y))
        {
            printf("Le roi se deplace d'une seule case.\n");
            return 0;
        }
        break;
    // Tour : refuse le coup s'il n'est pas en ligne droite.
    case 'T':
        if (!mouvement_tour(distance_x, distance_y))
        {
            printf("La tour se deplace en ligne droite.\n");
            return 0;
        }
        // Refuse le coup si une pièce bloque le chemin.
        if (!chemin_libre(plateau, coup))
        {
            printf("Une piece bloque le chemin.\n");
            return 0;
        }
        break;
    }
    return 1;
}