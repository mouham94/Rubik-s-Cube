#include <stdio.h>



int main() {

  int cells[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  int i_min = 0, i_max = 2, i_step = END_TO_BEG;
  int j_min = 0, j_max = 0, j_step = BEG_TO_END;

  for (int i = INIT(i_step, i_min, i_max); COND(i, i_step, i_min, i_max); i+=i_step) {
    for (int j = INIT(j_step, j_min, j_max); COND(j, j_step, j_min, j_max); j+=j_step) {
      printf("%d ", cells[i][j]);
    }
    printf("\n");
  }

    return 0;
}
