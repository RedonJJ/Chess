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

            // Un pion n'attaque qu'en diagonale, d'une case vers l'avant, même si la case est vide (utile pour le roque).
            if ((attaquant == BLANC && plateau[i][j] == 'P') || (attaquant == NOIR && plateau[i][j] == 'p'))
            {
                int direction;
                if (attaquant == BLANC)
                    direction = -1;
                else
                    direction = 1;
                if (cible.y - i == direction && abs(cible.x - j) == 1)
                    return 1;
                continue;
            }

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

// Retourne 1 si le coup laisse le roi du joueur en échec, sinon 0.
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
            for (int y_arrivee = 0; y_arrivee < TAILLE_PLATEAU; y_arrivee++)
            {
                for (int x_arrivee = 0; x_arrivee < TAILLE_PLATEAU; x_arrivee++)
                {
                    // Coup à essayer.
                    t_coup coup;
                    coup.x_start = x_depart;
                    coup.y_start = y_depart;
                    coup.x_end = x_arrivee;
                    coup.y_end = y_arrivee;

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

// Retourne 1 si le coup ressemble à un roque : le roi part de sa case de départ (e1 ou e8)
// et se déplace de deux colonnes sur la même ligne, sinon 0.
int est_un_roque(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup)
{
    if (toupper(plateau[coup.y_start][coup.x_start]) == 'R' && coup.x_start == 4 && (coup.y_start == 0 || coup.y_start == 7) && coup.y_end == coup.y_start && abs(coup.x_end - coup.x_start) == 2)
        return 1;
    return 0;
}

// Vérifie si le roque demandé est permis.
// Retourne COUP_VALIDE si oui, sinon ERREUR_ROQUE (ou ERREUR_PIECE_ADVERSE).
t_resultat verifier_roque(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur, t_roque droits)
{

    int ligne = coup.y_start;
    int petit = (coup.x_end == 6); // 1 = petit roque (côté h), 0 = grand roque (côté a)

    // Le roi doit appartenir au joueur.
    if ((joueur == BLANC && plateau[ligne][4] != 'R') || (joueur == NOIR && plateau[ligne][4] != 'r'))
        return ERREUR_PIECE_ADVERSE;

    // Le roque demandé doit encore être permis (ni le roi ni la tour n'ont bougé).
    int permis;

    if (joueur == BLANC && petit)
        permis = droits.petit_blanc;
    else if (joueur == BLANC)
        permis = droits.grand_blanc;
    else if (petit)
        permis = droits.petit_noir;
    else
        permis = droits.grand_noir;
    if (!permis)
        return ERREUR_ROQUE;

    // Colonne de la tour, et sens dans lequel le roi se déplace.
    int x_tour;
    int pas;
    if (petit)
    {
        x_tour = 7;
        pas = 1;
    }
    else
    {
        x_tour = 0;
        pas = -1;
    }

    // Toutes les cases entre le roi et la tour doivent être vides.
    for (int x = 4 + pas; x != x_tour; x = x + pas)
    {
        if (plateau[ligne][x] != CASE_VIDE)
            return ERREUR_ROQUE;
    }

    // Le roi ne doit pas être en échec, ni traverser ou arriver sur une case attaquée.
    int adversaire;

    if (joueur == BLANC)
        adversaire = NOIR;
    else
        adversaire = BLANC;
    for (int k = 0; k <= 2; k++)
    {
        t_case c;
        c.x = 4 + k * pas;
        c.y = ligne;
        if (case_attaque(plateau, c, adversaire))
            return ERREUR_ROQUE;
    }
    return COUP_VALIDE;
}

// Met à jour les droits de roque après un coup joué.
// Un roque est perdu si le roi bouge, ou si la tour concernée quitte sa case ou y est prise.
void maj_droits_roque(t_roque *droits, t_coup coup)
{
    // Le roi blanc (e1) ou noir (e8) quitte sa case de départ : ses deux roques sont perdus.
    if (coup.x_start == 4 && coup.y_start == 7)
    {
        droits->petit_blanc = 0;
        droits->grand_blanc = 0;
    }
    if (coup.x_start == 4 && coup.y_start == 0)
    {
        droits->petit_noir = 0;
        droits->grand_noir = 0;
    }

    // Une tour quitte son coin de départ, ou une pièce y arrive (la tour est prise).
    if ((coup.x_start == 7 && coup.y_start == 7) || (coup.x_end == 7 && coup.y_end == 7))
        droits->petit_blanc = 0;
    if ((coup.x_start == 0 && coup.y_start == 7) || (coup.x_end == 0 && coup.y_end == 7))
        droits->grand_blanc = 0;
    if ((coup.x_start == 7 && coup.y_start == 0) || (coup.x_end == 7 && coup.y_end == 0))
        droits->petit_noir = 0;
    if ((coup.x_start == 0 && coup.y_start == 0) || (coup.x_end == 0 && coup.y_end == 0))
        droits->grand_noir = 0;
}
