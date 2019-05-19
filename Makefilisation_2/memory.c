#include "fonctions.h"
#include "variables.h"

void *minuteurFonction(void *arg){
  int temps=0,Min=0;
    while(1){
      sleep(1);
      mvprintw(7,45,"%02d:%02d",Min,temps);
      refresh();
      temps=(temps+1)%60;
      if(temps==0) Min++;

    }
    return 0;
}

void AfficheLettres(char *cara,int haut,int lon){
  int z;
  int decalon=haut,decahaut=lon;
  char final[10];
  int i=0,j=0;
  int affichage[5][5];
  FILE* fichier = NULL;
  for(z=0;cara[z]!='\0';z++){
    if(cara[z]>='a' && cara[z]<='z'){
      sprintf(final,"alphabet/%c.txt",cara[z]);
            fichier = fopen( final, "r");
    
            if (fichier==NULL)
                      printf("%s\n",final);
        else{
          for(i=0;i<5;i++){
            fscanf(fichier,"%d %d %d %d %d",&affichage[i][0],&affichage[i][1],&affichage[i][2],&affichage[i][3],&affichage[i][4]);
          }
      
          for(i=0;i<5;i++){
            for(j=0;j<5;j++){
              if (affichage[i][j]==3)
               mvprintw(i+decalon,j+decahaut,"█");
        
            }
          }
          decahaut+=5;
        }   
      fclose(fichier);
    }
    else if(cara[z]==' ')
      decahaut+=2;
    else{
      decahaut=0;
      decalon+=6;
    }
    decahaut++;
  } 
}

int VerifEnsembleTableau(int taille,int Tab[taille]){
  int Premier=Tab[0];
  int i;
  for (i = 0; i < taille; i++)
  {
    if(Tab[i]!=Premier) return 1;
  }
  return 0;
}

void ContenuBoiteAffiche(int ContenuCoord,int mode,char Text[6]){
    switch (mode){
      case 0:
        sprintf(Text, "%d",ContenuCoord );
        break;
      case 1:
        sprintf(Text, "%c",ContenuCoord+96);
        break;
      case 2:
        sprintf(Text, "%d",ContenuCoord+64);
        break;
    }

}

int menu(char *chaine,...){
  va_list ap;
  va_start(ap,chaine);
  int r,NombreMot=0;
  char touche,taille[20];
  system("clear");

  printf("\033[%d;%dH",1,1);
  do
  {
      taille[NombreMot]=strlen(chaine)+3;
      NombreMot++;
      printf("%s\n",chaine);
      chaine = va_arg(ap, char *);
  } while(chaine != NULL);

  va_end(ap);

  printf("\033[%d;%dH",2,taille[1]);
  printf("<");
  touche=0;
  sleep(1);
  r=2;
    while(touche==0){
      touche=key_pressed();
      
      if (touche==122)
      {
        touche=0;
        if (r!=2){
          printf("\033[%d;%dH",r,taille[r-1]);
          printf(" ");
          r--;
          printf("\033[%d;%dH",r,taille[r-1]);
          printf("<");
          touche=0;
        }
      }
      else if (touche==115)
      {
        touche=0;
        if(r!=NombreMot)
        {
          printf("\033[%d;%dH",r,taille[r-1]);
          printf(" ");
          r++;
          printf("\033[%d;%dH",r,taille[r-1]);
          printf("<");
          touche=0;
        }
      }
      else if (touche==10)
      {
        
      }
      else
        touche=0;
    }  
    printf("%d\n",r-1); 

  return r-1;
}

void ChoixTailleMemory(int *NombreHaut,int *NombreLong,int *NombreElement,int *NombreGroupe){
  printf("Dans la selection de taille");
  int CombinaisonVoulu[100][2],PositionCombinaison=0,PossibiliteVoulu,NombreCase; 
  if(EtatPc!=1 ){
    *NombreElement=*NombreGroupe=1;
    (*NombreElement)=menu("NombreElement","1","2","3","4","5","6","7","8","9",(char*)0);
    (*NombreGroupe)=menu("NombreGroupe","1","2","3","4","5","6","7","8","9",(char*)0);
    (NombreCase)=(*NombreElement)*(*NombreGroupe);
    InitialisationNcurses();
    move(0,0);
    printw("Il y a en tout %d cases a faire",NombreCase);
    refresh();
    move(1,0); printw("Les differentes combinaisons possibles sont: ");
    for (int i = 2; i < NombreCase; i++)
    {
      if((NombreCase)%i==0){
        CombinaisonVoulu[PositionCombinaison][0]=i;
        CombinaisonVoulu[PositionCombinaison][1]=(NombreCase)/i;
        printw("\n            numero %d : %d * %d est possible ",PositionCombinaison,i,(NombreCase)/i);
        PositionCombinaison++;
      }
    }
    refresh();
    PossibiliteVoulu=Fleche(PositionCombinaison);
    *NombreLong=CombinaisonVoulu[PossibiliteVoulu][0];
    *NombreHaut=CombinaisonVoulu[PossibiliteVoulu][1];
    if(EtatPc==0){
      DonneesServeur[0]=*NombreLong;
      DonneesServeur[1]=*NombreHaut;
      DonneesServeur[2]=*NombreElement;
      DonneesServeur[3]=*NombreGroupe;
      mvprintw(10,10,"Attente client");
      //mvprintw(1,0,"serveur %d %d",DonneesServeur[0], DonneesServeur[1]);
      refresh();
      DialogueServeur(4,EtatPc);
      refresh();
    }
    //sleep(2);
    //send(ma_socket,NombreLong)
    //mvprintw(0,0,"Serveur");
    refresh();
    sleep(1);
  }
  else if (EtatPc==1){
    
    Init_Client();
    EtatPc=1;
    mvprintw(10,10,"Attente du serveur");
 
    DialogueClient(4,EtatPc);
    refresh();
    sleep(2);
    *NombreLong=DonneesClient[0];
    *NombreHaut=DonneesClient[1];
    *NombreElement=DonneesClient[2];
    *NombreGroupe=DonneesClient[3];
    refresh();
    sleep(1);
  }
  NombreCase=(*NombreHaut)*(*NombreLong);
  endwin();
}

void CreationMemory(int NombreHaut,int NombreLong,int NombreElement,int NombreGroupe, int Contenuboite[NombreHaut*NombreLong],int GrandeTaille){
  system("clear");
  int NombreCase=NombreLong*NombreHaut,PositionAleatoire;
  WINDOW *boite[100];
  for (int x = 0;x <NombreLong ; x++)
  {
    for (int y = 0;y <NombreHaut ; y++)
      {
       boite[NombreLong*y+x+1]= subwin(stdscr, 5+GrandeTaille, 6+GrandeTaille, 4+y*(4+GrandeTaille),7+x*(5+GrandeTaille));  
       wborder(boite[NombreLong*y+x+1], '|', '|', '-', '-', '*', '*', '*', '*');    
       wrefresh(boite[NombreLong*y+x+1]);
    //   sleep(1);
    //   mvprintw(40,10,"%2d %2d %4d",x, y, NombreLong*y+x+1);
    //   refresh();
       }    
  }
  attron(COLOR_PAIR(COLOR_BLACK));
  refresh();
  //sleep(3);


  for (int y = 0;y <NombreCase+4 ; y++){Contenuboite[y]=0;}
    for (int i = NombreElement;i <NombreCase+NombreElement ; i++)
    {
      do{
        PositionAleatoire=rand()%NombreCase;
      }while(Contenuboite[PositionAleatoire]!=0);
      Contenuboite[PositionAleatoire]=(i)/NombreElement;
      DonneesServeur[PositionAleatoire]=Contenuboite[PositionAleatoire];
   }
    if(EtatPc==0)
      DialogueServeur(NombreCase,EtatPc);
    else if(EtatPc==1){
      DialogueClient(NombreCase,EtatPc);
    for (int i = 0; i < NombreCase; i++)
    {
      Contenuboite[i]=DonneesClient[i];
    }
  }
  //mvprintw(34,0,"fin CreationMemory");
}

void LANCEMENT_JEU(int LongueurVoulu,int HauteurVoulu,int NombreElement,int NombreGroupe,int Contenuboite[16],int GrandeTaille )
{
  int HauteurTableau,LongueurTableau,Revele[NombreElement],NbrTour=0,Hauteur[NombreElement],Longueur[NombreElement],JoueurEnCours=0,Score[2];
  pthread_t minuteur;
  pthread_create(&minuteur, NULL, minuteurFonction, NULL);
  Score[0]=0;
  Score[1]=0;
  if(EtatPc==2){
    mvprintw(1,1,"Vous êtes le seul joueur");
  }
  else{
    mvprintw(1,1,"Vous êtes joueur %d",EtatPc+1);
    mvprintw(2,1,"Joueur 1: 0");
    mvprintw(3,1,"Joueur 2: 0");
  }
  int i;
  char ContenuAffiche[4];
  for (int j = 0; j < NombreGroupe;)
  {
    if(EtatPc!=2){
      if(JoueurEnCours==0){
        move(3,0);
        printw(" ");
        move(2,0);
      }
      else{
        move(2,0);
        printw(" ");
        move(3,0);
      }
      printw(">"); refresh();
    }
    i=0;
    
        while( i<NombreElement){
            if(JoueurEnCours==EtatPc || EtatPc==2 || EtatPc==3){
              click_souris();
              HauteurTableau=((L-4)/7),LongueurTableau=((C-7)/9);
              if(HauteurTableau>=0&&HauteurTableau<=HauteurVoulu-1&&LongueurTableau>=0&&LongueurTableau<=LongueurVoulu-1&& Contenuboite[4*HauteurTableau+LongueurTableau]!=(-2)){
                move(0,0);
                printw("Vous cliquez sur la position (%3d, %3d) et vous etes dans la case (%d,%d) de Contenu %d\n", L, C,HauteurTableau,LongueurTableau,Contenuboite[4*HauteurVoulu+LongueurTableau]); 
                move(5+HauteurTableau*7,9+LongueurTableau*9);
                ContenuBoiteAffiche(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau],1,ContenuAffiche);
                if (GrandeTaille==0) printw("%s",ContenuAffiche);
                else AfficheLettres(ContenuAffiche,5+HauteurTableau*8,9+LongueurTableau*9);
                refresh();
                //move(32,30);printw("%d",i);refresh();
                fprintf(stderr ," L %d C %d case (%d,%d) [%s] ", L, C, HauteurTableau,LongueurTableau, ContenuAffiche);
                NbrTour++;
                fprintf(stderr ,">Joueur en cours %d EtatPc %d ", JoueurEnCours, EtatPc);
                if (EtatPc==0) {
                  DonneesServeur[0]=HauteurTableau;
                  DonneesServeur[1]=LongueurTableau;
                  fprintf(stderr ,"execute DialogueServeur(2,0) ");
                  DialogueServeur(2,0); 
                } else if(EtatPc==1){
                  DonneesClient[0]=HauteurTableau;
                  DonneesClient[1]=LongueurTableau;
                  fprintf(stderr ,"execute DialogueClient(2,0) ");
                  DialogueClient(2,0);
                }   
                fprintf(stderr ," H %d , L %d\n", HauteurTableau, LongueurTableau);
                Revele[i]=(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau]);
                Longueur[i]=LongueurTableau;
                Hauteur[i]=HauteurTableau;
              }
            }
            else{
                fprintf(stderr ,"<Joueur en cours %d EtatPc %d ", JoueurEnCours, EtatPc);
                if (EtatPc==0) {
                  DonneesServeur[0]=9;
                  DonneesServeur[1]=0;
                  fprintf(stderr ,"execute DialogueClient(2,1) ");
                  DialogueServeur(2,1); 
                  HauteurTableau=DonneesServeur[0];
                  LongueurTableau=DonneesServeur[1];
                } else if(EtatPc==1) {
                  fprintf(stderr ,"execute DialogueClient(2,1) ");
                  DialogueClient(2,1);
                  HauteurTableau=DonneesClient[0];
                  LongueurTableau=DonneesClient[1];
                }
                fprintf(stderr ," H %d , L %d\n", HauteurTableau, LongueurTableau);
                move(0,0);
                printw("L'autre joueur a choisi la case (%d,%d)\n", HauteurTableau,LongueurTableau); 
                Revele[i]=(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau]); 
                Longueur[i]=LongueurTableau;
                Hauteur[i]=HauteurTableau;
                move(5+HauteurTableau*3,9+LongueurTableau*5);ContenuBoiteAffiche(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau],1,ContenuAffiche);printw("%s",ContenuAffiche);
                refresh();
                fprintf(stderr ," L %d C %d case (%d,%d) [%s] ", L, C, HauteurTableau,LongueurTableau, ContenuAffiche);
            }

            i++;
         }
        sleep(1);  
        if ((VerifEnsembleTableau(NombreElement,Revele)==0))
        {
          j++;
          if (EtatPc!=2)
          {
            Score[JoueurEnCours]++;
            if(JoueurEnCours==0){
             move(2,11);
            }
            else{
             move(3,11);
            }
            printw("%d",Score[JoueurEnCours]);
            refresh();
          }
          for (int i = 0; i < NombreElement; i++)
          {
            Contenuboite[LongueurVoulu*Hauteur[i]+Longueur[i]]=(-2);
            //printw(" Localisation %d",LongueurVoulu*Hauteur[i]+Longueur[i]); refresh();
            sleep(1);
          }
          move(30,30);
          printw(" Un groupe de trouvé"); refresh();
          
        }

      JoueurEnCours=(JoueurEnCours+1)%2;

    for (int i = 0; i < NombreElement; i++) {
        Revele[i]=Longueur[i]=Hauteur[i]=(-1);
    }
      for (int x = 0;x <=LongueurVoulu ; x++){for (int y = 0;y <=HauteurVoulu ; y++){if(Contenuboite[LongueurVoulu*y+x]!=(-2)){move(5+y*3,9+x*5);printw("  ");refresh();}}}
      move(30,30);
      printw("                          ");
      refresh();

  }
  pthread_cancel(minuteur);
//  (Sec2)=time(NULL)-Sec+((Min)*60);
  move(31,30); printw("Partie terminee"); refresh();
  while(getchar()!='q');

}
