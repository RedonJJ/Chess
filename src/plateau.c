#include "echecs.h"
void afficher_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU])
{

    // Boucles permettant l'affichage du tableau dans le terminal.
    for (int i = 0; i < TAILLE_PLATEAU; i++)
    {
        printf("%d | ", TAILLE_PLATEAU - i); // Affiche les coordonnées de 1 à 8.
        for (int j = 0; j < TAILLE_PLATEAU; j++)
        {
            printf("%c  ", plateau[i][j]);
        }
        printf("\n");
    }
    // Printf permettant de facilité la lecture dans le terminal.
    printf("  +-----------------------\n");
    printf("    ");
    // Affiche les coordonnées de A à H dans une boucle.
    for (int j = 0; j < TAILLE_PLATEAU; j++)
    {
        printf("%c  ", 'A' + j);
    }

    printf("\n");
}

void deplacer_piece(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup)
{
    // Déplace la pièce de la case départ à la case arrivée.
    plateau[coup.y_end][coup.x_end] = plateau[coup.y_start][coup.x_start];
    // Vide la case de départ (ne vérifie pas les règles).
    plateau[coup.y_start][coup.x_start] = '.';
}