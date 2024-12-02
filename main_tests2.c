#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"


int main (){
	srand(time(NULL));
	int i;
	Animal *liste_proie = NULL;
  	Animal *liste_predateur = NULL;
  	int energie=10;
  	const char* animaux= "animaux";
	
	
	for (i=0;i<20;i++){
		ajouter_animal(rand()%(SIZE_X+1), rand()%(SIZE_Y+1), energie, &liste_proie);
	}
	
	for (i=0;i<20;i++){
		ajouter_animal(rand()%(SIZE_X+1), rand()%(SIZE_Y+1), energie, &liste_predateur);
	}
	
	assert(compte_animal_rec(liste_proie)==20);
	assert(compte_animal_rec(liste_predateur)==20);
	assert(compte_animal_it(liste_predateur)==20);
	assert(compte_animal_it(liste_proie)==20);
	

	ecrire_ecosys(animaux,liste_predateur,liste_proie);
	afficher_ecosys(liste_proie,liste_predateur);



	liberer_liste_animaux(liste_predateur);
	liberer_liste_animaux(liste_proie);
	
	lire_ecosys(animaux,&liste_predateur, &liste_proie);
	
	
	return 0;
	
}
