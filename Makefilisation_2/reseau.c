#include "fonctions.h"
#include "variables.h"

struct sockaddr_in serverSockAddr;
struct sockaddr_in mon_address, client_address;

void fin(int i)
{
  shutdown(ma_socket,2);
  close(ma_socket);
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


void DialogueClient(int taille, int modePC){
  int n;
  int tmp;
  int to_server_socket = -1;
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

  if ( (cpid=fork() == 0) )
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
