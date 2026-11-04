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

// Une case du plateau x = colonne y = ligne.
typedef struct t_case
{

    // Colonne.
    int x;
    // Ligne.
    int y;

} t_case;

// COUP_VALIDE si le coup est autorisé, sinon la raison du refus.
typedef enum
{
    COUP_VALIDE,
    ERREUR_CASE_VIDE,
    ERREUR_PIECE_ADVERSE,
    ERREUR_PRISE_MEME_CAMP,
    ERREUR_CAVALIER,
    ERREUR_ROI,
    ERREUR_TOUR,
    ERREUR_FOU,
    ERREUR_DAME,
    ERREUR_PION,
    ERREUR_CHEMIN_BLOQUE,
    ERREUR_ROI_EN_ECHEC
} t_resultat;

void afficher_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]);
void init_plateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]);
int lire_coup(t_coup *coup);
t_resultat mouvement_valide(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur, t_case en_passant);
t_case trouver_roi(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur);
int case_attaque(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_case cible, int attaquant);
int est_en_echec(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur);
int laisse_roi_en_echec(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup, int joueur);
int a_un_coup_possible(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int joueur, t_case en_passant);
void deplacer_piece(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup);

#endif