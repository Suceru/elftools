#include "ElfHandler.hpp"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

#include <elf.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

ElfHandler::ElfHandler(const std::string& filename){
  _filename = filename;

  const auto elffile = _filename.c_str();

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

  _memorysize = static_cast<size_t>(statbuf.st_size);
  _memory = static_cast<char*>(mmap(NULL, _memorysize, PROT_READ, MAP_PRIVATE, fd, 0));
  if(_memory == MAP_FAILED){
    fprintf(stderr, "Could not map file in memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  
  if(close(fd) == -1){
    fprintf(stderr, "Could not close elf file %s! - ErrorNr: %d\n", elffile, errno);
    exit(EXIT_FAILURE);
  }
}

ElfHandler::~ElfHandler(){
  if(munmap(_memory, _memorysize) == -1){
    fprintf(stderr, "Could not unmap memory! - ErrorNr: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

ElfHandler::ArchClass ElfHandler::getArchClass(){
  switch(_memory[4]){
  case ELFCLASS32:
    return ArchClass::Bit32;
  case ELFCLASS64:
    return ArchClass::Bit64;
  default:
    return ArchClass::Unknown;
  }
}

bool ElfHandler::checkFile(){
  if(_memory[0] != 0x7F ||
     _memory[1] != 'E'  ||
     _memory[2] != 'L'  ||
     _memory[3] != 'F')
    return false;
  
  return true;
}
