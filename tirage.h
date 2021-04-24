#ifndef TIRAGE_H
#define TIRAGE_H

#define NB_CARACTERE_CARTE 20
#define TAILLE_PAQUET 50

typedef struct Carte Carte;
struct Carte
{
 char nom[NB_CARACTERE_CARTE];
 char description[500];
 int numeroCarte;
};

int initialisationJeu(Carte paquet[], int choixjeu);
void melangerPaquet(Carte paquet[], int);
void couperPaquet(Carte paquet[]);
void chargementDescription(int ligneACharger, char chaineChargee[]);


#endif
