#ifndef _APP_H_
#define _APP_H_

#include <stdint.h>
#include <stdio.h>

typedef struct typo_guard_app app_t;
struct typo_guard_app;

app_t* app_init();
void app_del(app_t *a);

// Restricts the number of columns to use.
//
// Required for pipeline mode, but optional for interactive.
void app_set_columns(app_t *a, int16_t columns);

// Process input & output.
int32_t app_process(app_t *a, FILE *in, FILE *out);

#endif /* _APP_H_ */
