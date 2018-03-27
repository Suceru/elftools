#include <iostream>
#include <string>
#include "ElfFile.hpp"

using namespace std;

void usage(const char *const program_name)
{
  cout << "Usage: " << program_name << " elf-file\n";
}

int main(int argc, char** argv){
  if(argc < 2){
    cerr << "Nr of arguments (" << argc-1 << ") not supported!\n";
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  const auto elffile = std::string{argv[1]};

  ElfFile elf(elffile);
  if(!elf.checkFile()){
    cerr << "Invalid file. Header does not match expected format!";
    exit(EXIT_FAILURE);
  }

  const auto archClass = elf.getArchClass();

  switch(archClass){
  case ElfFile::ArchClass::Bit32:
    cout << "32-bit\n";
    break;
  case ElfFile::ArchClass::Bit64:
    cout << "64-bit\n";
    break;
  default:
    cout << "unknown\n";
    break;
  }
  
  exit(EXIT_SUCCESS);
}
