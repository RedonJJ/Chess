#include "echecs.h"

// Affiche un message correspondant à la raison du refus du coup.
static void afficher_erreur(t_resultat resultat)
{
    switch (resultat)
    {
    case ERREUR_CASE_VIDE:
        printf("Il n'y a aucune piece sur cette case.\n");
        break;
    case ERREUR_PIECE_ADVERSE:
        printf("Ce n'est pas ta piece.\n");
        break;
    case ERREUR_CHEMIN_BLOQUE:
        printf("Une piece bloque le chemin.\n");
        break;
    case ERREUR_PRISE_MEME_CAMP:
        printf("Tu ne peux pas prendre tes propres pieces.\n");
        break;
    case ERREUR_CAVALIER:
        printf("Le cavalier se deplace en L.\n");
        break;
    case ERREUR_ROI:
        printf("Le roi se deplace d'une seule case.\n");
        break;
    case ERREUR_TOUR:
        printf("La tour se deplace en ligne droite.\n");
        break;
    case ERREUR_FOU:
        printf("Le fou se deplace en diagonale.\n");
        break;
    case ERREUR_DAME:
        printf("La dame se deplace en diagonale ou en ligne droite.\n");
        break;
    case ERREUR_PION:
        printf("Le pion avance d'une case (jusqu'a deux au premier coup) et prend en diagonale.\n");
        break;
    case ERREUR_ROI_EN_ECHEC:
        printf("Ce coup laisse ton roi en echec.\n");
        break;
    case ERREUR_ROQUE:
        printf("Roque impossible (roi ou tour deja deplaces, chemin occupe ou case attaquee).\n");
        break;
    case COUP_VALIDE:
        break;
    }
}

int main(void)
{
    // Crée le plateau de 64 cases (8x8).
    char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    // Place les pièces de départ et remplit les autres cases de CASE_VIDE.
    init_plateau(plateau);

    // Coup saisi par le joueur.
    t_coup coup;

    // Joueur dont c'est le tour (les blancs commencent).
    int joueur = BLANC;

    // Case sautée par un pion qui vient d'avancer de deux cases (-1 si aucune).
    t_case en_passant;

    en_passant.x = -1;
    en_passant.y = -1;

    // Droits de roque : tous permis au début de la partie.
    t_roque droits;
    droits.petit_blanc = 1;
    droits.grand_blanc = 1;
    droits.petit_noir = 1;
    droits.grand_noir = 1;

    printf("Pour jouer, tapez la case de depart puis la case d'arrivee separees d'un espace (ex: e2 e4).\n");
    while (1)
    {
        afficher_plateau(plateau);
        if (joueur == BLANC)
            printf("Au tour des blancs.\n");
        else
            printf("Au tour des noirs.\n");
        // Lit le coup du joueur (lire_coup affiche déjà un message si le format est faux).
        if (lire_coup(&coup) == 1)
        {

            // Vérifie si le coup respecte les règles établies.
            // Le roque a sa propre vérification, les autres coups passent par mouvement_valide.
            t_resultat resultat;
            if (est_un_roque(plateau, coup))
                resultat = verifier_roque(plateau, coup, joueur, droits);
            else
                resultat = mouvement_valide(plateau, coup, joueur, en_passant);

            // Vérifie si le coup ne laisse pas son roi en échec.
            if (resultat == COUP_VALIDE && laisse_roi_en_echec(plateau, coup, joueur) == 1)
                resultat = ERREUR_ROI_EN_ECHEC;
            if (resultat == COUP_VALIDE)
            {
                deplacer_piece(plateau, coup);

                // Le roi ou une tour a bougé (ou une tour a été prise) : met à jour les droits de roque.
                maj_droits_roque(&droits, coup);

                // Retient la case sautée si un pion vient d'avancer de deux cases sinon l'efface.
                if (toupper(plateau[coup.y_end][coup.x_end]) == 'P' && abs(coup.y_end - coup.y_start) == 2)
                {
                    en_passant.x = coup.x_end;
                    en_passant.y = (coup.y_end + coup.y_start) / 2;
                }
                else
                {
                    en_passant.x = -1;
                    en_passant.y = -1;
                }

                // Passe la main à l'autre joueur.
                if (joueur == BLANC)
                    joueur = NOIR;
                else
                    joueur = BLANC;

                // Fin de partie si le joueur n'a plus aucun coup autorisé.
                if (!a_un_coup_possible(plateau, joueur, en_passant))
                {
                    afficher_plateau(plateau);
                    if (est_en_echec(plateau, joueur))
                    {
                        printf("Echec et mat ! ");
                        // Le gagnant est l'autre joueur.
                        if (joueur == BLANC)
                            printf("Les noirs gagnent.\n");
                        else
                            printf("Les blancs gagnent.\n");
                    }
                    else
                        printf("Pat ! Match nul.\n");
                    return 0;
                }

                // Sinon, prévient le joueur si son roi est en échec.
                else if (est_en_echec(plateau, joueur))
                    printf("Echec !\n");
            }
            else
                // Explique au joueur pourquoi son coup est refusé.
                afficher_erreur(resultat);
        }
    }
    return 0;
}