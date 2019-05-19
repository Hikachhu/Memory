#include "fonctions.h"
#include "variables.h"

void EcriturePiece(int NbrPiece){
  printf("\nVous avez gagné %d pièces",NbrPiece);
  FILE* fichier = NULL;
  int PieceDejaObtenues;
  system("clear");

  fichier = fopen( "score/Piece.txt", "r");
  fscanf(fichier,"%d",&PieceDejaObtenues);
  PieceDejaObtenues=(((PieceDejaObtenues-8)/6)-3);
  fclose(fichier);

  fichier = fopen( "score/Piece.txt", "w+");
  NbrPiece+=PieceDejaObtenues;
  printf("\nNombre de pièce total :%d\n",NbrPiece);
  NbrPiece=(NbrPiece+3)*6+8;
  fprintf(fichier,"%d",NbrPiece);
  fclose(fichier);
}
