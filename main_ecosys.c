#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 100000

int main(void) {
    // Initialisation de la graine pour les nombres aléatoires (Exercice 4, questions 2 et 4)
    srand(time(NULL));
    
    float energie_initiale = 10.0;
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    int monde[SIZE_X][SIZE_Y];  // Tableau représentant l'herbe dans chaque case

    // Initialisation du monde à 0 (aucune herbe au départ)
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            monde[i][j] = 0;
        }
    }

    // Création et test de déplacement d'un animal
    Animal *animal = creer_animal(0, 20, 10.0);  // Position initiale (0, 20)
    animal->dir[0] = 1;  // Direction horizontale : droite
    animal->dir[1] = 1;  // Direction verticale : bas

    printf("Position initiale : x=%d, y=%d\n", animal->x, animal->y);
    bouger_animaux(animal);
    printf("Après déplacement : x=%d, y=%d\n", animal->x, animal->y);

    // Comptage initial des animaux
    int nb_animaux_initial = compte_animal_it(animal);
    printf("Nombre d'animaux au départ : %d\n", nb_animaux_initial);


	
    // Création de 20 proies avec énergie initiale
    for (int i = 0; i < NB_PROIES; i++) {
        Animal *nouvelle_proie = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_initiale);
        liste_proie = ajouter_en_tete_animal(liste_proie, nouvelle_proie);
    }

    // Création de 20 prédateurs avec énergie initiale
    for (int i = 0; i < NB_PREDATEURS; i++) {
        Animal *nouveau_predateur = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_initiale);
        liste_predateur = ajouter_en_tete_animal(liste_predateur, nouveau_predateur);
    }

    int iteration = 0;

    // Affichage initial de l'écosystème
    afficher_ecosys(liste_proie, liste_predateur);

    // Boucle principale de simulation (jusqu'à 50 itérations ou disparition des proies)
    while (liste_proie != NULL &&  liste_predateur!= NULL && iteration < 200) {
        rafraichir_proies(&liste_proie, monde);           // Mise à jour des proies
        rafraichir_predateurs(&liste_predateur, &liste_proie);  // Mise à jour des prédateurs
        rafraichir_monde(monde);                          // Mise à jour de l'herbe

        afficher_ecosys(liste_proie, liste_predateur);    // Affichage de l'écosystème
        usleep(T_WAIT);                                   // Pause avant la prochaine itération

        iteration++;
    }

free(animal);
liberer_liste_animaux(liste_proie);
liberer_liste_animaux(liste_predateur);


    return 0;
}


