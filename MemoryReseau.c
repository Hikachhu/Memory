#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ncurses.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#define DEF "\033[0;00m"
#define RED "\033[0;41m"
#define GREEN "\033[0;42m"
#define YELLOW "\033[0;43m"
#define BNOR "\033[0;44m"
#define VIO "\033[0;45m"
#define CYAN "\033[0;46m"
#define GRIS "\033[0;47m"

char SERVEURNAME[20];
int L,C,to_server_socket = -1,EtatPc;
void fin (int i);
void appli(int no_client_socket);
int ma_socket;
char *server_name = SERVEURNAME;
struct sockaddr_in serverSockAddr;
int client_socket;
struct sockaddr_in mon_address, client_address;
int mon_address_longueur, lg;
struct hostent *serverHostEnt;
long hostAddr;
long status;
int DonneesServeur[400],DonneesClient[400];

void EcriturePiece(int NbrPiece){
  printf("\nVous avez gagné %d pièces",NbrPiece);
  FILE* fichier = NULL;
  int PieceDejaObtenues;
  system("clear");

  fichier = fopen( "Piece.txt", "r");
  fscanf(fichier,"%d",&PieceDejaObtenues);
  PieceDejaObtenues=(((PieceDejaObtenues-8)/6)-3);
  fclose(fichier);

  fichier = fopen( "Piece.txt", "w+");
  NbrPiece+=PieceDejaObtenues;
  printf("\nNombre de pièce total :%d\n",NbrPiece);
  NbrPiece=(NbrPiece+3)*6+8;
  fprintf(fichier,"%d",NbrPiece);
  fclose(fichier);
}

void fin(int i)
{
  shutdown(ma_socket,2);
  close(ma_socket);
}

/**Initialisation de ncurses**/
void ncurses_initialiser() {
  initscr();            /* Démarre le mode ncurses */
  //cbreak();             /* Pour les saisies clavier (desac. mise en buffer) */
  //noecho();             /* Désactive l'affichage des caractères saisis */
  keypad(stdscr, TRUE); /* Active les touches spécifiques */
  refresh();            /* Met a jour l'affichage */
  curs_set(FALSE);      /* Masque le curseur */
}



/**Initialisation des couleurs**/
void ncurses_couleurs() {
  /* Vérification du support de la couleur */
  if(has_colors() == FALSE) {
    endwin();
    fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
    exit(EXIT_FAILURE);
  }

   /* Activation des couleurs */
  start_color();
  //init_color(COLOR_BLACK, 1000, 1000, 1000);
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

}

/**Initialisation de la souris**/
void ncurses_souris() {
  if(!mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION, NULL)) {
    endwin();
    fprintf(stderr, "Erreur lors de l'initialisation de la souris.\n");
    exit(EXIT_FAILURE);
  }

  if(has_mouse() != TRUE) {
    endwin();
    fprintf(stderr, "Aucune souris n'est détectée.\n");
    exit(EXIT_FAILURE);
  }
}
int click_souris()
{
  MEVENT event ;
  int ch;

  while((ch = getch()) != KEY_F(1)) 
  {
    switch(ch) 
    {
      case KEY_F(2): /*Pour quitter la boucle*/
        return 1;
      case KEY_MOUSE:
        if(getmouse(&event) == OK) 
        {
          fprintf(stderr, " event x %d y%d \n", event.x , event.y);
          C = event.x;
          L = event.y;
          if(event.bstate & BUTTON1_CLICKED ||event.bstate & BUTTON1_PRESSED)
          { 
            return 0;
          } else {
            fprintf(stderr, " event %lu %lx \n", event.bstate , event.bstate);
          }
        }
    }
  }
  return 0;
}

void InitialisationNcurses(){
  setlocale(LC_ALL, "");
  ncurses_initialiser(); //Initialisation de ncurses
  ncurses_souris(); //Initialisation de la souris
  ncurses_couleurs();
  bkgd(COLOR_PAIR(1)); /*pour changer la couleur du fond*/
}

char key_pressed() {
  struct termios oldterm, newterm;
  int oldfd;
  char c, result = 0;
  tcgetattr (STDIN_FILENO, &oldterm);
  newterm = oldterm;
  newterm.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
  oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
  c = getchar();
  tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
  fcntl (STDIN_FILENO, F_SETFL, oldfd);
  if (c != EOF) {
  ungetc(c, stdin);
  result = getchar();
  }
  return result;
}


void Init_Client(){

  bzero(&serverSockAddr,sizeof(serverSockAddr));
  hostAddr = inet_addr(SERVEURNAME);
  if ( (long)hostAddr != (long)-1)
    bcopy(&hostAddr,&serverSockAddr.sin_addr,sizeof(hostAddr));
  else
  {
    serverHostEnt = gethostbyname(SERVEURNAME);
    if (serverHostEnt == NULL)
    {
      printf("ca chie gethost\n");
      //exit(0);
      return;
    }
    bcopy(serverHostEnt->h_addr,&serverSockAddr.sin_addr,serverHostEnt->h_length);
  }
  serverSockAddr.sin_port = htons(30000);
  serverSockAddr.sin_family = AF_INET;
}

void Init_Serveur(){
  bzero(&mon_address,sizeof(mon_address));
  mon_address.sin_port = htons(30000);
  mon_address.sin_family = AF_INET;
  mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

  /* creation de socket */
  if ((ma_socket = socket(AF_INET,SOCK_STREAM,0))== -1)
  {
    printf("ca chie avec la creation\n");
    /*
  exit(0);
  */
    return ;
  }
  // signal(SIGINT,fin);
  /* bind serveur - socket */
  bind(ma_socket,(struct sockaddr *)&mon_address,sizeof(mon_address));

  /* ecoute sur la socket */
  listen(ma_socket,5);

  /* accept la connexion */
  mon_address_longueur = sizeof(client_address);
}

int Fleche(int NombrePossibite){  /* Initialisation de ncurses */
  int ch,posY,posX;
  char a='<';

  ncurses_initialiser();

  /* Place le curseur à la position de départ */
  posX = 45;
  posY = 2;
  mvaddch(posY, posX, a);
  refresh();
 
  /* Routine principale */
  while((ch = getch()) != 10) {
    /* On efface le curseur */
    mvaddch(posY, posX, ' ');
 
    /* On calcule la nouvelle position */
    switch(ch) {
      case KEY_UP:
         if(posY >= 3) posY--;
         break;
      case KEY_DOWN:
         if(posY <= NombrePossibite) posY++;
         break;
    }
 
    /* On affiche le curseur */
    mvaddch(posY, posX, a);
    refresh();
  }
  mvprintw(14,0,"%d",posY-2);
  refresh();
  sleep(1);
  return posY-2;
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

void DialogueClient(int taille, int modePC){
  int n;
  int tmp;
  //mvprintw(21,0,"client  %d",modePC); refresh();
  if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
      printf("ca chie creation socket client\n");
      //exit(0);
      return;
    }
    /* requete de connexion */
    if(connect( to_server_socket,
                (struct sockaddr *)&serverSockAddr,
                sizeof(serverSockAddr)) < 0 )
    {

      mvprintw(19,0,"Probleme connect DialogueClient\n");
      //exit(0);
      return;
    }
    if (modePC==1) {
      for (int i = 0; i < taille; i++)
      {
        while (read(to_server_socket,&tmp,sizeof(tmp))<0)   {mvprintw(0,0,"Probleme lecture %d\n",i);}
        DonneesClient[i]=ntohl(tmp);
        //mvprintw(22+i,50,"Client  lecture %d",DonneesClient[i]); refresh();
      }
    } else {
    //  while (read(to_server_socket,&tmp,sizeof(tmp))<0)   {mvprintw(0,0,"Probleme lecture YY\n");}
      for (int i = 0; i < taille; i++)
      {
        //mvprintw(22+i,50,"Client    ecrit %d",DonneesClient[i]); refresh();
        tmp=htonl(DonneesClient[i]);
        n=write(to_server_socket,&tmp,sizeof(tmp));
        if (n<0)   printf("Problème ecriture %d\n",i);
      }      
    }
    shutdown(to_server_socket,2);
    close(to_server_socket);
}

void DialogueServeur(int taille, int modePC){
  int n;
  int tmp;
  pid_t cpid;
  int status;
  int (*shared_array)[taille]; 
  client_socket = accept(ma_socket,
                         (struct sockaddr *)&client_address,
                         &mon_address_longueur);

  shared_array = mmap(NULL, sizeof *shared_array, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (cpid=fork() == 0)
  {
    fprintf(stderr ," connecté %d\n", modePC);
    //close(ma_socket);
    if (modePC==0) {
      for (int i = 0; i < taille; i++)
      {
        fprintf(stderr ,"serveur ecrit %d\n",DonneesServeur[i]);
        //mvprintw(22+i,0,"serveur ecrit %d",DonneesServeur[i]);        refresh();
        tmp=htonl(DonneesServeur[i]);
        n=write(client_socket,&tmp,sizeof(tmp));
        if (n<0)   printf("Problème ecriture %d\n",i);
      }
    } else{
     // fprintf(stderr ,"serveur ecrit %d\n",97);
      //tmp=htonl(97);
      //n=write(client_socket,&tmp,sizeof(tmp));
      //if (n<0)   printf("Problème ecriture XX\n");
      for (int i = 0; i < taille; i++)
      {
        while (read(client_socket,&tmp,sizeof(tmp))<0)   {mvprintw(0,0,"Probleme lecture %d\n",i);}
        // DonneesServeur[i]=ntohl(tmp);
        // //mvprintw(22+i,0,"serveur lecture %d %d",DonneesServeur[i],tmp);          refresh();
        // fprintf(stderr ,"serveur lis i=%d %d\n",i,DonneesServeur[i]);
        *shared_array[i]=ntohl(tmp);
        fprintf(stderr ,"serveur lis i=%d shared %d\n",i,*shared_array[i]);

      }      
    }
    //shutdown(client_socket,2);
    close(client_socket);
    _exit(EXIT_SUCCESS) ;//return ;
  }
  while(1) {
    if((cpid = wait(&status)) == -1) {
      if(errno == ECHILD) {
        break;
      } else {
        printf("erreur sur wait\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  if (modePC==1) { 
    for (int i = 0; i<taille; i++) 
      DonneesServeur[i]=*shared_array[i];
    munmap(shared_array, sizeof *shared_array);
  }
  //shutdown(ma_socket,2);
  //close(ma_socket);
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
  int i,j,lon=0,haut=0,r,NombreMot=0;
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
  int NombreAleatoire[100],CombinaisonVoulu[100][2],PositionAleatoire,PositionCombinaison=0,PossibiliteVoulu,NombreCase; 
  if(EtatPc!=1 ){
    int Ret,valE , valG;
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

void CreationMemory(int NombreHaut,int NombreLong,int NombreElement,int NombreGroupe, int Contenuboite[NombreHaut*NombreLong]){
  system("clear");
  int NombreCase=NombreLong*NombreHaut,PositionAleatoire;
  WINDOW *boite[100];
  for (int x = 0;x <NombreLong ; x++)
  {
    for (int y = 0;y <NombreHaut ; y++)
      {
       boite[NombreLong*y+x+1]= subwin(stdscr, 4, 6, 4+y*3,7+x*5);  
       wborder(boite[NombreLong*y+x+1], '|', '|', '-', '-', ' ', ' ', ' ', ' ');    
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
  mvprintw(34,0,"fin CreationMemory");
}

void LANCEMENT_JEU(int LongueurVoulu,int HauteurVoulu,int NombreElement,int NombreGroupe,int Contenuboite[16] )
{
  int HauteurTableau,LongueurTableau,Revele[NombreElement],NbrTour=0,Hauteur[NombreElement],Longueur[NombreElement],JoueurEnCours=0,Score[2],Sec=0,Sec2=0,Min=0;
  Score[0]=0;
  Score[1]=0;
  Sec=time(NULL);
  Sec2=time(NULL);
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
            Sec2=time(NULL);
            mvprintw(21,120,"%02d:%02d",Min,Sec2-Sec);
            if(JoueurEnCours==EtatPc || EtatPc==2 || EtatPc==3){
              signal(SIGALRM, AfficheDuree);
              alarm(1);
              click_souris();
              HauteurTableau=((L-4)/3),LongueurTableau=((C-7)/5);
              if(HauteurTableau>=0&&HauteurTableau<=HauteurVoulu-1&&LongueurTableau>=0&&LongueurTableau<=LongueurVoulu-1&& Contenuboite[4*HauteurTableau+LongueurTableau]!=(-2)){
                move(0,0);
                printw("Vous cliquez sur la position (%3d, %3d) et vous etes dans la case (%d,%d) de Contenu %d\n", L, C,HauteurTableau,LongueurTableau,Contenuboite[4*HauteurVoulu+LongueurTableau]); 
                move(5+HauteurTableau*3,9+LongueurTableau*5);
                ContenuBoiteAffiche(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau],0,ContenuAffiche);
                printw("%s",ContenuAffiche);
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
  (Sec2)=time(NULL)-Sec+((Min)*60);
  move(31,30); printw("Partie terminee"); refresh();
  while(getchar()!='q');

}

int main(int argc,char ** argv) {

  srand(time(NULL));
  int Contenuboite[100],NombreAleatoire[100],CombinaisonVoulu[100][2],NombreHaut,NombreLong,NombreElement,NombreGroupe,ChoixTuto,ChoixReseau;
  EtatPc=atoi(argv[3]);
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
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite);
    //  LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite);
      endwin();
      break;

    case 2:
      EtatPc=2;
      ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
      InitialisationNcurses();
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite);
      LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite);
      endwin(); 
      break; 

    case 3:
    EtatPc=3;
      ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
      InitialisationNcurses();
      CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite);
      LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite);
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
          InitialisationNcurses();
          CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite);
          sleep(1);
          LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite);
          endwin();
          break;
        case 2:
          Init_Serveur();
          EtatPc=0;
          ChoixTailleMemory(&NombreHaut,&NombreLong,&NombreElement,&NombreGroupe);
          InitialisationNcurses();
          CreationMemory(NombreHaut,NombreLong,NombreElement,NombreGroupe,Contenuboite);
          LANCEMENT_JEU(NombreLong,NombreHaut,NombreElement,NombreGroupe,Contenuboite);
          endwin();
          break;
      } 
  } 
 EcriturePiece((NombreElement-1)*(NombreGroupe-1));
 return 0;
}
