#ifndef ECHECS_H
#define ECHECS_H
#define TAILLE_PLATEAU 8
#define TAILLE_SAISIE 10
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct t_coup
{

    // Colonnes.
    int x_start;
    int x_end;
    // Lignes.
    int y_start;
    int y_end;

} t_coup;

void afficher_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]);
int lire_coup(t_coup *coup);
void deplacer_piece(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup);

#endif