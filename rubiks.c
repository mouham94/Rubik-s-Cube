#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rubiks.h"

/*******************/
/****** Tools ******/
/*******************/
// Couleurs initiales pour chaque face
int color[NBR_FAC] = {G, B, W, Y, R, O};

// Retrouner une chaine de caractères qui correspond à la couleur
char* select_color(T_COLOR couleur) {
  switch (couleur) {
    case R: return "R";
    case B: return "B";
    case G: return "G";
    case W: return "W";
    case Y: return "Y";
    case O: return "O";
    case LG: return "X";
    default:
      perror("Cette couleur n'existe pas");
      exit(EXIT_FAILURE);
  }
}

// Retourer des blocs colorés en fonction de la couleur
char* select_color2(T_COLOR couleur) {
  switch (couleur) {
    case R: return ANSI_COLOR_RED "▄" ANSI_COLOR_RESET;
    case B: return ANSI_COLOR_BLUE "▄" ANSI_COLOR_RESET;
    case G: return ANSI_COLOR_GREEN "▄" ANSI_COLOR_RESET;
    case W: return ANSI_COLOR_WHITE "▄" ANSI_COLOR_RESET;
    case Y: return ANSI_COLOR_YELLOW "▄" ANSI_COLOR_RESET;
    case O: return  ANSI_COLOR_ORANGE "▄" ANSI_COLOR_RESET;
    case LG: return ANSI_COLOR_GRAY "▄" ANSI_COLOR_RESET;
    default:
      perror("Cette couleur n'existe pas");
      exit(EXIT_FAILURE);
  }
}

// Récupérer l'indice dans le tableau box->faces d'une face (l'indice est entre 0 et 5)
int side_to_index(rubiks* box, T_SIDE face) {
  for (int i=0; i<NBR_FAC; i++) {
    if (box->faces[i].name == face) return i;
  }
  perror("Cette face n'existe pas");
  exit(EXIT_FAILURE);
}

// Allouer une zone mémoire de taille size
void* allocate(int size) {
  void* mem = (void*) malloc(size);
  if (!mem) {
    perror("Error during allocation");
    exit(EXIT_FAILURE);
  }
  return mem;
}

// Générer un nombre aléatoire dans l'intevalle [min; max]
int get_random(int min, int max) {
  return rand() % (max - min + 1) + min;
}

// Récupérer les cellules dans l'intervalle [i_deb; i_fin] et [j_deb; j_fin]
cellule* get_cells(rubiks* box, T_SIDE face, int itv[6]) {
  cellule* cells = (cellule*) allocate(sizeof(cellule) * NBR_CL);
  int k = 0;
  int idx = side_to_index(box, face);
  for (int i=itv[0]; i<=itv[1]; i+=itv[4]) {
    for (int j=itv[2]; j<=itv[3]; j+=itv[5]) {
      cells[k++] = box->faces[idx].cellules[i][j];
    }
  }
  return cells;
}

// Reporter des cellules
// Prendre une partie (une ligne ou une colonne) d'une face et la reporter sur une autre face
void set_cells2(rubiks* box, T_SIDE face_dst, int itv_dst[6], cellule* cells) {
  int k = 0;
  int idx = side_to_index(box, face_dst);
  for (int i=itv_dst[0]; i<=itv_dst[1]; i+=itv_dst[4]) {
    for (int j=itv_dst[2]; j<=itv_dst[3]; j+=itv_dst[5]) {
      box->faces[idx].cellules[i][j] = cells[k++];
    }
  }
}
void set_cells(rubiks* box, T_SIDE face_src, T_SIDE face_dst, int itv_src[6], int itv_dst[6]) {
  cellule* cells_src = get_cells(box, face_src, itv_src);
  set_cells2(box, face_dst, itv_dst, cells_src);
  free(cells_src);
}

// Créer un nouveau rubiks cube
rubiks* create_rubiks() {
  rubiks* box = (rubiks*) allocate(sizeof(rubiks));
  return box;
}

// Initialiser le rubiks cube en mettant sur chaque face les bonnes couleurs
void init_rubiks(rubiks* box) {
  for (int k=0; k<NBR_FAC; k++) {
    box->faces[k].name = k;
    for (int i=0; i<NBR_CL; i++) {
      for (int j=0; j<NBR_CL; j++) {
        box->faces[k].cellules[i][j].couleur = color[k];
      }
    }
  }
}

// Intialiser toutes cases du rubiks cube à gris
void blank_rubiks(rubiks* box) {
  for(int k=0; k<NBR_FAC; k++) {
    for (int i=0; i<NBR_CL; i++) {
      for (int j=0; j<NBR_CL; j++) {
        box->faces[k].cellules[i][j].couleur = LG;
      }
    }
  }
}

// Initialiser manuellement le rubiks cube
void fill_rubiks(rubiks* box, T_SIDE name, int i, int j, T_COLOR couleur) {

  // Filtrage des cas d'erreurs
  if ((i < 0 || i > NBR_CL) && (j < 0 || j > NBR_CL)) {
    perror("Erreur dans fill_rubiks");
    exit(EXIT_FAILURE);
  }

  // TODO : Rajouter des condtions plutard

  // Modifier la couleur de la cellule i, j se trouvant sur la face nommée name
  box->faces[side_to_index(box, name)].cellules[i][j].couleur = couleur;
}

// Faire une rotation pour une face dans le sens de la monte
// Voir l'image rotate_clockwise.png
void rotate_face_clockwise(rubiks* box, T_SIDE face) {
  puts("ENTER - rotate_face_clockwise");
  cellule cellules[NBR_CL][NBR_CL];
  int idx = side_to_index(box, face);
  // Copier la face dans cellules et faire la rotation
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      cellules[i][j] = box->faces[idx].cellules[NBR_CL-j-1][i];
    }
  }
  // Reporter les résultats de la rotation sur la face originale
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      box->faces[idx].cellules[i][j] = cellules[i][j];
    }
  }
  puts("ENTER - rotate_face_clockwise");
}
// Faire une rotation pour une face dans le sens inverse de la monte
void rotate_face_anticlockwise(rubiks* box, T_SIDE face) {
  puts("ENTER - rotate_face_anticlockwise");
  cellule cellules[NBR_CL][NBR_CL];
  int idx = side_to_index(box, face);
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      cellules[i][j] = box->faces[idx].cellules[j][NBR_CL-i-1];
    }
  }
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      box->faces[idx].cellules[i][j] = cellules[i][j];
    }
  }
  puts("EXIT  - rotate_face_anticlockwise");
}

// Faire la rotation des faces adjacentes à une face
void rotate_adjacent_faces(rubiks* box, T_SIDE faces[4], int idx[4][6]) {
  puts("ENTER - rotate_adjacent_faces");

  // C'est un algorithme de permutation entre plusieurs variables
  // save <-- faces[3]
  // faces[3] <-- faces[2] 
  // faces[2] <-- faces[1] 
  // faces[1] <-- faces[0]
  // faces[0] <-- save 

  cellule* save_cells = get_cells(box, faces[3], idx[3]);
  for (int i=3; i>0; i--) {
    set_cells(box, faces[i-1], faces[i], idx[i-1], idx[i]);
  }
  set_cells2(box, faces[0], idx[0], save_cells);
  free(save_cells);
  puts("EXIT - rotate_adjacent_faces");
}


// FRONT : Faire une rotation sur la face FRONT
//  - Sens de la montre
//  - Sens inverse
void FRONT_anticlockwise(rubiks* box, int angle) { // OK
  puts("ENTER - FRONT_anticlockwise");
  // angle peut etre = 1 ou 2
  // Si angle = 1 alors rotation d'1/4 de tour
  // Si angle = 2 alors rotation d'1/4 x 2 de tour
  for (int i=0; i<angle; i++) {
    // Rotation de la face
    rotate_face_anticlockwise(box, FRONT);

    // Rotation des faces adjacentes
    T_SIDE faces[4] = {DOWN, RIGHT, UP, LEFT}; // Toutes les faces adjacentes
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 2, 0, 0, 1, 1}, {2, 2, 0, 2, 1, 1}, {0, 2, 2, 2, 1, 1}}; // Colonnes ou lignes de chaque face 
    rotate_adjacent_faces(box, faces, idx); 
  }
  puts("EXIT - FRONT_anticlockwise");
}
void FRONT_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, FRONT);
    T_SIDE faces[4] = {DOWN, LEFT, UP, RIGHT};
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 2, 2, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {0, 2, 0, 0, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// BACK
void BACK_anticlockwise(rubiks* box, int angle) { //OK
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, BACK);
    T_SIDE faces[4] = {UP, RIGHT, DOWN, LEFT};
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 2, 2, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {0, 2, 0, 0, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}
void BACK_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, BACK);
    T_SIDE faces[4] = {UP, LEFT, DOWN, RIGHT};
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 2, 0, 0, 1, 1}, {2, 2, 0, 2, 1, 1}, {0, 2, 2, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// UP
void UP_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, UP);
    T_SIDE faces[4] = {BACK, LEFT, FRONT, RIGHT};
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}
void UP_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, UP);
    T_SIDE faces[4] = {BACK, RIGHT, FRONT, LEFT};
    int idx[4][6] = {{0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}, {0, 0, 0, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// DOWN
void DOWN_anticlockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, DOWN);
    T_SIDE faces[4] = {FRONT, LEFT, BACK, RIGHT};
    int idx[4][6] = {{2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}
void DOWN_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, DOWN);
    T_SIDE faces[4] = {FRONT, RIGHT, BACK, LEFT};
    int idx[4][6] = {{2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}, {2, 2, 0, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// LEFT
void LEFT_anticlockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, LEFT);
    T_SIDE faces[4] = {UP, BACK, DOWN, FRONT};
    int idx[4][6] = {{0, 2, 0, 0, 1, 1}, {0, 2, 2, 2, 1, 1}, {0, 2, 0, 0, 1, 1}, {0, 2, 0, 0, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}
void LEFT_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, LEFT);
    T_SIDE faces[4] = {UP, FRONT, DOWN, BACK};
    int idx[4][6] = {{0, 2, 0, 0, 1, 1}, {0, 2, 0, 0, 1, 1}, {0, 2, 0, 0, 1, 1}, {0, 2, 2, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// RIGHT
void RIGHT_anticlockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, RIGHT);
    T_SIDE faces[4] = {UP, FRONT, DOWN, BACK};
    int idx[4][6] = {{0, 2, 2, 2, 1, 1}, {0, 2, 2, 2, 1, 1}, {0, 2, 2, 2, 1, 1}, {0, 2, 0, 0, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}
void RIGHT_clockwise(rubiks* box, int angle) { // OK
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, RIGHT);
    T_SIDE faces[4] = {UP, BACK, DOWN, FRONT};
    int idx[4][6] = {{0, 2, 2, 2, 1, 1}, {0, 2, 0, 0, 1, 1}, {0, 2, 2, 2, 1, 1}, {0, 2, 2, 2, 1, 1}};
    rotate_adjacent_faces(box, faces, idx);
  }
}

// Réarrangement aléatoire du rubiks cube
void scramble_rubiks(rubiks* box) {

  // 100 mouvement de réarrangement
  for (int i=0; i<100; i++) {
    printf("%d ", i);
    // Tirer aléatoirement une face
    T_SIDE face = get_random(0, 5);

    // Tirer aléatoirement un sens
    // 1 --> Sens négatif : Sens de la montre
    // 0 --> Sens positif : Inverse
    int sens = get_random(0, 1);

    // Tirer aléatoirement un angle
    // 2 --> 1/2-tour = 1/4 * 2
    // 1 --> 1/4-tour
    int angle = get_random(1, 2);

    // Faire une rotation
    rotation(box, face, sens, angle);
  }

}

// Désallouer le rubiks cube (supprimer en mémoire)
void free_rubiks(rubiks* box) {
  free(box);
}

// Cette fontion permet de faire une rotation en utilisant
// 1. Une face
// 2. Le sens de la rotation
// 3. angle de la rotation (1/4 ou 1/2)
// Cette fonction appelle soit anticlockwise ou clockwise en fonction du sens de la rotation
void rotation(rubiks* box, T_SIDE face, int sens, int angle) {
  switch (face) {
    case FRONT:
      if (sens == 0) {
        FRONT_anticlockwise(box, angle);
      } else {
        FRONT_clockwise(box, angle);
      }
      break;
    case BACK:
      if (sens == 0) {
        BACK_anticlockwise(box, angle);
      } else {
        BACK_clockwise(box, angle);
      }
      break;
    case UP:
      if (sens == 0) {
        UP_anticlockwise(box, angle);
      } else {
        UP_clockwise(box, angle);
      }
      break;
    case DOWN:
      if (sens == 0) {
        DOWN_anticlockwise(box, angle);
      } else {
        DOWN_clockwise(box, angle);
      }
      break;
    case RIGHT:
      if (sens == 0) {
        RIGHT_anticlockwise(box, angle);
      } else {
        RIGHT_clockwise(box, angle);
      }
      break;
    case LEFT:
      if (sens == 0) {
        LEFT_anticlockwise(box, angle);
      } else {
        LEFT_clockwise(box, angle);
      }
      break;
    default:
      perror("This side doesn't exists");
  }
}


void display_rubiks(rubiks* box) { // OK
  // Utiliser CONIO pour afficher le rubiks
  face u = box->faces[side_to_index(box, UP)];
  face d = box->faces[side_to_index(box, DOWN)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];
  face f = box->faces[side_to_index(box, FRONT)];
  face b = box->faces[side_to_index(box, BACK)];

  printf("\n");
  printf("                         %s %s %s             \n", select_color(u.cellules[0][0].couleur), select_color(u.cellules[0][1].couleur), select_color(u.cellules[0][2].couleur));
  printf("                         %s %s %s             \n", select_color(u.cellules[1][0].couleur), select_color(u.cellules[1][1].couleur), select_color(u.cellules[1][2].couleur));
  printf("                         %s %s %s             \n", select_color(u.cellules[2][0].couleur), select_color(u.cellules[2][1].couleur), select_color(u.cellules[2][2].couleur));

  printf("                   %s %s %s", select_color(l.cellules[0][0].couleur), select_color(l.cellules[0][1].couleur), select_color(l.cellules[0][2].couleur));
  printf(" %s %s %s", select_color(f.cellules[0][0].couleur), select_color(f.cellules[0][1].couleur), select_color(f.cellules[0][2].couleur));
  printf(" %s %s %s", select_color(r.cellules[0][0].couleur), select_color(r.cellules[0][1].couleur), select_color(r.cellules[0][2].couleur));
  printf(" %s %s %s\n", select_color(b.cellules[0][0].couleur), select_color(b.cellules[0][1].couleur), select_color(b.cellules[0][2].couleur));

  printf("                   %s %s %s", select_color(l.cellules[1][0].couleur), select_color(l.cellules[1][1].couleur), select_color(l.cellules[1][2].couleur));
  printf(" %s %s %s", select_color(f.cellules[1][0].couleur), select_color(f.cellules[1][1].couleur), select_color(f.cellules[1][2].couleur));
  printf(" %s %s %s", select_color(r.cellules[1][0].couleur), select_color(r.cellules[1][1].couleur), select_color(r.cellules[1][2].couleur));
  printf(" %s %s %s\n", select_color(b.cellules[1][0].couleur), select_color(b.cellules[1][1].couleur), select_color(b.cellules[1][2].couleur));

  printf("                   %s %s %s", select_color(l.cellules[2][0].couleur), select_color(l.cellules[2][1].couleur), select_color(l.cellules[2][2].couleur));  
  printf(" %s %s %s", select_color(f.cellules[2][0].couleur), select_color(f.cellules[2][1].couleur), select_color(f.cellules[2][2].couleur));  
  printf(" %s %s %s", select_color(r.cellules[2][0].couleur), select_color(r.cellules[2][1].couleur), select_color(r.cellules[2][2].couleur));
  printf(" %s %s %s\n", select_color(b.cellules[2][0].couleur), select_color(b.cellules[2][1].couleur), select_color(b.cellules[2][2].couleur));

  printf("                         %s %s %s             \n", select_color(d.cellules[0][0].couleur), select_color(d.cellules[0][1].couleur), select_color(d.cellules[0][2].couleur));
  printf("                         %s %s %s             \n", select_color(d.cellules[1][0].couleur), select_color(d.cellules[1][1].couleur), select_color(d.cellules[1][2].couleur));
  printf("                         %s %s %s             \n", select_color(d.cellules[2][0].couleur), select_color(d.cellules[2][1].couleur), select_color(d.cellules[2][2].couleur));

  printf("\n");
}

void display_rubiks2(rubiks* box) { // OK
  // Récupérer toutes les faces du rubik's cube
  face u = box->faces[side_to_index(box, UP)];
  face d = box->faces[side_to_index(box, DOWN)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];
  face f = box->faces[side_to_index(box, FRONT)];
  face b = box->faces[side_to_index(box, BACK)];

  
  printf("\n");
  printf("                         %s %s %s             \n", select_color2(u.cellules[0][0].couleur), select_color2(u.cellules[0][1].couleur), select_color2(u.cellules[0][2].couleur));
  printf("                         %s %s %s             \n", select_color2(u.cellules[1][0].couleur), select_color2(u.cellules[1][1].couleur), select_color2(u.cellules[1][2].couleur));
  printf("                         %s %s %s             \n", select_color2(u.cellules[2][0].couleur), select_color2(u.cellules[2][1].couleur), select_color2(u.cellules[2][2].couleur));

  printf("                   %s %s %s", select_color2(l.cellules[0][0].couleur), select_color2(l.cellules[0][1].couleur), select_color2(l.cellules[0][2].couleur));
  printf(" %s %s %s", select_color2(f.cellules[0][0].couleur), select_color2(f.cellules[0][1].couleur), select_color2(f.cellules[0][2].couleur));
  printf(" %s %s %s", select_color2(r.cellules[0][0].couleur), select_color2(r.cellules[0][1].couleur), select_color2(r.cellules[0][2].couleur));
  printf(" %s %s %s\n", select_color2(b.cellules[0][0].couleur), select_color2(b.cellules[0][1].couleur), select_color2(b.cellules[0][2].couleur));

  printf("                   %s %s %s", select_color2(l.cellules[1][0].couleur), select_color2(l.cellules[1][1].couleur), select_color2(l.cellules[1][2].couleur));
  printf(" %s %s %s", select_color2(f.cellules[1][0].couleur), select_color2(f.cellules[1][1].couleur), select_color2(f.cellules[1][2].couleur));
  printf(" %s %s %s", select_color2(r.cellules[1][0].couleur), select_color2(r.cellules[1][1].couleur), select_color2(r.cellules[1][2].couleur));
  printf(" %s %s %s\n", select_color2(b.cellules[1][0].couleur), select_color2(b.cellules[1][1].couleur), select_color2(b.cellules[1][2].couleur));

  printf("                   %s %s %s", select_color2(l.cellules[2][0].couleur), select_color2(l.cellules[2][1].couleur), select_color2(l.cellules[2][2].couleur));  
  printf(" %s %s %s", select_color2(f.cellules[2][0].couleur), select_color2(f.cellules[2][1].couleur), select_color2(f.cellules[2][2].couleur));  
  printf(" %s %s %s", select_color2(r.cellules[2][0].couleur), select_color2(r.cellules[2][1].couleur), select_color2(r.cellules[2][2].couleur));
  printf(" %s %s %s\n", select_color2(b.cellules[2][0].couleur), select_color2(b.cellules[2][1].couleur), select_color2(b.cellules[2][2].couleur));

  printf("                         %s %s %s             \n", select_color2(d.cellules[0][0].couleur), select_color2(d.cellules[0][1].couleur), select_color2(d.cellules[0][2].couleur));
  printf("                         %s %s %s             \n", select_color2(d.cellules[1][0].couleur), select_color2(d.cellules[1][1].couleur), select_color2(d.cellules[1][2].couleur));
  printf("                         %s %s %s             \n", select_color2(d.cellules[2][0].couleur), select_color2(d.cellules[2][1].couleur), select_color2(d.cellules[2][2].couleur));

  printf("\n");
}


// Réaliser une croix parfaite
void step1_1(rubiks* box) {
  face u = box->faces[side_to_index(box, UP)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];

  if (/* Up side */
      u.cellules[0][1].couleur == W &&
      u.cellules[1][0].couleur == W &&
      u.cellules[1][1].couleur == W &&
      u.cellules[1][2].couleur == W &&
      u.cellules[2][1].couleur == G &&
      /* Left side */
      l.cellules[0][1].couleur == W &&
      l.cellules[1][1].couleur == G &&
      /* Right side */
      r.cellules[0][1].couleur == R &&
      r.cellules[1][1].couleur == R) {
      
      FRONT_clockwise(box, 1);
      UP_anticlockwise(box, 1);
      RIGHT_clockwise(box, 1);
      UP_clockwise(box, 1);
  }
}

void step1_2(rubiks* box) {
  face u = box->faces[side_to_index(box, UP)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];

  if (/* Up side */
      u.cellules[0][1].couleur == W &&
      u.cellules[1][0].couleur == W &&
      u.cellules[1][1].couleur == W &&
      /* Left side */
      l.cellules[1][1].couleur == G &&
      l.cellules[2][1].couleur == W &&
      /* Right side */
      r.cellules[0][1].couleur == R &&
      r.cellules[1][1].couleur == R) {
      
      FRONT_anticlockwise(box, 1);
      RIGHT_anticlockwise(box, 1);
      DOWN_anticlockwise(box, 1);
      RIGHT_clockwise(box, 1);
      FRONT_anticlockwise(box, 2);
  }
}

void step1_3(rubiks* box) {
  face u = box->faces[side_to_index(box, UP)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];

  if (/* Up side */
      u.cellules[0][1].couleur == W &&
      u.cellules[1][0].couleur == W &&
      u.cellules[1][1].couleur == W &&
      /* Left side */
      l.cellules[1][1].couleur == G &&
      l.cellules[1][2].couleur == W &&
      /* Right side */
      r.cellules[0][1].couleur == R &&
      r.cellules[1][1].couleur == R &&
      r.cellules[1][0].couleur == G) {
      
      RIGHT_anticlockwise(box, 1);
      DOWN_anticlockwise(box, 1);
      RIGHT_clockwise(box, 1);
      FRONT_anticlockwise(box, 2);
  }
}

void step1_4(rubiks* box) {
  face u = box->faces[side_to_index(box, UP)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];

  if (/* Up side */
      u.cellules[1][1].couleur == W &&
      u.cellules[2][1].couleur == R &&
      /* Left side */
      l.cellules[0][1].couleur == W &&
      l.cellules[1][1].couleur == G &&
      /* Right side */
      r.cellules[1][1].couleur == R) {
      
      FRONT_clockwise(box, 1);
      RIGHT_clockwise(box, 1);
  }
}

void step1_5(rubiks* box) {
  face u = box->faces[side_to_index(box, UP)];
  face l = box->faces[side_to_index(box, LEFT)];
  face r = box->faces[side_to_index(box, RIGHT)];

  if (/* Up side */
      u.cellules[1][1].couleur == W &&
      u.cellules[1][2].couleur == O &&
      /* Left side */
      l.cellules[1][1].couleur == G &&
      /* Right side */
      r.cellules[0][1].couleur == W &&
      r.cellules[1][1].couleur == R) {
      
      RIGHT_anticlockwise(box, 1);
      FRONT_anticlockwise(box, 1);
      UP_clockwise(box, 1);
  }
}

void step1(rubiks* box) {

  while (1) {
    step1_1(box);
    step1_2(box);
    step1_3(box);
    step1_4(box);
    step1_5(box);
  }

}


// Réaliser les coins blancs et la première couronne
void step2_1(rubiks* box) {
  face f = box->faces[side_to_index(box, FRONT)];
  face r = box->faces[side_to_index(box, RIGHT)];
  face d = box->faces[side_to_index(box, DOWN)];

  // Si l’une des couleurs du coin est blanche (Coin front inférieur à droite)
  if (f.cellules[2][2].couleur == W ||
      r.cellules[2][0].couleur == W ||
      d.cellules[0][2].couleur == W) {
      
      while (1) {
        int nbre = 0;

        if (f.cellules[2][2].couleur != W) {
          if (f.cellules[2][2].couleur == r.cellules[1][1].couleur ||
              f.cellules[2][2].couleur == d.cellules[1][1].couleur) {
            nbre++;
          }
        }

        if (r.cellules[2][0].couleur != W) {
          if (r.cellules[2][0].couleur == f.cellules[1][1].couleur ||
              r.cellules[2][0].couleur == d.cellules[1][1].couleur) {
            nbre++;
          }
        }
        
        if (d.cellules[0][2].couleur != W) {
          if (d.cellules[0][0].couleur == f.cellules[1][1].couleur ||
              d.cellules[0][2].couleur == r.cellules[1][1].couleur) {
            nbre++;
          }
        }

        if (nbre == 2) {
          break;
        }
        
        DOWN_clockwise(box, 1);
      }

  }
  
}

void step2_2(rubiks* box) {
  RIGHT_anticlockwise(box, 1);
  DOWN_anticlockwise(box, 1);
  RIGHT_clockwise(box, 1);
  DOWN_clockwise(box, 1);
}

void step2(rubiks* box) {
  while (1) {
    step2_1(box);
    step2_2(box);
  }
}

// L'algorithme de résolution
void solve_rubiks(rubiks* box) {
  step1(box);
  step2(box);
}