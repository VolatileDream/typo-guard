#include "app.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void usage(char *arg0) {
  printf("usage: %s\n\n", arg0);
  printf("  --columns|-c <100> : sets the column wrap limit\n");
  printf("  --help|-h|-? : prints this usage message\n");
}

int run(app_t *app, int argc, char* argv[]) {
  struct option options[] = {
    // { char* name, has_arg, int*flag, int }, // short name
    // Filter setup options
    { "columns", required_argument, 0, 'c' }, // c:

    // Funny options
    { "help", no_argument, 0, 'h' }, // h

    { 0, 0, 0, 0 },
  };

  char* columns = 0;

  while(true) {
    const int c = getopt_long(argc, argv, "c:h", options, 0);
    if (c == -1) {
      break;
    }

    switch(c) {
     case 'c':
      if (columns) {
        fprintf(stderr, "number of columns already set: %s\n", columns);
        return 6;
      }
      columns = optarg;
      break;
     case '?':
     case 'h':
      usage(argv[0]);
      return 1;
    }
  }

  if (columns) {
    int16_t c = atoi(columns);
    if (c > 0) {
      app_set_columns(app, c);
    }
  }

  return app_process(app, stdin, stdout);
}
int main(int argc, char **argv) {
  app_t *app = app_init();
  int rc = run(app, argc, argv);
  app_del(app);
  return rc;
}
