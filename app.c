#include "app.h"

#include "data/base16/libbase16.h"
#include "third-party/smhasher/src/PMurHash.h"

#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>

// Process the input & output interactively.
// Requires that stdin & stdout are connected to a terminal.
int32_t app_process_interactive(app_t *a, FILE *in, FILE *out, FILE *interactive);

// Simple pipeline processing, don't be interactive.
int32_t app_process_pipe(app_t *a, FILE *in, FILE *out);

struct typo_guard_app {
  int16_t columns;
};

app_t* app_init() {
  app_t *res = (app_t*) malloc(sizeof(app_t));
  res->columns = 100;
  return res;
}
void app_del(app_t* a) {
  free(a);
}

void app_set_columns(app_t *a, char* columns) {
  if (columns) {
    int16_t count = atoi(columns);
    a->columns = count;
  }
}

int32_t app_process(app_t *a, FILE *in, FILE *save, FILE *interactive) {
  if (isatty(fileno(in)) && interactive && isatty(fileno(interactive))) {
    return app_process_interactive(a, in, save, interactive);
  }
  return app_process_pipe(a, in, save);
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

void hash(char *buffer, uint16_t length, char (*out)[9]) {
  int32_t hash = PMurHash32(0, buffer, length);
  b16_itoa(hash, out);
}

int32_t app_process_pipe(app_t *a, FILE *in, FILE *out) {
  // Stack allocate 3x the column count.
  const size_t max_length = a->columns;
  char *buffer = (char*)alloca(max_length + 1);
  char b16[9] = {0};
  uint16_t length = 0;
  while (read_line(in, buffer, max_length, &length)) {
    if (length == 0) {
      // Print out empty lines without dummy hash info.
      fprintf(out, "--------\n");
      continue;
    }
    buffer[length] = 0;
    hash(buffer, length, &b16);
    fprintf(out, "%s %s\n", b16, buffer);
  }
  return !(ferror(in) || ferror(out));
}

void reprint_line(FILE *out, char* buffer, uint16_t length) {
  char b16[9] = {0};
  hash(buffer, length, &b16);
  fprintf(out, "\r%s %s", b16, buffer);
}

void unbuffer(FILE *f) {
  // Unbuffer in our application.
  setbuf(f, NULL);
  int fd = fileno(f);
  struct termios term;
  if (tcgetattr(fd, &term)) {
    perror("Unable to get terminal attributes");
    exit(1);
  }
  term.c_lflag &= ~(ECHO | ICANON);
  if (tcsetattr(fd, 0, &term)) {
    perror("Unable to set terminal attributes");
    exit(1);
  }
}

int32_t app_process_interactive(app_t *a, FILE *in, FILE *save, FILE *interactive) {
  unbuffer(in);
  unbuffer(interactive);

  const size_t max_length = a->columns;
  char *buffer = (char*) alloca(max_length + 2); // +2 for when we print backspace characters

  bzero(buffer, max_length + 2);

  uint16_t index = 0;
  while (!feof(in)) {
    int current = fgetc(in);
    switch (current) {
     case '\n':
     case EOF:
      if (index != 0) {
        buffer[index] = 0;
        reprint_line(interactive, buffer, index);
      } else {
        fprintf(interactive, "--------");
      }
      fprintf(interactive, "\n");
      fprintf(save, "%s\n", buffer);
      index = 0;
      bzero(buffer, max_length + 2);
      break;
     case '\x7f': // Backspace
      if (index == 0) { break; }
      fprintf(interactive, "\b \b");
      buffer[index - 1] = 0;
      buffer[index] = 0;
      index--;
      reprint_line(interactive, buffer, index);
      break;
     default:
      buffer[index] = (char) current;
      index++;
      reprint_line(interactive, buffer, index);
      if (index >= max_length) {
        fprintf(interactive, "\n");
        fprintf(save, "%s\n", buffer);
        index = 0;
        bzero(buffer, max_length + 2);
      }
      break;
    }
  }

  return !(ferror(in) || ferror(save) || ferror(interactive));
}

