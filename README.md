# Jeu d'échecs en C

Jeu d'échecs complet pour deux joueurs, qui se joue dans le terminal. Les deux joueurs jouent à tour de rôle sur le même ordinateur, en tapant leurs coups au clavier.

Projet réalisé en binôme dans le cadre d'une évaluation de langage C.

## Fonctionnalités

- Plateau 8 × 8 affiché dans le terminal, avec les coordonnées (A à H, 1 à 8)
- Alternance des tours entre les blancs et les noirs
- Déplacements de toutes les pièces : pion, cavalier, fou, tour, dame et roi
- Vérification que rien ne bloque le chemin (tour, fou et dame)
- Coups spéciaux :
  - **roque** (petit et grand), avec toutes ses conditions : roi et tour jamais déplacés, chemin libre, roi ni en échec ni sur une case attaquée pendant le roque
  - **prise en passant**
  - **promotion** du pion en dame
- **Échec** : annoncé au joueur, et un coup qui laisse son propre roi en échec est refusé
- **Échec et mat** et **pat** : détectés automatiquement, la partie s'arrête
- Un message d'erreur clair pour chaque coup refusé (format invalide, pièce adverse, mauvais déplacement, chemin bloqué, roque impossible, etc.)

## Compilation

Le projet se compile avec `make` (nécessite `gcc` et `make`) :

```
make        # compile le projet
make re     # recompile tout depuis zéro
make clean  # supprime les fichiers objets (.o)
make fclean # supprime les fichiers objets et l'exécutable
```

La compilation utilise les options `-Wall -Wextra -Werror`.

## Lancer une partie

```
./chess
```

Pour jouer, tapez la case de départ puis la case d'arrivée, séparées d'un espace :

```
Entrez votre coup : e2 e4
```

Les majuscules sont acceptées (`E2 E4`). Pour roquer, déplacez le roi de deux cases vers la tour (par exemple `e1 g1` pour le petit roque des blancs).

Pour quitter la partie, appuyez sur **Ctrl + D** (ou **Ctrl + C**).

### Les pièces

Les pièces blanches sont en **majuscules**, les pièces noires en **minuscules**. Les cases vides sont représentées par un point.

| Lettre | Pièce    |
|--------|----------|
| `R`    | Roi      |
| `D`    | Dame     |
| `T`    | Tour     |
| `F`    | Fou      |
| `C`    | Cavalier |
| `P`    | Pion     |

## Structure du projet

```
.
├── include/
│   └── echecs.h     # constantes, structures, enum des résultats et prototypes
├── src/
│   ├── main.c       # boucle de jeu, tours des joueurs, messages d'erreur et fin de partie
│   ├── plateau.c    # création, affichage du plateau et déplacement des pièces
│   ├── saisie.c     # lecture du coup au clavier et vérification du format
│   ├── pieces.c     # règles de déplacement de chaque pièce
│   └── regles.c     # échec, échec et mat, pat et roque
└── makefile
```

## Tests

Le jeu a été testé sur **200 parties complètes**, comparées coup par coup à un programme de référence indépendant qui applique les règles des échecs. À chaque saisie, le plateau affiché, l'acceptation ou le refus du coup et les annonces (échec, échec et mat, pat) ont été vérifiés.

| Élément testé | Nombre |
|---|---|
| Coups autorisés joués | 35 340 |
| Coups interdits essayés (tous refusés) | 26 591 |
| Saisies mal écrites (toutes refusées) | 14 509 |
| Échecs annoncés | 1 594 |
| Roques | 96 |
| Prises en passant | 38 |
| Promotions | 174 |
| Échecs et mat | 105 |
| Pats | 8 |
| **Erreurs détectées** | **0** |

Aucune fuite mémoire n'a été détectée avec Valgrind.

## Limites connues

- La promotion se fait toujours en dame.
- Les parties nulles par répétition, par la règle des 50 coups ou par manque de matériel ne sont pas gérées.
- La détection de l'échec et mat et du pat ne tient pas compte du roque. Cela ne change le résultat que dans des positions extrêmement rares, où le roque serait le seul coup possible.
