#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char* descr =
  "Reads lines from standard input or file, printing any that contain\n"
  "characters outside the 7-bit ASCII range.\n\n"
  "Flags:\n"
  "-n\tPrints line numbers\n";

static void
usage(const char* argv0) {
  fprintf(stderr, "Usage: %s [-n] [file]\n%s", argv0, descr);
  exit(EXIT_FAILURE);
}

/*
 * Returns nonzero iff one of the first len characters in lin has its 8th bit
 * set.
 */
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
 * Selectively prints lines from in containing 8th-bit-on characters to out,
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
        fprintf(out, "%6d\t", countfrom);
      }
      fwrite(lin, 1, ret, out);
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
  int r;
  int countfrom = -1;
  FILE* in = stdin;

  opterr = 0;
  while (-1 != (r = getopt(argc, argv, "+n"))) {
    switch (r) {
    case 'n':
      countfrom = 1;
      break;
    default:
      usage(argv[0]);
    }
  }

  if (optind == argc - 1) {
    if (NULL == (in = fopen(argv[optind], "r"))) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }
  }
  else if (optind != argc) {
    usage(argv[0]);
  }

  if (-1 == nonascln(stdout, in, countfrom)) {
    perror("nonascln");
    exit(EXIT_FAILURE);
  }

  if (EOF == fclose(in)) {
    perror("fclose");
  }
  return 0;
}
