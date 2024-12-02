#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"


/*==========================
    Paramètres de l'écosystème
===========================*/

float p_ch_dir = 0.01;                // Probabilité de changer de direction
float p_reproduce_proie = 0.7;        // Probabilité de reproduction des proies
float p_reproduce_predateur = 0.5;    // Probabilité de reproduction des prédateurs
int temps_repousse_herbe = -15;       // Temps de repousse de l'herbe après consommation


/*==========================
      PARTIE 1 : Gestion des Animaux
===========================*/

/* 
  Créer un nouvel animal avec une position, une énergie, 
  et des directions aléatoires.
*/
Animal* creer_animal(int x, int y, float energie) {
    Animal* na = (Animal*)malloc(sizeof(Animal));
    assert(na);
    na->x = x;
    na->y = y;
    na->energie = energie;
    na->dir[0] = rand() % 3 - 1;    // Direction aléatoire en X
    na->dir[1] = rand() % 3 - 1;    // Direction aléatoire en Y
    na->suivant = NULL;
    return na;
}

/* 
  Ajouter un animal en tête de la liste 
  et s'assurer qu'il n'a pas de suivant.
*/
Animal* ajouter_en_tete_animal(Animal* liste, Animal* animal) {
    assert(animal);
    assert(!animal->suivant);
    animal->suivant = liste;
    return animal;
}

/*
  Ajouter un animal nouvellement créé à la liste des animaux.
*/
void ajouter_animal(int x, int y, float energie, Animal** liste_animal) {
     assert(x<SIZE_X && x>=0);
     assert(y<SIZE_Y && y >=0);
    Animal* new_animal = creer_animal(x, y, energie);
    assert(new_animal);
    *liste_animal=ajouter_en_tete_animal(*liste_animal,new_animal);
}

/*
  Supprimer un animal spécifique de la liste.
*/
void enlever_animal(Animal** liste, Animal* animal) {
    Animal* precedent = NULL;
    Animal* courant = *liste;

    while (courant != animal && courant != NULL) {
        precedent = courant;
        courant = courant->suivant;
    }
    if (courant == NULL) return;

    if (precedent == NULL) {
        *liste = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }
    free(courant);
    return;
}

/* 
  Libérer toute la liste d'animaux.
*/
Animal* liberer_liste_animaux(Animal* liste) {
    Animal* courant = liste;
    while (courant) {
    	Animal* temp=courant;
        courant=courant->suivant;
        free(temp);
        
    }
    return NULL;
}

/*==========================
        Comptage des Animaux
===========================*/

unsigned int compte_animal_rec(Animal* la) {
    if (!la) return 0;
    return 1 + compte_animal_rec(la->suivant);
}

unsigned int compte_animal_it(Animal* la) {
    int cpt = 0;
    while (la) {
        ++cpt;
        la = la->suivant;
    }
    return cpt;
}

/*==========================
        Affichage et Sauvegarde de l'Écosystème
===========================*/

/*
  Afficher l'écosystème avec les animaux dans un tableau.
*/
void afficher_ecosys(Animal* liste_proie, Animal* liste_predateur) {
    unsigned int i, j;
    char ecosys[SIZE_X][SIZE_Y];
    Animal* pa = NULL;

    // Initialisation du tableau
    for (i = 0; i < SIZE_X; ++i) {
        for (j = 0; j < SIZE_Y; ++j) {
            ecosys[i][j] = ' ';
        }
    }

    // Ajouter les proies
    pa = liste_proie;
    while (pa) {
        ecosys[pa->x][pa->y] = '*';
        pa = pa->suivant;
    }

    // Ajouter les prédateurs
    pa = liste_predateur;
    while (pa) {
        ecosys[pa->x][pa->y] = (ecosys[pa->x][pa->y] == '*') ? '@' : 'O';
        pa = pa->suivant;
    }

    // Affichage du tableau
    printf("+");
    for (j = 0; j < SIZE_Y; ++j) {
        printf("-");
    }
    printf("+\n");

    for (i = 0; i < SIZE_X; ++i) {
        printf("|");
        for (j = 0; j < SIZE_Y; ++j) {
            putchar(ecosys[i][j]);
        }
        printf("|\n");
    }

    printf("+");
    for (j = 0; j < SIZE_Y; ++j) {
        printf("-");
    }
    printf("+\n");

    printf("Nb proies : %5d\tNb prédateurs : %5d\n", 
           compte_animal_it(liste_proie), compte_animal_it(liste_predateur));
}

/* 
  Sauvegarder l'état de l'écosystème dans un fichier.
*/
void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie) {
    FILE* fichier = fopen(nom_fichier, "w");
    if (!fichier) return;

    fprintf(fichier, "<proies>\n");
    for (Animal* courant = liste_proie; courant; courant = courant->suivant) {
        fprintf(fichier, "x=%d y=%d dir=[%d %d] e=%f\n", 
                courant->x, courant->y, courant->dir[0], courant->dir[1], courant->energie);
    }
    fprintf(fichier, "</proies>\n");

    fprintf(fichier, "<predateurs>\n");
    for (Animal* courant = liste_predateur; courant; courant = courant->suivant) {
        fprintf(fichier, "x=%d y=%d dir=[%d %d] e=%f\n", 
                courant->x, courant->y, courant->dir[0], courant->dir[1], courant->energie);
    }
    fprintf(fichier, "</predateurs>\n");

    fclose(fichier);
}

/*==========================
     Lecture de l'écosystème
===========================*/

void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) return;

    char ligne[256];
    int x, y, dir_x, dir_y;
    float energie;
    char type[16];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '<') {
            sscanf(ligne, "<%[^>]", type);
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '<') {
                if (ligne[0] == 'x') {
                    sscanf(ligne, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir_x, &dir_y, &energie);
                    Animal* new_animal = creer_animal(x, y, energie);
                    new_animal->dir[0] = dir_x;
                    new_animal->dir[1] = dir_y;

                    if (strcmp(type, "proies") == 0) {
                        ajouter_animal(x, y, energie, liste_proie);
                    } else {
                        ajouter_animal(x, y, energie, liste_predateur);
                    }
                }
            }
        }
    }
    fclose(fichier);
}

/*==========================
        Partie 2 : Dynamique des Animaux
===========================*/

/* 
  Déplacement des animaux dans le monde.
*/
void bouger_animaux(Animal* la) {


    while (la) {
        // Décision de changement de direction selon la probabilité
        if ((rand() % 100) / 100.0 < p_ch_dir) {
            // Changer de direction de manière aléatoire
            la->dir[0] = (rand() % 3) - 1;  // Direction aléatoire entre -1, 0, 1
            la->dir[1] = (rand() % 3) - 1;  // Direction aléatoire entre -1, 0, 1
        }

        // Déplacer l'animal selon sa direction
        la->x = (la->x + la->dir[0] + SIZE_X) % SIZE_X;
        la->y = (la->y + la->dir[1] + SIZE_Y) % SIZE_Y;

        // Passer à l'animal suivant
        la = la->suivant;
    }
}

/* 
  Reproduction des animaux avec probabilité donnée.
*/
void reproduce(Animal** liste_animal, float p_reproduce) {

	if(!liste_animal || !(*liste_animal) || p_reproduce==0.0)return;
    Animal* courant = *liste_animal;
    while (courant) {
        if ((float)rand() / RAND_MAX <= p_reproduce && courant->energie > 0) {
            Animal* fils = creer_animal(courant->x, courant->y, courant->energie / 2.0);
            *liste_animal = ajouter_en_tete_animal(*liste_animal, fils);
            courant->energie /= 2.0;
        }
        courant = courant->suivant;
    }
}



/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    if (!liste_proie || !(*liste_proie)) return;

    Animal *courant = *liste_proie;
    Animal *prev = NULL;
    
    bouger_animaux(courant);

    while (courant) {
        
        // Réduit l'énergie de la proie
        courant->energie -= 1;

        // Vérifie si la proie mange de l'herbe
        if (monde[courant->x][courant->y] > 0) {
            // La proie mange l'herbe et gagne de l'énergie
            courant->energie += monde[courant->x][courant->y];

            // Réinitialise l'herbe sur la case et met à jour le temps de repousse
            monde[courant->x][courant->y] = temps_repousse_herbe;
        }

        // Si l'énergie de la proie est épuisée, elle est retirée de la liste
        if (courant->energie <= 0) {
            Animal *temp = courant;
            courant = courant->suivant;  // Avance à l'animal suivant
            enlever_animal(liste_proie, temp);
        } else {
            courant = courant->suivant;  // Avance à l'animal suivant
        }
    }
    // Reproduction des proies
    reproduce(liste_proie, p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    while (l) {
        if (l->x == x && l->y == y) {
            return l;
        }
        l = l->suivant;
    }
    return NULL;
}

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
    if (!liste_predateur || !(*liste_predateur)) return;
    

    Animal *courant = *liste_predateur;
    
     // Déplace le prédateur
        bouger_animaux(courant);

    while (courant) {
       

        // Réduit l'énergie du prédateur
        
        courant->energie -= 1;

        // Vérifie s'il y a une proie sur la même case
        Animal *proie = animal_en_XY(*liste_proie, courant->x, courant->y);

        if (proie) {
            // Le prédateur mange la proie et récupère son énergie
            courant->energie += proie->energie;

            // Enlève la proie de la liste des proies
            enlever_animal(liste_proie, proie);
        }

        // Si l'énergie du prédateur est épuisée, il est retiré de la liste
        if (courant->energie <= 0) {
            Animal *temp = courant;
            courant = courant->suivant;
            enlever_animal(liste_predateur, temp);
        } else {
            courant = courant->suivant;
        }
    }
    // Reproduction des prédateurs
    reproduce(liste_predateur, p_reproduce_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]) {
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            // Incrémenter la quantité d'herbe sur chaque case
            monde[i][j] += 1;
        }
    }
}

