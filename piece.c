#include "fonctions.h"
#include "variables.h"

int Max(int n,int *tab){
  int max=tab[0];
  for (int i = 1; i < n; i++)
  {
    if(max<tab[i])
      max=tab[i];
  }
  return max;
}

void EcriturePiece(int NbrPiece){
  mvprintw(0,0,"\nVous avez gagné %d pièces",NbrPiece);
  FILE* fichier = NULL;
  int PieceDejaObtenues=0;
  system("clear");

  struct stat st = {0};

  if (stat("score", &st) == -1) {
    mkdir("score", 0700);
  }

  if (stat("score/Piece.txt", &st) == -1) {
      fichier = fopen( "score/Piece.txt", "w+");
      fprintf(fichier,"0");
      fclose(fichier);
  }

  fichier = fopen( "score/Piece.txt", "r");
  fscanf(fichier,"%d",&PieceDejaObtenues);
  fclose(fichier);

  fichier = fopen( "score/Piece.txt", "w+");
  NbrPiece+=PieceDejaObtenues;
  mvprintw(1,0,"nombre de pièce total :%d\n",NbrPiece);
  fprintf(fichier,"%d",NbrPiece);
  fclose(fichier);
  refresh();  
}

void LectureMode(int *Mode){
  FILE* fichier = NULL;
  fichier=fopen("Mode/Mode.txt","r");
  fscanf(fichier,"%d",Mode);
  fclose(fichier);
}

void ChangementMode(int Mode){
  FILE* fichier = NULL;
  fichier=fopen("Mode/Mode.txt","w+");
  fprintf(fichier,"%d",Mode);
  fclose(fichier);
  mvprintw(9,10,"sorti");
}


