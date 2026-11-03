#ifndef ECHECS_H
#define ECHECS_H
#define TAILLE_PLATEAU 8
#define TAILLE_SAISIE 10
#define CASE_VIDE '.'
#define BLANC 0
#define NOIR 1
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
void init_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]);
int lire_coup(t_coup *coup);
int mouvement_valide(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur);
void deplacer_piece(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup);

#endif