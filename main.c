#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rubiks.h"

// N'oublie pas de fixer le probleme de la rotation

rubiks* box;

// Tools
T_SIDE read_face() {
  int face = -1;
  do {
    printf("The side to set ?\n");
    printf("0 FRONT; ");
    printf("1 BACK; ");
    printf("2 UP; ");
    printf("3 DOWN; ");
    printf("4 RIGHT; ");
    printf("5 LEFT\n");
    printf("Side? ");
    scanf("%d", &face);
    printf("\n");
  } while (face < 0 || face > 5);
  return (T_SIDE) face;
}

T_COLOR read_color() {
  int color = -1;
  do {
    printf("Which color ?\n");
    printf("0 Red; ");
    printf("1 Blue; ");
    printf("2 Green; ");
    printf("3 White; ");
    printf("4 Yellow; ");
    printf("5 Orange; ");
    printf("6 Gray\n");
    printf("Color? ");
    scanf("%d", &color);
    printf("\n");
  } while (color < 0 || color > 6);
  return (T_COLOR) color;
}

int read_sens() {
  int sens = -1;
  do {
    printf("Which direction ?\n");
    printf("0 Anticlockwise; ");
    printf("1 Clockwise\n");
    printf("Direction? ");
    scanf("%d", &sens);
    printf("\n");
  } while (sens < 0 || sens > 1);
  return sens;
}

int read_angle() {
  int angle = -1;
  do {
    printf("Rotation angle ?\n");
    printf("1 1/4; ");
    printf("2 1/2\n");
    printf("Angle? ");
    scanf("%d", &angle);
    printf("\n");
  } while (angle < 1 || angle > 2);
  return angle;
}

void exit_game() {
  free_rubiks(box); // Désallouer le rubiks
  printf("Good bye!\n");
  exit(EXIT_SUCCESS);
}

void fill_menu() {
  // Entrez la face a modifier
  T_SIDE face = read_face();

  // Numero de la ligne et de la colonne
  int row, col;
  do {
    printf("Which ROW and COL ? \n");
    printf("row in [0,2]; col in [0,2]\n");
    printf("row,col? ");
    scanf("%d %d", &row, &col);
    printf("\n");
  } while (row < 0 || row > 2 || col < 0 || col > 2);

  // Couleur a attribuer
  T_COLOR color = read_color();

  fill_rubiks(box, face, row, col, color);
}

void play_menu() {
  T_SIDE face = read_face(); // Lire le nom de la face
  int sens = read_sens(); // Lire le sens de la rotation
  int angle = read_angle(); // Lire le nombre de tours
  rotation(box, face, sens, angle); // Rotation
}

void menu() {
  int choix = 0;
  do {
    printf("1 Scramble; ");
    printf("2 Reset; ");
    printf("3 Play; ");
    printf("4 Exit\n");
    printf("Action? ");
    scanf("%d", &choix);
    printf("\n");
  } while (choix < 1 || choix > 7);

  switch (choix) {
    case 1: // Scramble
      scramble_rubiks(box);
      break;
    case 2: // Reset
      init_rubiks(box);
      break;
    case 3: // Play
      play_menu();
      break;
    case 4: // Exit
      exit_game();
      break;
    default:
      perror("This choise doesn't exit");
      exit(EXIT_FAILURE);
  }

}

int main() {

  // Intialiser le générateur des nombres aléatoires
  srand(time(NULL));

  // Initialiser le rubiks cube
  box = create_rubiks(); // Création d'un nouveau rubik's cube
  init_rubiks(box); // Remplir les cases du rubik's cube avec les couleurs correspondates à chaque face

  // Lancer le jeu
  while (1) {
    system("clear");
    printf("\n                     Rubik's Cube v1.0");
    printf("\n                    ===================\n\n");
    //display_rubiks(box); Afficher le jeu avec des lettres
    display_rubiks2(box); // Afficher le jeu avec des cases colorées
    printf("\n");
    menu();
  }

  return 0;
}