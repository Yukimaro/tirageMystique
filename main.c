#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "tirage.h"

#define TAILLE_CARTE_X 185
#define TAILLE_CARTE_Y 300
#define TAILLEB_CARTE_X 92
#define TAILLEB_CARTE_Y 150
#define BOUTON_QUITTER 0
#define BOUTON_MELANGER 1
#define BOUTON_RECOMMENCER 1
#define BOUTON_TIRAGE 2

int AfficherTirage(SDL_Surface *ecran, SDL_Surface *imageDeFont, int *continuer, int nbCarte, int *selectionCarte, int selectionCartePrecedente, Carte paquet[TAILLE_PAQUET]);

void afficherMelanger(SDL_Surface *ecran, SDL_Surface *fondEcran, SDL_Rect positionFondEcran);

_Bool interpretationTirage(SDL_Surface *ecran, SDL_Surface *fondEcran, SDL_Rect positionFondEcran, int selection, Carte paquet[TAILLE_PAQUET]);

int testClicBouton(int positionSourisX, int positionSourisY, SDL_Rect positionBouton[], int tailleBoutonX[], int tailleBoutonY[], int nombreBouton)
{
 int i;

 //TEST SI IL Y A CLIC SUR BOUTON, SI OUI RETOURNE 1 SINON 0
 for(i=0; i<nombreBouton; i++)
 {
  //TEST POSITION DU CURSEUR DE LA SOURIS EN X
  if((positionSourisX>=positionBouton[i].x) && (positionSourisX<=(positionBouton[i].x+tailleBoutonX[i])))
  {
   //TEST POSITION DU CURSEUR DE LA SOURIS EN Y
   if((positionSourisY>=positionBouton[i].y) && (positionSourisY<=(positionBouton[i].y+tailleBoutonY[i])))
   {
    //SI LE CURSEUR EST SUR UN BOUTON RETOURNE LE NUMERO DU BOUTON
    return i;
   }
  }
 }
 return 40;
}

_Bool testClicCarte(int positionSourisX, int positionSourisY, SDL_Rect positionCarte[], int nbCarte, int *selectionCarte, int carteFinLigne[])
{
 int i, divise=2;

 //TEST SI IL Y A CLIC SUR CARTE, SI OUI RETOURNE 1 SINON 0
 for(i=0; i<=nbCarte; i++)
 {
  //TEST SI ON CLIC SUR UNE CARTE AFFICHER ENTIEREMENT OU PAS
  if((i==carteFinLigne[0]) || (i==carteFinLigne[1]) || (i==carteFinLigne[2]))
  {
   divise=1;
  }
  else
  {
   divise=2;
  }

  //TEST POSITION DU CURSEUR DE LA SOURIS EN X
  if((positionSourisX>=positionCarte[i].x) && (positionSourisX<=(positionCarte[i].x+(TAILLEB_CARTE_X/divise))))
  {
   //TEST POSITION DU CURSEUR DE LA SOURIS EN Y
   if((positionSourisY>=positionCarte[i].y) && (positionSourisY<=(positionCarte[i].y+(TAILLEB_CARTE_Y))))
   {
    //SI LE CURSEUR EST SUR UNE CARTE RETOURNE 1 SINON 0
    *selectionCarte=i;
    return 1;
   }
  }
 }
 return 0;
}

int main(int argc, char *argv[])
{
 //DECLARATION, INITIALISATION DES SURFACES ET DE LA GESTION EVENEMENT
 SDL_Surface *ecran=NULL;
 SDL_Surface *imageDeFond = NULL;
 SDL_Surface *apercuTirage=NULL;
 SDL_Surface *bouton[3] ={NULL};
 SDL_Rect positionFond;
 SDL_Rect positionApercuTirage;
 SDL_Rect positionBouton[3];
 SDL_Event event;
 int continuer=1, pause=1;
 int choixBouton=40;
 int clicBoutonDown=40;
 int nbCarte;
 int selectionCarte=0;
 int selectionCartePrecedente=40;
 int tailleBoutonX[]={94, 143, 88};
 int tailleBoutonY[]={27, 34, 27};
 _Bool restart=0;
 Carte paquet[TAILLE_PAQUET];

 //INITIALISATION DU PAQUET
  nbCarte=initialisationJeu(paquet, 1);

 //INITIALISATION DE LA SDL AVEC TEST ERREUR
 if(SDL_Init(SDL_INIT_VIDEO)== -1)
 {
  fprintf(stderr, "ERREUR D'INITIALISATION DDE LA SDL: %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
 }

 //ICONE FENETRE
 SDL_WM_SetIcon(IMG_Load("picture/icone/iconeTirageMystique.ico"), NULL);

 //INITIALISATION FENETRE TAILLE 800X600, COULEUR 32 BITS, MEMOIRE RAM
 ecran=SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE );

 //TITRE FENETRE
 SDL_WM_SetCaption("Tirage Mystique", NULL);

 if(ecran == NULL)
 {
  fprintf(stderr, "ERREUR CHARGEMENT MODE VIDEO : %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
 }

 //AFFECTATION DES POSITIONS DU FOND D'ECRAN ET DE LA CARTE SUR LA FENETRE
 positionFond.x=0;
 positionFond.y=0;
 positionApercuTirage.x=(800/3);
 positionApercuTirage.y=(600/4);
 positionBouton[BOUTON_QUITTER].x=25;
 positionBouton[BOUTON_QUITTER].y=500;
 positionBouton[BOUTON_MELANGER].x=positionApercuTirage.x+(TAILLE_CARTE_X/2)-(143/2);
 positionBouton[BOUTON_MELANGER].y=positionApercuTirage.y+(TAILLE_CARTE_Y+5);
 positionBouton[BOUTON_TIRAGE].x=positionBouton[BOUTON_MELANGER].x+(143/2)-(88/2);
 positionBouton[BOUTON_TIRAGE].y=positionBouton[BOUTON_MELANGER].y+40;

 //CHARGEMENT DES IMAGES DU FOND ECRAN ET DE LA CARTE
 imageDeFond=SDL_LoadBMP("picture/fdEcran/fdEcran-Tirage_Journalier.bmp");
 apercuTirage=IMG_Load("picture/carteMystique/verso.png");

 //BOUCLE ATTENDANT LA TOUCHE ECHAP OU CLIC SUR CROIX (MET EN PAUSE LE PROGRAMME)
 while((continuer) && (pause))
 {
  SDL_WaitEvent(&event);
  switch(event.type)
  {
   case SDL_QUIT:
        continuer=0;
	break;
   case SDL_KEYDOWN:
	switch(event.key.keysym.sym)
	{
	 case SDLK_ESCAPE:
              continuer=0;
              break;
	}
        break;
   case SDL_MOUSEBUTTONDOWN:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 clicBoutonDown=testClicBouton(event.button.x, event.button.y, positionBouton, tailleBoutonX, tailleBoutonY, 3);
	 choixBouton=40;
	}
	break;
   case SDL_MOUSEBUTTONUP:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 choixBouton=testClicBouton(event.button.x, event.button.y, positionBouton, tailleBoutonX, tailleBoutonY, 3);
	 clicBoutonDown=40;
	}
	break;
  }
  switch(choixBouton)
  {
   case BOUTON_QUITTER:
        continuer=0;
        break;
   case BOUTON_MELANGER:
        afficherMelanger(ecran, imageDeFond, positionFond);
        melangerPaquet(paquet, nbCarte);
        choixBouton = 40;
        break;
   case BOUTON_TIRAGE:
	pause=0;
        break;
  }

  //CHARGEMENT DE L'IMAGE BOUTON
  switch(clicBoutonDown)
  {
   case BOUTON_QUITTER:
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter_Actif.png");
        bouton[BOUTON_MELANGER]=IMG_Load("picture/bouton/boutonMelanger.png");
        bouton[BOUTON_TIRAGE]=IMG_Load("picture/bouton/boutonTirage.png");
        break;
   case BOUTON_MELANGER:
        bouton[BOUTON_MELANGER]=IMG_Load("picture/bouton/boutonMelanger_Actif.png");
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter.png");
        bouton[BOUTON_TIRAGE]=IMG_Load("picture/bouton/boutonTirage.png");
        break;
    case BOUTON_TIRAGE:
        bouton[BOUTON_TIRAGE]=IMG_Load("picture/bouton/boutonTirage_Actif.png");
        bouton[BOUTON_MELANGER]=IMG_Load("picture/bouton/boutonMelanger.png");
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter.png");
        break;
   default:
        bouton[BOUTON_MELANGER]=IMG_Load("picture/bouton/boutonMelanger.png");
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter.png");
        bouton[BOUTON_TIRAGE]=IMG_Load("picture/bouton/boutonTirage.png");
  }

  //COLLAGE DES IMAGES SUR LA FENETRE
  SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
  SDL_BlitSurface(apercuTirage, NULL, ecran, &positionApercuTirage);
  SDL_BlitSurface(bouton[BOUTON_QUITTER], NULL, ecran, &positionBouton[BOUTON_QUITTER]);
  SDL_BlitSurface(bouton[BOUTON_MELANGER], NULL, ecran, &positionBouton[BOUTON_MELANGER]);
  SDL_BlitSurface(bouton[BOUTON_TIRAGE], NULL, ecran, &positionBouton[BOUTON_TIRAGE]);

  //AFFICHAGE DE L'ECRAN
  SDL_Flip(ecran);
 }

 if(choixBouton==BOUTON_TIRAGE)
 {
  do
  {
   if(AfficherTirage(ecran, imageDeFond, &continuer, nbCarte, &selectionCarte, selectionCartePrecedente, paquet))
   {
    restart = interpretationTirage(ecran, imageDeFond, positionFond, selectionCarte, paquet);
    selectionCartePrecedente = selectionCarte;
   }
   else
   {
    restart = 0;
   }
  }while(restart);
 }

 //LIBERATION DE LA MEMOIRE
 SDL_FreeSurface(apercuTirage);
 SDL_FreeSurface(imageDeFond);
 SDL_FreeSurface(bouton[BOUTON_QUITTER]);
 SDL_FreeSurface(bouton[BOUTON_MELANGER]);
 SDL_FreeSurface(bouton[BOUTON_TIRAGE]);
 SDL_Quit();
 return EXIT_SUCCESS;
}

int AfficherTirage(SDL_Surface *ecran, SDL_Surface *imageDeFond, int *continuer, int nbCarte, int *selectionCarte, int selectionCartePrecedente, Carte paquet[TAILLE_PAQUET])
{
 //DECLARATION, INITIALISATION DES SURFACES ET DE LA GESTION EVENEMENT
 SDL_Surface *boutonQuitter=NULL;
 SDL_Surface *apercuTirage=NULL;
 SDL_Surface *apercuTirageRecto=NULL;
 SDL_Rect positionFond;
 SDL_Rect positionTirage[36];
 SDL_Rect positionBoutonQuitter;
 SDL_Event event;
 char chemin[50];
 int i, j=0;
 int choixBouton=40;
 int clicBoutonDown=40;
 _Bool clicCarte=0;
 int pause=1, suivant=0;
 int positionCarteTamponX, positionCarteTamponY;
 int carteFinLigne[3];
 int tailleBoutonX=94;
 int tailleBoutonY=27;

 //AFFECTATION DES POSITIONS DU FOND D'ECRAN ET DE LA CARTE SUR LA FENETRE
 positionFond.x=0;
 positionFond.y=0;
 positionCarteTamponX=10;
 positionCarteTamponY=100;
 positionBoutonQuitter.x=25;
 positionBoutonQuitter.y=570;

 //CHARGEMENT DES IMAGES DE CARTES VERSO
 apercuTirage=IMG_Load("picture/carteMystique/versoBis.png");

 //CHARGEMENT DES IMAGES DE CARTES RECTO
 sprintf(chemin, "picture/carteMystique/%dBis.png", paquet[selectionCartePrecedente].numeroCarte);
 apercuTirageRecto=IMG_Load(chemin);

 //COLLAGE DES IMAGES SUR LA FENETRE
 SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

 for(i=0; i<=nbCarte; i++)
 {
  if((positionCarteTamponX+TAILLEB_CARTE_X)>=800)
  {
   carteFinLigne[j]=i-1;
   positionCarteTamponY=positionCarteTamponY+TAILLEB_CARTE_Y+5;
   positionCarteTamponX=10;
   j++;
  }
  positionTirage[i].y=positionCarteTamponY;
  positionTirage[i].x=positionCarteTamponX;

  if(selectionCartePrecedente == i)
  { 
   SDL_BlitSurface(apercuTirageRecto, NULL, ecran, &positionTirage[i]);
  }
  else
  {
   SDL_BlitSurface(apercuTirage, NULL, ecran, &positionTirage[i]);
  }
  positionCarteTamponX=positionCarteTamponX+(TAILLEB_CARTE_X/2);
 }
 carteFinLigne[j]=nbCarte;

 //BOUCLE ATTENDANT LA TOUCHE ECHAP OU CLIC SUR CROIX (MET EN PAUSE LE PROGRAMME)
 while(*continuer && pause)
 {
  SDL_PollEvent(&event);
  switch(event.type)
  {
   case SDL_QUIT:
        *continuer=0;
	break;
   case SDL_KEYDOWN:
	switch(event.key.keysym.sym)
	{
	 case SDLK_ESCAPE:
              *continuer=0;
              break;
	}
        break;
   case SDL_MOUSEBUTTONDOWN:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 clicBoutonDown=testClicBouton(event.button.x, event.button.y, &positionBoutonQuitter, &tailleBoutonX, &tailleBoutonY, 1);
	 choixBouton=40;
	}
	break;
   case SDL_MOUSEBUTTONUP:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 choixBouton=testClicBouton(event.button.x, event.button.y, &positionBoutonQuitter, &tailleBoutonX, &tailleBoutonY, 1);
	 clicBoutonDown=40;
         clicCarte=testClicCarte(event.button.x, event.button.y, positionTirage, nbCarte, selectionCarte, carteFinLigne);
	}
	break;
  }

  //ACTION SUR BOUTON
  switch(choixBouton)
  {
   case BOUTON_QUITTER:
        *continuer=0;
	break;
  }

  //CHARGEMENT DE L'IMAGE BOUTON
  switch(clicBoutonDown)
  {
   case BOUTON_QUITTER:
        boutonQuitter=IMG_Load("picture/bouton/boutonQuitter_Actif.png");
        break;
   default:
        boutonQuitter=IMG_Load("picture/bouton/boutonQuitter.png");
  }
  
  //ACTION SUR CARTE
  if(clicCarte)
  {
   pause=0;
   suivant=1;
  }  

  //CHARGEMENT BOUTON ET TEXTE
  SDL_BlitSurface(boutonQuitter, NULL, ecran, &positionBoutonQuitter);

  //AFFICHAGE DE L'ECRAN
  SDL_Flip(ecran);
 }

 //LIBERATION DE LA MEMOIRE
 SDL_FreeSurface(apercuTirage);
 SDL_FreeSurface(apercuTirageRecto);
 SDL_FreeSurface(boutonQuitter);
 return suivant;
}

void afficherMelanger(SDL_Surface *ecran, SDL_Surface *fondEcran, SDL_Rect positionFondEcran)
{
 SDL_Surface *carteVerso=NULL;
 SDL_Rect positionPaquet, positionCarteActuel;
 SDL_Event event;
 int continuer=1, versLaDroite=1;
 int tempsPrecedent=0, tempsActuel=0, compteur=0;

 //CHARGEMENT DE L'IMAGE DU PAQUET ET CARTE A MELANGER
 carteVerso=IMG_Load("picture/carteMystique/versoBis.png");

 //INITIALISATION DES POSITIONS DU PAQUET ET CARTE A MELANGER
 positionPaquet.x=800/2-TAILLEB_CARTE_X/2;
 positionPaquet.y=600/2-TAILLEB_CARTE_Y/2;
 positionCarteActuel.x=positionPaquet.x;
 positionCarteActuel.y=positionPaquet.y;

 while(continuer)
 {
  SDL_PollEvent(&event);
  switch(event.type)
  {
   case SDL_QUIT:
        continuer=0;
        break;
  }

  //CHANGEMENT DIRECTION DE LA CARTE A MELANGER
  if(positionCarteActuel.x==TAILLEB_CARTE_X+positionPaquet.x+5)  
  {
   versLaDroite=0; 
  }
  else if(positionCarteActuel.x==positionPaquet.x)
  {
   versLaDroite=1;
   compteur++;
  }

  //DEFILEMENT IMAGE TOUTE LES 30MS
  tempsActuel=SDL_GetTicks();
  if(tempsActuel>=tempsPrecedent-30)
  {
   if(versLaDroite)
   {
    positionCarteActuel.x++;
   }
   else
   {
    positionCarteActuel.x--; 
   }

   tempsPrecedent=tempsActuel;
  }
  else
  {
   SDL_Delay(30-(tempsActuel-tempsPrecedent));
  }

  //QUITTE FONCTION A TROIS TOUR
  if(compteur==4)
  {
   continuer=0;
  }

  //COLLAGE DES IMAGES
  SDL_BlitSurface(fondEcran, NULL,ecran,&positionFondEcran);
  SDL_BlitSurface(carteVerso,NULL,ecran,&positionCarteActuel);
  SDL_BlitSurface(carteVerso,NULL,ecran,&positionPaquet);
  SDL_Flip(ecran);
 }
 SDL_FreeSurface(carteVerso);
}

_Bool interpretationTirage(SDL_Surface *ecran, SDL_Surface *fondEcran, SDL_Rect positionFondEcran, int selection, Carte paquet[TAILLE_PAQUET])
{
 SDL_Surface *carte=NULL;
 SDL_Surface *texteNom=NULL;
 SDL_Surface *texteDescription=NULL;
 SDL_Surface *bouton[2]={NULL};
 SDL_Rect positionCarte;
 SDL_Rect positionTexte;
 SDL_Rect positionTexteDescription;
 SDL_Rect positionBouton[2];
 SDL_Color couleurNoire = {0, 0, 0};
 SDL_Event event;
 _Bool continuer=1;
 _Bool recommencer=0;
 int choixBouton=40;
 int clicBoutonDown=40;
 int i=0, j=0;
 char chemin[50];
 char chaine[550]={""};
 char sousChaine[20]={""};
 int tailleBoutonX[]={94, 174};
 int tailleBoutonY[]={27, 30};
 TTF_Font *policeCarte = NULL;

 //CHARGEMENT DESCRIPTION DE LA CARTE SELECTIONNEE DANS CHAINE
 chargementDescription(paquet[selection].numeroCarte, chaine);

 //INITIALISATION IMAGE CARTE
 sprintf(chemin, "picture/carteMystique/%d.png", paquet[selection].numeroCarte);
 carte=IMG_Load(chemin);

 //INITIALISATION DE LA TTF PREPARATION POLICE ET CHARGEMENT TEXTURE TEXTENOM
 TTF_Init();
 policeCarte = TTF_OpenFont("font/nomCarte.ttf", 30);
 TTF_SetFontStyle(policeCarte, TTF_STYLE_UNDERLINE | TTF_STYLE_BOLD);
 texteNom=TTF_RenderText_Blended(policeCarte, paquet[selection].nom, couleurNoire);

 //CHANGEMENT POLICE DESCRIPTION
 policeCarte = TTF_OpenFont("font/descriptionCarte.ttf", 20);
 TTF_SetFontStyle(policeCarte, TTF_STYLE_NORMAL);

 //INITIALISATION POSITION 
 positionCarte.x=10;
 positionCarte.y=100;
 positionBouton[BOUTON_QUITTER].x=25;
 positionBouton[BOUTON_QUITTER].y=500;
 positionBouton[BOUTON_RECOMMENCER].x=25;
 positionBouton[BOUTON_RECOMMENCER].y=500 - (tailleBoutonY[1] + 10);
 positionTexte.x=400;
 positionTexte.y=100;
 positionTexteDescription.x=positionCarte.x+TAILLE_CARTE_X+5;
 positionTexteDescription.y=positionTexte.y+50;
 
 //COLLAGE DES IMAGES SUR LA FENETRE
 SDL_BlitSurface(fondEcran,NULL,ecran,&positionFondEcran); 
 SDL_BlitSurface(carte,NULL,ecran,&positionCarte); 
 SDL_BlitSurface(texteNom,NULL,ecran,&positionTexte); 

 //RECUPERATION DES MOTS DE LA DESCRIPTION 
 //COLLAGE DES MOTS SUR LE FOCUS DE L'ECRAN 
 while(chaine[i] != '\0')
 {
  if((chaine[i]!=' ') && (chaine[i+1] != '\0'))
  {
   sousChaine[j]=chaine[i];
   j++;  
  }
  else
  {
   texteDescription=TTF_RenderText_Blended(policeCarte, sousChaine, couleurNoire);
   if(positionTexteDescription.x + texteDescription->w > ecran->w)
   {
    positionTexteDescription.x=positionCarte.x+TAILLE_CARTE_X+5;
    positionTexteDescription.y=positionTexteDescription.y+texteDescription->h+1;
   }
   SDL_BlitSurface(texteDescription, NULL, ecran, &positionTexteDescription);
   positionTexteDescription.x=positionTexteDescription.x+texteDescription->w+4;
   do
   {
    sousChaine[j]='\0';
    j--;
   }while(j!=0);
  }
  i++;
 } 

 while(continuer)
 {
  SDL_PollEvent(&event);
  switch(event.type)
  {
   case SDL_QUIT:
        continuer=0;
        break;
   case SDL_KEYDOWN:
	switch(event.key.keysym.sym)
	{
	 case SDLK_ESCAPE:
              continuer=0;
              break;
	}
        break;
   case SDL_MOUSEBUTTONDOWN:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 clicBoutonDown=testClicBouton(event.button.x, event.button.y, positionBouton, tailleBoutonX, tailleBoutonY, 2);
	 choixBouton=40;
	}
	break;
   case SDL_MOUSEBUTTONUP:
	if(event.button.button== SDL_BUTTON_LEFT)
	{
	 choixBouton=testClicBouton(event.button.x, event.button.y, positionBouton, tailleBoutonX, tailleBoutonY, 2);
	 clicBoutonDown=40;
	}
	break;
  }

  //ACTION SUR BOUTON
  switch(choixBouton)
  {
   case BOUTON_QUITTER:
        continuer=0;
	break;
   case BOUTON_RECOMMENCER:
	recommencer = 1;
	continuer = 0;
	break;
  }

  //CHARGEMENT DE L'IMAGE BOUTON
  switch(clicBoutonDown)
  {
   case BOUTON_QUITTER:
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter_Actif.png");
        break;
   case BOUTON_RECOMMENCER:
	bouton[BOUTON_RECOMMENCER]=IMG_Load("picture/bouton/boutonRecommencer_Actif.png");
	break;
   default:
        bouton[BOUTON_QUITTER]=IMG_Load("picture/bouton/boutonQuitter.png");
	bouton[BOUTON_RECOMMENCER]=IMG_Load("picture/bouton/boutonRecommencer.png");
  }

  //COLLAGE IMAGE BOUTON QUITTER
  SDL_BlitSurface(bouton[BOUTON_QUITTER],NULL,ecran,&positionBouton[BOUTON_QUITTER]);
  SDL_BlitSurface(bouton[BOUTON_RECOMMENCER],NULL,ecran,&positionBouton[BOUTON_RECOMMENCER]);

  //AFFICHER ECRAN
  SDL_Flip(ecran);
 }

 //LIBERATION DE LA MEMOIRE
 SDL_FreeSurface(bouton[BOUTON_QUITTER]);
 SDL_FreeSurface(bouton[BOUTON_RECOMMENCER]);
 SDL_FreeSurface(carte);
 SDL_FreeSurface(texteNom);
 SDL_FreeSurface(texteDescription);
 TTF_CloseFont(policeCarte);
 TTF_Quit();

 return recommencer;
}
