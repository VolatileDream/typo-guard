#include "minunit.h"
#include <stdio.h>

jmp_buf __mu_test_buffer;
volatile int __mu_state = 0;
volatile int tests_run = 0;
volatile int asserts_run = 0;
volatile int failures = 0;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;
  printf("Asserts checked: %d\n", asserts_run);
  printf("Asserts failed: %d\n", failures);
  printf("Tests run: %d\n", tests_run);

  return failures == 0;
}
