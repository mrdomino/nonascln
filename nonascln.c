#define _POSIX_C_SOURCE 200809L
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

char* argv0;

const char* descr =
  "Reads lines from standard input, printing any that contain characters\n"
  "with the high bit set.\n";

static void
usage() {
  fprintf(stderr, "Usage: %s\n%s", argv0, descr);
  exit(EXIT_FAILURE);
}

static int
has_nonascii(const char* lin, ssize_t len) {
  while (len-- > 0) {
    if (lin[len] & 0x80) {
      return 1;
    }
  }
  return 0;
}

static int
nonascln(FILE* out, FILE* in) {
  char*   lin;
  size_t  len;
  ssize_t ret;
  int     err;

  len = 256;
  lin = malloc(len);
  if (0 == lin) {
    len = 0;
  }

  while (-1 != (ret = getline(&lin, &len, in))) {
    if (has_nonascii(lin, ret)) {
      fprintf(out, "%s", lin);
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
  argv0 = argv[0];

  if (argc != 1) {
    usage();
  }

  if (-1 == nonascln(stdout, stdin)) {
    err(EXIT_FAILURE, "nonascln");
  }
  return 0;
}
