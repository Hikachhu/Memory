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

void LANCEMENT_JEU(int Contenuboite[16],int LongueurVoulu,int HauteurVoulu,int NombreElement,int NombreGroupe)
{
	int HauteurTableau,LongueurTableau,Revele[NombreElement],NbrTour=0,Hauteur[NombreElement],Longueur[NombreElement],JoueurEnCours=0,Score[2];
	Score[0]=0;
	Score[1]=0;
	int i;
    	for (int j = 0; j < NombreGroupe; )
    	{
    		i=0;
    	    	while( i<NombreElement){
    	    		click_souris();
 		    		HauteurTableau=((L-4)/3),LongueurTableau=((C-7)/5);
		    		if(HauteurTableau>=0&&HauteurTableau<=HauteurVoulu-1&&LongueurTableau>=0&&LongueurTableau<=LongueurVoulu-1&& Contenuboite[4*HauteurTableau+LongueurTableau]!=(-2)){
		    			move(0,0);
			    	    printw("Vous cliquez sur la position (%3d, %3d) et vous etes dans la case (%d,%d)\n", L, C,HauteurTableau,LongueurTableau); 
			    		move(5+HauteurTableau*3,9+LongueurTableau*5);printw("%d",Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau]);
			    		move(32,30);printw("%d",i);refresh();
			    		refresh();
			    		NbrTour++;

		    			Revele[i]=(Contenuboite[LongueurVoulu*HauteurTableau+LongueurTableau]);
		    			Longueur[i]=LongueurTableau;
		    			Hauteur[i]=HauteurTableau;
		    			i++;
	    			}
 	    		}	
    		if (VerifEnsembleTableau(NombreElement,Revele)==0)
    		{
    			Score[JoueurEnCours]++;
    			j++;
    			if(JoueurEnCours==0){
					move(1,11);
				}
		    	else{
					move(2,11);
		    	}
		    	printw("%d",Score[JoueurEnCours]);
		    	refresh();
		    	for (int i = 0; i < NombreElement; i++)
		    	{
    				Contenuboite[LongueurVoulu*Hauteur[i]+Longueur[i]]=(-2);
    			}
    			move(30,30);
    			printw("Une paire de trouvee"); refresh();
    		
    		}

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
		   	for (int i = 0; i < NombreElement; i++)
	   		{
	   			Revele[i]=Longueur[i]=Hauteur[i]=(-1);
	   		}
   			for (int x = 0;x <LongueurVoulu ; x++){for (int y = 0;y <HauteurVoulu ; y++){if(Contenuboite[4*y+x]!=(-2)){move(5+y*3,9+x*5);printw("  ");refresh();}}}
   			move(30,30);
   			printw("                    ");
   			refresh();
	 	
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
    		printw("\n            numero %d : %d * %d est possible ",PositionCombinaison,i,NombreCase/i);
    		PositionCombinaison++;
    	}
    }
   
    PossibiliteVoulu=Fleche(PositionCombinaison);
    NombreLong=CombinaisonVoulu[PossibiliteVoulu][0],NombreHaut=CombinaisonVoulu[PossibiliteVoulu][1];
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



	for (int y = 0;y <NombreCase+4 ; y++)
    {Contenuboite[y]=0;}

	for (int i = NombreElement;i <NombreCase+NombreElement ; i++)
    {
    	do{
    		PositionAleatoire=rand()%NombreCase;
    	}while(Contenuboite[PositionAleatoire]!=0);
    	Contenuboite[PositionAleatoire]=(i)/NombreElement;
	}

   	/*for (int x = 0;x <NombreLong ; x++){for (int y = 0;y <NombreHaut ; y++){move(5+y*3,9+x*5);printw("%d",Contenuboite[NombreLong*y+x]);}}*/
	
	move(1,0);
	printw(">Joueur 1: 0");
	move(2,1);
	printw("Joueur 2: 0");
	refresh();
    LANCEMENT_JEU(Contenuboite,CombinaisonVoulu[PossibiliteVoulu][0],CombinaisonVoulu[PossibiliteVoulu][1],NombreElement,NombreGroupe); //Lancement du jeu
    endwin();    
    return 0;
}
