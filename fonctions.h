#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>


#define DEF "\033[0;00m"
#define RED "\033[0;41m"
#define GREEN "\033[0;42m"
#define YELLOW "\033[0;43m"
#define BNOR "\033[0;44m"
#define VIO "\033[0;45m"
#define CYAN "\033[0;46m"
#define GRIS "\033[0;47m"

extern char key_pressed() ;
extern void InitialisationNcurses();
extern int click_souris();
extern void ncurses_initialiser() ;
void *minuteurFonction(void *arg);
void fin(int i);
void Init_Client();
void Init_Serveur();
int Fleche(int NombrePossibite);
void DialogueClient(int taille, int modePC);
void DialogueServeur(int taille, int modePC);
void EcriturePiece(int NbrPiece);
void ncurses_souris() ;
void ncurses_couleurs() ;
int VerifEnsembleTableau(int taille,int Tab[taille]);
void ContenuBoiteAffiche(int ContenuCoord,int mode,char Text[6]);
int menu(char *chaine,...);
void ChoixTailleMemory(int *NombreHaut,int *NombreLong,int *NombreElement,int *NombreGroupe);
void CreationMemory(int NombreHaut,int NombreLong,int NombreElement,int NombreGroupe, int Contenuboite[NombreHaut*NombreLong][4],int GrandeTaille);
void LANCEMENT_JEU(int LongueurVoulu,int HauteurVoulu,int NombreElement,int NombreGroupe,int Contenuboite[16][4],int GrandeTaille,int difficulte,int mode );
void AfficheLettres(char *cara,int haut,int lon);
void LectureMode(int *Mode);
void ChangementMode(int Mode);
int Max(int n,int *tab);