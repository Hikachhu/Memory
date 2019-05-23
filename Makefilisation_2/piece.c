#include "fonctions.h"
#include "variables.h"

void EcriturePiece(int NbrPiece){
  mvprintw(0,0,"\nVous avez gagné %d pièces",NbrPiece);
  FILE* fichier = NULL;
  int PieceDejaObtenues;
  system("clear");

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
