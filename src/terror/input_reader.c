#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <terror/dbg.h>
#include <terror/bstrlib.h>
#include <terror/input_reader.h>

/*
 * Return the filesize of `filename` or -1.
 */

static inline off_t
file_size(bstring filename) {
  struct stat s;
  int rc = stat(bdata(filename), &s);

  if (rc < 0) return -1;
  return s.st_size;
}

/*
 * Read the contents of `filename` or return NULL.
 */

static inline bstring
file_read(bstring filename) {
  off_t len = file_size(filename);
  check(len >= 0, "Invalid file length.");

  char *buf = malloc(len + 1);
  check_mem(buf);

  int fd = open(bdata(filename), O_RDONLY);
  check(fd >= 0, "Error opening file.");

  ssize_t size = read(fd, buf, len);
  check(size == len, "Read length is invalid");

  bstring retval = bfromcstr(buf);
  free(buf);
  return retval;

error:
  return NULL;
}

static inline void
parse_string(bstring buf, BytecodeFile *file)
{

  struct bstrList *lines = bsplit(buf, '\n');
  int i = 0;
  int num_regs, num_locals, num_literals, num_instructions;

  bstring *line = lines->entry;

  // Get method name
  file->method = bmidstr(*line, 1, (*line)->mlen);
  line++;

  // Get counts
  sscanf(bdata(*line), ":%i:%i:%i:%i", 
    &num_regs, &num_locals, &num_literals, &num_instructions);

  file->registers_count    = num_regs;
  file->locals_count       = num_locals;
  file->literals_count     = num_literals;
  file->instructions_count = num_instructions;
  line++;

  // Parse the actual literals

  file->literals = malloc(sizeof(long) * num_literals);

  for(i=0; i < num_literals; i++) {
    if (bdata(*line)[0] == '"') {
      bstring strData = bmidstr(*line, 1, (*line)->mlen);
      file->literals[i] = (long)strdup(bdata(strData));
      bdestroy(strData);
    } else {
      file->literals[i] = (long)atoi(bdata(*line));
    }
    line++;
  }

  // Parse the instructions

  file->instructions = malloc(sizeof(Instruction*) * num_instructions);

  for(i=0; i < num_instructions; i++) {
    uint32_t num;
    sscanf(bdata(*line), "%x", &num);
    file->instructions[i] = Instruction_new(num);

    line++;
  }

  bstrListDestroy(lines);
}

BytecodeFile *BytecodeFile_new(bstring filename)
{
  BytecodeFile *file = calloc(1, sizeof(BytecodeFile));
  check_mem(file);

  file->filename = filename;

  bstring buf = file_read(filename);
  parse_string(buf, file);

  bdestroy(buf);

  return file;

error:
  return NULL;
}

void
BytecodeFile_destroy(BytecodeFile *file)
{
  int i = 0;

  if(file->filename) bdestroy(file->filename);
  if(file->method) bdestroy(file->method);
  if(file->literals_count > 0) free(file->literals);

  for(i = 0; i < file->instructions_count; i++) {
    Instruction_destroy(file->instructions[i]);
  }
  free(file->instructions);

  free(file);
}
