
#include "../brick_game/tetris/s21_tetris.h"
#include "check.h"

START_TEST(test_1) {
  work_with_field(0);
  init_data();
  GameInfo_t *info = get_instance();

  Point *dot = get_point();
  dot->state = Spawn;
  info->score = 70;
  while (dot->state != Game_Over) {
    machine();
  }

  ck_assert_int_eq(70, info->score);
  ck_assert_int_eq(70, info->high_score);
  analyze_field1();
  work_with_field(1);
}
END_TEST
START_TEST(test_2) {
  work_with_field(0);
  GameInfo_t *info = get_instance();
  for (int i = 16; i < height; i++) {
    for (int j = 0; j < width; j++) {
      info->field[i][j] = 2;
    }
  }
  analyze_field1();

  work_with_field(1);
}
END_TEST
START_TEST(test_3) {
  init_data();
  work_with_field(0);
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  dot->state = Moving;
  dot->c = 3;
  dot->x = 4;
  dot->y = 5;
  rotate_able();

  work_with_field(1);
}
END_TEST
START_TEST(test_4) {
  init_data();
  work_with_field(0);

  handling_start();
  updateCurrentState();
  userInput(Pause, false);
  work_with_field(1);
}
END_TEST
int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_1);
  tcase_add_test(tc1_1, test_2);
  tcase_add_test(tc1_1, test_3);
  tcase_add_test(tc1_1, test_4);
  int nf;
  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}