#ifndef __lib_base32__
#define __lib_base32__

#include <stdint.h>
#include <stdbool.h>

// Returns -1 if the integer passed is outside the range 0 to 31 (incl).
// Otherwise returns a character from the set:
// {0..9} u {A..F}
char b16_itoc(int32_t);

// Returns -1 if a bad character is passed.
// Otherwise returns a number from 0 to 31 (incl).
int32_t b16_ctoi(char);

// Converts a complete int32 into a null delimited string.
// Does not touch out_val[8], allowing it to be a null character.
bool b16_itoa(int32_t, char (*out_val)[9]);
// Converts a complete null delimited string into int32.
bool b16_atoi(char[9], int32_t *out_val);

#endif /* __lib_base32__ */
