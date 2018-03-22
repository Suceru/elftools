#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include "ElfFile.hpp"
#include "ElfHandler.hpp"

using namespace std;

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

  ElfFile elf(elffile);
  if(!elf.checkFile()){
    fprintf(stderr, "Invalid file. Header does not match expected format!");
    exit(EXIT_FAILURE);
  }

  printf("Starting to analyze Elf file\n");
  
  const auto archClass = elf.getArchClass();

  switch(archClass){
  case ElfFile::ArchClass::Bit32:
    printf(" -> Arch Class: 32-bit\n");
    break;
  case ElfFile::ArchClass::Bit64:
    printf(" -> Arch Class: 64-bit\n");
    break;
  default:
    printf(" -> Arch Class: unknown\n");
    break;
  }

  const auto handler = elf.getHandler();
  const auto& symbolTable = handler->getSymbolTable();

  for(const auto& symbol : symbolTable)
    cout << symbol.first << " @ " << symbol.second << "\n";

  exit(EXIT_SUCCESS);
}
