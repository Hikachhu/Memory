#include "fonctions.h"
#include "variables.h"

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