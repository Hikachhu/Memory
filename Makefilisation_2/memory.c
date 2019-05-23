#include "fonctions.h"
#include "variables.h"
/* Fonction de thread permettant de lancer la fonction du minuteur à afficher en parallèle */
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
/* Fonction permettant d'afficher une lettre à des coordonnées données */

void AfficheLettres(char *cara,int haut,int lon){
  int decalon=haut,decahaut=lon;
  char final[25];
  int i=0,j=0;
  int affichage;
  FILE* fichier = NULL;
    if(cara[0]>='a' && cara[0]<'z'){
      sprintf(final,"alphabet/%c.txt",cara[0]);
            fichier = fopen( final, "r");
    
            if (fichier==NULL)
                      printf("%s\n",final);
        else{
          for(i=0;i<5;i++){
            for(j=0;j<5;j++){
              fscanf(fichier,"%d",&affichage);
              if (affichage==3)
               mvprintw(i+decalon,j+decahaut,"█");
        
            }
          }
          decahaut+=5;
        }   
      fclose(fichier);
    }
    else if (cara[0]=='z'){
           for(i=0;i<5;i++){
            for(j=0;j<5;j++){
               mvprintw(i+decalon,j+decahaut," ");  
            }
          }
    } 
}
/* Fonction utilisé pour vérifié si tout les élément d'un tableau son égaux, dans ce cas un 0 est renvoyé sinon c'est un 1 */

int VerifEnsembleTableau(int taille,int Tab[taille]){
  int Premier=Tab[0];
  int i;
  for (i = 0; i < taille; i++)
  {
    if(Tab[i]!=Premier) return 1;
  }
  return 0;
}

/* Cette fonction valorise Text avec le contenu voulu en fonction du mode séléctionné et du contenu de la boite entré en paramètre */

void ContenuBoiteAffiche(int ContenuCoord,int mode,char Text[6]){
    char Examen[8]="bcefkhsu";
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
      case 3:
        sprintf(Text,"%c",Examen[ContenuCoord-1]);
        break;

    }

}

/* On prend une infinité de paramètre, le premier sert de titre au menu et le reste de choix possible. Le tout est encadré. */

int menu(char *chaine,...){
  WINDOW *boite;
  va_list ap;
  va_start(ap,chaine);
  int r,touche,NombreMot=0;
  int taille[20];
  clear();
  /* Partie permettant de créer le premier élément avec un soulignage et une couleur jaune */
  move(8,COLS/2-2);
  attron(A_UNDERLINE | COLOR_PAIR(2));
  taille[NombreMot]=strlen(chaine)+3;
  printw("%s\n",chaine);
  chaine = va_arg(ap, char *);
  attroff(A_UNDERLINE | COLOR_PAIR(2));

  NombreMot++;
  do /*Boucle Do-While utile à la lecture de tout les paramètres et à leurs affichages, de plus nous mémorisons la taille des mot lu pour plus tard afficher une flèches cohérente*/
  {
      taille[NombreMot]=(strlen(chaine));
      mvprintw(8+(NombreMot+1)*2,COLS/2+1-((taille[NombreMot])/2+0.5),"%s",chaine);
      chaine = va_arg(ap, char *);
      NombreMot++;
  } while(chaine != NULL);

  va_end(ap);

  /*Boite entourant le Menu */
  attron(COLOR_PAIR(2));
  boite= subwin(stdscr, 6+NombreMot*2, (Max(NombreMot-1,taille)+10), 6 ,COLS/2+(taille[0]/2)-Max(NombreMot-1,taille)-2);  
  wborder(boite, '|', '|', '-', '-', '*', '*', '*', '*');    
  wrefresh(boite);
  attroff(COLOR_PAIR(2));
  /*Gestion des déplacements dans le menu en ayant borné la fleche pour quelle ne dépasse pas du menu. Tout d'abord nous effaçons la flèche deja présente puis nous en créons une nouvelle aux coordonnées trouvés */
  mvprintw(12,COLS/2-5+taille[1]/2+8,"<");
  touche=0;
  refresh();
  r=2;
    while(touche==0){
      touche=getch();
      if (touche==259) /* touche vers le bas */
      {
        touche=0;
        if (r!=2){
          mvprintw(8+r*2,COLS/2-5+(taille[r-1])/2+8," ");
          r--;
          mvprintw(8+r*2,COLS/2-5+(taille[r-1])/2+8,"<");
          touche=0;
        }
      }
      else if (touche==258) /* touche vers le haut */
      {
        touche=0;
        if(r!=NombreMot)
        {
          mvprintw(8+r*2,COLS/2-5+(taille[r-1])/2+8," ");
          r++;
          mvprintw(8+r*2,COLS/2-5+(taille[r-1])/2+8,"<");
          touche=0;
        }
      }
      else if (touche==10)
      {
        
      }
      else
        touche=0;
    }  
    endwin();
  return r-1; /* retourne le rang ou se trouve le curseur -1 */
}
 /* cette fonction sert à trouver les hauteurs et longeurs permettant d'afficher la grille selon le nombre d'élément et de groupe voulu 
  Pour cela nous valorisons le coutenu de NombreHaut(La hauteur de la grille), NombreLong(La longeur) ainsi que le NombreElement et le Nombre Groupe */
void ChoixTailleMemory(int *NombreHaut,int *NombreLong,int *NombreElement,int *NombreGroupe){
  clear();
  int CombinaisonVoulu[100][2],PositionCombinaison=0,PossibiliteVoulu,NombreCase; 
  if(EtatPc!=1 ){
    *NombreElement=*NombreGroupe=1;
    (*NombreElement)=menu("NombreElement","1","2","3","4","5","6","7","8","9",(char*)0); /* Permet de séléctionner le nombre d'élément dans le Memory */
    (*NombreGroupe)=menu("NombreGroupe","1","2","3","4","5","6","7","8","9",(char*)0); /* Permet de selectionner le nombre de groupe dans le Memory */
    (NombreCase)=(*NombreElement)*(*NombreGroupe);
    clear();
    move(0,0);
    printw("Il y a en tout %d cases a faire",NombreCase);
    refresh();
    move(1,0); printw("Les differentes combinaisons possibles sont: ");
     /* Boucle for permettant de trouver les multiples du nombre de cases donnant ainsi les différentes disposition possible */
    for (int i = 2; i < NombreCase; i++)
    {
      if((NombreCase)%i==0){ /* un diviseur trouvé */
        CombinaisonVoulu[PositionCombinaison][0]=i;  /* Hauteur trouvé en fonction du diviseur */
        CombinaisonVoulu[PositionCombinaison][1]=(NombreCase)/i; /* longeur correspondant a la hauteur trouvé */
        printw("\n            numero %d : %d * %d est possible ",PositionCombinaison,i,(NombreCase)/i);  /* selection des possibite */
        PositionCombinaison++;
      }
    }

    refresh();
    PossibiliteVoulu=Fleche(PositionCombinaison);  /* Selection de la combinaison hauteur/longeur voulu */
    *NombreLong=CombinaisonVoulu[PossibiliteVoulu][0]; /* attribution de la longeur dans la variable final */
    *NombreHaut=CombinaisonVoulu[PossibiliteVoulu][1];  /* attribution de la hauteur dans la variable final */
    if(EtatPc==0){  /* transmission des information dans le cas du réseau */
      DonneesServeur[0]=*NombreLong;
      DonneesServeur[1]=*NombreHaut;
      DonneesServeur[2]=*NombreElement;
      DonneesServeur[3]=*NombreGroupe;
      mvprintw(10,10,"Attente client");
      refresh();
      DialogueServeur(4,EtatPc);
      refresh();
    }

    sleep(1);
  }
  else if (EtatPc==1){ /* reception des information dans le cas du réseau */
    
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
}

/* Cette fonction permet l'affichage et le remplissage de la grille de memory */
void CreationMemory(int NombreHaut,int NombreLong,int NombreElement,int NombreGroupe, int Contenuboite[NombreHaut*NombreLong][4],int GrandeTaille){
  clear();
  int NombreCase=NombreLong*NombreHaut,PositionAleatoire;
  WINDOW *boite[100];
  /* Affichage de la grille de memory en fonction de la hauteur voulu et de la longeur*/
  for (int x = 0;x <NombreLong ; x++)
  {
    for (int y = 0;y <NombreHaut ; y++)
      {
       boite[NombreLong*y+x+1]= subwin(stdscr, 5+GrandeTaille, 6+GrandeTaille, 4+y*(4+GrandeTaille),7+x*(5+GrandeTaille));  
       wborder(boite[NombreLong*y+x+1], '|', '|', '-', '-', '*', '*', '*', '*');    
       wrefresh(boite[NombreLong*y+x+1]);

       }    
  }
  refresh();

/* Remplissage aléatoire de la grille de memory */
  for (int y = 0;y <NombreCase+4 ; y++){Contenuboite[y][0]=0;}
    for (int i = NombreElement;i <NombreCase+NombreElement ; i++)
    {
      /* La boucle Do-While sert à trouver une position aléatoire où il n'y a pas encore de valeur */
      do{
        PositionAleatoire=rand()%NombreCase;
      }while(Contenuboite[PositionAleatoire][0]!=0);
      
      Contenuboite[PositionAleatoire][0]=(i)/NombreElement; /* Nous plaçons à cette position une valeur qui dépend du nombre de case déjà attribuée */
      DonneesServeur[PositionAleatoire]=Contenuboite[PositionAleatoire][0];
   }

   /* Echange information en réseau */
    if(EtatPc==0)
      DialogueServeur(NombreCase,EtatPc);
    else if(EtatPc==1){
      DialogueClient(NombreCase,EtatPc);
    for (int i = 0; i < NombreCase; i++)
    {
      Contenuboite[i][0]=DonneesClient[i];
    }
  }
}

/* Cette fonction permet de jouer à une partie de memory en fonction du mode de jeu voulu, du mode d'affichage, de la hauteur et longeur de la grille, et du contenu de sa grille */
void LANCEMENT_JEU(int LongueurVoulu,int HauteurVoulu,int NombreElement,int NombreGroupe,int Contenuboite[16][4],int GrandeTaille ,int difficulte,int mode)
{
  int HauteurTableau,LongueurTableau,Revele[NombreElement],Hauteur[NombreElement],Longueur[NombreElement],JoueurEnCours=0,Score[2],tempsD=time(NULL),souvenir,i;
  char ContenuAffiche[4];
  /* Initialisation à 0 des cartes en cours de lecture et déja trouvé */
  for (int i = 0; i < NombreGroupe*NombreElement; i++)
  {
    Contenuboite[i][2]=0;
  }
  for (int k = 0; k < NombreElement*NombreGroupe ; k++)
  {
    Contenuboite[k][1]=(0);
  }
  /* Lancement du minuteur sur un autre thread */
  pthread_t minuteur;
  pthread_create(&minuteur, NULL, minuteurFonction, NULL);
 /* Initialisation des scores */
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
  /* La boucle J est la boucle principale, nous pouvons la traduire en disant "tant que nous n'avons pas trouvé tout les groupes" le nombre de groupe trouvé étant J */
  for (int j = 0; j < NombreGroupe;)
  {
    if(EtatPc!=2||EtatPc!=5){
      if(JoueurEnCours==0){
        move(3,0);
        printw(">");
        move(2,0);
      }
      else{
        move(2,0);
        printw(">");
        move(3,0);
      }
      printw(" "); refresh();
    }
    i=0;
        /* Boucle secondaire traitent le nombre d'element trouvé */
        while( i<NombreElement){
            if(JoueurEnCours==EtatPc || EtatPc==2 || EtatPc==3||(EtatPc==5&&JoueurEnCours==0)){
              /* Tant que l'endroit où nous cliquons n'est pas dans le tableau ou est deja retouné ou alors est déja trouvé nous continuons */
              do{
                click_souris();
                HauteurTableau=floor((L-4)/8),LongueurTableau=floor((C-7)/10); /* conversion dans le tableau visuel */
              }while(!(L>=4 && HauteurTableau<=HauteurVoulu-1 && C>=7&&LongueurTableau<=LongueurVoulu-1&&Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][2]!=(1)) );
              
                Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][2]=1;/* La carte est retourné pour l'instant */
                /* Se positionne où il faut dans la grille pour afficher la lettre */
                move(5+HauteurTableau*7,9+LongueurTableau*9); 
                ContenuBoiteAffiche(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0],mode,ContenuAffiche); /* Donne le contenu à afficher dans ContenuAffiche en fonction du contenu et du mode */

                attron(COLOR_PAIR(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]%8));
                refresh();
                /* Affiche la lettre */
                if (GrandeTaille==0) printw("%s",ContenuAffiche);
                else AfficheLettres(ContenuAffiche,6+HauteurTableau*8,10+LongueurTableau*9);
                refresh();
                
                attroff(COLOR_PAIR(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]%8));
                
                fprintf(stderr ," L %d C %d case (%d,%d) [%s] ", L, C, HauteurTableau,LongueurTableau, ContenuAffiche);
                fprintf(stderr ,">Joueur en cours %d EtatPc %d ", JoueurEnCours, EtatPc);

                /* Echange information en réseau */
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

                /* Memorisation de la case et ses coordonnées dans les tableaux */
                Revele[i]=(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]);
                Longueur[i]=LongueurTableau;
                Hauteur[i]=HauteurTableau;
              
            }
            else if(EtatPc==5&&JoueurEnCours==1)
            {
              souvenir=rand()%difficulte;
              if(souvenir==0);

            }
            /* Partie réseau */
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
                Revele[i]=(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]); 
                Longueur[i]=LongueurTableau;
                Hauteur[i]=HauteurTableau;
                move(0,0);

                printw("Vous cliquez sur la position (%3d, %3d) et vous etes dans la case (%d,%d) de Contenu %d\n", L, C,HauteurTableau,LongueurTableau,Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]); 
                move(5+HauteurTableau*7,9+LongueurTableau*9);
                
                ContenuBoiteAffiche(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0],mode,ContenuAffiche);

                attron(COLOR_PAIR(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]%8));
                mvprintw(9,45,"contenu=%d & converti=%c",Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0],ContenuAffiche);
                refresh();

                if (GrandeTaille==0) printw("%s",ContenuAffiche);
                else AfficheLettres(ContenuAffiche,6+HauteurTableau*8,10+LongueurTableau*9);
                refresh();
                
                attroff(COLOR_PAIR(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau][0]%8));
                refresh();
                fprintf(stderr ," L %d C %d case (%d,%d) [%s] ", L, C, HauteurTableau,LongueurTableau, ContenuAffiche);
            }

            i++; /* Augmentation du nombre de carte trouvé dans le groupe */
         }
        sleep(1);  

          /* Remise à 0 des cartes en cours de révélation */
          for (int i = 0; i < NombreGroupe*NombreElement; i++)
          {
            Contenuboite[i][2]=0;
          }

        /* Vérifie si l'ensemble des éléments trouvé sont égaux */
        if ((VerifEnsembleTableau(NombreElement,Revele)==0))
        {
          JoueurEnCours=(JoueurEnCours+1)%2;
          for (int k = 0; k < NombreElement ; k++)
          {
            Contenuboite[LongueurVoulu*Hauteur[k]+Longueur[k]][1]=(1);
          }

          j++;/* Augmente le nombre de groupe trouvé */
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

          sleep(1);
          move(10,60);
          printw(" Un groupe de trouvé"); refresh();
          
        }

      /* Augmente d'un le numéro du joueur */
      JoueurEnCours=(JoueurEnCours+1)%2;

    for (int i = 0; i < NombreElement; i++) {
        Revele[i]=Longueur[i]=Hauteur[i]=(-1);
    }
      for (int x = 0;x <=LongueurVoulu ; x++){
        for (int y = 0;y <=HauteurVoulu ; y++){
          if(Contenuboite[LongueurVoulu*y+x][1]!=(1)){ 
            if (GrandeTaille==0) printw(" ");
            else AfficheLettres("z",6+y*8,10+x*9);refresh();
          }
        }
      }
      refresh();

  }
  tempsD=(time(NULL)-tempsD);
  mvprintw(8,45,"%d",tempsD);
  /* Arrete le thread du timer */
  pthread_cancel(minuteur);
  move(31,30); printw("Partie terminee appuyez sur q pour continuer, vous avez mis %d secondes à faire le memory",tempsD); refresh();
  while(getchar()!='q');

}
