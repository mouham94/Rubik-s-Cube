#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rubiks.h"

//----------------//
//      DATA      //
//----------------//
T_SIDE FRONT_ANTICLOCK[4] = {DOWN, RIGHT, UP, LEFT};
T_SIDE FRONT_CLOCKWISE[4] = {DOWN, LEFT, UP, RIGHT};
T_SIDE BACK_ANTICLOCK[4]  = {UP, RIGHT, DOWN, LEFT};
T_SIDE BACK_CLOCKWISE[4]  = {UP, LEFT, DOWN, RIGHT};
T_SIDE UP_ANTICLOCK[4]    = {BACK, LEFT, FRONT, RIGHT};
T_SIDE UP_CLOCKWISE[4]    = {BACK, RIGHT, FRONT, LEFT};
T_SIDE DOWN_ANTICLOCK[4]  = {FRONT, LEFT, BACK, RIGHT};
T_SIDE DOWN_CLOCKWISE[4]  = {FRONT, RIGHT, BACK, LEFT};
T_SIDE LEFT_ANTICLOCK[4]  = {UP, BACK, DOWN, FRONT};
T_SIDE LEFT_CLOCKWISE[4]  = {UP, FRONT, DOWN, BACK};
T_SIDE RIGHT_ANTICLOCK[4] = {UP, FRONT, DOWN, BACK};
T_SIDE RIGHT_CLOCKWISE[4] = {UP, BACK, DOWN, FRONT};

int FRONT_ANTICLOCK_CONFIG[4][6] = {{BEG, BEG, BEG, END, BEG_TO_END, END_TO_BEG}, {BEG, END, BEG, BEG, BEG_TO_END, BEG_TO_END}, {END, END, BEG, END, BEG_TO_END, BEG_TO_END}, {BEG, END, END, END, END_TO_BEG, BEG_TO_END}};
int FRONT_CLOCKWISE_CONFIG[4][6] = {{BEG, BEG, BEG, END, BEG_TO_END, END_TO_BEG}, {BEG, END, END, END, END_TO_BEG, BEG_TO_END}, {END, END, BEG, END, BEG_TO_END, BEG_TO_END}, {BEG, END, BEG, BEG, BEG_TO_END, BEG_TO_END}};
int BACK_ANTICLOCK_CONFIG[4][6]  = {{BEG, BEG, BEG, END, 1, 1}, {BEG, END, END, END, 1, 1}, {END, END, BEG, END, 1, 1}, {BEG, END, BEG, BEG, 1, 1}};
int BACK_CLOCKWISE_CONFIG[4][6]  = {{BEG, BEG, BEG, END, 1, 1}, {BEG, END, BEG, BEG, 1, 1}, {END, END, BEG, END, 1, 1}, {BEG, END, END, END, 1, 1}};
int UP_ANTICLOCK_CONFIG[4][6]    = {{BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}};
int UP_CLOCKWISE_CONFIG[4][6]    = {{BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}, {BEG, BEG, BEG, END, 1, 1}};
int DOWN_ANTICLOCK_CONFIG[4][6]  = {{END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}};
int DOWN_CLOCKWISE_CONFIG[4][6]  = {{END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}, {END, END, BEG, END, 1, 1}};
int LEFT_ANTICLOCK_CONFIG[4][6]  = {{BEG, END, BEG, BEG, 1, 1}, {BEG, END, END, END, 1, 1}, {BEG, END, BEG, BEG, 1, 1}, {BEG, END, BEG, BEG, 1, 1}};
int LEFT_CLOCKWISE_CONFIG[4][6]  = {{BEG, END, BEG, BEG, 1, 1}, {BEG, END, BEG, BEG, 1, 1}, {BEG, END, BEG, BEG, 1, 1}, {BEG, END, END, END, 1, 1}};
int RIGHT_ANTICLOCK_CONFIG[4][6] = {{BEG, 2, 2, 2, 1, 1}, {BEG, 2, 2, 2, 1, 1}, {BEG, 2, 2, 2, 1, 1}, {BEG, 2, BEG, BEG, 1, 1}};
int RIGHT_CLOCKWISE_CONFIG[4][6] = {{BEG, 2, 2, 2, 1, 1}, {BEG, 2, BEG, BEG, 1, 1}, {BEG, 2, 2, 2, 1, 1}, {BEG, 2, 2, 2, 1, 1}};

/*******************/
/****** Tools ******/
/*******************/
int color[NBR_FAC] = {G, B, W, Y, R, O};

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

int side_to_index(rubiks* box, T_SIDE face) {
  for (int i=0; i<NBR_FAC; i++) {
    if (box->faces[i].name == face) return i;
  }
  perror("Cette face n'existe pas");
  exit(EXIT_FAILURE);
}

void* allocate(int size) {
  void* mem = (void*) malloc(size);
  if (!mem) {
    perror("Error during allocation");
    exit(EXIT_FAILURE);
  }
  return mem;
}

int get_random(int min, int max) {
  return rand() % (max - min + 1) + min;
}

cellule* get_cells(rubiks* box, T_SIDE face, int itv[6]) {
  cellule* cells = (cellule*) allocate(sizeof(cellule) * NBR_CL);
  int k = 0;
  int idx = side_to_index(box, face);
  for (int i = INIT(itv[4], itv[0], itv[1]); COND(i, itv[4], itv[0], itv[1]); i+=itv[4]) {
    for (int j = INIT(itv[5], itv[2], itv[3]); COND(j, itv[5], itv[2], itv[3]); j+=itv[5]) {
      cells[k++] = box->faces[idx].cellules[i][j];
    }
  }
  return cells;
}

void set_cells2(rubiks* box, T_SIDE face_dst, int itv_dst[6], cellule* cells) {
  int k = 0;
  int idx = side_to_index(box, face_dst);
  for (int i = INIT(itv_dst[4], itv_dst[0], itv_dst[1]); COND(i, itv_dst[4], itv_dst[0], itv_dst[1]); i+=itv_dst[4]) {
    for (int j = INIT(itv_dst[5], itv_dst[2], itv_dst[3]); COND(j, itv_dst[5], itv_dst[2], itv_dst[3]); j+=itv_dst[5]) {
      box->faces[idx].cellules[i][j] = cells[k++];
    }
  }
}

void set_cells(rubiks* box, T_SIDE face_src, T_SIDE face_dst, int itv_src[6], int itv_dst[6]) {
  cellule* cells_src = get_cells(box, face_src, itv_src);
  set_cells2(box, face_dst, itv_dst, cells_src);
  free(cells_src);
}

rubiks* create_rubiks() {
  rubiks* box = (rubiks*) allocate(sizeof(rubiks));
  return box;
}

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

void blank_rubiks(rubiks* box) {
  for(int k=0; k<NBR_FAC; k++) {
    for (int i=0; i<NBR_CL; i++) {
      for (int j=0; j<NBR_CL; j++) {
        box->faces[k].cellules[i][j].couleur = LG;
      }
    }
  }
}

void fill_rubiks(rubiks* box, T_SIDE name, int i, int j, T_COLOR couleur) {
  if ((i < 0 || i > NBR_CL) && (j < 0 || j > NBR_CL)) {
    perror("Erreur dans fill_rubiks");
    exit(EXIT_FAILURE);
  }
  box->faces[side_to_index(box, name)].cellules[i][j].couleur = couleur;
}

void rotate_face_clockwise(rubiks* box, T_SIDE face) {
  cellule cellules[NBR_CL][NBR_CL];
  int idx = side_to_index(box, face);
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      cellules[i][j] = box->faces[idx].cellules[NBR_CL-j-1][i];
    }
  }
  for (int i=0; i<NBR_CL; i++) {
    for (int j=0; j<NBR_CL; j++) {
      box->faces[idx].cellules[i][j] = cellules[i][j];
    }
  }
}

void rotate_face_anticlockwise(rubiks* box, T_SIDE face) {
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
}

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

void FRONT_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, FRONT);
    rotate_adjacent_faces(box, FRONT_ANTICLOCK, FRONT_ANTICLOCK_CONFIG); 
  }
}

void FRONT_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, FRONT);
    rotate_adjacent_faces(box, FRONT_CLOCKWISE, FRONT_CLOCKWISE_CONFIG);
  }
}

void BACK_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, BACK);
    rotate_adjacent_faces(box, BACK_ANTICLOCK, BACK_ANTICLOCK_CONFIG);
  }
}

void BACK_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, BACK);
    rotate_adjacent_faces(box, BACK_CLOCKWISE, BACK_CLOCKWISE_CONFIG);
  }
}

void UP_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, UP);
    rotate_adjacent_faces(box, UP_ANTICLOCK, UP_ANTICLOCK_CONFIG);
  }
}

void UP_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, UP);
    rotate_adjacent_faces(box, UP_CLOCKWISE, UP_CLOCKWISE_CONFIG);
  }
}

void DOWN_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, DOWN);
    rotate_adjacent_faces(box, DOWN_ANTICLOCK, DOWN_ANTICLOCK_CONFIG);
  }
}

void DOWN_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, DOWN);
    rotate_adjacent_faces(box, DOWN_CLOCKWISE, DOWN_CLOCKWISE_CONFIG);
  }
}

void LEFT_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, LEFT);
    rotate_adjacent_faces(box, LEFT_ANTICLOCK, LEFT_ANTICLOCK_CONFIG);
  }
}

void LEFT_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, LEFT);
    rotate_adjacent_faces(box, LEFT_CLOCKWISE, LEFT_CLOCKWISE_CONFIG);
  }
}

void RIGHT_anticlockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_anticlockwise(box, RIGHT);
    rotate_adjacent_faces(box, RIGHT_ANTICLOCK, RIGHT_ANTICLOCK_CONFIG);
  }
}

void RIGHT_clockwise(rubiks* box, int angle) {
  for (int i=0; i<angle; i++) {
    rotate_face_clockwise(box, RIGHT);
    rotate_adjacent_faces(box, RIGHT_CLOCKWISE, RIGHT_CLOCKWISE_CONFIG);
  }
}

void scramble_rubiks(rubiks* box) {
  for (int i=0; i<100; i++) {
    T_SIDE face = get_random(0, 5);
    int sens = get_random(0, 1);
    int angle = get_random(1, 2);
    rotation(box, face, sens, angle);
  }
}

void free_rubiks(rubiks* box) {
  free(box);
}

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

void display_rubiks(rubiks* box) {
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

void display_rubiks2(rubiks* box) {
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
