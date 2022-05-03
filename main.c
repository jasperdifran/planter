#include "calendar.h"

int main(void) {
  print_intro();
  struct calendar *cal = new_cal();
  write_cal(cal);
  free_cal(cal);
}