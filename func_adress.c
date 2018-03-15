#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

#include <elf.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

void usage(const char *const program_name)
{
  printf("Usage: %s func-name elf-file\n", program_name);
}

int main(int argc, char** argv){
  if(argc != 3){
    fprintf(stderr, "Nr of arguments (%d) not supported!\n", argc-1);
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  char *const elffile = argv[2];
  
  int fd = open(elffile, O_RDONLY);
  if(fd == -1){
    fprintf(stderr, "Could not open elf file %s! - ErrorNr: %d\n", elffile, errno);
    exit(EXIT_FAILURE);
  }

  struct stat statbuf;
  if(fstat(fd, &statbuf) == -1){
    fprintf(stderr, "Could not resolve file size! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  const int memory_size = statbuf.st_size;
  void* memory = mmap(NULL, memory_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(memory == MAP_FAILED){
    fprintf(stderr, "Could not map file in memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  printf("Starting to analyze Elf file\n");
  Elf64_Ehdr* elf_header = (Elf64_Ehdr*)memory;

  // TODO: do cool stuff here
  
  if(munmap(memory, memory_size) == -1){
    fprintf(stderr, "Could not unmap memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  
  if(close(fd) == -1){
    fprintf(stderr, "Could not close elf file %s! - ErrorNr: %d\n", elffile, errno);
    exit(EXIT_FAILURE);
  }
  
  exit(EXIT_SUCCESS);
}
