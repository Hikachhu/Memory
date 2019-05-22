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
