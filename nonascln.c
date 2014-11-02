#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int
has_nonascii(const char* lin, ssize_t r) {
  while (r-- > 0) {
    if (lin[r] == 0 || lin[r] & 0x80) {
      return 1;
    }
  }
  return 0;
}

int
main() {
  char*   lin;
  size_t  len;
  ssize_t r;

  len = 256;
  lin = malloc(len);
  if (0 == lin) {
    len = 0;
  }

  while (-1 != (r = getline(&lin, &len, stdin))) {
    if (has_nonascii(lin, r)) {
      printf("%s", lin);
    }
  }
  if (0 != errno) {
    err(EXIT_FAILURE, "getline");
  }

  free(lin);
  return 0;
}
