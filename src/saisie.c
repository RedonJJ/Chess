#include "echecs.h"

// Vérifie que c'est une lettre de la colonne (A à H).
static int colonne_valide(char c)
{
    // tolower accepte les majuscules (E2), il les convertis automatiquement en minuscules (e2).
    if (tolower(c) >= 'a' && tolower(c) <= 'h')
        return 1;
    return 0;
}

// Vérifie que c'est un chiffre de la ligne (1 à 8).
static int ligne_valide(char c)
{
    if (c >= '1' && c <= '8')
        return 1;
    return 0;
}

// Convertit une lettre de colonne en indice 'x' du tableau,
// un caractère est un nombre en C ('a' = 97, 'e' = 101),
// donc lettre - 'a' donne sa position : 'a' -> 0, 'e' -> 4, 'h' -> 7.
// tolower permet d'accepter aussi les lettre majuscules et les changera en minuscule (E -> e).
static int colonne_vers_x(char c)
{
    return tolower(c) - 'a';
}

// Convertit un chiffre de la ligne en indice 'y' du tableau,
// c - '0' transforme le caractère en nombre : '2' -> 2,
// TAILLE - nombre inverse le sens car la ligne 8 est en haut du plateau :
// ligne :  8 7 6 5 4 3 2 1,
// index : 0 1 2 3 4 5 6 7,
// Exemples : '2' -> 8(la taille du tableau) - 2(la valeur de '2' - '0') = 6.
static int ligne_vers_y(char c)
{
    return TAILLE_PLATEAU - (c - '0');
}

int lire_coup(t_coup *coup)
{
    char *saisie = malloc(TAILLE_SAISIE * sizeof(char));
    if (saisie == NULL)
        return 0;
    printf("Entrez votre coup : ");

    if (fgets(saisie, TAILLE_SAISIE, stdin) == NULL)
    {
        free(saisie);
        printf("\nFin de la partie.\n");
        exit(0);
    }

    // Vérifie à chaque index si la saisie de l'utilisateur est correct.
    if (!colonne_valide(saisie[0]) || !ligne_valide(saisie[1]) || saisie[2] != ' ' || !colonne_valide(saisie[3]) || !ligne_valide(saisie[4]) || saisie[5] != '\n')
    {
        printf("Format invalide. Exemple de coup : e2 e4\n");
        free(saisie);
        return 0;
    }

    coup->x_start = colonne_vers_x(saisie[0]);
    coup->y_start = ligne_vers_y(saisie[1]);
    coup->x_end = colonne_vers_x(saisie[3]);
    coup->y_end = ligne_vers_y(saisie[4]);

    free(saisie);

    return 1;
}