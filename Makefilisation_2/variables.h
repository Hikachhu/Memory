#ifndef DEF_VARS
#define DEF_VARS

char SERVEURNAME[20];
char *server_name;

int L,C,EtatPc;
unsigned  int mon_address_longueur;
int client_socket;
int ma_socket;

int lg;
struct hostent *serverHostEnt;
long hostAddr;
long status;
int DonneesServeur[400],DonneesClient[400];

void fin (int i);
void appli(int no_client_socket);
#endif
