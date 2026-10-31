#include "echecs.h"

// Remplit le plateau avec la position de départ de chaque pièce.
void init_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU])
{
    // Ordre d'affichage des pièces de la première et de la dernière ligne.
    char *pieces = "tcfdrfct";

    // Pour chaque colonne j, place les pièces et pions des deux camps.
    for (int j = 0; j < TAILLE_PLATEAU; j++)
    {
        plateau[0][j] = pieces[j];          // Pièces noires.
        plateau[1][j] = 'p';                // Pions noirs.
        plateau[6][j] = 'P';                // Pions blancs.
        plateau[7][j] = toupper(pieces[j]); // Pièces blanches (toupper permet de passer les minuscules en majuscules).
    }
    // Remplit les lignes 2 à 5 avec des cases vides.
    for (int i = 2; i < 6; i++)
    {
        // Ajoute les points sur les cases vides.
        for (int j = 0; j < TAILLE_PLATEAU; j++)
        {
            plateau[i][j] = CASE_VIDE;
        }
    }
}

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
    plateau[coup.y_start][coup.x_start] = CASE_VIDE;
}