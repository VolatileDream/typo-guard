#include "minunit.h"
#include <stdint.h>
#include "libbase16.h"

mu_test(b16_convert_to_int) {
  mu_assert("", b16_ctoi('0') == 0);
  mu_assert("", b16_ctoi('1') == 1);
  mu_assert("", b16_ctoi('2') == 2);
  mu_assert("", b16_ctoi('3') == 3);
  mu_assert("", b16_ctoi('4') == 4);
  mu_assert("", b16_ctoi('5') == 5);
  mu_assert("", b16_ctoi('6') == 6);
  mu_assert("", b16_ctoi('7') == 7);
  mu_assert("", b16_ctoi('8') == 8);
  mu_assert("", b16_ctoi('9') == 9);
  mu_assert("", b16_ctoi('A') == 10);
  mu_assert("", b16_ctoi('B') == 11);
  mu_assert("", b16_ctoi('C') == 12);
  mu_assert("", b16_ctoi('D') == 13);
  mu_assert("", b16_ctoi('E') == 14);
  mu_assert("", b16_ctoi('F') == 15);
}

mu_test(b16_converts_to_char) {
  mu_assert("", b16_itoc(0) == '0');
  mu_assert("", b16_itoc(1) == '1');
  mu_assert("", b16_itoc(2) == '2');
  mu_assert("", b16_itoc(3) == '3');
  mu_assert("", b16_itoc(4) == '4');
  mu_assert("", b16_itoc(5) == '5');
  mu_assert("", b16_itoc(6) == '6');
  mu_assert("", b16_itoc(7) == '7');
  mu_assert("", b16_itoc(8) == '8');
  mu_assert("", b16_itoc(9) == '9');
  mu_assert("", b16_itoc(10) == 'A');
  mu_assert("", b16_itoc(11) == 'B');
  mu_assert("", b16_itoc(12) == 'C');
  mu_assert("", b16_itoc(13) == 'D');
  mu_assert("", b16_itoc(14) == 'E');
  mu_assert("", b16_itoc(15) == 'F');
}

mu_test(b16_conversion_from_string) {
  int32_t val = -1;
  mu_expect(b16_atoi("00000000", &val));
  mu_assert("Conversion of 00000000 to int failed", val == 0);
}

mu_test(b16_conversion_to_string) {
  char val[9] = {0};
  mu_expect(b16_itoa(0, &val));
  for (int i = 0; i < 8; i++) {
    mu_assert("", val[i] == '0');
  }
}

mu_test(b16_conversion_from_almost_max_int) {
  int32_t val = 0x7FFFFFFF;
  char buf[9] = {0};
  char expect[9] = "7FFFFFFF";

  mu_expect(b16_itoa(val, &buf));
  for (int i = 0; i < 9; i++) {
    mu_assert("", expect[i] == buf[i]);
  }
}
