#include "s21_tetris.h"

void clear_field() {
  GameInfo_t *info = get_instance();
  for (int i = 0; i < height1; i++) {
    for (int j = 0; j < width1; j++) {
      if (info->field[i][j] == 1) {
        info->field[i][j] = 0;
      }
    }
  }
}
void analyze_field1() {
  GameInfo_t *info = get_instance();
  int i = 0;
  int count_line = 0;
  for (i = height1 - 1; i >= 0; i--) {
    int flag = 0;
    for (int j = 0; j < width1; j++) {
      if (info->field[i][j] != 2) {
        flag = 1;
        break;
      }
    }
    if (flag == 0) {
      analyze_field2(i);
      i = i + 1;
      count_line++;
    }
  }
  info->score += 200 * pow(2, count_line - 1) - 100;
  if (info->high_score < info->score) {
    info->high_score = info->score;
    play_w_file("w", info->high_score);
  }
  info->level = info->score / 600 + 1;
  info->speed = (info->level - 1) * 100;
}
void analyze_field2(int i) {
  GameInfo_t *info = get_instance();
  for (int z = i; z > 0; z--) {
    for (int j = 0; j < width1; j++) {
      info->field[z][j] = info->field[z - 1][j];
    }
  }
  for (int j = 0; j < width1; j++) {
    info->field[0][j] = 0;
  }
}
void full_clear() {
  GameInfo_t *info = get_instance();
  for (int i = 0; i < height1; i++) {
    for (int j = 0; j < width1; j++) {
      info->field[i][j] = 0;
    }
  }
}
int play_w_file(char *opt, int score) {
  int res = 0;
  FILE *fp = fopen("highscore_tetris.txt", opt);
  char str[100];
  if (strcmp(opt, "r") == 0 && fp) {
    fgets(str, 100, fp);
    res = atoi(str);

  } else if (strcmp(opt, "w") == 0 && fp) {
    sprintf(str, "%d", score);
    fputs(str, fp);
  }
  if (fp != NULL) fclose(fp);

  return res;
}
void work_with_field(int a) {
  GameInfo_t *info = get_instance();
  if (a == 0) {  //
    info->field = malloc(sizeof(int *) * height1);
    for (int i = 0; i < height1; i++) {
      info->field[i] = malloc(sizeof(int) * width1);
    }
  } else if (a == 1) {
    if (info->field != NULL) {  // <--- проверка!
      for (int i = 0; i < height1; i++) {
        free(info->field[i]);
        info->field[i] = NULL;
      }
      free(info->field);
      info->field = NULL;
    }
  }
}