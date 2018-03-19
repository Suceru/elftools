#include <string>
#include <cstdio>
#include "ElfHandler.hpp"

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

  const auto elffile = std::string{argv[2]};

  ElfHandler elf(elffile);
  if(!elf.checkFile()){
    fprintf(stderr, "Invalid file. Header does not match expected format!");
    exit(EXIT_FAILURE);
  }

  printf("Starting to analyze Elf file\n");
  
  const auto archClass = elf.getArchClass();

  switch(archClass){
  case ElfHandler::ArchClass::Bit32:
    printf(" -> Arch Class: 32-bit\n");
    break;
  case ElfHandler::ArchClass::Bit64:
    printf(" -> Arch Class: 64-bit\n");
    break;
  default:
    printf(" -> Arch Class: unknown\n");
    break;
  }
  
  exit(EXIT_SUCCESS);
}
