#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <terror/dbg.h>
#include <terror/input_reader.h>

/*
 * Return the filesize of `filename` or -1.
 */

static inline off_t
file_size(const char *filename) {
  struct stat s;
  int rc = stat(filename, &s);

  if (rc < 0) return -1;
  return s.st_size;
}

/*
 * Read the contents of `filename` or return NULL.
 */

static inline char *
file_read(const char *filename) {
  off_t len = file_size(filename);
  check(len < 0, "Invalid file length.");

  char *buf = malloc(len + 1);
  check_mem(buf);

  int fd = open(filename, O_RDONLY);
  check(fd < 0, "Error opening file.");

  ssize_t size = read(fd, buf, len);
  check(size != len, "???");

  return buf;

error:
  return NULL;
}


BytecodeFile *BytecodeFile_new(const char *filename)
{
  BytecodeFile *file = calloc(1, sizeof(BytecodeFile));
  check_mem(file);

  file->filename = strdup(filename);

  char *buf = file_read(filename);
  free(buf);

  return file;

error:
  return NULL;
}
