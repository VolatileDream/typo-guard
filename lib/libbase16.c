#include "libbase16.h"

static char *mapping = "0123456789ABCDEF";

char b16_itoc(int32_t i) {
  if (i < 0 || 15 < i) {
    return -1;
  }
  return mapping[i];
}

int32_t b16_ctoi(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  } else {
    return -1;
  }
}

bool b16_itoa(int32_t input, char (*out_str)[9]) {
  for (int i = 0; i < 8; i++) {
    int32_t bits = (input >> (i * 4)) & 0xF;
    int pos = 7 - i;
    (*out_str)[pos] = b16_itoc(bits);
    if ((*out_str)[pos] == -1) {
      return false;
    }
  }
  return true;
}

bool b16_atoi(char str[9], int32_t *out_val) {
  int32_t val = 0;
  for (int i = 0; i < 8; i++) {
    int32_t v = b16_ctoi(str[i]);
    if (v == -1) {
      return false;
    }
    val = (val << 4) | v;
  }
  *out_val = val;
  return true;
}

