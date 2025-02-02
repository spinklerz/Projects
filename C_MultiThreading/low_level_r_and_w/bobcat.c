#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define MAX_LEN 64
int main(int argc, char *argv[]) {
  char Buffer[MAX_LEN];
  int fd, bytes;
  bool ErrorCode = 0;
  size_t nbytes = MAX_LEN;
  struct stat file_stats;
  if (argc == 1) {
    while ((bytes = read(0, Buffer, nbytes)) > 0) {
      write(1, Buffer, bytes);
    }
  }
  for (int i = 1; i < argc; i++) {
    fd = open(argv[i], O_RDONLY);
    if (stat(argv[i], &file_stats) == 0) {
      if (S_ISDIR(file_stats.st_mode)) {
        warnx("%s: is a Directory", argv[i]);
        ErrorCode = 1;
      }
    }
    if (!strcmp(argv[i], "-")) {
      while ((bytes = read(0, Buffer, nbytes)) > 0) {
        write(1, Buffer, bytes);
      }
      continue;
    } else if (fd == -1) {
      ErrorCode = 1;
      warn("%s", argv[i]);
      close(fd);
      continue;
    }
    while ((bytes = read(fd, Buffer, nbytes)) > 0) {
      write(1, Buffer, bytes);
    }
    close(fd);
  }
  return ErrorCode;
}

/*
no args check
test_stdin_no_args (0/4) Check
test_short_write (0/4) NotCheck
test_error_no_file (0/4) Check
test_error_open_directory (0/4) NotCheck
test_error_permission (0/4) Check
test_error_stdin_dir (0/4) NotCheck
test_error_mix (0/4)
test_error_write (0/4)
*/
