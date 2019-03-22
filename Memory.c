#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <locale.h>

int L,C;

/**Initialisation de ncurses**/
void ncurses_initialiser() {
  initscr();	        /* Démarre le mode ncurses */
  cbreak();	        /* Pour les saisies clavier (desac. mise en buffer) */
  noecho();             /* Désactive l'affichage des caractères saisis */
  keypad(stdscr, TRUE);	/* Active les touches spécifiques */
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
  if(!mousemask(ALL_MOUSE_EVENTS, NULL)) {
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
	  C = event.x;
	  L = event.y;
	  if(event.bstate & BUTTON1_CLICKED)
	  { 
	    return 0;
	  }
	}
    }
  }
  return 0;
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
 
  return posY;
}

void LANCEMENT_JEU(int Contenuboite[16])
{
	int HauteurTableau,LongueurTableau,PremierRevele=-1,DeuxiemeRevele=-1,NbrTour=0,PremiereHauteur=(-1),PremiereLongueur=(-1),DeuxiemeHauteur=(-1),DeuxiemeLongueur=(-1),NombreRevele=0,JoueurEnCours=0,Score[2];
	Score[0]=0;
	Score[1]=0;
	while (click_souris()==0)
	{
    	move(0,0);
    	HauteurTableau=((L-4)/3),LongueurTableau=((C-7)/5);
    	if(HauteurTableau>=0&&HauteurTableau<=3&&LongueurTableau>=0&&LongueurTableau<=3&&!(LongueurTableau==PremiereLongueur&&HauteurTableau==PremiereHauteur)&& Contenuboite[4*HauteurTableau+LongueurTableau]!=(-2)){
    	    printw("Vous cliquez sur la position (%3d, %3d) et vous etes dans la case (%d,%d)\n", L, C,HauteurTableau,LongueurTableau); 
    		move(5+HauteurTableau*3,9+LongueurTableau*5);printw("%d",Contenuboite[4*HauteurTableau+LongueurTableau]);
    		refresh();
    		NbrTour++;
    		if(PremierRevele!=-1){
    			DeuxiemeRevele=(Contenuboite[4*HauteurTableau+LongueurTableau]);
    			DeuxiemeLongueur=LongueurTableau;
    			DeuxiemeHauteur=HauteurTableau;
    		}
    		else{
    			PremierRevele=(Contenuboite[4*HauteurTableau+LongueurTableau]);
    			PremiereLongueur=LongueurTableau;
    			PremiereHauteur=HauteurTableau;
    		}

    		if (PremierRevele==DeuxiemeRevele)
    		{
    			NombreRevele++;
    			Score[JoueurEnCours]++;
    			if(JoueurEnCours==0){
					move(1,11);
				}
		    	else{
					move(2,11);
		    	}
		    	printw("%d",Score[JoueurEnCours]);
		    	refresh();
    			Contenuboite[4*PremiereHauteur+PremiereLongueur]=(-2);
    			Contenuboite[4*DeuxiemeHauteur+DeuxiemeLongueur]=(-2);
    			move(30,30);
    			printw("Une paire de trouvee"); refresh();
    		}

    		if (NbrTour==2)
    		{
    			JoueurEnCours=(JoueurEnCours+1)%2;
				if(JoueurEnCours==0){
					move(2,0);
					printw(" ");
					move(1,0);
		    	}
		    	else{
					move(1,0);
					printw(" ");
		    		move(2,0);
		    	}
				printw(">");
	    		move(20,50);
	    		printw("%d",JoueurEnCours);
				refresh();
		   		sleep(1);
    			NbrTour=0;
    			PremierRevele=-1;
    			DeuxiemeRevele=-1;
    			PremiereLongueur=PremiereHauteur=DeuxiemeHauteur=DeuxiemeLongueur=(-1);
	   			fflush(stdout);
	   			for (int x = 0;x <4 ; x++){for (int y = 0;y <4 ; y++){if(Contenuboite[4*y+x]!=(-2)){move(5+y*3,9+x*5);printw(" ");}}}
	   			move(30,30);
	   			printw("                    ");
	   			refresh();
		 	}
    	}
		if (NombreRevele==8)
	 	{
	 		break;
	 	}
  
	}
    move(31,30);
    printw("Partie terminee"); refresh();
    while(getchar()!='q');

}

int main(int argc,char ** argv) {
	setlocale(LC_ALL, "");
    ncurses_initialiser(); //Initialisation de ncurses
    ncurses_souris(); //Initialisation de la souris
    ncurses_couleurs();
    bkgd(COLOR_PAIR(1)); /*pour changer la couleur du fond*/

    WINDOW *boite[100];
    srand(time(NULL));
   	int Contenuboite[100],NombreAleatoire[100],CombinaisonVoulu[100][2];
    int PossibiliteVoulu,PositionCombinaison=0,NombreLong=4,NombreHaut=4,PositionAleatoire,NombreElement=atoi(argv[1]),NombreGroupe=atoi(argv[2]),NombreCase=NombreElement*NombreGroupe;
    initscr();
    move(0,0);printw("Il y a en tout %d cases a faire",NombreCase);
    refresh();
    move(1,0); printw("Les differentes combinaisons possibles sont: ");
    for (int i = 2; i < NombreCase; i++)
    {
    	if(NombreCase%i==0){
    		CombinaisonVoulu[PositionCombinaison][0]=i;
    		CombinaisonVoulu[PositionCombinaison][1]=NombreCase/i;
    		printw("\nCombinaison numero %d : %d * %d est possible ",PositionCombinaison,i,NombreCase/i);
    		PositionCombinaison++;
    	}
    }
   
    PossibiliteVoulu=Fleche(PositionCombinaison);
    refresh();
    system("clear");
    usleep(100);
    move(0,0);
    refresh();
    for (int x = 0;x <NombreLong ; x++)
    {
    	for (int y = 0;y <NombreHaut ; y++)
    	{
			boite[NombreLong*y+x+1]= subwin(stdscr, 4, 6, 4+y*3,7+x*5);  
			wborder(boite[NombreLong*y+x+1], '|', '|', '-', '-', '+', '+', '+', '+');  	
			wrefresh(boite[NombreLong*y+x+1]);
    	}		
    }



	for (int y = 0;y <20 ; y++)
    {Contenuboite[y]=0;}

	for (int i = 1;i <=16 ; i++)
    {
    	do{
    		PositionAleatoire=rand()%16;
    	}while(Contenuboite[PositionAleatoire]!=0);
    	Contenuboite[PositionAleatoire]=(i+1)/2;
	}

   	/*for (int x = 0;x <NombreLong ; x++){for (int y = 0;y <NombreHaut ; y++){move(5+y*3,9+x*5);printw("%d",Contenuboite[NombreLong*y+x]);}}*/
	
	move(1,0);
	printw(">Joueur 1: 0");
	move(2,1);
	printw("Joueur 2: 0");
	refresh();
    LANCEMENT_JEU(Contenuboite); //Lancement du jeu
    endwin();    
    return 0;
}
