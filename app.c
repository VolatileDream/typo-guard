#include "app.h"

#include "libbase16.h"
#include "PMurHash.h"

#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

// Process the input & output interactively.
// Requires that stdin & stdout are connected to a terminal.
int32_t app_process_interactive(app_t *a, FILE *in, FILE *out);

// Simple pipeline processing, don't be interactive.
int32_t app_process_pipe(app_t *a, FILE *in, FILE *out);

struct typo_guard_app {
  int16_t columns;
};

app_t* app_init() {
  app_t *res = (app_t*) malloc(sizeof(app_t));
  res->columns = 0;
  return res;
}
void app_del(app_t* a) {
  free(a);
}

int16_t app_get_columns(app_t *a) {
  if (a->columns > 0) {
    return a->columns;
  }
  return 100;
}
void app_set_columns(app_t *a, int16_t columns) {
  a->columns = columns;
}

int32_t app_process(app_t *a, FILE *in, FILE *out) {
  if (isatty(fileno(in)) && isatty(fileno(out))) {
    fprintf(stderr, "tty not supported yet :c\n");
    //return app_process_interactive(a, in, out);
  }
  return app_process_pipe(a, in, out);
}

bool read_line(FILE *in, char *buf, uint16_t length, uint16_t *out_consumed) {
  uint16_t len = 0;
  while (len < length) {
    int c = fgetc(in);
    if (c == EOF) {
      return false;
    } else if (c == '\n') {
      break;
    }
    buf[len] = c;
    len++;
  }
  *out_consumed = len;
  return true;
}

int32_t app_process_pipe(app_t *a, FILE *in, FILE *out) {
  // Stack allocate 3x the column count.
  const size_t max_length = app_get_columns(a);
  char *buffer = (char*)alloca(max_length + 1);
  char b16[9] = {0};
  uint16_t length = 0;
  while (read_line(in, buffer, max_length, &length)) {
    if (length == 0) { continue; }
    buffer[length] = 0;
    int32_t hash = PMurHash32(0, buffer, length);
    b16_itoa(hash, &b16);
    fprintf(out, "%s %s\n", b16, buffer);
  }
  return !(ferror(in) || ferror(out));
}

/*
int32_t app_process_interactive(app_t *a, FILE *in, FILE *out) {

}
*/
