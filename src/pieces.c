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

// Retourne 1 si le fou se déplace en diagonale sinon 0.
static int mouvement_fou(int distance_x, int distance_y)
{
    if (distance_x == distance_y)
        return 1;
    return 0;
}

// Retourne 1 si la dame se déplace en diagonale ou en ligne droite sinon 0.
static int mouvement_dame(int distance_x, int distance_y)
{
    if (mouvement_fou(distance_x, distance_y) || mouvement_tour(distance_x, distance_y))
        return 1;
    return 0;
}

static int mouvement_pion(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur, t_case en_passant)
{

    // Direction et ligne de départ selon la couleur des pions.
    int direction;
    int ligne_depart;

    if (joueur == BLANC)
    {
        ligne_depart = 6;
        direction = -1;
    }
    else
    {
        ligne_depart = 1;
        direction = 1;
    }

    // Distance horizontale sans signe : la prise peut se faire à gauche ou à droite.
    int distance_x = abs(coup.x_end - coup.x_start);
    // Écart vertical avec son signe : indique si le pion avance ou recule.
    int ecart_y = coup.y_end - coup.y_start;

    // Le pion avance d'une case tout droit si la case d'arrivée est vide.
    if (distance_x == 0 && ecart_y == direction && plateau[coup.y_end][coup.x_end] == CASE_VIDE)
        return 1;

    // Depuis sa ligne de départ le pion peut avancer de deux cases, si la case d'arrivée et la case du milieu sont vides.
    if (distance_x == 0 && ecart_y == direction * 2 && ligne_depart == coup.y_start && plateau[coup.y_start + direction][coup.x_start] == CASE_VIDE && plateau[coup.y_end][coup.x_end] == CASE_VIDE)
        return 1;

    // Condition de la prise de pièce en diagonale.
    if (distance_x == 1 && ecart_y == direction && plateau[coup.y_end][coup.x_end] != CASE_VIDE)
        return 1;

    // Prise en passant, le pion va en diagonale sur la case vide que le pion adverse vient de sauter en avançant de deux cases.
    if (distance_x == 1 && ecart_y == direction && coup.x_end == en_passant.x && coup.y_end == en_passant.y)
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

static int appartient_au_joueur(char piece, int joueur)
{
    if (joueur == BLANC && isupper(piece))
        return 1;
    if (joueur == NOIR && islower(piece))
        return 1;
    return 0;
}

// Vérifie si le coup respecte les règles.
// Retourne COUP_VALIDE si le coup est autorisé, sinon retourne la raison du refus.
t_resultat mouvement_valide(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], t_coup coup, int joueur, t_case en_passant)
{

    // Nombre de cases parcourues horizontalement (x) et verticalement (y),
    // abs() rend le résultat positif : -2 devient 2,
    // On veut savoir de combien de cases la pièce bouge, pas dans quel sens.
    int distance_x = abs(coup.x_end - coup.x_start);
    int distance_y = abs(coup.y_end - coup.y_start);

    // Refuse le coup si la case de départ est une CASE_VIDE.
    if (plateau[coup.y_start][coup.x_start] == CASE_VIDE)
        return ERREUR_CASE_VIDE;

    // Refuse le coup si la pièce appartient à l'adversaire.
    if (!appartient_au_joueur(plateau[coup.y_start][coup.x_start], joueur))
        return ERREUR_PIECE_ADVERSE;

    // Refuse le coup si la case d'arrivée contient une pièce du même camp.
    if (meme_couleur(plateau[coup.y_start][coup.x_start], plateau[coup.y_end][coup.x_end]))
        return ERREUR_PRISE_MEME_CAMP;

    // Type de la pièce en majuscule pour traiter pièces blanches et noires de la même façon.
    char type = toupper(plateau[coup.y_start][coup.x_start]);

    // Vérifie la règle de déplacement propre à chaque pièce.
    switch (type)
    {
    // Cavalier : refuse le coup s'il ne forme pas un L.
    case 'C':
        if (!mouvement_cavalier(distance_x, distance_y))
            return ERREUR_CAVALIER;
        break;
    // Roi : refuse le coup s'il bouge de plus d'une case.
    case 'R':
        if (!mouvement_roi(distance_x, distance_y))
            return ERREUR_ROI;
        break;
    // Tour : refuse le coup s'il n'est pas en ligne droite.
    case 'T':
        if (!mouvement_tour(distance_x, distance_y))
            return ERREUR_TOUR;
        // Refuse le coup si une pièce bloque le chemin.
        if (!chemin_libre(plateau, coup))
            return ERREUR_CHEMIN_BLOQUE;
        break;
    // Fou : refuse le coup s'il n'est pas en diagonale.
    case 'F':
        if (!mouvement_fou(distance_x, distance_y))
            return ERREUR_FOU;
        // Refuse le coup si une pièce bloque le chemin.
        if (!chemin_libre(plateau, coup))
            return ERREUR_CHEMIN_BLOQUE;
        break;
        // Dame : refuse le coup s'il n'est pas une diagonale ou une ligne droite.
    case 'D':
        if (!mouvement_dame(distance_x, distance_y))
            return ERREUR_DAME;
        // Refuse le coup si une pièce bloque le chemin.
        if (!chemin_libre(plateau, coup))
            return ERREUR_CHEMIN_BLOQUE;
        break;
    case 'P':
        if (!mouvement_pion(plateau, coup, joueur, en_passant))
            return ERREUR_PION;
        break;
    }
    return COUP_VALIDE;
}