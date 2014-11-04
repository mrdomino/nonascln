#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static const char* descr =
  "Reads lines from standard input, printing any that contain characters\n"
  "outside the 7-bit ASCII range.\n";

static void
usage(const char* argv0) {
  fprintf(stderr, "Usage: %s\n%s", argv0, descr);
  exit(EXIT_FAILURE);
}

static int
has_nonascii(const char* lin, ssize_t len) {
  while (len-- > 0) {
    if ((char)0 != (lin[len] & 0x80)) {
      return 1;
    }
  }
  return 0;
}

/*
 * Selectively print lines from in containing 8th-bit-on characters to out,
 * with line counts starting at countfrom if countfrom is non-negative.
 */
static int
nonascln(FILE* out, FILE* in, int countfrom) {
  char*   lin;
  size_t  len;
  ssize_t ret;
  int     err;

  len = 256;
  lin = calloc(1, len);
  if (0 == lin) {
    len = 0;
  }

  while (-1 != (ret = getline(&lin, &len, in))) {
    assert(lin != 0);
    if (has_nonascii(lin, ret)) {
      if (countfrom >= 0) {
        fprintf(out, "%6d\t%s", countfrom, lin);
      }
      else fprintf(out, "%s", lin);
    }
    if (countfrom >= 0) {
      countfrom++;
    }
  }
  err = errno;

  free(lin);

  if (0 != err) {
    errno = err;
    return -1;
  }
  else return 0;
}

int
main(int argc, char* argv[]) {
  if (argc > 1) {
    usage(argv[0]);
  }

  if (-1 == nonascln(stdout, stdin, 1)) {
    err(EXIT_FAILURE, "nonascln");
  }
  return 0;
}
