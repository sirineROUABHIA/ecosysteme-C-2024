# ecosysteme-C-2024
Présentation du Projet : Simulation d’un Écosystème

ROUABHIA Sirine 21318210 DANT2



Ce projet modélise un écosystème simplifié dans lequel interagissent proies, prédateurs et ressources (herbe) dans une grille 2D. Il permet d’observer les comportements de déplacement, reproduction, et survie des animaux en fonction de leur environnement.

Fonctionnement Principal :

    Initialisation : Le programme initialise un monde 2D vide (sans herbe) et génère des listes de proies et de prédateurs positionnés aléatoirement. Chaque animal possède une énergie de départ.

    Cycle de Vie des Animaux :
        Déplacement : Les animaux se déplacent aléatoirement à chaque itération.
        Interaction avec l’Herbe (Proies) : Les proies mangent l’herbe pour regagner de l’énergie ; l’herbe repousse après un certain temps.
        Interaction avec les Proies (Prédateurs) : Les prédateurs consomment les proies rencontrées, récupérant leur énergie.
        Reproduction : Les animaux se reproduisent selon un taux défini, générant de nouveaux individus.

    Conditions d’Énergie et Mortalité : Si l’énergie d’un animal atteint zéro, il meurt et est retiré de la liste.

    Simulation et Affichage : La simulation affiche l’état de l’écosystème à chaque itération jusqu'à un maximum de 200 itérations ou jusqu'à l'extinction des proies.

Instructions pour Lancer la Simulation :

    Compiler le programme avec "make", puis exécuter avec "./ecosys" . Le programme affiche les positions et états des animaux à chaque cycle, permettant de suivre leur évolution.
