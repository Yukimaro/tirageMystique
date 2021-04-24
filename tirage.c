
/* FICHIER DE DEFINITIONS DES FONCTIONS PROPRES ÀU TIRAGE
CHARGE LES CARTES, MELANGE LE PAQUET,
COUPE LE PAQUET EN 2 ET CHARGER LE TIRAGE
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tirage.h"
#include <string.h>


int initialisationJeu(Carte paquet[TAILLE_PAQUET], int choixJeu)
{
 FILE *fichier = NULL;
 int ligneFichier = 0;
 int caractereActuel = 0;
 int i=0, j=0;

 //OUVERTURE DU FICHIER JEUXDECARTE EN MODE LECTEUR SEUL
 fichier = fopen("jeuxDeCarte.txt" ,"r");

 //SI LOUVERTURE A REUSSIE
 if(fichier != NULL)
 {
  //BOUCLE PERMETTANT DE CHOISIR LA BONNE LIGNE DES NOMS CARTES A INITIALISER
  //ET QUI PARCOURT UN A UN LES CARACTERES D'UNE LIGNE DU FICHIER
  do
  {
   //INCREMENTATION DE LA LIGNE DU FICHIER SI ON ARRIVE EN BOUT DE LIGNE
   if(caractereActuel == '\n')
   {
    ligneFichier++;
    j=0;
   }
   caractereActuel = fgetc(fichier);
   //CONDITION POUR PASSER AU NOM DE LA CARTE SUIVANTE SUR LA LIGNE
   if(caractereActuel == ';')
   {
    j++;
    i=0;
   }
   //CONDITION POUR SAUVEGARDER LE NOM DES CARTES DU JEU CHOISIT PAR L'UTILISATEUR
   //EN EVITANT D'ENREGISTRER LE CARACTERE RETOUR A LA LIGNE '\n'
   else if((choixJeu == ligneFichier) && (caractereActuel != '\n'))
   {
    paquet[j].nom[i] = caractereActuel;
    paquet[j].numeroCarte = j;
    i++;
   }
  }while((ligneFichier != choixJeu ) || (caractereActuel != '\n'));

  //FERMETURE DU FICHIER, LIBERATION DE LA MEMOIRE
  fclose(fichier);
 }

 //SI L'OUVERTURE DU FICHIER NE FONCTIONNE PAS
 else
 {
  printf("ERREUR OUVERTURE FICHIER\n");
 }

 return j;
}

//--------------------------------------------------------------------

void melangerPaquet(Carte paquet[], int Nombre)
{
    srand(time(NULL));
    int NumeroCarte = 0;
    int i = 0;
    Carte stock = {"",""};
    Carte stock2 ={"",""};
    int transition = 0;
    for (i=0; i<=Nombre; i++)
    {
        NumeroCarte = (rand() % (Nombre - 0 + 1)) + 0;
        stock = paquet[NumeroCarte];
        transition = NumeroCarte;
        NumeroCarte = (rand() % (Nombre - 0 + 1)) + 0;
        stock2 = paquet[NumeroCarte];
        paquet[transition]=stock2;
        paquet[NumeroCarte]=stock;
    }
}

//--------------------------------------------------------------------

void couperPaquet(Carte paquet[])
{

}

//--------------------------------------------------------------------

void chargementDescription(int ligneACharger, char chaineChargee[])
{
 FILE *fichier=NULL;
 int i=0;
 char *positionEntree=NULL;

 fichier=fopen("descriptionCarte.txt", "r");
 if(fichier==NULL)
 {
  printf("ERREUR LORS DE L'OUVERTURE DU FICHIER\n");
  exit(0);
 }

 do
 {
  fgets(chaineChargee,550,fichier);
  i++; 
 }while(i != ligneACharger+1);

 positionEntree = strchr(chaineChargee, '\n');
 if (positionEntree != NULL)
 {	
  *positionEntree = '\0';
 }
 
fclose(fichier);
}
