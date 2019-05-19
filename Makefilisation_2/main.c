#include "fonctions.h"
#include "variables.h"

int main(int argc,char ** argv) {
 // system("play -q ’foret.mp3’ &");
  srand(time(NULL));
  int Contenuboite[100],NombreHaut,NombreLong,NombreElement,NombreGroupe,ChoixTuto,ChoixReseau,GrandeTaille=4;
  system("clear");
  int ChoixCategorie1=menu("Meu","Tuto","Solo","Duo","Reseau","Reglage","quitter",(char*)0);
  switch (ChoixCategorie1){
    case 1:
      EtatPc=2;
      ChoixTuto= menu("Niveau","2x2","2x3","2x8","3x3",(char*)0);
      InitialisationNcurses();
      switch(ChoixTuto){
        case 1:
          NombreHaut=2;NombreLong=2;NombreElement=2;NombreGroupe=2;
          break;
        case 2:
          NombreHaut=2;NombreLong=3;NombreElement=2;NombreGroupe=3;
          break;
        case 3:
          NombreHaut=4;NombreLong=4;NombreElement=2;NombreGroupe=8;
          break;
        case 4:
          NombreHaut=3;NombreLong=3;NombreElement=3;NombreGroupe=3;
          break;
      }
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      endwin();
      break;

    case 2:
      EtatPc=2;
      ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
      InitialisationNcurses();
      if((NombreHaut+NombreLong)>=12)
         GrandeTaille=0;
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      endwin(); 
      break; 

    case 3:
    EtatPc=3;
      ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
      InitialisationNcurses();
      if((NombreHaut+NombreLong)>=12) 
        GrandeTaille=0;
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
      break;


    case 4:
      printf("Adresse IP local de l'autre ordi ?");
      scanf("%s",SERVEURNAME);
      if(strcmp(SERVEURNAME,"dev") == 0){ 
         SERVEURNAME[0]='1';SERVEURNAME[1]='2';SERVEURNAME[2]='7';SERVEURNAME[3]='.';SERVEURNAME[4]='0';SERVEURNAME[5]='.';SERVEURNAME[6]='0';SERVEURNAME[7]='.';SERVEURNAME[8]='1';
      }
      while(getchar()!='\n');
      ChoixReseau=menu("Jouer en réseau","Rejoindre un joueur","Créer une partie",(char*)0);
      switch(ChoixReseau){
        case 1:
          EtatPc=1;
          Init_Client();
          ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
          if((NombreHaut+NombreLong)>=12) GrandeTaille=0;
          InitialisationNcurses();
          CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
          sleep(1);
          LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
          endwin();
          break;
        case 2:
          Init_Serveur();
          EtatPc=0;
          ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
          InitialisationNcurses();
          if((NombreHaut+NombreLong)>=12) GrandeTaille=0;
          CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
          LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite,GrandeTaille);
          endwin();
          break;
      } 
  } 
 EcriturePiece((NombreElement-1)*(NombreGroupe-1));
 return 0;
}
