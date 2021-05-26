#ifndef __RUBIKS_H__
#define __RUBIKS_H__

#define ANSI_COLOR_RED      "\x1B[38;2;255;0;0m"
#define ANSI_COLOR_BLUE     "\x1B[38;2;0;0;255m"
#define ANSI_COLOR_GREEN    "\x1B[38;2;0;255;0m"
#define ANSI_COLOR_WHITE    "\x1B[38;2;255;255;255m"
#define ANSI_COLOR_YELLOW   "\x1B[38;2;255;255;0m"
#define ANSI_COLOR_ORANGE   "\x1B[38;2;255;127;0m"
#define ANSI_COLOR_GRAY     "\x1B[38;2;100;100;100m"
#define ANSI_COLOR_RESET    "\x1b[0m"

#define NBR_FAC 6 // Nombre de faces
#define NBR_CL 3 // Nombre de lignes = nombre de colonnes pour chaque face

// Définition des types
typedef enum { FRONT, BACK, UP, DOWN, RIGHT, LEFT } T_SIDE;
typedef enum { R, B, G, W, Y, O, LG } T_COLOR;

typedef enum { BEG_TO_END=1, END_TO_BEG=-1, } T_STEP;
typedef enum { BEG=0, END=2 } T_BOUNDS;

#define INIT(step, min, max) (step == BEG_TO_END ? (min) : (max))
#define COND(x, step, min, max) (step == BEG_TO_END ? (x <= max) : (x >= min))

typedef struct _cellule {
  T_COLOR couleur;
} cellule;

typedef struct _face {
  cellule cellules[3][3];
  T_SIDE name;
} face;

typedef struct _rubiks {
  face faces[6];
} rubiks;

// Signatures des fonctions
rubiks* create_rubiks();
void init_rubiks(rubiks* box);
void blank_rubiks(rubiks* box);
void fill_rubiks(rubiks* box, T_SIDE name, int i, int j, T_COLOR couleur);
void scramble_rubiks(rubiks* box);
void free_rubiks(rubiks* box);
void horizontal_rotation(rubiks* box);
void vertical_rotation(rubiks* box);
void rotation(rubiks* box, T_SIDE face, int sens, int angle);
void display_rubiks(rubiks* box);
void display_rubiks2(rubiks* box);
#endif
